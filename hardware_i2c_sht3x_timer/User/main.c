#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Mpu6050.h"
#include "SHT3X.h"
#include "SHT3X_Ctrl.h"
#include "SHT3X_Reg.h"
#include "timer.h"



uint16_t Data, Data1, tData, RhData, sht3x_counter;
int main(void)
{
	Timer_Init();
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
//		OLED_ShowNum(2, 1, i, 4);
//		Delay_s(2);
//		Sht3xRead_T_RH(&tData, &RhData);
		OLED_ShowString(2,3," C");
//		OLED_ShowNum(2,1,tData,2);
		OLED_ShowString(3,1,"RH ");
//		OLED_ShowNum(3,4,RhData,2);
		
	}
}
void call_sht3x(){
	sht3x_counter++;
	if(sht3x_counter > 2){
		Sht3xRead_T_RH(&tData, &RhData);
		OLED_ShowNum(2,1,tData,2);
		OLED_ShowNum(3,4,RhData,2);
		sht3x_counter = 0;
	}
}
 
void TIM2_IRQHandler (void){											// 1 second; 
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){
		call_sht3x();
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		
	}
}

