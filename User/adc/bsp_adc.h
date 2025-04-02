#ifndef __BSP_ADC_H
#define	__BSP_ADC_H


#include "stm32f10x.h"

//这个宏是我们自己写的用来方便我们自己的.
//我们的头文件应当和固件库(stm32f10x_adc.h)中定义的函数变量区分开!
//在.c文件中需要调用大量固件库函数进行设置和初始化, 我们自己的宏不能影响.




// ADC 编号选择(选adc1还是adc2还是adc3)
// 可以是 ADC1/2，如果使用ADC3，中断相关的要改成ADC3的
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd

//我们的头文件应当和固件库(stm32f10x_adc.h)中定义的函数变量区分开! ADCx在固件库中有定义, 所以我们这里加一个_
#define    ADC_x                          ADC1		//选择adc1
#define    ADC_CLK                       RCC_APB2Periph_ADC1	//adc1挂载在APB2总线。



// ADC GPIO宏定义
// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC

//这两行实际上选择了PC1引脚. 如果要用别的adc输入引脚, 要修改GPIOC为GPIOX(X=A,B,C), GPIO_PIN_1修改为对应编号.
#define    ADC_PORT                      GPIOC
#define    ADC_PIN                       GPIO_Pin_1


// ADC 通道宏定义
#define    ADC_CHANNEL                   ADC_Channel_11		//pc1对应通道11.请查询ADC输入通道和pin的表格,



//有关DMA的定义
										
#define    ADC_DMA_CHANNEL               DMA1_Channel1


// ADC 中断相关宏定义(注意这里是adc1,2的写法, 要用adc3需要修改)
// ADC1_2_IRQn				是STM32 定义的 ADC1/ADC2 共享的中断号。
// ADC1_2_IRQHandler	是stm32官方库的adc1,2的共同中断处理函数.
//#define    ADC_IRQ                       ADC1_2_IRQn
//#define    ADC_IRQHandler                ADC1_2_IRQHandler

//#define    ADC_IRQ                       ADC3_IRQn
//#define    ADC_IRQHandler                ADC3_IRQHandler




void ADCx_Init(void);




#endif





