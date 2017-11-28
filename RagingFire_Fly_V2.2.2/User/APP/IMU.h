#ifndef _IMU_H_
#define	_IMU_H_
#include "stm32f10x.h"
#include "struct_all.h"

/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);
void Get_Eulerian_Angle(struct _out_angle *angle);

#endif

