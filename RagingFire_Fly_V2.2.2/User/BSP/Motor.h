#ifndef _Motor_H_
#define _Motor_H_
#include "stm32f10x.h"

/******************************************************************************
							全局函数声明
*******************************************************************************/ 
void Motor_Init(void);
void Motor_Out(int16_t duty1,int16_t duty2,int16_t duty3,int16_t duty4);

#endif
