#ifndef PEER_H
#define PEER_H

#include <stdint.h>
#include <stdbool.h>
#include <signal.h>
#include "socket.h"
#include "queue.h"

typedef struct 
{
    volatile sig_atomic_t alive_flag;
    volatile sig_atomic_t keep_running;
    socket_t socket;
    int32_t fd;
    queue_t *queue_tx;
    queue_t *queue_rx;
} peer_t;

void peer_create( peer_t *self );

void peer_run( peer_t *self );

void peer_stop( peer_t *self );

void peer_destroy( peer_t *self );

#endif