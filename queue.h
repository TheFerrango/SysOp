#ifndef QUEUE_H
#define QUEUE_H

#define MAX_LENGTH 256
#include <string.h>
#include <stdlib.h>


typedef node;
struct node 
{
  char val[MAX_LENGTH];
  node* next;
};
typedef node* queue;
struct queue 
{
  node * tail;
  node * head;
};

void init(queue *);
int enqueue(const char *,queue *);
int dequeue(char *,queue *);
void print (const queue);



#endif




