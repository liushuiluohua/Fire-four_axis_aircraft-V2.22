#ifndef _CONTROL_H_
#define _CONTROL_H_
#include "stm32f10x.h"
#include "struct_all.h"

/******************************************************************************
							ȫ�ֱ�������
*******************************************************************************/ 
extern uint8_t Rc_Lock;//1������0����

/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
void RC_LOCK(void);
void RC_Limit(struct _Rc *rc);
void Control_Angle(struct _out_angle *angle,struct _Rc *rc);
void Control_Gyro(struct _SI_float *gyro,struct _Rc *rc,uint8_t Lock);

#endif
