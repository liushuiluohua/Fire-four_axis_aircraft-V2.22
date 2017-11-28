#include "Maths.h"
#include "struct_all.h"

/******************************************************************************
����ԭ�ͣ�	void Get_Radian(struct _gyro *Gyro_in,struct _SI_float *Gyro_out)
��    �ܣ�	���ٶ���ԭʼ����תΪ����
*******************************************************************************/ 
void Get_Radian(struct _gyro *Gyro_in,struct _SI_float *Gyro_out)
{
	Gyro_out->x = (float)(Gyro_in->x * RawData_to_Radian);
	Gyro_out->y = (float)(Gyro_in->y * RawData_to_Radian);
	Gyro_out->z = (float)(Gyro_in->z * RawData_to_Radian);
}

/******************************************************************************
����ԭ�ͣ�	uint8_t Get_Checksum(uint8_t mydata[])
��    �ܣ�	MWC��λ��---����У����
*******************************************************************************/ 
uint8_t Get_Checksum(uint8_t mydata[])
{	
	uint8_t i;
    uint8_t checksum=0;
	uint8_t length = mydata[3];

    for(i=0;i<length+2;i++)
    {
        checksum ^= (mydata[3+i]&0xFF);
    }
    return checksum;
}

// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
// ���ټ��㿪���ŵĵ���
float invSqrt(float x)
{
  float halfx = 0.5f * x;
  float y = x;
  long i = *(long*)&y;
  i = 0x5f3759df - (i>>1);
  y = *(float*)&i;
  y = y * (1.5f - (halfx * y * y));
  return y;
}
