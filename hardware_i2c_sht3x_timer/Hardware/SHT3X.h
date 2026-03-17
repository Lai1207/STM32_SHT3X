#include <stdint.h>
#ifndef __SHT3X_H
#define __SHT3X_H


void Sht3x_Init(void);
uint16_t Sht3x_ReadReg(uint16_t Command);
void Sht3x_WriteReg(uint16_t Command);
void Sht3x_Read2Reg(uint16_t Command,uint16_t *tData, uint16_t *hData);



#endif