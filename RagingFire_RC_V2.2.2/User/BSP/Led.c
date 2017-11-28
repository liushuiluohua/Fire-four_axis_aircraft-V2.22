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
����ԭ��:	static void Delay_led(uint16_t n)
��������:	��ʱ
*******************************************************************************/ 
static void Delay_led(uint16_t n)
{	
	uint16_t i,j;
	for(i=0;i<n;i++)
		for(j=0;j<8888;j++);
} 

/******************************************************************************
����ԭ��:	void LED_Init(void)
��������:	LED��ʼ��
*******************************************************************************/ 
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Structure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_Structure.GPIO_Pin =  Bee_Pin;	//������
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(Bee_GPIO, &GPIO_Structure);
	
	GPIO_Structure.GPIO_Pin =  LEDRed_Pin;	//LED Red
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(LEDRed_GPIO, &GPIO_Structure);
	
	GPIO_Structure.GPIO_Pin =  LEDGreen_Pin;//LED Green
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(LEDGreen_GPIO, &GPIO_Structure);
}

/******************************************************************************
����ԭ��:	void LED_ON_OFF(void)
��������:	LED������˸
*******************************************************************************/ 
void LED_ON_OFF(void)
{
	uint8_t i ;
	for(i=0;i<3;i++)
	{
		LEDRed_ON;LEDGreen_OFF;Bee_ON;
		Delay_led(100);
		LEDGreen_ON;LEDRed_OFF;Bee_OFF;
		Delay_led(100);	
	}	
	Bee_OFF;
	LEDRed_OFF;
	LEDGreen_OFF;
	Delay_led(100);
}

/******************************************************************************
����ԭ�ͣ�	void LEDRed_ON_Frequency(uint8_t Light_Frequency)
��    �ܣ�	��LED����ĳ��Ƶ����˸����Ҫѭ��ɨ��ú�����
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
		time_count = (uint16_t)(Timer3_Frequency / Light_Frequency / 2 );	
		
		if( Now_time - Last_time >= time_count)
		{
			LEDRed_OFF;
			Light_On=0;
			Last_time = Timer3_Count;
		}
	}
	else
	{
		time_count = (uint16_t)(Timer3_Frequency / Light_Frequency / 2 );
		
		if( Now_time - Last_time >= time_count)
		{
			LEDRed_ON;
			Light_On=1;
			Last_time = Timer3_Count;
		}
	}
}

/******************************************************************************
����ԭ�ͣ�	void LED_Show(void)
��    �ܣ�	LEDָʾ����˸
*******************************************************************************/ 
void LED_Show(void)
{
	if( (Battery_Fly>200 && Battery_Fly<330) || (Battery_Rc>200 && Battery_Rc<360) )//��ѹ����
	{
		Bee_ON;
		LED3_ON_Frequency(10);//��ɫLED10HZ��˸
	}
	else if(Mode)//��βģʽ
	{
		Bee_OFF;
		LED3_ON_Frequency(4);//��ɫLED4HZ��˸
	}
	else//����βģʽ
	{
		Bee_OFF;
		LED3_ON_Frequency(1);//��ɫLED1HZ��˸
	}
}
