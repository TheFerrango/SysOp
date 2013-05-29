#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include "queue.h"
#include "logs.h"


/* thread functions */
void *tr();
void *te();
void *td();
void *tw();

/* extra functions */
void get_random(int bytes, char* output);
void xor(char* input, char* mask);

/* thread and semaphore handles, statuses  */
pthread_t t_tr, t_te, t_td, t_tw;
sem_t sem_tr, sem_te, sem_td;
int s_tr = -1, s_te = -1, s_td = -1, s_tw = -1;

/* queues for thread message passing */
queue *q_s, *q_r, *q_se, *q_sd;

/* execution value */
int alive = 1, td_alive = 1, te_alive = 1;


int main() {
    /* queues initialization */
    q_s = malloc(sizeof(queue));
    init(q_s);
    q_r = malloc(sizeof(queue));
    init(q_r);
    q_se = malloc(sizeof(queue));
    init(q_se);
    q_sd = malloc(sizeof(queue));
    init(q_sd);

    /* semaphores initialization */
    sem_init(&sem_tr, 0, 0);
    sem_init(&sem_te, 0, 0);
    sem_init(&sem_td, 0, 0);

    /* starting first thread (TR, reading one) */
    s_tr = pthread_create(&t_tr, NULL, tr, NULL);

    /* waiting for all thread to finish before exiting */
    pthread_join(t_tr, NULL);
    pthread_join(t_te, NULL);
    pthread_join(t_td, NULL);
    pthread_join(t_tw, NULL);

    /* freeing resources */
    sem_destroy(&sem_tr);
    sem_destroy(&sem_te);
    free(q_s);
    free(q_r);
    free(q_se);
    free(q_sd);

    return 0;
}

void* tr() {
    /* during first run, starting TE thread */
    if (s_te < 0) {
        s_te = pthread_create(&t_te, NULL, te, NULL);
    }
    char input[MAX_LENGTH];

    /* thread main loop */
    while (alive > 0) {
        log_info("tr","tr started");
        /* reading user input */
        fgets(input, sizeof(input), stdin);
        if (strcmp(input, "quit\n\0") == 0) {
            /* exiting on "quit" command */
            alive = 0;
            continue;
        } else {
            if (enqueue(input, q_s) < 1) {
                /* critical error (nearly impossible) */
                alive = 0;
                exit(1);
            }
        }
        /* signaling new data is ready */
        sem_post(&sem_tr);
    }
    /* escape route (unlocks awaiting threads when closing everythin) */
    sem_post(&sem_tr);
    return NULL;
}

void* te() {
    /* during first run, starting TD thread */
    if (s_td < 0) {
        s_td = pthread_create(&t_td, NULL, td, NULL);
    }
    char input[MAX_LENGTH];
    char output[MAX_LENGTH];

    /* thread main loop */
    while (1) {
        /* waiting for some data to be processed */
        sem_wait(&sem_tr);
        log_error("te","te started");
        if (dequeue(input, q_s) < 1) {
            /* no data ready to read (the program is shutting down) */
            break;
        }
        /* getting some random data */
        get_random(strlen(input), output);
        /* saving the random data */
        enqueue(output, q_r);
        printf("R:\t %s\n",output);

        /* encryption */
        xor(input, output);
        /* saving encrypted data */
        enqueue(input, q_se);
        printf("SE:\t %s\n",input);
        /* signaling new data is ready */
        sem_post(&sem_te);
    }
    /* escape route (unlocks awaiting threads when closing everything) */
    sem_post(&sem_te);
    te_alive = 0;
    return NULL;
}

void* td() {
    /* during first run, starting TW thread */
    if (s_tw < 0) {
        s_tw = pthread_create(&t_tw, NULL, tw, NULL);
    }
    char input[MAX_LENGTH];
    char mask[MAX_LENGTH];


    /* thread main loop */
    while (1) {
        /* waiting for some data to be processed */
        sem_wait(&sem_te);
        log_debug("td","td started");
        if (dequeue(input, q_se) < 1) {
            /* no data ready to read (the program is shutting down) */
            break;
        }
        if (dequeue(mask, q_r) < 1) {
            /* no data ready to read (the program is shutting down) */
            break;
        }
        /* decryption */
        xor(input, mask);
        /* saving decrypted data */
        enqueue(input, q_sd);
        /* signaling new data is ready */
        sem_post(&sem_td);
    }
    /* escape route (unlocks awaiting threads when closing everythin) */
    sem_post(&sem_td);
    return NULL;
}

void* tw() {
    char output[MAX_LENGTH];

    /* thread main loop */
    while (1) {
        /* waiting for some data to be processed */
        sem_wait(&sem_td);
        if (dequeue(output, q_sd) < 1) {
            /* no data ready to read (the program is shutting down) */
            break;
        } else {
            /* printing decrypted data */
            printf("SD:\t %s\n", output);
        }
    }
    return NULL;
}

// Returns the specified number of random bytes
void get_random(int bytes, char* output) {

    int random_data = open("/dev/random", O_RDONLY);
    if (random_data < 0){
        /* error opening stream */
        exit(2);
    }

    int i = 0;
    int offset = 33;

    for (; i < bytes; i++){
        read(random_data, &output[i], sizeof(char));
        output[i] = (char)((abs((int)output[i]) % 93) + offset);
    }
    close(random_data);
    output[i] = '\0';
}

// XOR between input and mask
void xor(char* input, char* mask) {
    char result[MAX_LENGTH];
    int i = 0;
    for (; i < strlen(input); i++) {
        result[i] = input[i] ^ mask[i];
    }
    strncpy(input, result, strlen(result));
    input[i] = '\0';
}
