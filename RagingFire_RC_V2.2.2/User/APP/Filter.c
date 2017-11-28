//***************************************************************************************
//�һ�΢�����������Դ���Ȩ���һ��Ŷ����У�δ���һ��Ŷ�ͬ�⣬�������������ϴ�����Դ�롣
//�뱾�������鼮<<���������DIY-����STM32΢������>>���ɱ�����������ʽ���棬���ݶԱ��װ�
//������������Լ�Ӳ����ض�������ϸ�Ľ��⣬����Ȥ�����ѿ��ԴӸ�����깺��
//�뱾������׵�Ӳ����http://fire-dragon.taobao.com
//������������˸���������ĸĽ�������ʱ�����Ǳ�����ϵ��
//QQ��16053729    �һ�QQȺ��234879071
//***************************************************************************************
#include "Filter.h"
#include "struct_all.h"

/******************************************************************************
����ԭ�ͣ�	void RC_Limit(struct _Rc *rc)
��    �ܣ�	����ң��ָ�Χ
*******************************************************************************/ 
void RC_Limit(struct _Rc *rc)
{
	rc->THROTTLE = (rc->THROTTLE<=1000)?1000:rc->THROTTLE; 
	rc->THROTTLE = (rc->THROTTLE>=2000)?2000:rc->THROTTLE; 
	rc->PITCH = (rc->PITCH<=1000)?1000:rc->PITCH; 
	rc->PITCH = (rc->PITCH>=2000)?2000:rc->PITCH; 
	rc->ROLL = (rc->ROLL<=1000)?1000:rc->ROLL; 
	rc->ROLL = (rc->ROLL>=2000)?2000:rc->ROLL; 
	rc->YAW  = (rc->YAW<=1000)?1000:rc->YAW; 
	rc->YAW  = (rc->YAW>=2000)?2000:rc->YAW; 
	rc->AUX1 = (rc->AUX1<=1000)?1000:rc->AUX1; 
	rc->AUX1 = (rc->AUX1>=2000)?2000:rc->AUX1; 
	rc->AUX2 = (rc->AUX2<=1000)?1000:rc->AUX2; 
	rc->AUX2 = (rc->AUX2>=2000)?2000:rc->AUX2; 
	rc->AUX3 = (rc->AUX3<=1000)?1000:rc->AUX3; 
	rc->AUX3 = (rc->AUX3>=2000)?2000:rc->AUX3; 
}

#define Filter_Num 5
/******************************************************************************
����ԭ�ͣ�	void Rc_Filter( struct _Rc *Rc_in,struct _Rc *Rc_out)
��    �ܣ�	ң��ָ���˲�
*******************************************************************************/ 
void Rc_Filter( struct _Rc *Rc_in,struct _Rc *Rc_out)
{
	static uint16_t Filter_A[Filter_Num],Filter_B[Filter_Num],Filter_C[Filter_Num],Filter_D[Filter_Num];
	static uint8_t Filter_count;
	uint32_t Filter_sum_A=0,Filter_sum_B=0,Filter_sum_C=0,Filter_sum_D=0;
	uint8_t i=0;
//////////////////////////////////////////////////////////////////
//			��ADֵ����ң������
//////////////////////////////////////////////////////////////////
#ifdef American_Mode//������
	Rc_in->THROTTLE = 	1000 + (uint16_t)(0.25f*ADC_Value[4]) - offset0;//����
	Rc_in->PITCH 	= 	2000 - (uint16_t)(0.25f*ADC_Value[7]) - offset2;//������
#else //�ձ���
	Rc_in->THROTTLE = 	2000 - (uint16_t)(0.25f*ADC_Value[7]) - offset0;//����
	Rc_in->PITCH 	= 	1000 + (uint16_t)(0.25f*ADC_Value[4]) - offset2;//������
#endif
	
	Rc_in->YAW 		= 	1000 + (uint16_t)(0.25f*ADC_Value[5]) - offset1;//����
	Rc_in->ROLL 	=	2000 - (uint16_t)(0.25f*ADC_Value[6]) - offset3;//����
	
	Rc_in->AUX1 	= 	1000 + (uint16_t)(0.25f*ADC_Value[2]) - offset4;//������΢��
	Rc_in->AUX2	 	= 	1000 + (uint16_t)(0.25f*ADC_Value[0]) - offset5;//����΢��
	Rc_in->AUX3 	= 	1000 + (uint16_t)(0.25f*ADC_Value[1]) - offset6;//����΢��
	
	Battery_Rc = (uint16_t)(2.0f*ADC_Value[3]/ADC_Value[8]*1.2f*100);//ң�ص�ѹֵ��100��
//////////////////////////////////////////////////////////////////
//			��ң������������1000~2000
//////////////////////////////////////////////////////////////////
	RC_Limit(Rc_in);
	Do_Offset();//ң���ϵ���Զ����У���һ���󼴶������	
//////////////////////////////////////////////////////////////////
//			ң��ָ���˲�
//////////////////////////////////////////////////////////////////
	Filter_A[Filter_count] = Rc_in->PITCH;
	Filter_B[Filter_count] = Rc_in->ROLL;
	Filter_C[Filter_count] = Rc_in->THROTTLE;
	Filter_D[Filter_count] = Rc_in->YAW;
	
	for(i=0;i<Filter_Num;i++)
	{
		Filter_sum_A += Filter_A[i];
		Filter_sum_B += Filter_B[i];
		Filter_sum_C += Filter_C[i];
		Filter_sum_D += Filter_D[i];
	}	
	
	Rc_out->PITCH 		= Filter_sum_A / Filter_Num;
	Rc_out->ROLL  		= Filter_sum_B / Filter_Num;
	Rc_out->THROTTLE 	= Filter_sum_C / Filter_Num;
	Rc_out->YAW 	 	= Filter_sum_D / Filter_Num;
	
	Filter_count++;
	if(Filter_count == Filter_Num)
		Filter_count=0;
}


