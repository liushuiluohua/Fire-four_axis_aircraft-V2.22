//***************************************************************************************
//烈火微型四轴飞行器源码版权归烈火团队所有，未经烈火团队同意，请勿随意在网上传播本源码。
//与本软件相关书籍<<四轴飞行器DIY-基于STM32微控制器>>，由北航出版社正式出版，内容对本套包
//含的所有软件以及硬件相关都做了详细的讲解，有兴趣的网友可以从各大书店购买。
//与本软件配套的硬件：http://fire-dragon.taobao.com
//如果有网友做了各种有意义的改进，请随时与我们保持联系。
//QQ：16053729    烈火QQ群：234879071
//***************************************************************************************
#include "struct_all.h"

/******************************************************************************
函数原型:	static void Delay_led(uint16_t n)
功　　能:	延时
*******************************************************************************/ 
static void Delay_led(uint16_t n)
{	
	uint16_t i,j;
	for(i=0;i<n;i++)
		for(j=0;j<8888;j++);
} 

/******************************************************************************
函数原型:	void LED_Init(void)
功　　能:	LED初始化
*******************************************************************************/ 
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Structure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_Structure.GPIO_Pin =  Bee_Pin;	//蜂鸣器
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(Bee_GPIO, &GPIO_Structure);
	
	GPIO_Structure.GPIO_Pin =  LEDRed_Pin;	//LED Red
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(LEDRed_GPIO, &GPIO_Structure);
	
	GPIO_Structure.GPIO_Pin =  LEDGreen_Pin;//LED Green
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(LEDGreen_GPIO, &GPIO_Structure);
}

/******************************************************************************
函数原型:	void LED_ON_OFF(void)
功　　能:	LED开机闪烁
*******************************************************************************/ 
void LED_ON_OFF(void)
{
	uint8_t i ;
	for(i=0;i<3;i++)
	{
		LEDRed_ON;LEDGreen_OFF;Bee_ON;
		Delay_led(100);
		LEDGreen_ON;LEDRed_OFF;Bee_OFF;
		Delay_led(100);	
	}	
	Bee_OFF;
	LEDRed_OFF;
	LEDGreen_OFF;
	Delay_led(100);
}

/******************************************************************************
函数原型：	void LEDRed_ON_Frequency(uint8_t Light_Frequency)
功    能：	让LED按照某个频率闪烁（需要循环扫描该函数）
参    数：   Light_Frequency，闪烁频率（HZ）
*******************************************************************************/ 
void LED3_ON_Frequency(uint8_t Light_Frequency)
{
	uint16_t time_count;
	static uint8_t Light_On;
	static uint32_t Last_time,Now_time;
	
	Now_time = Timer3_Count;
	if( Light_On )
	{
		time_count = (uint16_t)(Timer3_Frequency / Light_Frequency / 2 );	
		
		if( Now_time - Last_time >= time_count)
		{
			LEDRed_OFF;
			Light_On=0;
			Last_time = Timer3_Count;
		}
	}
	else
	{
		time_count = (uint16_t)(Timer3_Frequency / Light_Frequency / 2 );
		
		if( Now_time - Last_time >= time_count)
		{
			LEDRed_ON;
			Light_On=1;
			Last_time = Timer3_Count;
		}
	}
}

/******************************************************************************
函数原型：	void LED_Show(void)
功    能：	LED指示灯闪烁
*******************************************************************************/ 
void LED_Show(void)
{
	if( (Battery_Fly>200 && Battery_Fly<330) || (Battery_Rc>200 && Battery_Rc<360) )//低压报警
	{
		Bee_ON;
		LED3_ON_Frequency(10);//红色LED10HZ闪烁
	}
	else if(Mode)//锁尾模式
	{
		Bee_OFF;
		LED3_ON_Frequency(4);//红色LED4HZ闪烁
	}
	else//非锁尾模式
	{
		Bee_OFF;
		LED3_ON_Frequency(1);//红色LED1HZ闪烁
	}
}
