#ifndef _LED_H_
#define _LED_H_
#include "stm32f10x.h"

/******************************************************************************
							�궨��
*******************************************************************************/ 
#define Bee_GPIO		GPIOB
#define Bee_Pin			GPIO_Pin_10
#define LEDRed_GPIO		GPIOB
#define LEDRed_Pin		GPIO_Pin_12
#define LEDGreen_GPIO	GPIOB
#define LEDGreen_Pin	GPIO_Pin_11

#define Bee_ON 			Bee_GPIO->BSRR    	= Bee_Pin 		//������
#define Bee_OFF  		Bee_GPIO->BRR   	= Bee_Pin
#define LEDRed_ON 		LEDRed_GPIO->BRR    = LEDRed_Pin 	//��LED��
#define LEDRed_OFF  	LEDRed_GPIO->BSRR   = LEDRed_Pin
#define LEDGreen_ON  	LEDGreen_GPIO->BRR  = LEDGreen_Pin	//��LED��
#define LEDGreen_OFF 	LEDGreen_GPIO->BSRR = LEDGreen_Pin		

/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
void LED_Init(void);
void LED_ON_OFF(void);
void LED3_ON_Frequency(uint8_t Light_Frequency);
void LED_Show(void);
	
#endif
