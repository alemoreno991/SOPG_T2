/**
 * @file serial.c
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

/******************************************************************************
 *  Inclusions (library's order: standard C, others, user header files)
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "serial.h"
#include "SerialManager.h"

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

/******************************************************************************
 *  Prototypes (declarations) of private functions  
 *****************************************************************************/

/******************************************************************************
 *  Implementations of public functions
 *****************************************************************************/

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

/******************************************************************************
 *  Implementations of private functions
 *****************************************************************************/

