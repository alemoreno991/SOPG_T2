#ifndef SERIAL_MANAGER_H
#define SERIAL_MANAGER_H

int serial_open(int pn,int baudrate);
void serial_send(char* pData,int size);
int serial_receive(char* buf,int size);
void serial_close(void);

#endif