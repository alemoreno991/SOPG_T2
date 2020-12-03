#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>

typedef enum
{
    WR,
    RD,
    RDWR
} channel_config;

typedef struct 
{
    int32_t fd;
} socket_t;

void Socket_create( socket_t *self, int32_t file_descriptor );

bool Socket_send( socket_t *self, const char *msg, int32_t len );

int Socket_recv( socket_t *self, char *recv_buffer, int32_t recv_len );

bool Socket_shutdownchannel( socket_t *self, channel_config selector );

bool Socket_destroy( socket_t *self );


#endif
