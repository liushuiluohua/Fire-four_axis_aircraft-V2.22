//***************************************************************************************
//�һ�΢�����������Դ���Ȩ���һ��Ŷ����У�δ���һ��Ŷ�ͬ�⣬�������������ϴ�����Դ�롣
//�뱾�������鼮<<���������DIY-����STM32΢������>>���ɱ�����������ʽ���棬���ݶԱ��װ�
//������������Լ�Ӳ����ض�������ϸ�Ľ��⣬����Ȥ�����ѿ��ԴӸ�����깺��
//�뱾������׵�Ӳ����http://fire-dragon.taobao.com
//������������˸���������ĸĽ�������ʱ�����Ǳ�����ϵ��
//QQ��16053729    �һ�QQȺ��234879071
//***************************************************************************************
#include "struct_all.h"

/*---��value����ֱֵ��д��5110---*/
void PutNumber(uint16_t value,uint8_t column,uint8_t row)
{
	uint8_t num;
	
	SetXY(6 * row,column);
	num = value/1000%10;//ǧλ
	WriteChar(num+ 0x10);
	num = value/100%10;	//��λ
	WriteChar(num+ 0x10);
	num = value/10%10;	//ʮλ
	WriteChar(num+ 0x10);
	num = value%10;		//��λ
	WriteChar(num+ 0x10);
}

/*---��ѹ��ʾ---*/
void Play_Battery(uint16_t value,uint8_t column,uint8_t row,uint8_t turn_count)
{	
	uint8_t Battery = value/10;
	switch(turn_count)
	{
		case 1:
			switch (Battery)
			{
				case 42:	PutIcon((char*)(icon + 7),column,row);
					break;
				case 41:	PutIcon((char*)(icon + 6),column,row);
					break;
				case 40:	PutIcon((char*)(icon + 5),column,row);
					break;
				case 39:	PutIcon((char*)(icon + 4),column,row);
					break;
				case 38:	PutIcon((char*)(icon + 3),column,row);
					break;
				case 37:	PutIcon((char*)(icon + 2),column,row);
					break;
				case 36:	PutIcon((char*)(icon + 1),column,row);
					break;
				default:	PutIcon((char*)(icon + 0),column,row);
					break;
			}	break;
		case 2:
			SetXY(row+10,column);
			Battery = value/100%10;	//��λ
			WriteChar(Battery+ 0x10);
			WriteChar('.'-0x20);	//С����
			Battery = value/10%10;	//ʮ��λ
			WriteChar(Battery+ 0x10);
			Battery = value%10;		//�ٷ�λ
			WriteChar(Battery+ 0x10);
				break;
	}
}

/*---LCDɨ����ʾ---*/
void LCD_Play(void)
{
	static uint8_t turn_count;
	turn_count++;
	switch(turn_count)
	{
		case 1: Play_Battery(Battery_Fly,0,30,1);	break;
		case 2:	Play_Battery(Battery_Fly,0,30,2);	break;
		case 3: Play_Battery(Battery_Rc, 1,30,1);	break;
		case 4:	Play_Battery(Battery_Rc, 1,30,2);	break;
		case 5:	PutNumber(Rc.AUX1,3,0);	break;
		case 6:	PutNumber(Rc.AUX2,3,5);	break;
		case 7:	PutNumber(Rc.AUX3,3,10);   break;
		case 8:	PutHanziStr((char*)(Hanzi0),2,0,4);
				turn_count=0;break;
	}
}
