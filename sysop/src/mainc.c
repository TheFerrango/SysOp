#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
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
pthread_mutex_t m_q_s, m_q_r, m_q_se, m_q_sd;

queue *q_s, *q_r, *q_se, *q_sd;

int s_tr = -1, s_te = -1, s_td = -1, s_tw = -1;
int alive = 1;


int main() {    
    printf("starting\n");
    q_s = malloc(sizeof(queue));
    q_r = malloc(sizeof(queue));
    q_se = malloc(sizeof(queue));
    q_sd = malloc(sizeof(queue));
    
    pthread_mutex_init(&m_q_s, NULL);
    pthread_mutex_init(&m_q_r, NULL);
    pthread_mutex_init(&m_q_se, NULL);
    pthread_mutex_init(&m_q_sd, NULL);
    
    s_tr = pthread_create(&t_tr, NULL, tr, NULL); 
    
    pthread_join(t_tr, NULL);
    pthread_join(t_te, NULL);
    pthread_join(t_td, NULL);
    pthread_join(t_tw, NULL);
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
        pthread_mutex_lock(&m_q_s);        
              
        fgets(input, sizeof(input), stdin);
        printf("%s\n", input);
        if (strcmp(input, "quit\n\0") == 0) {
            printf("tr exiting\n");
            alive = 0;
        } else {
            if (enqueue(input, q_s) < 1) {
                printf("error\n");
                alive = 0;
            }
        }        
        pthread_mutex_unlock(&m_q_s);
        sleep(1);
        printf("unlocked");
    }
    
}

void* te() {
    printf("te started\n");
    if (s_td < 0) {
        //s_td = pthread_create(&t_td, NULL, td, NULL);
    }
    while (alive > 0) {
        printf("alive!");
        char input[256];
        pthread_mutex_lock(&m_q_s);
        printf("locked!");
        if (dequeue(input, q_s) < 1) {
            pthread_mutex_unlock(&m_q_s);
            sleep(1);
            continue;
        }
        printf("stored input: %s\n", input);
        pthread_mutex_unlock(&m_q_s);
        char output[256];
        get_random(strlen(input), output);
        printf("random: %s\n", output);
        pthread_mutex_lock(&m_q_se);
        pthread_mutex_lock(&m_q_r);        
        enqueue(output, q_r);
        xor(input, output);
        printf("encrypted input: %s\n", input);
        enqueue(input, q_se);
        pthread_mutex_unlock(&m_q_r);
        pthread_mutex_unlock(&m_q_se);
        sleep(0);
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
