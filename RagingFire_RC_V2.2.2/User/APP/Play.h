#ifndef PLAY_H_
#define PLAY_H_
#include "stm32f10x.h"

/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
void PutNumber(uint16_t value,uint8_t column,uint8_t row);/*---��value����ֱֵ��д��5110---*/
void Play_Battery(uint16_t value,uint8_t column,uint8_t row,uint8_t turn_count);
void LCD_Play(void);
	
#endif
