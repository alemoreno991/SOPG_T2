#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "server.h"
#include "peer.h"
#include "queue.h"
#include "serial.h"

static void* thread_peer( void *param );
static void* thread_serial_rx( void *param );
static void* thread_serial_tx( void *param );

bool Server_Create( server_t *self, char port[] )
{
    if ( true == SocketServer_create( &self->skt_server, port ) &&
	     true == SocketServer_bind_listen( &self->skt_server )  )
    {
        self->keep_running = true;
        return true;
    }

    return false;
}

bool Server_Run( server_t *self )
{
	int32_t fd;
    pthread_t *threads = NULL;
    peer_t *threads_param = NULL;
    uint32_t num_threads = 0;
    
    serial_t serial_handle;
    pthread_t serial_tx, serial_rx;
    serial_cfg serial_config;

    queue_t queue1, queue2;

	queue_create( &queue1 );
	queue_create( &queue2 );

    serial_config.baudrate = 115200;
    serial_config.port_number = 1;
    serial_config.queue_rx = &queue1;
    serial_config.queue_tx = &queue2;

    Serial_Create( &serial_handle, &serial_config );
    pthread_create( &serial_rx, NULL, thread_serial_rx, (void *) &serial_handle );
    pthread_create( &serial_tx, NULL, thread_serial_tx, (void *) &serial_handle );

    while ( true == self->keep_running )
	{
        fd = SocketServer_accept( &self->skt_server );

        num_threads++;
        if ( NULL == ( threads = (pthread_t *) realloc( threads, 
                                    num_threads * sizeof( pthread_t ) ) ) )
        {
            fprintf( stderr, "Error asignando memoria para almacenar threads\n");
            return false;
        }

        if ( NULL == ( threads_param = (peer_t *) realloc( threads_param, 
                                            num_threads * sizeof( peer_t ) ) ) )
        {
            fprintf( stderr, "Error asignando memoria para almacenar threads_param\n");
            return false;
        }

        threads_param[num_threads -1].keep_running = true;
        threads_param[num_threads -1].fd = fd;
        threads_param[num_threads -1].queue_rx = &queue2;
        threads_param[num_threads -1].queue_tx = &queue1;

        pthread_create( &threads[num_threads - 1], 
                        NULL, 
                        thread_peer, 
                        (void *) &threads_param[num_threads - 1] );
    }

    printf("Servidor cerrandose... (no acepta más conexiones)\n");
    for (size_t i = 0; i < num_threads; i++)
    {
        peer_stop( &threads_param[i] );
        pthread_join( threads[i], NULL );
        printf("Peer %ld cerrado\n", i );
    }

    printf("Peers CERRADOS...\n");

    Serial_Stop( &serial_handle );
    pthread_join( serial_rx, NULL );
    pthread_join( serial_tx, NULL );

    queue_destroy( &queue1 );
	queue_destroy( &queue2 );

    return true;
}

bool Server_Stop( server_t *self )
{
    self->keep_running = false;
    SocketServer_shutdownchannel( &self->skt_server, RDWR );
}

bool Server_Destroy( server_t *self )
{
    SocketServer_destroy( &self->skt_server );
    return true;
}

void* thread_peer( void *param )
{
    peer_t *peer = (peer_t *) param;
    
    peer_create( peer );
    peer_run( peer );
    peer_destroy( peer );
}

void* thread_serial_rx( void *param )
{
    serial_t *serial_handle = (serial_t *) param;

    printf( "Hilo serial_rx creado!\n" );

    while ( Serial_IsAlive( serial_handle ) )
    {
        Serial_Rx( serial_handle );
        usleep( 1000 );
    }
}

void* thread_serial_tx( void *param )
{
    serial_t *serial_handle = (serial_t *) param;

    printf( "Hilo serial_tx creado!\n" );

    while ( Serial_IsAlive( serial_handle ) )
    {
        Serial_Tx( serial_handle );
        usleep( 1000 );
    }

}