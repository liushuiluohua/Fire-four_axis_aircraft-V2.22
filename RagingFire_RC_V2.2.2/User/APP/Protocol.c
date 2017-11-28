//***************************************************************************************
//�һ�΢�����������Դ���Ȩ���һ��Ŷ����У�δ���һ��Ŷ�ͬ�⣬�������������ϴ�����Դ�롣
//�뱾�������鼮<<���������DIY-����STM32΢������>>���ɱ�����������ʽ���棬���ݶԱ��װ�
//������������Լ�Ӳ����ض�������ϸ�Ľ��⣬����Ȥ�����ѿ��ԴӸ�����깺��
//�뱾������׵�Ӳ����http://fire-dragon.taobao.com
//������������˸���������ĸĽ�������ʱ�����Ǳ�����ϵ��
//QQ��16053729    �һ�QQȺ��234879071
//***************************************************************************************
#include "struct_all.h"

/******************************************************************************
							�궨��
*******************************************************************************/ 
#define VERSION 222	//�汾��Ϣ
#define QUADX   3	

#define MSP_VERSION 0
#define CAP_PLATFORM_32BIT          ((uint32_t)1 << 31)
#define CAP_BASEFLIGHT_CONFIG       ((uint32_t)1 << 30)
#define CAP_DYNBALANCE              ((uint32_t)1 << 2)
#define CAP_FW_FLAPS                ((uint32_t)1 << 3)

uint16_t THROTTLE1,THROTTLE2,THROTTLE3,THROTTLE4;
/******************************************************************************
����ԭ��:	void Print_MSP_RC(uint8_t direction)
��������:	����ң������
��    ����   direction��'>';������λ����'<';�����ɿ�
*******************************************************************************/ 
void Print_MSP_RC(uint8_t direction)
{
	uint8_t	data[32];
	uint8_t count;
	
	data[0] = '$';
	data[1] = 'M';
	data[2] = direction;//���ݷ�������
	data[3] = 16;
	data[4] = MSP_RC;
	
	data[5] =  Filter_Rc.ROLL & 0xFF ;	
	data[6] = (Filter_Rc.ROLL >> 8)& 0xFF;
	data[7] =  Filter_Rc.PITCH & 0xFF ;	
	data[8] = (Filter_Rc.PITCH >> 8)& 0xFF;
	data[9] =  Filter_Rc.YAW & 0xFF ;	
	data[10] =(Filter_Rc.YAW >> 8)& 0xFF;
	data[11] = Filter_Rc.THROTTLE & 0xFF ;	
	data[12] =(Filter_Rc.THROTTLE >> 8)& 0xFF;
	
	data[13] =  Rc.AUX1 & 0xFF ;	
	data[14] = (Rc.AUX1 >> 8)& 0xFF;
	data[15] =  Rc.AUX2 & 0xFF ;	
	data[16] = (Rc.AUX2 >> 8)& 0xFF;
	data[17] =  Rc.AUX3 & 0xFF ;	
	data[18] = (Rc.AUX3 >> 8)& 0xFF;
	if(direction == '<')//��������
	{
		Rc.AUX4 = 0;
		if(Mode)//Mode����
			Rc.AUX4 |= 	Lock_Mode;//��βģʽ
		if(Fun)//Fun����
			Rc.AUX4 |=  Led_Mode;//ҹ��ģʽ
		
		data[19] =  Rc.AUX4 & 0xFF ;	
		data[20] = (Rc.AUX4 >> 8)& 0xFF;
		data[21] = Get_Checksum(data);
		NRF_Send_TX(data,32); 
	}
	else if(direction == '>')//������λ��
	{
		Rc.AUX4    = 1000+ Battery_Rc;
		
		data[19] =  Rc.AUX4 & 0xFF ;	
		data[20] = (Rc.AUX4 >> 8)& 0xFF;
		data[21] = Get_Checksum(data);
		for(count=0;count<22;count++)
		{
			PrintHexU8(data[count]);
		}
	}
}

