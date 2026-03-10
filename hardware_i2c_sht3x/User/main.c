#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Mpu6050.h"
#include "SHT3X.h"
#include "SHT3X_Ctrl.h"



uint16_t Data, tData, RhData;
int main(void)
{
	OLED_Init();
	Sht3x_Init();
	Delay_s(1);
	
	Data = Sht3x_ReadReg(0xF32D);
	
	Delay_s(1);
	OLED_ShowHexNum(1,1,Data,4);
	

	
	while(1){
		Delay_s(2);
		OLED_Clear();
		Read_T_RH(&tData, &RhData);
		OLED_ShowString(2,1,"c ");
		OLED_ShowNum(2,3,tData,2);
		OLED_ShowString(3,1,"RH ");
		OLED_ShowNum(3,4,RhData,2);
		
	}
}
 
