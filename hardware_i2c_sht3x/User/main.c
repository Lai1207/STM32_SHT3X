#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Mpu6050.h"
#include "SHT3X.h"
#include "SHT3X_Ctrl.h"
#include "SHT3X_Reg.h"



uint16_t Data, Data1, tData, RhData;
int main(void)
{
	OLED_Init();
	Sht3x_Init();
	Delay_s(1);
	Sht3xReset();
	

//	Data = Sht3x_ReadReg(SHT3X_STATUS);
	OLED_ShowHexNum(1,1,Data,4);
	
	Sht3x_WriteReg(SHT3X_05MPS_HIGH);
	
	Delay_s(1);
	Data1 = Sht3x_ReadReg(SHT3X_STATUS);
	OLED_ShowHexNum(1,1,Data1,4);

	
	while(1){
		Delay_s(2);
		Sht3xRead_T_RH(&tData, &RhData);
		OLED_ShowString(2,3," C");
		OLED_ShowNum(2,1,tData,2);
		OLED_ShowString(3,1,"RH ");
		OLED_ShowNum(3,4,RhData,2);
		
	}
}
 
