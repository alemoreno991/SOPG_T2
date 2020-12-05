/**
 * @file socket_server.c
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
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
 *  Includes ad-hoc
 */
#include "socket_server.h"

/******************************************************************************
 *  Inclusions of private function dependencies
 *****************************************************************************/

/******************************************************************************
 *  Definitions of private data types (order: Enums, Typedefs)
 *****************************************************************************/

/******************************************************************************
 *  Definition macros of private constants
 *****************************************************************************/
#define MAX_LISTEN_QUEUE 20

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

bool _Bind( socket_server_t *self, struct addrinfo *result );    

bool _Listen( socket_server_t *self );

/******************************************************************************
 *  Implementations of public functions
 *****************************************************************************/

bool SocketServer_create( socket_server_t *self, char port[] ) 
{    
    if (port == NULL) 
    {
        fprintf(stderr, "Se ha olvidado explicitar el puerto\n");
        return false;
    }
    
    if ( NULL == (self->port = (char *) malloc( strlen(port) * sizeof(char) )))
    {
        fprintf(stderr, "Problema al alocar memoria\n");
        return false;
    }
    else
    {
        strncpy(self->port, port, strlen(port) + 1);
    }
    
    return true;
}

bool SocketServer_bind_listen( socket_server_t *self ) {
    struct addrinfo hints, *result;
    bool bind_flag;
    
    /* Cargo la estructura addrinfo */
    memset( &hints, 0, sizeof(struct addrinfo) );
    hints.ai_family = AF_INET;          // Trabajamos con IPv4
    hints.ai_socktype = SOCK_STREAM;    // Trabajamos con TCP
    hints.ai_flags = AI_PASSIVE;        // Configuro para usar como servidor 
    hints.ai_protocol = 0;              // Trabajo con cualquier protocolo
    hints.ai_canonname = NULL;          // No entiendo que configura esto
    hints.ai_addr = NULL;               // No entiendo que configura esto
    hints.ai_next = NULL;               // No entiendo que configura esto

    /* Construyo el addrinfo */
    if ( getaddrinfo( NULL, self->port, &hints, &result ) != 0 )
    {
        fprintf(stderr, "Problema getaddrinfo\n");
        return false;
    }

    /* Bindeo */
    bind_flag = _Bind( self, result );

    /* Libero la estructura addrinfo, ya no la voy a usar más */
    freeaddrinfo( result );

    if ( false == bind_flag )
    {
        return false;
    }

    /* Intento "escuchar" el socket */
    return _Listen( self );
}

int SocketServer_accept( socket_server_t *self ) {
    int peer_fd;  /* File descriptor del nuevo cliente */

     /* Acepto un cliente */   
    if ( -1 == ( peer_fd  = accept( self->fd, NULL, NULL ) ) ) {
        fprintf( stderr, "Error aceptando un nuevo cliente\n");
        return false;
    }

    return peer_fd;
}

bool SocketServer_shutdownchannel( socket_server_t *self, channel_config selector )
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

bool SocketServer_destroy( socket_server_t *self ) {
    if ( NULL != self->port )
    {
        free( self->port );
    }

    close( self->fd );
    
    return true;    
}


/******************************************************************************
 *  Implementations of private functions
 *****************************************************************************/

bool _Bind( socket_server_t *self, struct addrinfo *result )
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

        /* Intento bindear */
        if ( 0 == bind( self->fd, rp->ai_addr, rp->ai_addrlen ) )
        {
            /* Si logro bindear salgo del loop */
            break;
        }   

        /* En caso de no haber podido bindear cierro el socket */
        close( self->fd );
    }
    
    /* Manejo errores (si no pude bindear contra ningun socket */
    if ( rp == NULL )
    {
        fprintf(stderr, "Bind fallido\n");
        return false;
    }

    return true;
}    

bool _Listen( socket_server_t *self )
{
    /* Intento hacer que el socket "escuche") */
    if ( -1 == listen( self->fd, MAX_LISTEN_QUEUE ) ) 
    {
        /* En caso de no poder hacer que "escuche" cierro todo */
        fprintf( stderr, "Listen fallido\n");
        close( self->fd );
        return false;
    }

    return true;
}