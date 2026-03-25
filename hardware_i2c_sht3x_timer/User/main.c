#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "SHT3X.h"
#include "SHT3X_Ctrl.h"
#include "SHT3X_Reg.h"
#include "timer.h"
#include "Serial.h"
#include <String.h>



uint16_t Data, Data1, tData, RhData, sht3x_counter;
static uint8_t Sht_Mode;
int main(void)
{
	Timer_Init();
	OLED_Init();
	Sht3x_Init();
	Serial_Init();
	
	Delay_s(1);
	Sht3xReset();

	Sht3x_WriteReg(SHT3X_05MPS_HIGH);
	
	Delay_s(1);
	Data1 = Sht3x_ReadReg(SHT3X_STATUS);

	OLED_ShowString(1,1,"mode_1");
	OLED_ShowString(2,3," C");
	OLED_ShowString(3,1,"RH ");
	

	
	while(1){
		if(RxFlag == 1){
			if(strcmp(RxData_Packect, "mode1") == 0){								//Mode1  2 second
				Sht3x_WriteReg(Break_Command);
				Sht3x_WriteReg(SHT3X_05MPS_HIGH);
				Sht_Mode = 0;
				OLED_ShowString(1,1,"mode_1");
				Serial_Printf("mode_1\r\n");
			}else if(strcmp(RxData_Packect, "mode2") == 0){							//Mode2  1 second
				Sht3x_WriteReg(Break_Command);
				Sht3x_WriteReg(SHT3X_1MPS_HIGH);
				Sht_Mode = 1;
				OLED_ShowString(1,1,"mode_2");
				Serial_Printf("mode_2\r\n");
			}else if(strcmp(RxData_Packect, "mode3") == 0){							//Mode3  0.5 second
				Sht3x_WriteReg(Break_Command);
				Sht3x_WriteReg(SHT3X_2MPS_HIGH);
				Sht_Mode = 2;
				OLED_ShowString(1,1,"mode_3");
				Serial_Printf("mode_3\r\n");
			}
			else{
				OLED_ShowString(1,1,RxData_Packect);
			}
			RxFlag = 0;
		}
	}
}
void call_sht3x(){
	sht3x_counter++;
	switch(Sht_Mode){
		case 0:																		//Mode1
			if(sht3x_counter > 3){									
				Sht3xRead_T_RH(&tData, &RhData);
				OLED_ShowNum(2,1,tData,2);
				OLED_ShowNum(3,4,RhData,2);
				Serial_Printf("c : %d RH: %d \r\n",tData,RhData);
				sht3x_counter = 0;
			}
		break;
		
		case 1:																		//Mode2
			if(sht3x_counter > 1){
				Sht3xRead_T_RH(&tData, &RhData);
				OLED_ShowNum(2,1,tData,2);
				OLED_ShowNum(3,4,RhData,2);
				Serial_Printf("c : %d RH: %d \r\n",tData,RhData);
				sht3x_counter = 0;
			}
		break;
		
		case 2:																		//Mode3
			if(sht3x_counter > 0){
				Sht3xRead_T_RH(&tData, &RhData);
				OLED_ShowNum(2,1,tData,2);
				OLED_ShowNum(3,4,RhData,2);
				Serial_Printf("c : %d RH: %d \r\n",tData,RhData);
				sht3x_counter = 0;
			}
		break;
		
	}
}
 
void TIM2_IRQHandler (void){											// 0.5 second 
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){
		call_sht3x();
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

