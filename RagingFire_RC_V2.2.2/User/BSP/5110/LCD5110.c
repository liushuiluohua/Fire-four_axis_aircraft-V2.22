#include "struct_all.h"

typedef struct 
{
	u8 x;
	u8 y;
}point;

/*------5110屏幕尺寸和功能宏定义------*/
#define DATA	1								//数据
#define CMD 	0								//命令
#define LCD_X	84								//液晶屏横坐标宽度
#define LCD_ROW LCD_X							//液晶屏列宽度
#define LCD_Y	48								//液晶屏纵坐标高度
#define LCD_COLUMN 		LCD_Y / 8				//液晶屏行高度
#define LCD_AREA 		LCD_X * LCD_Y			//液晶屏面积
#define LCD_AREA_BYTE 	LCD_COLUMN * LCD_ROW	//液晶屏字节容量

/*------5110端口和引脚宏定义--------*/

#define LCD5110_PORT 	  	GPIOB			//5110所在端口
#define LCD_SCLK_BIT 	  	GPIO_Pin_15		//SCLK端口位
#define LCD_SDIN_BIT 		GPIO_Pin_3		//SDIN端口位
#define LCD_DC_BIT 	 		GPIO_Pin_5		//DC端口位
#define LCD_SCE_BIT			GPIO_Pin_4		//SCE端口位
#define LCD_REST_BIT 		GPIO_Pin_8		//REST端口位

/*------5110引脚动作宏定义-----------*/ 

#define LCD_SCLK_H	GPIOA->BSRR = LCD_SCLK_BIT	//SCLK高电平
#define LCD_SCLK_L  GPIOA->BRR  = LCD_SCLK_BIT	//SCLK低电平

#define LCD_SDIN_H	LCD5110_PORT->BSRR = LCD_SDIN_BIT
#define LCD_SDIN_L  LCD5110_PORT->BRR  = LCD_SDIN_BIT

#define LCD_DC_H	LCD5110_PORT->BSRR = LCD_DC_BIT
#define LCD_DC_L  	LCD5110_PORT->BRR  = LCD_DC_BIT

#define LCD_SCE_H	LCD5110_PORT->BSRR = LCD_SCE_BIT
#define LCD_SCE_L  	LCD5110_PORT->BRR  = LCD_SCE_BIT

#define LCD_REST_H	LCD5110_PORT->BSRR = LCD_REST_BIT
#define LCD_REST_L  LCD5110_PORT->BRR  = LCD_REST_BIT

#define RCC_LCD5110_PORT 	RCC_APB2Periph_GPIOB	//5110 RCC使能


/*------初始化5110端口和引脚--------*/
static void LcdPortInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_LCD5110_PORT | RCC_APB2Periph_AFIO, ENABLE);		
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//关闭JTAG-DP,开启SW-DP，释放PB3，PB4

    GPIO_InitStructure.GPIO_Pin = LCD_SDIN_BIT| LCD_DC_BIT| LCD_SCE_BIT| LCD_REST_BIT; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(LCD5110_PORT, &GPIO_InitStructure); 
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

/******************************************************************************
				以下是基本操作和初始化函数
*******************************************************************************/ 

