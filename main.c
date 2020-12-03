#include <unistd.h>
#define _POSIX_C_SOURCE 200112L	/* Esto lo pongo acá porque <unistd.h> usa otra fuente de posix para trabajar con usleep() */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>
#include <pthread.h>
#include <signal.h>
#include "server.h"

static sig_atomic_t keep_running = true;

void* server_execute( void *param );

void desbloquearSign(void);
void bloquearSign(void);

void sig_handler(int sig);

int main(void)
{
	server_t server;
	pthread_t thread_server;
	
	// signal ctrl+c
	struct sigaction sa;
	sa.sa_handler = sig_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1 ||
		sigaction(SIGTERM, &sa, NULL) == -1  ) 
	{
		perror("sigaction");
		exit(1);
	}
	

	/* Bloqueo las señales para que los hilos que cree las tengan enmascaradas */
	bloquearSign();
	pthread_create( &thread_server, NULL, server_execute, (void *) &server );

	/* Desbloqueo las señales solamente para este hilo (hilo principal)*/
	desbloquearSign();

	while ( true == keep_running )
	{
		usleep(2000);
	}

	printf("Frenar server\n");
	Server_Stop( &server );

	pthread_join( thread_server, NULL );

	return 0;
}

void* server_execute( void *param )
{
	server_t *server_handle = (server_t *) param;
	Server_Create( server_handle, "10000" );
	Server_Run( server_handle );
	Server_Destroy( server_handle );
}

void bloquearSign(void)
{
    sigset_t set;
    
	sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &set, NULL);
}

void desbloquearSign(void)
{
    sigset_t set;
    
	sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGTERM);
    pthread_sigmask(SIG_UNBLOCK, &set, NULL);
}

void sig_handler(int sig)
{
	keep_running = false;
}