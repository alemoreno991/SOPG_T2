/**
 * @file server.h
 * @author Alejandro M (ale.moreno991@gmail.com)
 * @brief API del servidor 
 * 
 * @version 2020-09-15 --------- Alejandro Moreno -------- v0.0
 * 
 * @copyright Copyright (c) 2020
 * 
 */

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef SERVER_H
#define SERVER_H


/******************************************************************************
 *  Inclusions of public function dependencies
 *****************************************************************************/
#include <signal.h>
#include "socket_server.h"
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
    socket_server_t skt_server;
    volatile sig_atomic_t keep_running;
} server_t;

typedef struct 
{
    queue_t *queue_tx;
    queue_t *queue_rx;
} server_config;

/******************************************************************************
 *  Prototypes (declarations) of public functions
 *****************************************************************************/

/**
 * @brief Constructor del objeto
 * 
 * @param self 
 * @param port Puerto en el que se desea habilitar el servidor
 * @return true 
 * @return false 
 */
bool Server_Create( server_t *self, char port[] );

/**
 * @brief Pone a correr el servidor
 * 
 * @param self 
 * @return true 
 * @return false 
 */
bool Server_Run( server_t *self );

/**
 * @brief Permite cerrar ordenadamente el servidor
 * 
 * @param self 
 * @return true 
 * @return false 
 */
bool Server_Stop( server_t *self );

/**
 * @brief Destruye el servidor
 * 
 * @param self 
 * @return true 
 * @return false 
 */
bool Server_Destroy( server_t *self );

/*=====[Avoid multiple inclusion - end]======================================*/
#endif /* SERVER_H */