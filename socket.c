#define _POSIX_C_SOURCE 200112L

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

#define INVALID_FD -1

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

bool _Connect( socket_t *self, struct addrinfo *result )
{
    struct addrinfo *rp;

    /* Itero por las posibles conexiones que cumplen con lo que yo especifiqué */
    for ( rp = result; rp != NULL; rp = rp->ai_next )
    {
        /* Intento generar el socket */
        if ( -1 == ( self->fd = socket( rp->ai_family, 
                                        rp->ai_socktype, 
                                        rp->ai_protocol ) ) )
        {
            /* Si no pude crear el socket entonces voy a la siguiente iteración */
            continue; 
        }

        /* Intento conectar */
        if ( -1 != connect( self->fd, rp->ai_addr, rp->ai_addrlen ) )
        {
            /* Si logro conectar salgo del loop */
            break;
        }

        /* En caso de no haber podido bindear cierro el socket */
        close( self->fd );
    }

    /* Manejo errores (si no pude conectar contra ningun socket) */
    if ( rp == NULL )
    {
        fprintf(stderr, "Conexion fallido\n");
        return false;
    }

    return true;
}

