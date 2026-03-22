#ifndef __SERIAL_H
#define __SERIAL_H

#include "stdio.h"

extern uint16_t RxFlag;
extern char RxData_Packect[100];
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);

#endif
