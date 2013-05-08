#ifndef QUEUE_H
#define QUEUE_H

#define MAX_LENGTH 256
#include <string.h>
#include <stdlib.h>


typedef struct node
{
  char val[MAX_LENGTH];
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




