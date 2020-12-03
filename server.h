#ifndef SERVER_H
#define SERVER_H

#include <signal.h>
#include "socket_server.h"
#include "socket.h"
#include "queue.h"

typedef struct 
{
    socket_server_t skt_server;
    volatile sig_atomic_t keep_running;
} server_t;

typedef struct 
{
    queue_t *queue_tx;
    queue_t *queue_rx;
} server_config;

bool Server_Create( server_t *self, char port[] );

bool Server_Run( server_t *self );

bool Server_Stop( server_t *self );

bool Server_Destroy( server_t *self );


#endif