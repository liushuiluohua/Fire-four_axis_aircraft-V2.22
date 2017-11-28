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
函数原型：	static void Delay_led(u16 n)
功    能：	延时
*******************************************************************************/ 
void Delay_led(u16 n)
{	
	uint16_t i,j;
	for(i=0;i<n;i++)
		for(j=0;j<8500;j++);
}

/******************************************************************************
函数原型：	void LED_Init(void)
功    能：	LED初始化
*******************************************************************************/ 
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Structure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	
	//改变指定管脚的映射 GPIO_Remap_SWJ_Disable SWJ 完全禁用（JTAG+SW-DP）
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	//改变指定管脚的映射 GPIO_Remap_SWJ_JTAGDisable ，JTAG-DP 禁用 + SW-DP 使能
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	GPIO_Structure.GPIO_Pin =  LED2_Pin;	//LED2：电源旁的指示灯
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(LED2_GPIO, &GPIO_Structure);
	
	GPIO_Structure.GPIO_Pin =  LED3_Pin;	//LED3：机身周围的夜间灯	
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(LED3_GPIO, &GPIO_Structure);
}

/******************************************************************************
函数原型：	void LED_ON_OFF(void)
功    能：	LED开机闪烁
*******************************************************************************/ 
void LED_ON_OFF(void)
{
	uint8_t i ;
	for(i=0;i<3;i++)
	{
		LED2_ON;LED3_OFF;
		Delay_led(200);
		LED3_ON;LED2_OFF;
		Delay_led(200);	
	}
	for(i=0;i<3;i++)
	{
		LED2_ON;LED3_OFF;
		Delay_led(100);
		LED3_ON;LED2_OFF;
		Delay_led(100);	
	}	
	LED2_OFF;
	LED3_OFF;
	Delay_led(100);
}
/******************************************************************************
函数原型：	void LED3_ON_Frequency(uint8_t Light_Frequency)
功    能：	让LED3按照某个频率闪烁（需要循环扫描该函数）
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
		time_count = (uint16_t)(Timer3_Frequency / Light_Frequency / 2);	
		
		if( Now_time - Last_time >= time_count)
		{
			LED3_OFF;
			Light_On=0;
			Last_time = Timer3_Count;
		}
	}
	else
	{
		time_count = (uint16_t)(Timer3_Frequency / Light_Frequency / 2);
		
		if( Now_time - Last_time >= time_count)
		{
			LED3_ON;
			Light_On=1;
			Last_time = Timer3_Count;
		}
	}
}

