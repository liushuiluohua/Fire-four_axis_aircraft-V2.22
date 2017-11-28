#ifndef _STRUCT_ALL_H_
#define _STRUCT_ALL_H_
#include "stm32f10x.h"

#include "Key.h"
#include "Led.h"
#include "Uart.h" 
#include "Maths.h" 
#include "Timer3.h"
#include "DMA_ADC.h"
#include "Protocol.h"
#include "SPI_NRF24L01.h"

/******************************************************************************
							�궨��
*******************************************************************************/ 
#define American_Mode//ע�͵��ú��ʾʹ���ձ��֣�����Ĭ��ʹ��������
#define Lock_Mode (1<<0)//��β
#define Led_Mode  (1<<1)//ҹ��ģʽ

/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
void BSP_Int(void);
void EEPROM_INIT(void);
void EEPROM_SAVE_OFFSET(void);
void EEPROM_READ_OFFSET(void);
	
/******************************************************************************
							�ṹ������
*******************************************************************************/ 
/* pid���� */
struct _pid
{
	uint8_t kp;	//0.1
	uint8_t ki;	//0.001
	uint8_t kd;	//1
};
extern struct _pid pid[9];

/* ң��ͨ�� */
struct _Rc
{
	uint16_t THROTTLE;
	uint16_t YAW;
	uint16_t PITCH;
	uint16_t ROLL;
	
	uint16_t AUX1;
	uint16_t AUX2;
	uint16_t AUX3;
	uint16_t AUX4;
};
extern struct _Rc Rc;
extern struct _Rc Filter_Rc;

#endif

