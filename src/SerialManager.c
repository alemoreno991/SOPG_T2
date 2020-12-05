/**
 * @file SerialManager.c
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
#include "rs232.h"
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
static int portNumber;

/******************************************************************************
 *  Prototypes (declarations) of private functions  
 *****************************************************************************/

/******************************************************************************
 *  Implementations of public functions
 *****************************************************************************/

int serial_open(int pn,int baudrate)
{
	portNumber = pn;
	return OpenComport(portNumber,baudrate);
}


void serial_send(char* pData,int size)
{
	SendBuf(portNumber, (unsigned char*)pData, size);
}

void serial_close(void)
{
	CloseComport(portNumber);
}

int serial_receive(char* buf,int size)
{
	return PollComport(portNumber, (unsigned char*) buf, size);
}

/******************************************************************************
 *  Implementations of private functions
 *****************************************************************************/
