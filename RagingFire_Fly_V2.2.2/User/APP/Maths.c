#include "Maths.h"
#include "struct_all.h"

/******************************************************************************
函数原型：	void Get_Radian(struct _gyro *Gyro_in,struct _SI_float *Gyro_out)
功    能：	角速度由原始数据转为弧度
*******************************************************************************/ 
void Get_Radian(struct _gyro *Gyro_in,struct _SI_float *Gyro_out)
{
	Gyro_out->x = (float)(Gyro_in->x * RawData_to_Radian);
	Gyro_out->y = (float)(Gyro_in->y * RawData_to_Radian);
	Gyro_out->z = (float)(Gyro_in->z * RawData_to_Radian);
}

/******************************************************************************
函数原型：	uint8_t Get_Checksum(uint8_t mydata[])
功    能：	MWC上位机---计算校验码
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
// 快速计算开根号的倒数
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