/******************************************************************************
����ԭ��:	void Print_MSP_PID(void)
��������:	����PID����
*******************************************************************************/ 
void Print_MSP_PID(void)
{
	uint8_t	data[40];
	uint8_t count;

	data[0] = '$';
	data[1] = 'M';
	data[2] = '>';//������λ��
	data[3] = 27;
	data[4] = MSP_PID;

	data[5] =  pid[0].kp & 0xFF;
	data[6] =  pid[0].ki & 0xFF;
	data[7] =  pid[0].kd & 0xFF;
	
	data[8] =  pid[1].kp & 0xFF;
	data[9] =  pid[1].ki & 0xFF;
	data[10]=  pid[1].kd & 0xFF;

	data[11] =  pid[2].kp & 0xFF;
	data[12] =  pid[2].ki & 0xFF;
	data[13] =  pid[2].kd & 0xFF;
	
	data[14] =  pid[3].kp & 0xFF;
	data[15] =  pid[3].ki & 0xFF;
	data[16] =  pid[3].kd & 0xFF;
	
	data[17] =  pid[4].kp & 0xFF;
	data[18] =  pid[4].ki & 0xFF;
	data[19] =  pid[4].kd & 0xFF;
	
	data[20] =  pid[5].kp & 0xFF;
	data[21] =  pid[5].ki & 0xFF;
	data[22] =  pid[5].kd & 0xFF;
	
	data[23] =  pid[6].kp & 0xFF;
	data[24] =  pid[6].ki & 0xFF;
	data[25] =  pid[6].kd & 0xFF;
	
	data[26] =  pid[7].kp & 0xFF;
	data[27] =  pid[7].ki & 0xFF;
	data[28] =  pid[7].kd & 0xFF;
	
	data[29] =  pid[8].kp & 0xFF;
	data[30] =  pid[8].ki & 0xFF;
	data[31] =  pid[8].kd & 0xFF;
	
	data[32] = Get_Checksum(data);
			
	for(count=0;count<33;count++)
	{
		PrintHexU8(data[count]);
	}
}

/******************************************************************************
����ԭ��:	void Print_MSP_IDENT(void)
��������:	���Ͱ汾��Ϣ
*******************************************************************************/ 
void Print_MSP_IDENT(void)
{
	uint8_t	data[13];
	uint8_t count;

	data[0] = '$';
	data[1] = 'M';
	data[2] = '>';//������λ��
	data[3] = 7;
	data[4] = MSP_IDENT;
	
	data[5] =  VERSION;
	data[6] =  QUADX;
	data[7] =  MSP_VERSION;
	
	data[8] =  0;
	data[9] =  0;
	data[10]=  0;
	data[11] = 12;

	data[12] = Get_Checksum(data);
	for(count=0;count<13;count++)
	{
		PrintHexU8(data[count]);
	}
}

/******************************************************************************
����ԭ��:	void Print_MSP_MOTOR_PINS(void)
��������:	����������IO����
*******************************************************************************/ 
void Print_MSP_MOTOR_PINS(void)
{
	uint8_t	data[14];
	uint8_t count;

	data[0] = '$';
	data[1] = 'M';
	data[2] = '>';//������λ��
	data[3] = 8;
	data[4] = MSP_MOTOR_PINS;
	
	data[5] =  3;
	data[6] =  1;
	
	data[7] =  2;
	data[8] =  0;
	
	data[9]  = 0;
	data[10] = 0;
	
	data[11] = 0;
	data[12] = 0;

	data[13] = Get_Checksum(data);
	for(count=0;count<14;count++)
	{
		PrintHexU8(data[count]);
	}	
}

/******************************************************************************
����ԭ��:	void Print_MSP_ANALOG(void)
��������:	���͵�ѹ����
*******************************************************************************/ 
void Print_MSP_ANALOG(void)
{
	uint8_t	data[13];
	uint8_t count;

	data[0] = '$';
	data[1] = 'M';
	data[2] = '>';//������λ��
	data[3] = 7;
	data[4] = MSP_ANALOG;
	
	data[5] =  (uint8_t)(Battery_Fly/10.0f)& 0xFF;
	
	data[6] =  0;
	data[7] =  0;
	
	data[8] =  0;
	data[9]  = 0;
	
	data[10] = 0;
	data[11] = 0;
	
	data[12] = Get_Checksum(data);
	for(count=0;count<13;count++)
	{
		PrintHexU8(data[count]);
	}
}

