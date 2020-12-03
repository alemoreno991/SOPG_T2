#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

typedef struct node {
    char *ptr;
    struct node *next;
} node_t;

typedef struct 
{
    node_t *queue;
    pthread_mutex_t mutex;
} queue_t;

void queue_create( queue_t *self );

void queue_push( queue_t *self, char *ptr );

char* queue_pop( queue_t *self );

void queue_destroy( queue_t *self );


#endif