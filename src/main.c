/**
 * @file main.c
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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>
#include <pthread.h>
#include <signal.h>
#include "server.h"

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
static sig_atomic_t keep_running = true;

/******************************************************************************
 *  Prototypes (declarations) of private functions  
 *****************************************************************************/

void* server_execute( void *param );

void desbloquearSign(void);

void bloquearSign(void);

/******************************************************************************
 *  Prototypes (declarations) of interrupt functions  
 *****************************************************************************/

void sig_handler(int sig);

/******************************************************************************
 *  Implementations of public functions
 *****************************************************************************/

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

/******************************************************************************
 *  Implementations of private functions
 *****************************************************************************/

void* server_execute( void *param )
{
	server_t *server_handle = (server_t *) param;
	Server_Create( server_handle, "10000" );
	Server_Run( server_handle );
	Server_Destroy( server_handle );

	return NULL;
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

/******************************************************************************
 *  Implementations of Interrupt functions
 *****************************************************************************/

void sig_handler(int sig)
{
	keep_running = false;
}