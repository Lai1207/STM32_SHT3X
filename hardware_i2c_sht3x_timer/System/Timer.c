#include "stm32f10x.h"                  // Device header

void Timer_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);								//tim時鐘開啟
	
	TIM_InternalClockConfig(TIM2);																		//使用內部時鐘(預設)
	
	TIM_TimeBaseInitTypeDef TIM_TimerBaseStructure;
	TIM_TimerBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;					//時鐘分類 數位濾波器採樣速率
	TIM_TimerBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;			//計數模式
	TIM_TimerBaseStructure.TIM_Period = 10000 - 1;										//ARR
	TIM_TimerBaseStructure.TIM_Prescaler = 3600 - 1;									//PSC
	TIM_TimerBaseStructure.TIM_RepetitionCounter = 0;									//重複計數器 這裡是使用通用所以不用
	
	TIM_TimeBaseInit(TIM2, &TIM_TimerBaseStructure);									//初始化時基單元
	
	TIM_ClearFlag(TIM2,TIM_IT_Update);																//手動清除中斷旗標
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);												//中斷控制
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);																								//啟用定時器
}

//void TIM2_IRQHandler (void){
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){
//		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//	}
//}
