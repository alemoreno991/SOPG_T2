/**
 * @file serial.h
 * @author Alejandro M (ale.moreno991@gmail.com)
 * @brief API de la comunicación serial 
 * 
 * @details La comunicación serie esta diseñada para funcionar por polling
 * La manera en que un dato se transmite o se recibe es mediante una cola
 * de transmisión y recepción, respectivamente. Las mencionadas colas son 
 * indicadas por el usuario al crear el objeto. Luego, se debe encargar 
 * de llamar periodicamente a las funciones \p Serial_Tx y \p Serial_Rx 
 * para transmitir lo que esté pendiente y recibir lo que corresponda y encolarlo.
 * 
 * 
 * @version 2020-09-15 --------- Alejandro Moreno -------- v0.0
 * 
 * @copyright Copyright (c) 2020
 * 
 */

/*=====[Avoid multiple inclusion - begin]====================================*/
#ifndef SERIAL_H
#define SERIAL_H

/******************************************************************************
 *  Inclusions of public function dependencies
 *****************************************************************************/
#include <signal.h>
#include <stdint.h>
#include "queue.h"

/******************************************************************************
 *  Definition macros of public constants
 *****************************************************************************/

/******************************************************************************
 *  Public function-like macros
 *****************************************************************************/

/******************************************************************************
 *  Definitions of public data types
 *****************************************************************************/

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


/******************************************************************************
 *  Prototypes (declarations) of public functions
 *****************************************************************************/

/**
 * @brief Constructor del objeto
 * 
 * @param self 
 * @param cfg Configuración inicial
 */
void Serial_Create( serial_t *self, serial_cfg *cfg );

/**
 * @brief Transmite lo que tenga almacenado en la cola de transmición
 * 
 * @param self 
 */
void Serial_Tx( serial_t *self );

/**
 * @brief Recibe lo que haya entrado por el puerto serie y lo almacena en la 
 * cola de recepción
 * 
 * @details Es una función no bloqueante. Por lo que que hay que invocarla regularmente
 * para no perderse ningun mensaje (polling)
 * 
 * @param self 
 */
void Serial_Rx( serial_t *self );

/**
 * @brief Permite saber si la comunicación sigue viva
 * 
 * @param self 
 * @return true 
 * @return false 
 */
bool Serial_IsAlive( serial_t *self );

/**
 * @brief Frena la comunicación
 * 
 * @param self 
 */
void Serial_Stop( serial_t *self );

/**
 * @brief Destructor del objeto
 * 
 * @param self 
 */
void Serial_Destroy( serial_t *self );

/*=====[Avoid multiple inclusion - end]======================================*/
#endif /* SERIAL_H */