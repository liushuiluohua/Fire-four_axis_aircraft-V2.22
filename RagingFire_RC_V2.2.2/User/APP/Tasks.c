//***************************************************************************************
//�һ�΢�����������Դ���Ȩ���һ��Ŷ����У�δ���һ��Ŷ�ͬ�⣬�������������ϴ�����Դ�롣
//�뱾�������鼮<<���������DIY-����STM32΢������>>���ɱ�����������ʽ���棬���ݶԱ��װ�
//������������Լ�Ӳ����ض�������ϸ�Ľ��⣬����Ȥ�����ѿ��ԴӸ�����깺��
//�뱾������׵�Ӳ����http://fire-dragon.taobao.com
//������������˸���������ĸĽ�������ʱ�����Ǳ�����ϵ��
//QQ��16053729    �һ�QQȺ��234879071
//***************************************************************************************
#include "Tasks.h"
#include "Filter.h"

/******************************************************************************
����ԭ��:	static void Nvic_Init(void)
��������:	NVIC��ʼ��
*******************************************************************************/ 
static void Nvic_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//NVIC_PriorityGroup 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ�����
    //Timer3
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	//Nrf2401�ж�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//MODE����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//FUN����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************
����ԭ�ͣ�	void BSP_Int(void)
��    �ܣ�	Ӳ��������ʼ��
*******************************************************************************/ 
void BSP_Int(void)
{
	LED_Init();//LED��ʼ��
	LED_ON_OFF();//LED��˸
	
	KEY_Init();//������ʼ��
	Uart1_Init(115200);	//���ڳ�ʼ����������115200��8λ���ݣ�1λֹͣλ��������żУ��
	Timer3_Init(500);//Timer3�ж�500HZ
	Nvic_Init();//�ж����ȼ���ʼ��	
	
	PrintString("\r\n RagingFire_RC V2.2.2 \r\n");//�汾��
	ADC1_Init();//ADC��DMA��ʼ��

	SPI2_Init();//SPI2��ʼ��
	NRF24L01_Init(40,TX);//2401ѡ��40ͨ��������ģʽ
 	NRF24L01_Check();//���2401�Ƿ�����
	
	Bsp_Int_Ok = 1;
}

/******************************************************************************
����ԭ�ͣ�	void Task_500HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ500HZ����
*******************************************************************************/ 
void Task_500HZ(void)
{
	Nrf_Connect();//NRF24L01���Ӻ���
	Rc_Filter(&Rc,&Filter_Rc);//����ң��ָ��+�˲�
}

/******************************************************************************
����ԭ�ͣ�	void Task_100HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ100HZ����
*******************************************************************************/ 
void Task_100HZ(void)
{
	Print_MSP_RC('<');//����ң���źŸ��ɿ�
}

/******************************************************************************
����ԭ�ͣ�	void Task_25HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ25HZ����
*******************************************************************************/ 
void Task_25HZ(void)
{
	Print_MSP_RC('>');//����ң���źŸ���λ��
	LED_Show();//LEDָʾ����˸
	Print_MSP_MOTOR();//���͵��ת�ٸ���λ��
}

/******************************************************************************
����ԭ�ͣ�	void Task_4HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ4HZ����
*******************************************************************************/ 
void Task_4HZ(void)
{
	static uint8_t turn_count;
	
	turn_count++;
	switch(turn_count)
	{
		case 1: Print_MSP_ANALOG();		break;
		case 2: Print_MSP_IDENT();		break;
		case 3: Print_MSP_ANALOG();		break;				
		case 4: Print_MSP_MOTOR_PINS();	
				turn_count=0; 			break;
	}
}
