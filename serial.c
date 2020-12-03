
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "serial.h"
#include "SerialManager.h"

void Serial_Create( serial_t *self, serial_cfg *cfg )
{
    self->keep_running = true;
    self->queue_rx = cfg->queue_rx;
    self->queue_tx = cfg->queue_tx;

    self->buffer_rx = (char *) malloc( 9 * sizeof( char ) );
    self->bytes_received = 0;
    serial_open( cfg->port_number, cfg->baudrate );
}

void Serial_Tx( serial_t *self )
{
    char *msg;

    if( NULL != ( msg = queue_pop( self->queue_tx ) ) )
    {
        serial_send( msg, strlen(msg) );
    }
}

void Serial_Rx( serial_t *self )
{
    int32_t bytes_recv;

    bytes_recv = serial_receive( &self->buffer_rx[self->bytes_received], 9 - self->bytes_received); 
    self->bytes_received += bytes_recv;

    if ( self->bytes_received >= 9 )
    {
        queue_push( self->queue_rx, self->buffer_rx );
        self->buffer_rx = (char *) malloc( 9 * sizeof( char ) );
        self->bytes_received = 0;
    }
}

bool Serial_IsAlive( serial_t *self )
{
    return self->keep_running;
}

void Serial_Stop( serial_t *self )
{
    self->keep_running = false;
}

void Serial_Destroy( serial_t *self )
{
    serial_close();
}