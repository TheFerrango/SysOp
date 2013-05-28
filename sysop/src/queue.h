#ifndef QUEUE_H
#define QUEUE_H


#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "utils.h"


typedef struct node {
    char *val;
    struct node* next;
} node;

typedef struct  queue {
    pthread_mutex_t queue_lock;
    struct node * tail;
    struct node * head;
} queue;

void init(queue *);
int enqueue(const char *,queue *);
int dequeue(char *,queue *);


#endif




