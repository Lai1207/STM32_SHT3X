#include "stm32f10x.h"                  // Device header
#include "SHT3X_REG.h"

uint8_t Sht3x_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT){
    uint32_t Timeout = 10000; 
    
    while(I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS){

        if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF) == SET){
            I2C_ClearFlag(I2Cx, I2C_FLAG_AF); 
            I2C_GenerateSTOP(I2Cx, ENABLE);   
            return 0;
        }
        
        Timeout --;
        if(Timeout == 0){
            I2C_GenerateSTOP(I2Cx, ENABLE);   
            return 0; 
        }
    }
    return 1; 
}

void Sht3x_Init(void){
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 ;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_ClockSpeed = 50000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Ack =  I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit ;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	
	I2C_Init(I2C2, &I2C_InitStructure);

}
void Sht3x_WriteReg(uint16_t Command){
	uint16_t MSB, LSB;
	
	I2C_GenerateSTART(I2C2, ENABLE);
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, SHT3X_ADDRESS, I2C_Direction_Transmitter);
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	MSB = Command >> 8;
	I2C_SendData(I2C2, MSB);
	Sht3x_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	LSB = Command << 8;
	LSB >>= 8;
	I2C_SendData(I2C2, LSB);
	Sht3x_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTOP(I2C2, ENABLE);
}

uint16_t Sht3x_ReadReg(uint16_t Command){
	uint16_t MSB, LSB, MData, LData, Data;
	
	I2C_GenerateSTART(I2C2, ENABLE);
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, SHT3X_ADDRESS, I2C_Direction_Transmitter);
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
		
	
	
	MSB = Command >> 8;
	I2C_SendData(I2C2, MSB);
	Sht3x_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	LSB = Command << 8;
	LSB >>= 8;
	I2C_SendData(I2C2, LSB);
	Sht3x_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C2, ENABLE);
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, SHT3X_ADDRESS, I2C_Direction_Receiver);
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
//	I2C_AcknowledgeConfig(I2C2, DISABLE);
	
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
	MData = I2C_ReceiveData(I2C2);


	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
	LData = I2C_ReceiveData(I2C2);
	
	Data = (MData << 8) | LData;
	
	return Data;
}
