#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint16_t RxFlag;
char RxData_Packect[100];

void Serial_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	//Usart interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//Nvic init
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t Byte){
	USART_SendData(USART1, Byte);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

//uint8_t Serial_GetFlag(){
//	if(RxFlag == 1){
//		RxFlag = 0;
//		return 1;
//	}
//	return 0;
//}

//USART IRQ
void USART1_IRQHandler(void){
	
	static uint8_t RxStat;
	static uint8_t cRxPacket;
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET){
		
		uint8_t RxData = USART_ReceiveData(USART1);
		if(RxStat == 0){
			
			if(RxData == '@' && RxFlag == 0){
				RxStat = 1;
				cRxPacket = 0;
			}

		}else if(RxStat == 1){
			if(RxData == '\r'){
				RxStat = 2;
			}else{
				RxData_Packect[cRxPacket] = RxData;
				cRxPacket ++;
			}
		}else if(RxStat == 2){
			if(RxData == '\n'){
				RxStat = 0;
				RxData_Packect[cRxPacket] = '\0';
				RxFlag = 1;
			}
		}
	}
}
