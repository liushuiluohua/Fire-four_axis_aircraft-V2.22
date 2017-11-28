//***************************************************************************************
//�һ�΢�����������Դ���Ȩ���һ��Ŷ����У�δ���һ��Ŷ�ͬ�⣬�������������ϴ�����Դ�롣
//�뱾�������鼮<<���������DIY-����STM32΢������>>���ɱ�����������ʽ���棬���ݶԱ��װ�
//������������Լ�Ӳ����ض�������ϸ�Ľ��⣬����Ȥ�����ѿ��ԴӸ�����깺��
//�뱾������׵�Ӳ����http://fire-dragon.taobao.com
//������������˸���������ĸĽ�������ʱ�����Ǳ�����ϵ��
//QQ��16053729    �һ�QQȺ��234879071
//***************************************************************************************
#include "stm32f10x.h"
//#include "stdio.h"//Ϊ������ʹ��printf����ӣ�������ʹ�á�

#include "IMU.h"
#include "Tasks.h"
#include "Maths.h"
#include "Filter.h"
#include "Control.h"
#include "struct_all.h"

int main(void)
{
	BSP_Int();	//�ײ�������ʼ��
	while(1)
	{
		if(Count_1ms>=1)
		{	
			Count_1ms = 0;
			Task_1000HZ();
		}
		if(Count_2ms>=2)
		{
			Count_2ms = 0;
			Task_500HZ();
		}
		if(Count_4ms>=4)
		{
			Count_4ms = 0;
			Task_250HZ();
		}
	}
}

//int fputc(int ch, FILE *f)	//Ϊ������ʹ��printf����ӣ�������ʹ�á�
//{
//	USART_SendData(USART1, (uint8_t) ch);
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//	return ch;
//}
