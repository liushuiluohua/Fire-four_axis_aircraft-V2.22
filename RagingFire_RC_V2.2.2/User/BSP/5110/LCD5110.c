#include "struct_all.h"

typedef struct 
{
	u8 x;
	u8 y;
}point;

/*------5110��Ļ�ߴ�͹��ܺ궨��------*/
#define DATA	1								//����
#define CMD 	0								//����
#define LCD_X	84								//Һ������������
#define LCD_ROW LCD_X							//Һ�����п��
#define LCD_Y	48								//Һ����������߶�
#define LCD_COLUMN 		LCD_Y / 8				//Һ�����и߶�
#define LCD_AREA 		LCD_X * LCD_Y			//Һ�������
#define LCD_AREA_BYTE 	LCD_COLUMN * LCD_ROW	//Һ�����ֽ�����

/*------5110�˿ں����ź궨��--------*/

#define LCD5110_PORT 	  	GPIOB			//5110���ڶ˿�
#define LCD_SCLK_BIT 	  	GPIO_Pin_15		//SCLK�˿�λ
#define LCD_SDIN_BIT 		GPIO_Pin_3		//SDIN�˿�λ
#define LCD_DC_BIT 	 		GPIO_Pin_5		//DC�˿�λ
#define LCD_SCE_BIT			GPIO_Pin_4		//SCE�˿�λ
#define LCD_REST_BIT 		GPIO_Pin_8		//REST�˿�λ

/*------5110���Ŷ����궨��-----------*/ 

#define LCD_SCLK_H	GPIOA->BSRR = LCD_SCLK_BIT	//SCLK�ߵ�ƽ
#define LCD_SCLK_L  GPIOA->BRR  = LCD_SCLK_BIT	//SCLK�͵�ƽ

#define LCD_SDIN_H	LCD5110_PORT->BSRR = LCD_SDIN_BIT
#define LCD_SDIN_L  LCD5110_PORT->BRR  = LCD_SDIN_BIT

#define LCD_DC_H	LCD5110_PORT->BSRR = LCD_DC_BIT
#define LCD_DC_L  	LCD5110_PORT->BRR  = LCD_DC_BIT

#define LCD_SCE_H	LCD5110_PORT->BSRR = LCD_SCE_BIT
#define LCD_SCE_L  	LCD5110_PORT->BRR  = LCD_SCE_BIT

#define LCD_REST_H	LCD5110_PORT->BSRR = LCD_REST_BIT
#define LCD_REST_L  LCD5110_PORT->BRR  = LCD_REST_BIT

#define RCC_LCD5110_PORT 	RCC_APB2Periph_GPIOB	//5110 RCCʹ��


/*------��ʼ��5110�˿ں�����--------*/
static void LcdPortInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_LCD5110_PORT | RCC_APB2Periph_AFIO, ENABLE);		
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//�ر�JTAG-DP,����SW-DP���ͷ�PB3��PB4

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
				�����ǻ��������ͳ�ʼ������
*******************************************************************************/ 

/*---дһ���ֽڵ�5110(d_c���������ݻ�������)---*/
void LcdWrite(u8 byte,u8 d_c)
{
	u8 i;
	
	LCD_SCE_L;	//ʹ��Ƭѡ

	if(d_c == DATA)	//ѡ���͵������ݻ�������
		LCD_DC_H;
	else 
		LCD_DC_L;

	for(i=0;i<8;i++)//����һ���ֽ�
	{
		LCD_SCLK_L;
		if(byte&(0x80>>i)) 
			LCD_SDIN_H;
		else
			LCD_SDIN_L;

		LCD_SCLK_H;
	}
	LCD_SCLK_L;

	LCD_SCE_H;	//ʧ��Ƭѡ
}
/*---��ʼ��5110---*/
void InitLcd(void)
{
	LcdPortInit();	//��ʼ��5110�˿ں�����
	LCD_REST_L;		//���͸�λ�źų�ʼ��5110�ڲ��Ĵ���
	LCD_REST_H;

	LcdWrite(0x21,CMD);//ʹ����չ��������LCDģʽ
	LcdWrite(0xc8,CMD);//����ƫ�õ�ѹ
	LcdWrite(0x06,CMD);//�¶�У��
	LcdWrite(0x13,CMD);//1:48
	LcdWrite(0x20,CMD);//ʹ�û�������
	LcdWrite(0x0c,CMD);//�趨��ʾģʽ����׼��ʾ
	
	LcdClearAll();//����

	PutChar('F'-0x20,0,1);
	PutChar('L'-0x20,0,2);
	PutChar('Y'-0x20,0,3);

	PutChar('R'-0x20,1,1);
	PutChar('C'-0x20,1,2);
	
	PutChar('A'-0x20,2,0); PutChar('U'-0x20,2,1); PutChar('X'-0x20,2,2); PutChar('1'-0x20,2,3);
	PutChar('A'-0x20,2,5); PutChar('U'-0x20,2,6); PutChar('X'-0x20,2,7); PutChar('2'-0x20,2,8);
	PutChar('A'-0x20,2,10);PutChar('U'-0x20,2,11);PutChar('X'-0x20,2,12);PutChar('3'-0x20,2,13);
}
/*---����5110�к�������----*/
void SetXY(u8 row,u8 column)
{
	if(row >= LCD_ROW) row = 0;			//��������������
	if(column >= LCD_COLUMN) column = 0;//��������������
	
	LcdWrite(0x40 | column,CMD);		//����������
	LcdWrite(0x80 | row,CMD);			//����������
}

