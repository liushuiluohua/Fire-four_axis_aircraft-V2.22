//***************************************************************************************
//�һ�΢�����������Դ���Ȩ���һ��Ŷ����У�δ���һ��Ŷ�ͬ�⣬�������������ϴ�����Դ�롣
//�뱾�������鼮<<���������DIY-����STM32΢������>>���ɱ�����������ʽ���棬���ݶԱ��װ�
//������������Լ�Ӳ����ض�������ϸ�Ľ��⣬����Ȥ�����ѿ��ԴӸ�����깺��
//�뱾������׵�Ӳ����http://fire-dragon.taobao.com
//������������˸���������ĸĽ�������ʱ�����Ǳ�����ϵ��
//QQ��16053729    �һ�QQȺ��234879071
//***************************************************************************************
#include "struct_all.h"

uint8_t offset=1;//����У��ҡ�˼���ť�е�
int16_t offset0,offset1,offset2,offset3;
int16_t offset4,offset5,offset6;
uint8_t Mode=0,Fun=0;

/******************************************************************************
����ԭ��:	void KEY_Init(void)
��������:	������ʼ��
*******************************************************************************/ 
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;	//MODE����
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;	//FUN����
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
   
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);
    EXTI_InitStructure.EXTI_Line=EXTI_Line1|EXTI_Line9;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//�ⲿ�ж�
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//�½��ش���
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

/******************************************************************************
����ԭ��:	void Do_Offset(void)
��������:	ҡ��У׼
*******************************************************************************/ 
void Do_Offset(void)
{
	if(offset)
	{		
		static uint8_t count=0;
		static int32_t count0,count1,count2,count3;
		static int32_t count4,count5,count6;
		if(count==0)
		{
			offset0 = 0;offset1 = 0;offset2 = 0;offset3 = 0;
			offset4 = 0;offset5 = 0;offset6 = 0;
			count0  = 0;count1  = 0;count2  = 0;count3  = 0;
			count4  = 0;count5  = 0;count6  = 0;
			count = 1;
			return;
		}
		else
		{
			count++;
			count0 += Rc.THROTTLE;
			count1 += Rc.YAW;
			count2 += Rc.PITCH;
			count3 += Rc.ROLL;
			count4 += Rc.AUX1;
			count5 += Rc.AUX2;
			count6 += Rc.AUX3;
		}
		if(count==51)
		{
			count--;
			offset0 = count0 / count - 1500;
			offset1 = count1 / count - 1500;
			offset2 = count2 / count - 1500;
			offset3 = count3 / count - 1500;
			offset4 = count4 / count - 1500;
			offset5 = count5 / count - 1500;
			offset6 = count6 / count - 1500;
			count = 0;
			offset = 0;
			Bee_ON;
		}
		
	}
}
