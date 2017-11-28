//***************************************************************************************
//烈火微型四轴飞行器源码版权归烈火团队所有，未经烈火团队同意，请勿随意在网上传播本源码。
//与本软件相关书籍<<四轴飞行器DIY-基于STM32微控制器>>，由北航出版社正式出版，内容对本套包
//含的所有软件以及硬件相关都做了详细的讲解，有兴趣的网友可以从各大书店购买。
//与本软件配套的硬件：http://fire-dragon.taobao.com
//如果有网友做了各种有意义的改进，请随时与我们保持联系。
//QQ：16053729    烈火QQ群：234879071
//***************************************************************************************
#include "struct_all.h"

uint16_t Battery_Fly,Battery_Rc;//四轴电压和遥控电压的100倍
uint16_t ADC_Value[M]; 

/******************************************************************************
函数原型：	static void ADC1_GPIO_Config(void)
功    能：	ADC的IO初始化
*******************************************************************************/ 
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 打开GPIO和ADC、DMA部件的时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //72M/6=12,ADC最大时间不能超过14M
	
	
	/* 将GPIO配置为输入模式 */	
  	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);
}

/******************************************************************************
函数原型：	static void ADC1_Mode_Config(void)
功    能：	ADC及DMA配置
*******************************************************************************/ 
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	/* DMA 通道1 配置 */
	DMA_DeInit(DMA1_Channel1);//将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//DMA外设ADC基地址;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Value;//DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//内存作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize = M;//DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//工作在循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA通道x拥有高优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//根据DMA_InitStruct中指定的参数初始化DMA的通道

	/* 使能 DMA 通道1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
//////////////////////////////////////////////////////////////////////////////////////////////
	ADC_DeInit(ADC1); //将外设 ADC1 的全部寄存器重设为缺省值
	ADC_TempSensorVrefintCmd(ENABLE);//使能内部参照电压（1.2V）
	/* ADC1 配置 */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式 每个ADC独立工作
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//使用扫描模式  scan位设置
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//cont位设置 连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//EXTSEL 选择启动规则通道组转换的外部事件 设置成有软件控制
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对齐 由软件置位和清楚   这里设置成右对齐
	ADC_InitStructure.ADC_NbrOfChannel = M;//规则通道序列长度 这些位由软件定义在规则通道转换序列中的通道数目 1个转换 指定由多少个通道被转换
	ADC_Init(ADC1, &ADC_InitStructure);

	/* 设置 ADC1 使用8转换通道，在规则组里的转换顺序--->1，普通采样时间为 71.5 周期 */ 	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_71Cycles5 );//AUX2 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_71Cycles5 );//AUX3 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_71Cycles5 );//AUX1 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5);//AUX4 (电池电压)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_71Cycles5 );//THROTTLE
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_71Cycles5 );//YAW
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_71Cycles5 );//PITCH
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_71Cycles5 );//ROLL
	ADC_RegularChannelConfig(ADC1, ADC_Channel_17,9, ADC_SampleTime_239Cycles5);//内部参照电压（1.2V）	

	/* 使能 ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	/* 使能 ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/* 复位 ADC1 的校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/* 等待 ADC1 校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* 开始 ADC1 校准 */
	ADC_StartCalibration(ADC1);
	/* 等待 ADC1 校准完成 */
	while(ADC_GetCalibrationStatus(ADC1));
	/* 使能指定的ADC1的软件转换启动功能 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/******************************************************************************
函数原型：	void ADC1_Init(void)
功    能：	ADC1初始化
*******************************************************************************/ 
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}

/******************************************************************************
函数原型：	void Voltage_Printf(void)
功    能：	输出AD各通道电压值一百倍
*******************************************************************************/ 
void Voltage_Printf(void)
{
	uint8_t i;
	uint16_t Voltage;
	
	PrintString("\r\n");
	for(i=0;i<8;i++)
	{
		Voltage = (uint16_t)(2.0f*ADC_Value[i]/ADC_Value[8]*1.2f*100);
		PrintU8(i);
		PrintString("通道： ");
		PrintU16(Voltage); 
		PrintString("\r\n");
	}
}
