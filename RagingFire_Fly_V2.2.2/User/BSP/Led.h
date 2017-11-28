#ifndef _LED_H_
#define _LED_H_
#include "stm32f10x.h"

/******************************************************************************
							宏定义
*******************************************************************************/ 
#define LED2_GPIO	GPIOB
#define LED2_Pin	GPIO_Pin_3
#define LED3_GPIO	GPIOC
#define LED3_Pin	GPIO_Pin_13

#define LED2_ON 	LED2_GPIO->BRR  = LED2_Pin //电源旁的指示灯
#define LED2_OFF  	LED2_GPIO->BSRR = LED2_Pin
#define LED3_ON  	LED3_GPIO->BSRR = LED3_Pin//机身周围的夜间灯	
#define LED3_OFF 	LED3_GPIO->BRR  = LED3_Pin		

/******************************************************************************
							全局函数声明
*******************************************************************************/ 
void LED_Init(void);
void Delay_led(u16 n);
void LED_ON_OFF(void);
void LED3_ON_Frequency(uint8_t Light_Frequency);
	
#endif