/*---写一个字节到5110(d_c决定是数据还是命令)---*/
void LcdWrite(u8 byte,u8 d_c)
{
	u8 i;
	
	LCD_SCE_L;	//使能片选

	if(d_c == DATA)	//选择发送的是数据还是命令
		LCD_DC_H;
	else 
		LCD_DC_L;

	for(i=0;i<8;i++)//发送一个字节
	{
		LCD_SCLK_L;
		if(byte&(0x80>>i)) 
			LCD_SDIN_H;
		else
			LCD_SDIN_L;

		LCD_SCLK_H;
	}
	LCD_SCLK_L;

	LCD_SCE_H;	//失能片选
}
/*---初始化5110---*/
void InitLcd(void)
{
	LcdPortInit();	//初始化5110端口和引脚
	LCD_REST_L;		//发送复位信号初始化5110内部寄存器
	LCD_REST_H;

	LcdWrite(0x21,CMD);//使用扩展命令设置LCD模式
	LcdWrite(0xc8,CMD);//设置偏置电压
	LcdWrite(0x06,CMD);//温度校正
	LcdWrite(0x13,CMD);//1:48
	LcdWrite(0x20,CMD);//使用基本命令
	LcdWrite(0x0c,CMD);//设定显示模式，标准显示
	
	LcdClearAll();//清屏

	PutChar('F'-0x20,0,1);
	PutChar('L'-0x20,0,2);
	PutChar('Y'-0x20,0,3);

	PutChar('R'-0x20,1,1);
	PutChar('C'-0x20,1,2);
	
	PutChar('A'-0x20,2,0); PutChar('U'-0x20,2,1); PutChar('X'-0x20,2,2); PutChar('1'-0x20,2,3);
	PutChar('A'-0x20,2,5); PutChar('U'-0x20,2,6); PutChar('X'-0x20,2,7); PutChar('2'-0x20,2,8);
	PutChar('A'-0x20,2,10);PutChar('U'-0x20,2,11);PutChar('X'-0x20,2,12);PutChar('3'-0x20,2,13);
}
/*---设置5110行和列坐标----*/
void SetXY(u8 row,u8 column)
{
	if(row >= LCD_ROW) row = 0;			//纠正错误列输入
	if(column >= LCD_COLUMN) column = 0;//纠正错误行输入
	
	LcdWrite(0x40 | column,CMD);		//设置行坐标
	LcdWrite(0x80 | row,CMD);			//设置列坐标
}

/*---清空整个5110屏幕----*/
void LcdClearAll(void)
{
	u16 i;
	
	SetXY(0,0);	//坐标回到原点
	for(i = 0;i < LCD_AREA_BYTE;i ++)	
	{
		LcdWrite(0,DATA);//写0到5110以清除该字节数据
	}
}
/******************************************************************************
		以下是高度为8的各种操作函数(包括6*8字符和字符串，6*8符号，10*8图标)
*******************************************************************************/ 

//--every row contains 14 characters,there are 6 rows (font = 6 * 8)--//
//-----------------   	row		  ----------------------//
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 0
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 1
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 2
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 3
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 4
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 5
//------------------------------------------------------//

/*---直接写一个ASCII字符(6*8)到5110(不需要坐标，紧跟上个字符或图标)---*/
void WriteChar(u8 value)
{
	u8 i;
	for(i = 0;i < 6;i ++)
	LcdWrite(ASCII_6_8[value][i],DATA);
}

/*---直接写一个ASCII字符(6*8)到5110---*/
void PutChar(u8 value,u8 column,u8 row)
{
	if(row > 13) row = 0;
	SetXY(6 * row,column);
	WriteChar(value);
}

/*---直接写一个字符串(6*8)到5110(不需要坐标，紧跟上个字符或图标)---*/
void WriteStr(char * str)
{
	while(*str)
	{
		WriteChar(*str ++);
	}
}
/*--直接写一个字符串(6*8)到5110--*/
void PutStr(char * str,u8 column,u8 row)
{
	if(row > 13) row = 0;
	SetXY(6 * row,column);
	WriteStr(str);
}
/*---画sign(6*8)(不需要坐标，紧跟上个字符或图标)---*/
void WriteSign(char * sign)
{
	u8 i;
	for(i = 0;i < 6;i ++)
	LcdWrite(sign[i],DATA);
}
/*----画sign(6*8)----*/
void PutSign(char * sign,u8 column,u8 row)
{
	SetXY(row,column);
	WriteSign(sign);
}
/*---画Icon(10*8)(不需要坐标，紧跟上个字符或图标)---*/
void WriteIcon(char * icon)
{
	u8 i;
	for(i = 0;i < 10;i ++)
	LcdWrite(icon[i],DATA);
}
/*--画Icon(10*8),row(0~74)--*/
void PutIcon(char * icon,u8 column,u8 row)
{
	if(row > (LCD_ROW - 10))//如果需要换行
	{
		row = 0;
		column += 1;
	}
	SetXY(row,column);
	WriteIcon(icon);
}


