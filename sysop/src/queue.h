/*
A. A 2012/2013
Corso di laurea di informatica

Buratti Alberto (145552), 
Lotto Lorenzo (151775), 
Morandi Mirko(151778)
*/

#ifndef QUEUE_H
#define QUEUE_H

#define MAX_LENGTH 256

#include <string.h>
#include <stdlib.h>
#include <pthread.h>



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
int emptyp (const queue *q);


#endif




