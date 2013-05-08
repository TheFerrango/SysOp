#include "queue.h"



static int emptyp (const queue *q) 
{
  return q->head == NULL?1:0;
}

void init(queue *q)
{
  q->head = q->tail = NULL;
}

int enqueue(const char *string,queue *q)
{
  node * np = new node;
  if (np==NULL)
    return 0;

  strncpy(np->val,string,sizeof(np->val));
  np->next=NULL;

  if (emptyp(&q)) 
    q->head=q->tail=np;
  else 
  {
    q->tail->next=np;
    q->tail=np;
  }
  return 1;
}

int dequeue(char *s,queue *q)
{ 
  node * first;
  if (emptyp(&q))
    return 0;
  first = q->head;
  strncpy(s,first->val,sizeof(s));
  q->head = q->head->next;
  delete first; 
  return 1;
}

void print(const queue *q) 
{ 
  if (!emptyp(&q)) 
  {
    node * first=q->head;
    do 
    {
      printf("%s\n",first->val);
      first = first->next;
    } while (first != NULL);
  }
}

