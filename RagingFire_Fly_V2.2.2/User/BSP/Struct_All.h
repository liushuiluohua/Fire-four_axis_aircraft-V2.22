#ifndef _STRUCT_ALL_H_
#define _STRUCT_ALL_H_
#include "stm32f10x.h"

#include "Led.h"
#include "Uart.h" 
#include "Motor.h"
#include "Timer3.h"
#include "DMA_ADC.h"
#include "Protocol.h"
#include "I2C_MPU6050.h"
#include "SPI_NRF24L01.h"

/******************************************************************************
							�궨��
*******************************************************************************/
#define Lock_Mode (1<<0)//��β
#define Led_Mode  (1<<1)//ҹ��ģʽ

/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
void Nvic_Init(void);

void EEPROM_INIT(void);
void EEPROM_SAVE_ACC_OFFSET(void);
void EEPROM_READ_ACC_OFFSET(void);
void EEPROM_SAVE_GYRO_OFFSET(void);
void EEPROM_READ_GYRO_OFFSET(void);
void EEPROM_SAVE_PID(void);
void EEPROM_READ_PID(void);
void PID_Reset(void);
/******************************************************************************
							�ṹ������
*******************************************************************************/ 
/* MPU6050--���ٶȼƽṹ�� */
struct _acc
{
	int16_t x;
	int16_t y;
	int16_t z;
};
extern struct _acc acc;
extern struct _acc filter_acc;
extern struct _acc offset_acc;

/* MPU6050--�����ǽṹ�� */
struct _gyro
{
	int16_t x;
	int16_t y;
	int16_t z;
};
extern struct _gyro gyro;
extern struct _gyro filter_gyro;
extern struct _gyro offset_gyro;

/* float�ṹ�� */
struct _SI_float
{
	float x;
	float y;
	float z;
};
extern struct _SI_float SI_acc;	
extern struct _SI_float SI_gyro;

/* ��̬����--�Ƕ�ֵ */
struct _out_angle
{
	float yaw;
	float roll;
	float pitch;
};
extern struct _out_angle out_angle;

/* ң������ */
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

/* pid���� */
struct _pid
{
	float kp;
	float ki;
	float kd;
	float integral;
	
	float output;
};
extern struct _pid pitch;
extern struct _pid roll;
extern struct _pid gyro_pitch;
extern struct _pid gyro_roll;
extern struct _pid gyro_yaw;

#endif

