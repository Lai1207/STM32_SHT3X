#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Mpu6050.h"
#include "SHT3X.h"



uint16_t Data;
uint16_t Data1;
int main(void)
{
	OLED_Init();
	
	Sht3x_Init();
	
	Data = Sht3x_ReadReg(0xF32D);
	OLED_ShowString(1, 1, "Data:");
	OLED_ShowHexNum(1,6,Data,4);
	
	//Mpu6050_WriteReg(0x6B, 0x00);
	//Mpu6050_WriteReg(0x19, 0xAA);
	
	//uint8_t ID = Mpu6050_ReadReg(0x19);
	
	
	//OLED_ShowHexNum(1, 1, ID, 2);
	

	
	while(1){
		Data1 = Sht3x_ReadReg(0x2032);
		OLED_ShowString(2, 1, "Data1:");
		OLED_ShowHexNum(2,6,Data,4);
//		Mpu6050_GatData(&AX, &AY, &AZ, &GX, &GY, &GZ);
//		OLED_ShowSignedNum(2, 1, AX, 5);
//		OLED_ShowSignedNum(3, 1, AY, 5);
//		OLED_ShowSignedNum(4, 1, AZ, 5);
//		OLED_ShowSignedNum(2, 8, GX, 5);
//		OLED_ShowSignedNum(3, 8, GY, 5);
//		OLED_ShowSignedNum(4, 8, GZ, 5);
	}
}
 
