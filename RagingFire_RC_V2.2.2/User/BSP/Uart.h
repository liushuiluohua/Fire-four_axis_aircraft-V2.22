#ifndef _UART_H_
#define _UART_H_
#include "stm32f10x.h"

/******************************************************************************
							ȫ�ֱ�������
*******************************************************************************/ 
extern uint8_t RxBuff[2][50];//���ڽ��ջ�����
extern uint8_t Line0,Line1;	 //���ڽ���˫�����л�

/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
void Uart1_Init(uint32_t baud);
void PrintHexU8(uint8_t data);
void PrintHexS16(int16_t num);
void PrintS8( int8_t num);
void PrintU8(uint8_t num);
void PrintS16( int16_t num);
void PrintU16(uint16_t num);
void PrintString(uint8_t *s);
void Uart_Send(uint8_t *s ,uint8_t len);	

#endif
