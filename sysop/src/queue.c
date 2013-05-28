#include "queue.h"
#include <pthread.h>

static int emptyp (const queue *q)
{
    return q->head == NULL?1:0;
}

void init(queue *q)
{
    pthread_mutex_init(&q->queue_lock, NULL);  
    q->head = malloc(sizeof(node));
    q->tail = malloc(sizeof(node));
    q->head = NULL;
    q->tail = NULL;
}

int enqueue(const char *string,queue *q)
{
    pthread_mutex_lock(&q->queue_lock);
    node *np = malloc(sizeof(node));
    np->val = malloc(sizeof(char)*(MAX_LENGTH + 1));
    if (np==NULL) {
        pthread_mutex_unlock(&q->queue_lock);
        return 0;
    }
    strcpy(np->val,string);
    np->next=NULL;

    if (emptyp(q)) {
        q->head=q->tail=np;
    }
    else
    {
        q->tail->next = np;
        q->tail=np;
    }
    pthread_mutex_unlock(&q->queue_lock);
    return 1;
}

int dequeue(char *s,queue *q)
{
    pthread_mutex_lock(&q->queue_lock);
    node * first;
    if (emptyp(q)) {
        pthread_mutex_unlock(&q->queue_lock);
        return 0;
    }
    first = q->head;

    strcpy(s,first->val);
    q->head = (struct node*) q->head->next;

    free(first->val);
    free(first);
    pthread_mutex_unlock(&q->queue_lock);
    return 1;
}


