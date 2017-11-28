//***************************************************************************************
//�һ�΢�����������Դ���Ȩ���һ��Ŷ����У�δ���һ��Ŷ�ͬ�⣬�������������ϴ�����Դ�롣
//�뱾�������鼮<<���������DIY-����STM32΢������>>���ɱ�����������ʽ���棬���ݶԱ��װ�
//������������Լ�Ӳ����ض�������ϸ�Ľ��⣬����Ȥ�����ѿ��ԴӸ�����깺��
//�뱾������׵�Ӳ����http://fire-dragon.taobao.com
//������������˸���������ĸĽ�������ʱ�����Ǳ�����ϵ��
//QQ��16053729    �һ�QQȺ��234879071
//***************************************************************************************
#include "struct_all.h"

uint8_t TxCount=0;
uint8_t Count=0;

static uint8_t TxBuff[256];	//���ڷ��ͻ�����
volatile uint8_t RxBuffer[50];//���ڽ��ջ�����

/******************************************************************************
����ԭ�ͣ�	void Uart1_Init(uint32_t baud)
��    �ܣ�	���ڳ�ʼ��
��    ����  baud��������
*******************************************************************************/ 
void Uart1_Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/* ��GPIO��USART������ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/* ��USART Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* ��USART Rx��GPIO����Ϊ��������ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* ����USART���� */
	USART_InitStructure.USART_BaudRate = baud;//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//��λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һλֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//��������żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//����+����ģʽ
	USART_Init(USART1, &USART_InitStructure);
	/* ʹ�ܴ��ڽ����ж� */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	/* ��5����ʹ�� USART�� ������� */
	USART_Cmd(USART1, ENABLE);
	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART1, USART_FLAG_TC);//�巢����ɱ�־λ
}

/******************************************************************************
����ԭ�ͣ�	void USART1_IRQHandler(void)
��    �ܣ�	�����ж�
*******************************************************************************/ 
void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)//ORE�ж�
	{
		USART_ReceiveData(USART1);
	}
	
	if(USART1->SR & USART_SR_TC)//�����ж�
	{
		USART1->DR = TxBuff[TxCount++];//дDR����жϱ�־          
		if(TxCount == Count)
		{
			USART1->CR1 &= ~USART_CR1_TXEIE;//�ر�TXE�ж�
		}
	}
	
	if(USART1->SR & USART_SR_RXNE)//�����ж� 
	{
		volatile int8_t com_data ;
		com_data = USART1->DR;
	}
}

//��HEX����ʽ���U8������
void PrintHexU8(uint8_t data)
{
	TxBuff[Count++] = data;  
	if(!(USART1->CR1 & USART_CR1_TXEIE))
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //��TXE�ж�
}
//��HEX����ʽ���S16������
void PrintHexS16(int16_t num)
{
	PrintHexU8((uint8_t)(num & 0xff00) >> 8);//�ȷ��͸�8λ���ٷ��͵�8λ
	PrintHexU8((uint8_t)(num & 0x00ff));
}
//���ַ�����ʽ���S8������
void PrintS8(int8_t num)
{
	uint8_t  bai,shi,ge;
	if(num<0)
	{
		PrintHexU8('-');
		num=-num;
	}
	else 
		PrintHexU8(' ');	
	bai=num/100;
	shi=num%100/10;
	ge =num%10;
	PrintHexU8('0'+bai);
	PrintHexU8('0'+shi);
	PrintHexU8('0'+ge);
}
//���ַ�����ʽ���U8������
void PrintU8(uint8_t num)
{
	uint8_t  bai,shi,ge;
	bai=num/100;
	shi=num%100/10;
	ge =num%10;
	PrintHexU8('0'+bai);
	PrintHexU8('0'+shi);
	PrintHexU8('0'+ge);
}
//���ַ�����ʽ���S16������ 
void PrintS16(int16_t num)
{	
	uint8_t w5,w4,w3,w2,w1;
	if(num<0)
	{
		PrintHexU8('-');
		num=-num;
	}
	else 
		PrintHexU8(' ');
	
	w5=num%100000/10000;
	w4=num%10000/1000;
	w3=num%1000/100;
	w2=num%100/10;
	w1=num%10;
	PrintHexU8('0'+w5);
	PrintHexU8('0'+w4);
	PrintHexU8('0'+w3);
	PrintHexU8('0'+w2);
	PrintHexU8('0'+w1);
}
//���ַ�����ʽ���U16������
void PrintU16(uint16_t num)
{	
	uint8_t w5,w4,w3,w2,w1;
	
	w5=num%100000/10000;
	w4=num%10000/1000;
	w3=num%1000/100;
	w2=num%100/10;
	w1=num%10;
	PrintHexU8(' ');
	PrintHexU8('0'+w5);
	PrintHexU8('0'+w4);
	PrintHexU8('0'+w3);
	PrintHexU8('0'+w2);
	PrintHexU8('0'+w1);
}
//����ַ���
void PrintString(uint8_t *s)
{
	uint8_t *p;
	p=s;
	while(*p != '\0')
	{
		PrintHexU8(*p);
		p++;
	}
}
