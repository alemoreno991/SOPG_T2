/**
 * @file peer.c
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

#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L
/******************************************************************************
 *  Inclusions (library's order: standard C, others, user header files)
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "peer.h"
#include "socket.h"
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

static void* _peer_run_rx( void *self );

static void* _peer_run_tx( void *self );

/******************************************************************************
 *  Implementations of public functions
 *****************************************************************************/

void peer_create( peer_t *self )
{
    printf( "Nuevo peer creado\n" );
    Socket_create( &self->socket, self->fd );
    self->alive_flag = true;
}

void peer_run( peer_t *self )
{
    pthread_t thread;
    
    pthread_create( &thread, NULL, _peer_run_rx, (void *) self );

    _peer_run_tx( self );

    pthread_join( thread, NULL );
}

void peer_stop( peer_t *self )
{
    self->keep_running = false;
    Socket_shutdownchannel( &self->socket, RDWR );
}

void peer_destroy( peer_t *self )
{
    printf( "Peer destruido\n" );
	Socket_destroy( &self->socket );
    self->alive_flag = false;
}

/******************************************************************************
 *  Implementations of private functions
 *****************************************************************************/

void* _peer_run_tx( void *param )
{
    peer_t *peer = (peer_t *) param;
    char *msg;

	while ( true == peer->keep_running )
	{
        if ( NULL != ( msg = queue_pop( peer->queue_tx ) ) )
        {
            if ( false == Socket_send( &peer->socket, msg, strlen( msg ) ) )
            {
                break;
            }
        }
        /* falta hacer free al msg */
        usleep(50);
	}
    printf("Peer cerrandose... (canal tx)\n");

}

void* _peer_run_rx( void *param )
{
    peer_t *peer = (peer_t *) param;
	char msg[200] = {0};

	while ( true == peer->keep_running )
	{
        if ( 10 == Socket_recv( &peer->socket, msg, 10 ) )
        {
            queue_push( peer->queue_rx, msg );
        }
        else
        {
            break;
        }
    }
    printf("Peer cerrandose... (canal rx)\n");

}