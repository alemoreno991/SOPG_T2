#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue.h"

void queue_create( queue_t *self )
{
    pthread_mutex_init( &self->mutex, NULL );
    self->queue = NULL;
}

void queue_push( queue_t *self, char *ptr )
{
    node_t *new_node = (node_t *) malloc(sizeof(node_t));
    if (!new_node) 
    {
        fprintf( stderr, "Error allocando memoria para cola\n");
        return;
    }

    new_node->ptr = ptr;
    
    pthread_mutex_lock( &self->mutex );
    new_node->next = self->queue;
    self->queue = new_node;    
    pthread_mutex_unlock( &self->mutex );
}

char* queue_pop( queue_t *self )
{
    node_t *current, *prev = NULL;
    char *retval = NULL;

    pthread_mutex_lock( &self->mutex );
    if (self->queue == NULL) 
    {
        pthread_mutex_unlock( &self->mutex );    
        return NULL;
    }

    current = self->queue;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

    retval = current->ptr;
    free(current);
    
    if (prev)
        prev->next = NULL;
    else
        self->queue = NULL;

    pthread_mutex_unlock( &self->mutex );

    return retval;    
}

void queue_destroy( queue_t *self )
{
    node_t *current, *prev = NULL;

    pthread_mutex_lock( &self->mutex );
    current = self->queue;
    while ( current != NULL ) 
    {
        prev = current;
        current = current->next;
        free(prev);
    }
    pthread_mutex_unlock( &self->mutex );

    pthread_mutex_destroy( &self->mutex );
}