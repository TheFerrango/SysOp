#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include "queue.h"


void *tr();
void *te();
void *td();
void *tw();
void get_random(int bytes, char* output);
void xor(char* input, char* mask);

pthread_t t_tr, t_te, t_td, t_tw;
sem_t sem_tr, sem_te;

queue *q_s, *q_r, *q_se, *q_sd;

int s_tr = -1, s_te = -1, s_td = -1, s_tw = -1;
int alive = 1;


int main() {    
    printf("starting\n");
    q_s = malloc(sizeof(queue));
    q_r = malloc(sizeof(queue));
    q_se = malloc(sizeof(queue));
    q_sd = malloc(sizeof(queue));
    
    sem_init(&sem_tr, 0, 0);
    sem_init(&sem_te, 0, 0);
    
    s_tr = pthread_create(&t_tr, NULL, tr, NULL); 
    
    pthread_join(t_tr, NULL);
    pthread_join(t_te, NULL);
    pthread_join(t_td, NULL);
    pthread_join(t_tw, NULL);
    
    sem_destroy(&sem_tr);
    sem_destroy(&sem_te);
    printf("exiting\n");
    return 0;   
}

void* tr() {
    printf("tr started\n");  
    if (s_te < 0) {    
        s_te = pthread_create(&t_te, NULL, te, NULL);
    } 
    char input[256]; // use max_length
    init(q_s);    
    while (alive > 0) {        
        fgets(input, sizeof(input), stdin);
        printf("%s\n", input);
        if (strcmp(input, "quit\n\0") == 0) {
            printf("tr exiting\n");
            alive = 0;
        } else {
            if (enqueue(input, q_s) < 1) {
                printf("error\n");
                alive = 0;
            } else {
                sem_post(&sem_tr);
            }
        }
    }
    
}

void* te() {    
    printf("te started\n");
    if (s_td < 0) {
        //s_td = pthread_create(&t_td, NULL, td, NULL);
    }
    while (alive > 0) {
        char input[256];
        sem_wait(&sem_tr);
        if (dequeue(input, q_s) < 1) {
            continue;
        }
        printf("stored input: %s\n", input);
        char output[256];
        get_random(strlen(input), output);
        printf("random: %s\n", output);      
        enqueue(output, q_r);
        xor(input, output);
        printf("encrypted input: %s\n", input);
        enqueue(input, q_se);
        sem_post(&sem_te);
    }       
    printf("te exiting\n");
}

void* td() {
    printf("td started\n");
    s_tw = pthread_create(&t_tw, NULL, tw, NULL);
    printf("td exiting\n");
}

void* tw() {
    printf("tw started\n");
    printf("tw exiting\n");    
}

void get_random(int bytes, char* output) {
    int random_data = open("/dev/random", O_RDONLY);
    if (random_data < 0){
        printf("random error");
        //log_error("Cant open /dev/random", 1, 1);
    }
    int i = 0;
    for (; i < bytes; i++){
        read(random_data, &output[i], sizeof(char));
    }
    close(random_data);
}

void xor(char* input, char* mask) {
    int i = 0;
    for (; i < strlen(input); i++) {
        input[i] = input[i] ^ mask[i];
    }
}