/******************************************************************************
����ԭ��:	void Print_MSP_SET_PID(void)
��������:	���÷ɿ�PID����
*******************************************************************************/ 
void Print_MSP_SET_PID(void)
{
	uint8_t	data[32];

	data[0] = '$';
	data[1] = 'M';
	data[2] = '<';//�����ɿ�
	data[3] = 9;
	data[4] = MSP_SET_PID;
	
	data[5] =  pid[0].kp & 0xFF;
	data[6] =  pid[0].ki & 0xFF;
	data[7] =  pid[0].kd & 0xFF;
	
	data[8] =  pid[1].kp & 0xFF;
	data[9] =  pid[1].ki & 0xFF;
	data[10]=  pid[1].kd & 0xFF;
	
	data[11] =  pid[2].kp & 0xFF;
	data[12] =  pid[2].ki & 0xFF;
	data[13] =  pid[2].kd & 0xFF;

	data[14] = Get_Checksum(data);
	
	NRF_Send_TX(data,32);
}

/******************************************************************************
����ԭ��:	void Print_MSP_MOTOR(void)
��������:	���͵��PWMֵ
*******************************************************************************/ 
void Print_MSP_MOTOR(void)
{
	uint8_t	data[32];
	uint8_t count;
	
	data[0] = '$';
	data[1] = 'M';
	data[2] = '>';//������λ��
	data[3] = 16;
	data[4] = MSP_MOTOR;
	
	data[5] =   (THROTTLE4) & 0xFF;
	data[6] =  ((THROTTLE4) >> 8) & 0xFF;

	data[7] =   (THROTTLE2) & 0xFF;
	data[8] =  ((THROTTLE2) >> 8) & 0xFF;

	data[9] =   (THROTTLE3) & 0xFF;
	data[10] =  ((THROTTLE3) >> 8) & 0xFF;

	data[11] =   (THROTTLE1) & 0xFF;
	data[12] =  ((THROTTLE1) >> 8) & 0xFF;
/////////////////////////////////
	data[13] = 0;
	data[14] = 0;
	
	data[15] = 0;
	data[16] = 0;
	
	data[17] = 0;
	data[18] = 0;

	data[19] = 0;
	data[20] = 0;
	
	data[21] = Get_Checksum(data);
	
	for(count=0;count<22;count++)
	{
		PrintHexU8(data[count]);
	}	
}

/******************************************************************************
����ԭ��:	void Uart_Check(void)
��������:	���ڻ������ݴ���
*******************************************************************************/ 
void Uart_Check(void)
{
	uint8_t Line_address;
	if(Line0==1 || Line1==1)
	{
		if(Line0==1)//ȷ���������
		{
			Line0 = 0;Line_address = 0;
		}
		else
		{
			Line1 = 0;Line_address = 1;
		}
		
		switch (RxBuff[Line_address][4])
		{
			case MSP_SET_PID : //����PID����
					pid[0].kp = RxBuff[Line_address][5];
					pid[0].ki = RxBuff[Line_address][6];
					pid[0].kd = RxBuff[Line_address][7];

					pid[1].kp = RxBuff[Line_address][8];
					pid[1].ki = RxBuff[Line_address][9];
					pid[1].kd = RxBuff[Line_address][10];

					pid[2].kp = RxBuff[Line_address][11];
					pid[2].ki = RxBuff[Line_address][12];
					pid[2].kd = RxBuff[Line_address][13];
					Print_MSP_SET_PID();
				break;
			case MSP_PID :	//��ȡPID����
				Print_MSP_PID();
				break;
			case MSP_ACC_CALIBRATION : 	//У�����ٶȼ�
				NRF_Send_TX(RxBuff[Line_address],32);
				break;
			case MSP_MAG_CALIBRATION : 	//ԭ��У�������ƣ���������У��������
				NRF_Send_TX(RxBuff[Line_address],32);
				break;
			case MSP_RESET_CONF : 		//����PID
			{
				NRF_Send_TX(RxBuff[Line_address],32);
				break;
			}
		}
	}
}
