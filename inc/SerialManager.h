/**
 * @file SerialManager.h
 * @author Alejandro M (ale.moreno991@gmail.com)
 * @brief Descripción breve 
 * 
 * @version 2020-09-15 --------- Alejandro Moreno -------- v0.0
 * 
 * @copyright Copyright (c) 2020
 * 
 */

/*=====[Avoid multiple inclusion - begin]====================================*/
#ifndef SERIAL_MANAGER_H
#define SERIAL_MANAGER_H

/******************************************************************************
 *  Inclusions of public function dependencies
 *****************************************************************************/

/******************************************************************************
 *  Definition macros of public constants
 *****************************************************************************/

/******************************************************************************
 *  Public function-like macros
 *****************************************************************************/

/******************************************************************************
 *  Definitions of public data types
 *****************************************************************************/

/******************************************************************************
 *  Prototypes (declarations) of public functions
 *****************************************************************************/

/**
 * @brief Abre una comunicación seria en el puerto ( \p pn ) indicado y 
 * con el \p baudrate indicado.
 * 
 * @param pn 
 *      - 0: /ttyUSB0
 *      - 1: /ttyUSB1
 * @param baudrate Baudrate de la comunicación
 *      - 115200 (lo más común)
 * @return int 
 */
int serial_open(int pn,int baudrate);

/**
 * @brief Envía un mensaje por el canal
 * 
 * @param pData Mensaje a enviar
 * @param size  Tamaño del mensaje
 */
void serial_send(char* pData,int size);

/**
 * @brief Recibe un mensaje desde el canal
 * 
 * @param pData Buffer donde se recibe el mensaje
 * @param size  Tamaño del buffer
 */
int serial_receive(char* buf,int size);

/**
 * @brief Cierra la comunicación
 * 
 */
void serial_close(void);

/*=====[Avoid multiple inclusion - end]======================================*/
#endif /* SERIAL_MANAGER_H */