#include "queue.h"


static int emptyp (const queue *q)
{
  return q->head == NULL?1:0;
}

void init(queue *q)
{

  q->head= malloc(sizeof(node));
  q->tail= malloc(sizeof(node));

  q->head = NULL;
  q->tail = NULL;
}

int enqueue(const char *string,queue *q)
{
  node *np = malloc(sizeof(node));
  np->val = malloc(sizeof(char)*(MAX_LENGTH + 1));
  if (np==NULL)
    return 0;
  strcpy(np->val,string);
  np->next=NULL;

  if (emptyp(q))
    q->head=q->tail=np;
  else
  {
    q->tail->next = np;
    q->tail=np;
  }
  return 1;
}

int dequeue(char *s,queue *q)
{
  node * first;
  if (emptyp(q))
    return 0;
  first = q->head;

  strcpy(s,first->val);
  q->head = (struct node*) q->head->next;

  free(first->val);
  free(first);
  return 1;
}


