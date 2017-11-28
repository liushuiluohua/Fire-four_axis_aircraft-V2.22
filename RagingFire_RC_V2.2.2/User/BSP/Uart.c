//***************************************************************************************
//烈火微型四轴飞行器源码版权归烈火团队所有，未经烈火团队同意，请勿随意在网上传播本源码。
//与本软件相关书籍<<四轴飞行器DIY-基于STM32微控制器>>，由北航出版社正式出版，内容对本套包
//含的所有软件以及硬件相关都做了详细的讲解，有兴趣的网友可以从各大书店购买。
//与本软件配套的硬件：http://fire-dragon.taobao.com
//如果有网友做了各种有意义的改进，请随时与我们保持联系。
//QQ：16053729    烈火QQ群：234879071
//***************************************************************************************
#include "struct_all.h"

static uint8_t TxCount=0;
static uint8_t Count=0;

static  uint8_t TxBuff[256];//串口发送缓冲区
uint8_t RxBuff[2][50];		//串口接收缓冲区

uint8_t Line0,Line1;	//串口接收双缓冲切换
/******************************************************************************
函数原型：	void Uart1_Init(uint32_t baud)
功    能：	串口初始化
参    数：  baud，波特率
*******************************************************************************/ 
void Uart1_Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 打开GPIO和USART部件的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* 将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 将USART Rx的GPIO配置为浮空输入模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 配置USART参数 */
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	/* 使能串口接收中断 */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* 第5步：使能 USART， 配置完毕 */
	USART_Cmd(USART1, ENABLE);

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART1, USART_FLAG_TC);//清发送完成标志位
}

/******************************************************************************
函数原型：	void USART1_IRQHandler(void)
功    能：	串口中断
*******************************************************************************/ 
void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)//ORE中断
	{
		USART_ReceiveData(USART1);
	}
	
	if(USART1->SR & USART_SR_TC && (USART1->CR1 & USART_CR1_TXEIE) )//发送中断
	{
		USART1->DR = TxBuff[TxCount++];//写DR清除中断标志          
		if(TxCount == Count)
		{
			USART1->CR1 &= ~USART_CR1_TXEIE;//关闭TXE中断
		}
	}
	
	if(USART1->SR & USART_SR_RXNE)//接收中断
	{
		static uint8_t Head = 0;
		static uint8_t Line = 0;
		uint8_t com_data = USART1->DR;
		
		if(Head==0)	//寻找帧头
		{
			if(com_data=='$')	
			{
				RxBuff[Line][0] = com_data;
				Head = 1;
			}
		}
		else if(Head==1)
		{
			if(com_data=='M')	
			{
				RxBuff[Line][1] = com_data;
				Head = 2;
			}
			else
				Head = 0;
		}
		else if(Head==2)
		{
			if(com_data=='<')//上位机发送给MWC
			{
				RxBuff[Line][2] = com_data;
				Head = 3;
			}
			else
				Head = 0;
		}
		else
		{
			RxBuff[Line][Head] = com_data;
			Head ++;
		}
		
		if(Head==RxBuff[Line][3]+6)	//数据接收完毕
		{
			Head = 0;
			if(Line)
			{ 
				Line = 0; //切换缓存
				Line1 = 1;
			}
			else
			{
				Line = 1;
				Line0 = 1;
			}
		}
	}
}


//以HEX的形式输出U8型数据
void PrintHexU8(uint8_t data)
{
	TxBuff[Count++] = data;  
	if(!(USART1->CR1 & USART_CR1_TXEIE))
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //打开TXE中断
}
//以HEX的形式输出S16型数据
void PrintHexS16(int16_t num)
{
	PrintHexU8((uint8_t)(num & 0xff00) >> 8);//先发送高8位，再发送低8位
	PrintHexU8((uint8_t)(num & 0x00ff));
}
//以字符的形式输出S8型数据
void PrintS8(int8_t num)
{
	uint8_t  bai,shi,ge;
	if(num<0)
	{
		PrintHexU8('-');
		num=-num;
	}
	else 
		PrintHexU8(' ');	
	bai=num/100;
	shi=num%100/10;
	ge =num%10;
	PrintHexU8('0'+bai);
	PrintHexU8('0'+shi);
	PrintHexU8('0'+ge);
}
//以字符的形式输出U8型数据
void PrintU8(uint8_t num)
{
	uint8_t  bai,shi,ge;
	bai=num/100;
	shi=num%100/10;
	ge =num%10;
	PrintHexU8('0'+bai);
	PrintHexU8('0'+shi);
	PrintHexU8('0'+ge);
}
//以字符的形式输出S16型数据 
void PrintS16(int16_t num)
{	
	uint8_t w5,w4,w3,w2,w1;
	if(num<0)
	{
		PrintHexU8('-');
		num=-num;
	}
	else 
		PrintHexU8(' ');
	
	w5=num%100000/10000;
	w4=num%10000/1000;
	w3=num%1000/100;
	w2=num%100/10;
	w1=num%10;
	PrintHexU8('0'+w5);
	PrintHexU8('0'+w4);
	PrintHexU8('0'+w3);
	PrintHexU8('0'+w2);
	PrintHexU8('0'+w1);
}
//以字符的形式输出U16型数据
void PrintU16(uint16_t num)
{	
	uint8_t w5,w4,w3,w2,w1;
	
	w5=num%100000/10000;
	w4=num%10000/1000;
	w3=num%1000/100;
	w2=num%100/10;
	w1=num%10;
	PrintHexU8(' ');
	PrintHexU8('0'+w5);
	PrintHexU8('0'+w4);
	PrintHexU8('0'+w3);
	PrintHexU8('0'+w2);
	PrintHexU8('0'+w1);
}
//输出字符串
void PrintString(uint8_t *s)
{
	uint8_t *p;
	p=s;
	while(*p != '\0')
	{
		PrintHexU8(*p);
		p++;
	}
}
//串口发送数组中前len个数据
void Uart_Send(uint8_t *s ,uint8_t len)
{
	while(len)
	{
		PrintHexU8(*s);
		s++;
		len--;
	}
}

