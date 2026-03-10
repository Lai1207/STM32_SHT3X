#include "stm32f10x.h"                  // Device header
#include "SHT3X_REG.h"

uint8_t Sht3x_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT){
  uint32_t Timeout;
	Timeout = 1000000;
	while(I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS){
		
		Timeout --;
		
		if(Timeout == 0){
			break;
		}
	}
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
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);									//Ev5
	
	I2C_Send7bitAddress(I2C2, SHT3X_ADDRESS, I2C_Direction_Transmitter);
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);		//Ev6
	
	MSB = Command >> 8;
	I2C_SendData(I2C2, MSB);
	Sht3x_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING);							//Ev8
	
	LSB = Command << 8;
	LSB >>= 8;
	I2C_SendData(I2C2, LSB);
	Sht3x_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED);							//Ev8_2
	
	I2C_GenerateSTOP(I2C2, ENABLE);
}

uint16_t Sht3x_ReadReg(uint16_t Command){
	uint16_t MSB, LSB, MData, LData, Data;
	
	I2C_GenerateSTART(I2C2, ENABLE);
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);										//Ev5
	
	I2C_Send7bitAddress(I2C2, SHT3X_ADDRESS, I2C_Direction_Transmitter);
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);			//Ev6
	
	MSB = Command >> 8;
	I2C_SendData(I2C2, MSB);
	Sht3x_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING); 							//Ev8
	
	LSB = Command << 8;
	LSB >>= 8;
	I2C_SendData(I2C2, LSB);
	Sht3x_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED);								//Ev8_2
	
	I2C_GenerateSTART(I2C2, ENABLE);
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);										//Ev5
	
	I2C_Send7bitAddress(I2C2, SHT3X_ADDRESS, I2C_Direction_Receiver);
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);					//Ev6
	
	
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);									//Ev7
	MData = I2C_ReceiveData(I2C2);

	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);									//Ev7
	LData = I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	Data = (MData << 8) | LData;
	
	return Data;
}
void Sht3x_Read2Reg(uint16_t Command, uint16_t *tData, uint16_t *hData){
	uint16_t MSB, LSB, MData, LData, Data;
	
	I2C_GenerateSTART(I2C2, ENABLE);
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);										//Ev5
	
	I2C_Send7bitAddress(I2C2, SHT3X_ADDRESS, I2C_Direction_Transmitter);
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);			//Ev6
	
	MSB = Command >> 8;
	I2C_SendData(I2C2, MSB);
	Sht3x_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING); 							//Ev8
	
	LSB = Command << 8;
	LSB >>= 8;
	I2C_SendData(I2C2, LSB);
	Sht3x_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED);								//Ev8_2
	
	I2C_GenerateSTART(I2C2, ENABLE);
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);										//Ev5
	
	I2C_Send7bitAddress(I2C2, SHT3X_ADDRESS, I2C_Direction_Receiver);
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);					//Ev6
	
	
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);									//Ev7
	MData = I2C_ReceiveData(I2C2);
	
	
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);									//Ev7
	LData = I2C_ReceiveData(I2C2);
	
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);									//Ev7 CRC
	I2C_ReceiveData(I2C2);
	
	*tData = (MData << 8) | LData;
	
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);									//Ev7
	MData = I2C_ReceiveData(I2C2);
	
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);									//Ev7
	LData = I2C_ReceiveData(I2C2);

	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	Sht3x_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);									//Ev7 CRC
	I2C_ReceiveData(I2C2);
	
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	*hData = (MData << 8) | LData;
	
	
	
}
