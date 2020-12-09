/**
 * @file queue.c
 * @author Alejandro Moreno (ale.moreno991@gmail.com)
 * @brief Acá se hace una breve descripción del archivo
 * 
 * @version 2020-12-04 --------- Alejandro Moreno -------- v0.0
 * 
 * @copyright Copyright (c) 2020
 * 
 * @details Acá se puede hacer una descripción más detallada del archivo. 
 * Quizás, incorporando cosas más específicas.
 * 
 */

/******************************************************************************
 *  Inclusions (library's order: standard C, others, user header files)
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue.h"

/******************************************************************************
 *  Inclusions of private function dependencies
 *****************************************************************************/

/******************************************************************************
 *  Definitions of private data types (order: Enums, Typedefs)
 *****************************************************************************/

/******************************************************************************
 *  Definition macros of private constants
 *****************************************************************************/

/******************************************************************************
 *  Private function-like macros
 *****************************************************************************/

/******************************************************************************
 *  Definitions of external public global variables
 *****************************************************************************/

/******************************************************************************
 *  Definitions of public global variables
 *****************************************************************************/

/******************************************************************************
 *  Definitions of private global variables
 *****************************************************************************/

/******************************************************************************
 *  Prototypes (declarations) of private functions  
 *****************************************************************************/

/******************************************************************************
 *  Implementations of public functions
 *****************************************************************************/

void queue_create( queue_t *self )
{
    if( 0 != pthread_mutex_init( &self->mutex, NULL ) )
    {
        exit (EXIT_FAILURE);
    }
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


/******************************************************************************
 *  Implementations of private functions
 *****************************************************************************/
