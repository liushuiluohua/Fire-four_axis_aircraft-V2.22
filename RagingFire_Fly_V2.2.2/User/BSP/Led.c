//***************************************************************************************
//�һ�΢�����������Դ���Ȩ���һ��Ŷ����У�δ���һ��Ŷ�ͬ�⣬�������������ϴ�����Դ�롣
//�뱾�������鼮<<���������DIY-����STM32΢������>>���ɱ�����������ʽ���棬���ݶԱ��װ�
//������������Լ�Ӳ����ض�������ϸ�Ľ��⣬����Ȥ�����ѿ��ԴӸ�����깺��
//�뱾������׵�Ӳ����http://fire-dragon.taobao.com
//������������˸���������ĸĽ�������ʱ�����Ǳ�����ϵ��
//QQ��16053729    �һ�QQȺ��234879071
//***************************************************************************************
#include "struct_all.h"

/******************************************************************************
����ԭ�ͣ�	static void Delay_led(u16 n)
��    �ܣ�	��ʱ
*******************************************************************************/ 
void Delay_led(u16 n)
{	
	uint16_t i,j;
	for(i=0;i<n;i++)
		for(j=0;j<8500;j++);
}

/******************************************************************************
����ԭ�ͣ�	void LED_Init(void)
��    �ܣ�	LED��ʼ��
*******************************************************************************/ 
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Structure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	
	//�ı�ָ���ܽŵ�ӳ�� GPIO_Remap_SWJ_Disable SWJ ��ȫ���ã�JTAG+SW-DP��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	//�ı�ָ���ܽŵ�ӳ�� GPIO_Remap_SWJ_JTAGDisable ��JTAG-DP ���� + SW-DP ʹ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	GPIO_Structure.GPIO_Pin =  LED2_Pin;	//LED2����Դ�Ե�ָʾ��
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(LED2_GPIO, &GPIO_Structure);
	
	GPIO_Structure.GPIO_Pin =  LED3_Pin;	//LED3��������Χ��ҹ���	
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(LED3_GPIO, &GPIO_Structure);
}

/******************************************************************************
����ԭ�ͣ�	void LED_ON_OFF(void)
��    �ܣ�	LED������˸
*******************************************************************************/ 
void LED_ON_OFF(void)
{
	uint8_t i ;
	for(i=0;i<3;i++)
	{
		LED2_ON;LED3_OFF;
		Delay_led(200);
		LED3_ON;LED2_OFF;
		Delay_led(200);	
	}
	for(i=0;i<3;i++)
	{
		LED2_ON;LED3_OFF;
		Delay_led(100);
		LED3_ON;LED2_OFF;
		Delay_led(100);	
	}	
	LED2_OFF;
	LED3_OFF;
	Delay_led(100);
}
/******************************************************************************
����ԭ�ͣ�	void LED3_ON_Frequency(uint8_t Light_Frequency)
��    �ܣ�	��LED3����ĳ��Ƶ����˸����Ҫѭ��ɨ��ú�����
��    ����   Light_Frequency����˸Ƶ�ʣ�HZ��
*******************************************************************************/ 
void LED3_ON_Frequency(uint8_t Light_Frequency)
{
	uint16_t time_count;
	static uint8_t Light_On;
	static uint32_t Last_time,Now_time;
	
	Now_time = Timer3_Count;
	if( Light_On )
	{
		time_count = (uint16_t)(Timer3_Frequency / Light_Frequency / 2);	
		
		if( Now_time - Last_time >= time_count)
		{
			LED3_OFF;
			Light_On=0;
			Last_time = Timer3_Count;
		}
	}
	else
	{
		time_count = (uint16_t)(Timer3_Frequency / Light_Frequency / 2);
		
		if( Now_time - Last_time >= time_count)
		{
			LED3_ON;
			Light_On=1;
			Last_time = Timer3_Count;
		}
	}
}

