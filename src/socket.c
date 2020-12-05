/**
 * @file socket.c
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

#define _POSIX_C_SOURCE 200112L
/******************************************************************************
 *  Inclusions (library's order: standard C, others, user header files)
 *****************************************************************************/

/*
 *  Includes comunes de C
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

/*
 *  Includes necesarios para TCP/IP
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
 *  Includes ad-hoc
 */
#include "socket.h"
/******************************************************************************
 *  Inclusions of private function dependencies
 *****************************************************************************/

/******************************************************************************
 *  Definitions of private data types (order: Enums, Typedefs)
 *****************************************************************************/

/******************************************************************************
 *  Definition macros of private constants
 *****************************************************************************/

#define INVALID_FD -1

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

void Socket_create( socket_t *self, int32_t file_descriptor )
{
    self->fd = file_descriptor; 
}

bool Socket_send( socket_t *self, const char *msg, int len )
{
    int bytes_send = 0;

    /* Envío los bytes (asegurandome que se envíen todos) */
    for ( int i = 0; i < len; i += bytes_send )
    {
        /* verifico si hubo algun error en el envío */
        if ( -1 == ( bytes_send = send( self->fd, 
                                        &msg[i], 
                                        len - i, 
                                        MSG_NOSIGNAL ) ) )
        {
            fprintf(stderr, "Error en el envío de datos\n");
            return false;
        }
    }

    return true;
}

int Socket_recv( socket_t *self, char *recv_buffer, int recv_len )
{
    int bytes_recv = 0;

    /* Recibo un mensaje verificando que estoy capturando todos los bytes */
    for ( int i = 0; i < recv_len; i += bytes_recv ) 
    {
        /* Checkeo que no haya ocurrido un error en la comunicación */
        if ( -1 == ( bytes_recv = recv( self->fd, 
                                        &recv_buffer[i], 
                                        recv_len - i, 
                                        0) ) )
        {
            fprintf(stderr, "Error en la recepción de datos\n");
            return -1;
        }
        else
        {
            /* En caso que el otro extremo haya cerrado el canal de transmisión
             * cierro el canal de recepción */
            if ( bytes_recv == 0 ) 
            {
                fprintf(stderr, "DEBUG: recv_buffer: %s\n", recv_buffer );
                fprintf(stderr, "DEBUG: bytes_recv: %d\n", bytes_recv );
                fprintf(stderr, "DEBUG: total_bytes: %d\n", i );
                Socket_shutdownchannel( self, RD );
                break;
            }            
        }
    }

    return bytes_recv;
}

bool Socket_shutdownchannel( socket_t *self, channel_config selector )
{
    switch (selector) {
        case WR:
            shutdown( self->fd, SHUT_WR);
            break;

        case RD:
            shutdown( self->fd, SHUT_RD);
            break;

        case RDWR:
            shutdown( self->fd, SHUT_RDWR);
            break;
    }

    return true;
}

bool Socket_destroy( socket_t *self )
{
    close( self->fd );
    
    return true;  
}

/******************************************************************************
 *  Implementations of private functions
 *****************************************************************************/