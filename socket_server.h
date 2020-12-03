#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <stdbool.h>
#include <stdint.h>
#include "socket.h"

typedef struct 
{
    int32_t fd;
    char *port;
} socket_server_t;


bool SocketServer_create(socket_server_t *self, char port[] );

bool SocketServer_bind_listen(socket_server_t *self );

int32_t SocketServer_accept(socket_server_t *self );

bool SocketServer_shutdownchannel(socket_server_t *self, channel_config selector );
    
bool SocketServer_destroy(socket_server_t *self );

#endif
