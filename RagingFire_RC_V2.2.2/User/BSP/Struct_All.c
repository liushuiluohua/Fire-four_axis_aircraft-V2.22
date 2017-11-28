//***************************************************************************************
//�һ�΢�����������Դ���Ȩ���һ��Ŷ����У�δ���һ��Ŷ�ͬ�⣬�������������ϴ�����Դ�롣
//�뱾�������鼮<<���������DIY-����STM32΢������>>���ɱ�����������ʽ���棬���ݶԱ��װ�
//������������Լ�Ӳ����ض�������ϸ�Ľ��⣬����Ȥ�����ѿ��ԴӸ�����깺��
//�뱾������׵�Ӳ����http://fire-dragon.taobao.com
//������������˸���������ĸĽ�������ʱ�����Ǳ�����ϵ��
//QQ��16053729    �һ�QQȺ��234879071
//***************************************************************************************
#include "EEPROM.h"
#include "struct_all.h"

/* pid���� */
struct _pid pid[9];

/* ң��ͨ�� */
struct _Rc Rc;
struct _Rc Filter_Rc;

uint8_t Bsp_Int_Ok = 0;

///////////////////////////////////////////////////////////
#define EEPROM_offset0		0
#define EEPROM_offset1		1
#define EEPROM_offset2		2
#define EEPROM_offset3		3
#define EEPROM_offset4		4
#define EEPROM_offset5		5
#define EEPROM_offset6		6

uint16_t data_0,data_1,data_2,data_3;
uint16_t VirtAddVarTab[NumbOfVar] = {
	0x9A00,0x9A01,0x9A02,0x9A03,0x9A04,0x9A05,0x9A06
};//�����ַ

void EEPROM_INIT(void)//��ʼ��ģ���EEPROM����ȡ����
{
	EE_Init();
	EEPROM_READ_OFFSET();
}

void EEPROM_SAVE_OFFSET(void)//����ҡ����ƫ����
{
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset0], offset0);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset1], offset1);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset2], offset2);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset3], offset3);
	
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset4], offset4);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset5], offset5);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset6], offset6);
}

void EEPROM_READ_OFFSET(void)//��ȡҡ����ƫ����
{
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset0], &data_0);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset1], &data_1);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset2], &data_2);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset3], &data_3);
	offset0 = (int16_t) data_0;
	offset1 = (int16_t) data_1;
	offset2 = (int16_t) data_2;
	offset3 = (int16_t) data_3;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset4], &data_0);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset5], &data_1);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset6], &data_2);
	offset4 = (int16_t) data_0;
	offset5 = (int16_t) data_1;
	offset6 = (int16_t) data_2;
}
