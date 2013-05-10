#ifndef QUEUE_H
#define QUEUE_H


#include <string.h>
#include <stdlib.h>
#include "utils.h"


typedef struct node
{
  char val[MAX_LENGTH + 1];
  struct node* next;
}node;

typedef struct  queue
{
  struct node * tail;
  struct node * head;
}queue;

void init(queue *);
int enqueue(const char *,queue *);
int dequeue(char *,queue *);
void print_queue(const queue*);


#endif




