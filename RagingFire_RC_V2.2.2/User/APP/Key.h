#ifndef _KEY_H_
#define _KEY_H_
#include "stm32f10x.h"

/******************************************************************************
							全局变量声明
*******************************************************************************/ 
extern uint8_t offset;
extern int16_t offset0,offset1,offset2,offset3;
extern int16_t offset4,offset5,offset6;
extern uint8_t Mode,Fun;

/******************************************************************************
							全局函数声明
*******************************************************************************/ 
void KEY_Init(void);
void Do_Offset(void);

#endif
