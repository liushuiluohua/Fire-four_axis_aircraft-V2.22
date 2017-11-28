//***************************************************************************************
//烈火微型四轴飞行器源码版权归烈火团队所有，未经烈火团队同意，请勿随意在网上传播本源码。
//与本软件相关书籍<<四轴飞行器DIY-基于STM32微控制器>>，由北航出版社正式出版，内容对本套包
//含的所有软件以及硬件相关都做了详细的讲解，有兴趣的网友可以从各大书店购买。
//与本软件配套的硬件：http://fire-dragon.taobao.com
//如果有网友做了各种有意义的改进，请随时与我们保持联系。
//QQ：16053729    烈火QQ群：234879071
//***************************************************************************************
#include "struct_all.h"

/*---将value的数值直接写到5110---*/
void PutNumber(uint16_t value,uint8_t column,uint8_t row)
{
	uint8_t num;
	
	SetXY(6 * row,column);
	num = value/1000%10;//千位
	WriteChar(num+ 0x10);
	num = value/100%10;	//百位
	WriteChar(num+ 0x10);
	num = value/10%10;	//十位
	WriteChar(num+ 0x10);
	num = value%10;		//个位
	WriteChar(num+ 0x10);
}

/*---电压显示---*/
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
			Battery = value/100%10;	//个位
			WriteChar(Battery+ 0x10);
			WriteChar('.'-0x20);	//小数点
			Battery = value/10%10;	//十分位
			WriteChar(Battery+ 0x10);
			Battery = value%10;		//百分位
			WriteChar(Battery+ 0x10);
				break;
	}
}

/*---LCD扫描显示---*/
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
