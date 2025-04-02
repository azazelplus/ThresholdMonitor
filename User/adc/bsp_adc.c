#include "bsp_adc.h"


__IO uint16_t ADC_ConvertedValue;


//1.初始化adc要用的gpio.
static void ADCx_GPIO_Config(void){

	
	//定义一个初始化结构体变量.
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 打开 ADC IO端口时钟
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );
	
	// 配置 ADC IO 引脚模式
	// 必须为模拟输入
	//GPIO.pin是结构体GPIO_InitTypeDef的成员. 表示要配置的引脚. 此处ADC_PIN在头文件宏中定义为`GPIO_Pin_1`.
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//模拟输出模式
	
	// 初始化 ADC IO. ADC_PORT在宏中定义为`GPIOC`
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);		

}


/**
  * @brief  配置ADC工作模式
  * @param  无
  * @retval 无
  */

//ADC模式配置
static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;	
	
	//打开DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	// 打开ADC时钟
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );

	
	//复位DMA控制器
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	/*-----------------配置DMA初始化结构体------------------------------*/
	
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStruct.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADC_x->DR ) );
	
	// 存储器地址，实际上就是一个内部SRAM的变量
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
	
	// 数据源来自外设
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// 缓冲区大小为1，缓冲区的大小应该等于存储器的大小
	DMA_InitStruct.DMA_BufferSize = 1;
	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// 存储器地址固定
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable; 
	
	// 外设数据大小为半字，即两个字节
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// 存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// 循环传输模式
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;	

	// DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	
	// 禁止存储器到存储器模式，因为是从外设到存储器P->M
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	
	// 初始化DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStruct);
	
	// 使能 DMA 通道
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	
	
	
	
	/*-----------------配置ADC初始化结构体------------------------*/
	//先配置结构体的第一个成员:ADC_Mode. adc模式选择. 我们现在只使用一个adc, 所以选择`独立模式`.(去固件库stm32f10x_adc.h, line94找. 先在FWLB文件夹找到stm32f10x_adc.c,然后在其include的前者跳转即可.
	//en, 初始化结构体的6个成员也要在stm32f10x_adc.h里查看结构体typedef哦.
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	
	//第二个成员是ADC_ScanConvMode(see stm32f10x_adc.h, line56). 是否扫描. 扫描是针对多通道的. 选择DISABLE(how can i know the options?? see .h pls.).
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	
	//第三个成员是ADC_ContinuousConvMode, 是否连续转换, enable.
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	
	
	//4th member: ADC_ExternalTrigConv, adc触发模式. choose 软件触发.(不使用外部触发, 外部触发是定时器/外部gpio)
	//选项参考.h文件, line123~138. 
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	
	//5th: dataalign.
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	
	//6th: channel nember.
	ADC_InitStruct.ADC_NbrOfChannel = 1;


	//初始化结束后, 调用adc_init.该函数的作用是: 按照上述配置好的参数, 去改写各种设置adc功能的寄存器, 实现你的配置.
	//(.h, line429: `void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);`)
	ADC_Init(ADC_x, &ADC_InitStruct);



	//3.1配置adc时钟. 前往固件库stm32f10x_rcc.c, line766:`void RCC_ADCCLKConfig(uint32_t RCC_PCLK2)`
	//函数的形参选择: 见stm32f10x_rcc.c, line760~764处说明. 此处选择8分之一分频.
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
		
	
	
	//3.2配置`通道转换顺序`, `采样时间`
	//查看stm32f10x_adc.h, line442, 得到函数ADC_RegularChannelConfig.每个参数...
	//1.哪个adc, 
	//2.哪个通道, 
	//3.转换顺序(cause 我们这里只用一个adc通道所以它的rank就是1咯.)
	//4.采样时间. 见stm32f10x_adc.c, line579~588. 这里我们选择55.5个周期.
	
	//ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);
	//更快的周期!
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL, 1, ADC_SampleTime_28Cycles5);
	
	//DMA project doesnt need this
	#if 0
	//4.1使能adc转换完成中断, 配置adc优先级.
	//去adc.h中找到IT(interrupt)字样的函数: void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState);
	//1.选的哪个adc
	//2.中断类型. ADC_IT_EOC, 规则通道.
	//3.enable/diable
	ADC_ITConfig(ADC_x, ADC_IT_EOC, ENABLE);
	#endif
	
	
	//4.使能ADC的DMA请求.
	ADC_DMACmd(ADC_x, ENABLE);
	
	
	//5.使能adc(就是打开adc), 准备开始转换.
	//(最快的编程方法: 把函数定义给ai看, 让她告诉你形参怎麽设置.)
	ADC_Cmd(ADC_x, ENABLE);
	
	
	//6.校准adc.
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(ADC_x);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADC_x));
	
	// ADC开始校准
	ADC_StartCalibration(ADC_x);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADC_x));
	
	
	//7.软件触发adc, 开始转换.
	// 由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}

//DMA项目不需要中断.
#if 0
//8.编写中断服务函数, 读取ADC转换的数据.
//这个函数的作用是配置 ADC 的 NVIC（嵌套向量中断控制器），也就是告诉 STM32，当 ADC 触发中断时，该如何处理。
static void ADC_NVIC_Config(void)
{
	////声明一个 NVIC_InitTypeDef 结构体变量，用来配置 NVIC.
  NVIC_InitTypeDef NVIC_InitStructure;
	// 优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	
	//选择要配置的中断源，这里是 ADC_IRQ（ADC 的中断）。
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQ;
	
	//设置 抢占优先级 = 1，子优先级 = 1。
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	//启用 ADC 中断。
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	//调用 STM32 库函数 NVIC_Init()，把上面的配置写入 NVIC。
  NVIC_Init(&NVIC_InitStructure);
}
#endif

//包装一下上面我们写的初始化函数.
void ADCx_Init(void){
	
	//DMA项目不需要配置NVIC外设
	//ADC_NVIC_Config();
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}
/*********************************************END OF FILE**********************/
