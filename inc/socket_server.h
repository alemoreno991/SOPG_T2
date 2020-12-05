/**
 * @file socket_server.h
 * @author Alejandro M (ale.moreno991@gmail.com)
 * @brief API para manejar sockets en un servidor
 * 
 * @version 2020-09-15 --------- Alejandro Moreno -------- v0.0
 * 
 * @copyright Copyright (c) 2020
 * 
 */

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

/******************************************************************************
 *  Inclusions of public function dependencies
 *****************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "socket.h"

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
    int32_t fd;
    char *port;
} socket_server_t;

/******************************************************************************
 *  Prototypes (declarations) of public functions
 *****************************************************************************/

/**
 * @brief Crea el objeto
 * 
 * @param self 
 * @param port Puerto
 * @return true 
 * @return false 
 */
bool SocketServer_create(socket_server_t *self, char port[] );

/**
 * @brief Hace el bind y el listen 
 * 
 * @param self 
 * @return true 
 * @return false 
 */
bool SocketServer_bind_listen(socket_server_t *self );

/**
 * @brief Acepta una nueva conexión
 * 
 * @param self 
 * @return int32_t File descriptor correspondiente al socket con la nueva conexión
 */
int32_t SocketServer_accept(socket_server_t *self );

/**
 * @brief Permite cerrar un canal (solo lectura, solo escritura o lectura-escritura) 
 * 
 * @param self 
 * @param selector Indica el modo en que se desea cerrar el canal
 *      - RD ( cerrar la lectura )
 *      - WD ( cerrar la escritura )
 *      - RDWD ( cerrar la lectura y escritura )
 * 
 * @return true 
 * @return false 
 */
bool SocketServer_shutdownchannel(socket_server_t *self, channel_config selector );
    
/**
 * @brief Destructor del objeto
 * 
 * @param self 
 * @return true 
 * @return false 
 */
bool SocketServer_destroy(socket_server_t *self );

/*=====[Avoid multiple inclusion - end]======================================*/
#endif /* SOCKET_SERVER_H */
