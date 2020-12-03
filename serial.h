#ifndef SERIAL_H
#define SERIAL_H

#include <signal.h>
#include <stdint.h>
#include "queue.h"

typedef struct 
{
    volatile sig_atomic_t keep_running;     
    
    queue_t *queue_tx;

    queue_t *queue_rx;
    int32_t bytes_received;
    char *buffer_rx;
} serial_t;

typedef struct 
{
    int32_t port_number;
    int32_t baudrate;
    
    queue_t *queue_tx;
    queue_t *queue_rx;
} serial_cfg;

void Serial_Create( serial_t *self, serial_cfg *cfg );

void Serial_Tx( serial_t *self );

void Serial_Rx( serial_t *self );

bool Serial_IsAlive( serial_t *self );

void Serial_Stop( serial_t *self );

void Serial_Destroy( serial_t *self );

#endif