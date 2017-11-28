//***************************************************************************************
//�һ�΢�����������Դ���Ȩ���һ��Ŷ����У�δ���һ��Ŷ�ͬ�⣬�������������ϴ�����Դ�롣
//�뱾�������鼮<<���������DIY-����STM32΢������>>���ɱ�����������ʽ���棬���ݶԱ��װ�
//������������Լ�Ӳ����ض�������ϸ�Ľ��⣬����Ȥ�����ѿ��ԴӸ�����깺��
//�뱾������׵�Ӳ����http://fire-dragon.taobao.com
//������������˸���������ĸĽ�������ʱ�����Ǳ�����ϵ��
//QQ��16053729    �һ�QQȺ��234879071
//***************************************************************************************
#include "struct_all.h"

uint32_t Timer3_Count = 0;//��¼Timer3�жϴ���
uint16_t Timer3_Frequency;//Timer3�ж�Ƶ��

/******************************************************************************
����ԭ�ͣ�	void Timer3_Init(uint16_t Handler_Frequency)
��    �ܣ�	��ʼ����ʱ��3
��    ����   Handler_FrequencyΪTimer3�ж�Ƶ��
*******************************************************************************/ 
void Timer3_Init(uint16_t Handler_Frequency)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	Timer3_Frequency = Handler_Frequency;
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = 1000*1000/Handler_Frequency ;//װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;	//��Ƶϵ��
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //���ָ�ʱ��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);//����жϱ�־
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3,ENABLE);//ʹ�ܶ�ʱ��3
}
