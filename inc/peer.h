/**
 * @file peer.h
 * @author Alejandro M (ale.moreno991@gmail.com)
 * @brief API para administrar las nuevas conexiones aceptadas 
 * 
 * @version 2020-09-15 --------- Alejandro Moreno -------- v0.0
 * 
 * @copyright Copyright (c) 2020
 * 
 */

/*=====[Avoid multiple inclusion - begin]====================================*/
#ifndef PEER_H
#define PEER_H

/******************************************************************************
 *  Inclusions of public function dependencies
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>
#include "socket.h"
#include "queue.h"


/******************************************************************************
 *  Definition macros of public constants
 *****************************************************************************/

/******************************************************************************
 *  Public function-like macros
 *****************************************************************************/

/******************************************************************************
 *  Definitions of public data types
 *****************************************************************************/

typedef struct 
{
    volatile sig_atomic_t alive_flag;
    volatile sig_atomic_t keep_running;
    socket_t socket;
    int32_t fd;
    queue_t *queue_tx;
    queue_t *queue_rx;
} peer_t;


/******************************************************************************
 *  Prototypes (declarations) of public functions
 *****************************************************************************/

/**
 * @brief Constructor del objeto
 * 
 * @param self 
 */
void peer_create( peer_t *self );

/**
 * @brief Poner a correr un peer 
 * 
 * @param self 
 */
void peer_run( peer_t *self );

/**
 * @brief Permite frenar un peer
 * 
 * @param self 
 */
void peer_stop( peer_t *self );

/**
 * @brief Destructor del objeto
 * 
 * @param self 
 */
void peer_destroy( peer_t *self );

/*=====[Avoid multiple inclusion - end]======================================*/
#endif /* PEER_H */