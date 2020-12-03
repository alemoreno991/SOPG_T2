#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "peer.h"
#include "socket.h"
#include "queue.h"

static void* _peer_run_rx( void *self );
static void* _peer_run_tx( void *self );

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