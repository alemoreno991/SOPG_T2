/**
 * @file queue.h
 * @author Alejandro M (ale.moreno991@gmail.com)
 * @brief Descripción breve 
 * 
 * @version 2020-09-15 --------- Alejandro Moreno -------- v0.0
 * 
 * @copyright Copyright (c) 2020
 * 
 */

/*=====[Avoid multiple inclusion - begin]====================================*/
#ifndef QUEUE_H
#define QUEUE_H

/******************************************************************************
 *  Inclusions of public function dependencies
 *****************************************************************************/
#include <pthread.h>


/******************************************************************************
 *  Definition macros of public constants
 *****************************************************************************/

/******************************************************************************
 *  Public function-like macros
 *****************************************************************************/

/******************************************************************************
 *  Definitions of public data types
 *****************************************************************************/

typedef struct node {
    char *ptr;
    struct node *next;
} node_t;

typedef struct 
{
    node_t *queue;
    pthread_mutex_t mutex;
} queue_t;

/******************************************************************************
 *  Prototypes (declarations) of public functions
 *****************************************************************************/

/**
 * @brief Constructor del objeto
 * 
 * @param self 
 */
void queue_create( queue_t *self );

/**
 * @brief Coloca un elemento en la cola
 * 
 * @param self 
 * @param ptr Puntero al elemento que se desea encolar
 */
void queue_push( queue_t *self, char *ptr );

/**
 * @brief Saca un elemento de la cola y devuelve su puntero
 * 
 * @param self 
 * @return char* puntero al objeto extraído
 */
char* queue_pop( queue_t *self );

/**
 * @brief Destructor del objeto
 * 
 * @param self 
 */
void queue_destroy( queue_t *self );


/*=====[Avoid multiple inclusion - end]======================================*/
#endif /* QUEUE_H */