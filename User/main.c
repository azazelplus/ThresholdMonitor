//2025.3.25.azazel
 //这是来自USART1接发项目文件夹的基础上进行的adc单通道中断读取项目的基础上的阈值检测.
 //的基础上的USART1_ADC_DMA.
 //的基础上的USART1_ADC_DMA_TIMled.
 

 
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "bsp_tim.h"


//声明一个外部变量ADC_ConvertedValue. 
// __IO是stm32官方库(CMSIS规范)定义的一个宏(可以F12看一下), 即#define __IO volatile.
//volatile 是 C 语言的关键字，用于告诉编译器：“这个变量的值可能会被外部（如硬件、DMA 或中断）修改，所以每次使用它都要重新读取，而不能优化！
extern __IO uint16_t ADC_ConvertedValue;


extern uint8_t led_timer_count;
extern uint8_t control_count;


//局部变量.用来保存转换计算后的电压值
float ADC_ConvertedValueLocal;


//运行/休眠占空比
#define A  		5				//运行时间
#define R  		10			//休息时间
// 设置电压阈值
#define THRESHOLD_VOLTAGE 0.75f



void enter_sleep_mode(void)
{
    // 关闭 LED
    GPIO_SetBits(GPIOB, GPIO_Pin_0);  

    // 关闭 ADC 和 DMA
    ADC_Cmd(ADC_x, DISABLE);
    DMA_Cmd(ADC_DMA_CHANNEL, DISABLE);

    // 进入低功耗模式.
		//`__WFI();`会暂停 CPU，直到有中断发生
		//TIM7 触发中断后，CPU 恢复运行，才会继续执行 WFI() 之后的代码
    __WFI();  

    // 唤醒后重新打开 ADC 和 DMA
    ADC_Cmd(ADC_x, ENABLE);
    DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);
}


//阈值比较函数. 比较当前dma中的数据和设定阈值. 如果发生超限, 点亮LED并且给LED生命计时器+50.
void threshold_comp(){
					if (ADC_ConvertedValueLocal >= THRESHOLD_VOLTAGE)//如果超过阈限
				{
						// 将 LED 置低，点亮 LED
						GPIO_ResetBits(GPIOB, GPIO_Pin_0);
						// 重置计时器为 50（ 50 * 中断触发频率(0.1s) = 5s）
						led_timer_count = 50;
				}	
}






// 一会儿软件延时用到的delay函数.
// 如果不考虑编译器优化，for 循环的汇编代码大致需要 4~5 个 CPU 时钟周期 执行一次循环。
// delay(k)->大约5k个cpu(主频 72MHz)周期后才进行一次main
/*
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
} 
*/

	


// PB0 绿色 LED 配置函数
void LED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 开启 GPIOB 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // 配置 PB0 为推挽输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 默认关闭 LED
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

/**************************************************************************/



	
	
int main(void)
{	
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_Config();
	printf("\r\n ----usart working fine.----\r\n");
	//初始化adc和它的dma
	ADCx_Init();	
	//初始化LED
	LED_GPIO_Config();
	//初始化TIM
	BasicTIM_Init();	
	
  while(1)
	{		
			//电压值=寄存器值/4096*3.3
			ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; 
		
			
			//printf("\r\n The current AD value = 0x%04X \r\n", 
			//       ADC_ConvertedValue); 			
			//把电压值用%f(单精度)打印出来.
			printf("\r\n The current AD value = %f V \r\n",
						 ADC_ConvertedValueLocal); 

			
			/****************************排查错误打印**********************************/
	#if 0
			uint8_t led_status;
			uint8_t pin_input_status, pin_output_status;
			//打印当前 LED 状态

			led_status = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_0);
			printf("\r\n Current LED Status: %s \r\n", led_status ? "ON" : "OFF");
			if(ADC_ConvertedValueLocal >= THRESHOLD_VOLTAGE){
				printf("\r\n threshold: %s \r\n", "OVER!!!");
			}else{
				printf("\r\n threshold: %s \r\n", "SAFE.");
			}
		
			// 读取 GPIOB 端口的输入和输出状态
			pin_input_status = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
			pin_output_status = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_0);
			printf("GPIOB Pin0 Input Status: %d, Output Status: %d\r\n", pin_input_status, pin_output_status);

			printf("led_timer_count is: %d\n", led_timer_count);
			
			printf("TIM6 CNT: %d\n", TIM6->CNT);
			
			printf("TIM->CR1 is: 0x%X\n",TIM6->CR1);//CR1寄存器最低位为1表示TIM6已经使能
			printf("\r\n\r\n");
	#endif	
			
			printf("TIM7 CNT: %d\n", TIM7->CNT);//TIM7的计数?
			printf("TIM7->CR1 is: 0x%X\n",TIM7->CR1);//TIM7使能了吗
			printf("control_count is: 0x%X\n",control_count);//control_count的值?
			
			/***************************排查错误打印结束***************************/	
				
			if (control_count < A){  		
				threshold_comp();
			}else if(control_count <R){
				enter_sleep_mode();
			}else control_count = 0;	//重新计时
		}
	
}
/*********************************************END OF FILE**********************/
