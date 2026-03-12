#include "stm32f10x.h"                  // Device header
#include "SHT3X.h"
#include "SHT3X_Reg.h"
#include <math.h>

void Sht3xRead_T_RH(uint16_t *tData, uint16_t *RhData){
	uint16_t t16, rh16;
	Sht3x_Read2Reg(SHT3X_RESULTS, &t16, &rh16);
	*tData = -45 + 175 * t16 / (pow(2,16) - 1); 
	*RhData  = 100 * rh16 /  (pow(2,16) - 1);
}
void Sht3xReset(){
	Sht3x_WriteReg(SHT3X_RESET);
}