/*---�������5110��Ļ----*/
void LcdClearAll(void)
{
	u16 i;
	
	SetXY(0,0);	//����ص�ԭ��
	for(i = 0;i < LCD_AREA_BYTE;i ++)	
	{
		LcdWrite(0,DATA);//д0��5110��������ֽ�����
	}
}
/******************************************************************************
		�����Ǹ߶�Ϊ8�ĸ��ֲ�������(����6*8�ַ����ַ�����6*8���ţ�10*8ͼ��)
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

/*---ֱ��дһ��ASCII�ַ�(6*8)��5110(����Ҫ���꣬�����ϸ��ַ���ͼ��)---*/
void WriteChar(u8 value)
{
	u8 i;
	for(i = 0;i < 6;i ++)
	LcdWrite(ASCII_6_8[value][i],DATA);
}

/*---ֱ��дһ��ASCII�ַ�(6*8)��5110---*/
void PutChar(u8 value,u8 column,u8 row)
{
	if(row > 13) row = 0;
	SetXY(6 * row,column);
	WriteChar(value);
}

/*---ֱ��дһ���ַ���(6*8)��5110(����Ҫ���꣬�����ϸ��ַ���ͼ��)---*/
void WriteStr(char * str)
{
	while(*str)
	{
		WriteChar(*str ++);
	}
}
/*--ֱ��дһ���ַ���(6*8)��5110--*/
void PutStr(char * str,u8 column,u8 row)
{
	if(row > 13) row = 0;
	SetXY(6 * row,column);
	WriteStr(str);
}
/*---��sign(6*8)(����Ҫ���꣬�����ϸ��ַ���ͼ��)---*/
void WriteSign(char * sign)
{
	u8 i;
	for(i = 0;i < 6;i ++)
	LcdWrite(sign[i],DATA);
}
/*----��sign(6*8)----*/
void PutSign(char * sign,u8 column,u8 row)
{
	SetXY(row,column);
	WriteSign(sign);
}
/*---��Icon(10*8)(����Ҫ���꣬�����ϸ��ַ���ͼ��)---*/
void WriteIcon(char * icon)
{
	u8 i;
	for(i = 0;i < 10;i ++)
	LcdWrite(icon[i],DATA);
}
/*--��Icon(10*8),row(0~74)--*/
void PutIcon(char * icon,u8 column,u8 row)
{
	if(row > (LCD_ROW - 10))//�����Ҫ����
	{
		row = 0;
		column += 1;
	}
	SetXY(row,column);
	WriteIcon(icon);
}


/******************************************************************************
		�����Ǹ߶�Ϊ16�ĸ��ֲ�������(����8*16�ַ����ַ�����16*16����)
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

/*-----ֱ��дһ��ASCII�ַ�(8*16)��5110------*/
void PutWideChar(u8 value,u8 column,u8 row)
{
	u8 i;
	//д�ϰ���
	SetXY(row,column << 1);
	for(i = 0;i < 8;i ++)
		LcdWrite(ASCII_8_16[value - 0x20][i],DATA);
	//д�°���
	SetXY(row,(column << 1) + 1);
	for(i = 8;i < 16;i ++)
		LcdWrite(ASCII_8_16[value - 0x20][i],DATA);
}
/*--ֱ��дһ���ַ���(6*8)��5110���Զ����У�row(0~76)--*/
void PutWideStr(char * str,u8 column,u8 row)
{
	u8 i = 0;
	
	while(*str)
	{	
		if(row + (i << 3) > (LCD_ROW - 8))//�����Ҫ����
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

/*-----ֱ��дһ������(16*16)��5110------*/
void PutHanzi(char Hanzi[],u8 column,u8 row)
{
	u8 i;
	//д�ϰ���
	SetXY(row,column << 1);
	for(i = 0;i < 16;i ++)
	LcdWrite(Hanzi[i],DATA);
	//д�°���
	SetXY(row,(column << 1) + 1);
	for(i = 16;i < 32;i ++)
		LcdWrite(Hanzi[i],DATA);
}
/*--дһ������(16*16),�Զ�����,row(0~58)--*/
void PutHanziStr(char Hanzi[],u8 column,u8 row,u8 num)
{
	u8 num_i,i = 0;
	for(num_i = 0;num_i < num;num_i ++)
	{
		if(row + (num_i << 4) > (LCD_ROW - 16))	//�����Ҫ����
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
		�����ǻ�ͼ�ĸ��ֲ�������(����84*48,�߶�Ϊ8�ı�����ͼƬ)
*******************************************************************************/ 
//---------------   	x		  --------------------//
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 0
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 1
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 2
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 3
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 4
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 5
//----------------------------------------------------//

/*---��picture(84*48)��ȫ��---*/
void DrawPicture(char bitmap[])
{
	u16 i;
	for(i = 0;i < LCD_AREA_BYTE;i ++)
		LcdWrite(bitmap[i],DATA);
}

/*---��picture(�߶ȱ�����8�ı���)---*/
void DrawBmp(char bitmap[],u8 x,u8 y,u8 width,u8 height)
{
	u8 width_i,height_i;
	height = (height >> 3);
	//����height_i��
	for(height_i = 0;height_i < height;height_i ++)
	{
		SetXY(x,y + height_i);
		//�����е�width��
		for(width_i = 0;width_i < width;width_i ++)
		{
			LcdWrite(bitmap[width_i + height_i * width],DATA);
		}
	}
}
