//***************************************************************************************
//烈火微型四轴飞行器源码版权归烈火团队所有，未经烈火团队同意，请勿随意在网上传播本源码。
//与本软件相关书籍<<四轴飞行器DIY-基于STM32微控制器>>，由北航出版社正式出版，内容对本套包
//含的所有软件以及硬件相关都做了详细的讲解，有兴趣的网友可以从各大书店购买。
//与本软件配套的硬件：http://fire-dragon.taobao.com
//如果有网友做了各种有意义的改进，请随时与我们保持联系。
//QQ：16053729    烈火QQ群：234879071
//***************************************************************************************
#include "Tasks.h"
#include "Filter.h"

/******************************************************************************
函数原型:	static void Nvic_Init(void)
功　　能:	NVIC初始化
*******************************************************************************/ 
static void Nvic_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//NVIC_PriorityGroup 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分组
    //Timer3
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//先占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//从优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	//Nrf2401中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//串口
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//MODE按键
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//FUN按键
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************
函数原型：	void BSP_Int(void)
功    能：	硬件驱动初始化
*******************************************************************************/ 
void BSP_Int(void)
{
	LED_Init();//LED初始化
	LED_ON_OFF();//LED闪烁
	
	KEY_Init();//按键初始化
	Uart1_Init(115200);	//串口初始化：波特率115200，8位数据，1位停止位，禁用奇偶校验
	Timer3_Init(500);//Timer3中断500HZ
	Nvic_Init();//中断优先级初始化	
	
	PrintString("\r\n RagingFire_RC V2.2.2 \r\n");//版本号
	ADC1_Init();//ADC及DMA初始化

	SPI2_Init();//SPI2初始化
	NRF24L01_Init(40,TX);//2401选择40通道，发送模式
 	NRF24L01_Check();//检测2401是否正常
	
	Bsp_Int_Ok = 1;
}

/******************************************************************************
函数原型：	void Task_500HZ(void)
功    能：	主循环中运行频率为500HZ任务
*******************************************************************************/ 
void Task_500HZ(void)
{
	Nrf_Connect();//NRF24L01连接函数
	Rc_Filter(&Rc,&Filter_Rc);//计算遥控指令+滤波
}

/******************************************************************************
函数原型：	void Task_100HZ(void)
功    能：	主循环中运行频率为100HZ任务
*******************************************************************************/ 
void Task_100HZ(void)
{
	Print_MSP_RC('<');//发送遥控信号给飞控
}

/******************************************************************************
函数原型：	void Task_25HZ(void)
功    能：	主循环中运行频率为25HZ任务
*******************************************************************************/ 
void Task_25HZ(void)
{
	Print_MSP_RC('>');//发送遥控信号给上位机
	LED_Show();//LED指示灯闪烁
	Print_MSP_MOTOR();//发送电机转速给上位机
}

/******************************************************************************
函数原型：	void Task_4HZ(void)
功    能：	主循环中运行频率为4HZ任务
*******************************************************************************/ 
void Task_4HZ(void)
{
	static uint8_t turn_count;
	
	turn_count++;
	switch(turn_count)
	{
		case 1: Print_MSP_ANALOG();		break;
		case 2: Print_MSP_IDENT();		break;
		case 3: Print_MSP_ANALOG();		break;				
		case 4: Print_MSP_MOTOR_PINS();	
				turn_count=0; 			break;
	}
}