/******************************************************************************
		以下是高度为16的各种操作函数(包括8*16字符和字符串，16*16汉字)
*******************************************************************************/ 

//--every row contains 10 characters,there are 3 rows (font = 8 * 16)--//
//-----------       row       ------------//
	//0 1 2 3 ... 81 82 83//	column = 0
	//0 1 2 3 ... 81 82 83//
	//0 1 2 3 ... 81 82 83//	column = 1
	//0 1 2 3 ... 81 82 83//
	//0 1 2 3 ... 81 82 83//	column = 2
	//0 1 2 3 ... 81 82 83//
//-----------------------------------------//

/*-----直接写一个ASCII字符(8*16)到5110------*/
void PutWideChar(u8 value,u8 column,u8 row)
{
	u8 i;
	//写上半行
	SetXY(row,column << 1);
	for(i = 0;i < 8;i ++)
		LcdWrite(ASCII_8_16[value - 0x20][i],DATA);
	//写下半行
	SetXY(row,(column << 1) + 1);
	for(i = 8;i < 16;i ++)
		LcdWrite(ASCII_8_16[value - 0x20][i],DATA);
}
/*--直接写一个字符串(6*8)到5110，自动换行，row(0~76)--*/
void PutWideStr(char * str,u8 column,u8 row)
{
	u8 i = 0;
	
	while(*str)
	{	
		if(row + (i << 3) > (LCD_ROW - 8))//如果需要换行
		{
			row = 0;
			i = 0;
			column += 1;
		}
		PutWideChar(*str,column,row + (i << 3));
		str++;
		i++;
	}
}

//--every row contains 5 characters,there are 3 rows (font = 16 * 16)--//
//----------      row       --------------//
	//0 1 2 3 ... 81 82 83//	column = 0
	//0 1 2 3 ... 81 82 83//
	//0 1 2 3 ... 81 82 83//	column = 1
	//0 1 2 3 ... 81 82 83//
	//0 1 2 3 ... 81 82 83//	column = 2
	//0 1 2 3 ... 81 82 83//
//----------------------------------------//

/*-----直接写一个汉字(16*16)到5110------*/
void PutHanzi(char Hanzi[],u8 column,u8 row)
{
	u8 i;
	//写上半行
	SetXY(row,column << 1);
	for(i = 0;i < 16;i ++)
	LcdWrite(Hanzi[i],DATA);
	//写下半行
	SetXY(row,(column << 1) + 1);
	for(i = 16;i < 32;i ++)
		LcdWrite(Hanzi[i],DATA);
}
/*--写一串汉字(16*16),自动换行,row(0~58)--*/
void PutHanziStr(char Hanzi[],u8 column,u8 row,u8 num)
{
	u8 num_i,i = 0;
	for(num_i = 0;num_i < num;num_i ++)
	{
		if(row + (num_i << 4) > (LCD_ROW - 16))	//如果需要换行
		{
			row = 0;
			i = 0;
			column += 1;
		}
		PutHanzi(Hanzi + (num_i << 5),column,row + (i << 4));
		i ++;
	}
}

/******************************************************************************
		以下是画图的各种操作函数(包括84*48,高度为8的倍数的图片)
*******************************************************************************/ 
//---------------   	x		  --------------------//
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 0
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 1
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 2
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 3
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 4
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 5
//----------------------------------------------------//

/*---画picture(84*48)，全屏---*/
void DrawPicture(char bitmap[])
{
	u16 i;
	for(i = 0;i < LCD_AREA_BYTE;i ++)
		LcdWrite(bitmap[i],DATA);
}

/*---画picture(高度必须是8的倍数)---*/
void DrawBmp(char bitmap[],u8 x,u8 y,u8 width,u8 height)
{
	u8 width_i,height_i;
	height = (height >> 3);
	//画第height_i行
	for(height_i = 0;height_i < height;height_i ++)
	{
		SetXY(x,y + height_i);
		//画该行的width列
		for(width_i = 0;width_i < width;width_i ++)
		{
			LcdWrite(bitmap[width_i + height_i * width],DATA);
		}
	}
}
