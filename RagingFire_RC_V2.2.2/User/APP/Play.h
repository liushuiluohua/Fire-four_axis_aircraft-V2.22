#ifndef PLAY_H_
#define PLAY_H_
#include "stm32f10x.h"

/******************************************************************************
							全局函数声明
*******************************************************************************/ 
void PutNumber(uint16_t value,uint8_t column,uint8_t row);/*---将value的数值直接写到5110---*/
void Play_Battery(uint16_t value,uint8_t column,uint8_t row,uint8_t turn_count);
void LCD_Play(void);
	
#endif
