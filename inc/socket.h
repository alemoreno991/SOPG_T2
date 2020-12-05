/**
 * @file socket.h
 * @author Alejandro M (ale.moreno991@gmail.com)
 * @brief API para manejar un socket generico
 * 
 * @version 2020-09-15 --------- Alejandro Moreno -------- v0.0
 * 
 * @copyright Copyright (c) 2020
 * 
 */

/*=====[Avoid multiple inclusion - begin]====================================*/
#ifndef SOCKET_H
#define SOCKET_H

/******************************************************************************
 *  Inclusions of public function dependencies
 *****************************************************************************/
#include <stdint.h>

/******************************************************************************
 *  Definition macros of public constants
 *****************************************************************************/

/******************************************************************************
 *  Public function-like macros
 *****************************************************************************/

/******************************************************************************
 *  Definitions of public data types
 *****************************************************************************/
typedef enum
{
    WR,
    RD,
    RDWR
} channel_config;

typedef struct 
{
    int32_t fd;
} socket_t;


/******************************************************************************
 *  Prototypes (declarations) of public functions
 *****************************************************************************/

/**
 * @brief Constructor del objeto
 * 
 * @details Se crea un handle del socket a partir de su file descriptor
 * 
 * @param self 
 * @param file_descriptor File descriptor del socket
 */
void Socket_create( socket_t *self, int32_t file_descriptor );

/**
 * @brief Envía un mensaje por el socket
 * 
 * @param self 
 * @param msg Mensaje a transmitir
 * @param len Largo del mensaje a transmitir
 * @return true     Exito en el envío   
 * @return false    Falla en el envío
 */
bool Socket_send( socket_t *self, const char *msg, int32_t len );

/**
 * @brief Recibe un mensaje por el socket
 * 
 * @details Es necesario saber de antemano cual es la longitud del mensaje
 * a recibir
 * 
 * @param self 
 * @param recv_buffer Buffer donde se almacena el mensaje recibido
 * @param recv_len    Largo esperado del mensaje a recibir (en bytes)
 * @return int  
 *      - >0 indica la cantidad de bytes recibidos
 *      - 0  indica que el otro extremo cerró el canal de comunicación
 *      - <0 indica un error en la comunicación
 */
int Socket_recv( socket_t *self, char *recv_buffer, int32_t recv_len );

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
bool Socket_shutdownchannel( socket_t *self, channel_config selector );

/**
 * @brief Destructor del objeto
 * 
 * @param self 
 * @return true 
 * @return false 
 */
bool Socket_destroy( socket_t *self );

/*=====[Avoid multiple inclusion - end]======================================*/
#endif /* SOCKET_H */