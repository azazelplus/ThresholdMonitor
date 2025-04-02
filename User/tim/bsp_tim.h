#ifndef __BSP
#define __BSP

//称为上帝之手, 最牛逼的头文件
#include "stm32f10x.h"
#include "bsp_tim.h"



/********************基本定时器TIM参数定义，只限TIM6、7************/

//定时器硬件模块选择
#define            BASIC_TIM                   TIM6

//定时器所在总线时钟势使能函数
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd

//定义定时器 TIM6 所挂载的时钟标识，即 TIM7 在 APB1 总线上的时钟。
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM6

//定义了定时器的自动重装载寄存器 (ARR)=999. 定时器会从 0 计数到 999，共 1000 个计数周期, 共1000*100us=100ms触发一次中断.
#define            BASIC_TIM_Period            1000-1

//定义了定时器的预分频器（Prescaler）的值。此时P=71, 定时器计数时钟为CLK_APB1/(Prescaler+1)=72MHz/7200=10kHz, 即T=100us
#define            BASIC_TIM_Prescaler         7199

//定义了 TIM6 的中断请求号IRQn
#define            BASIC_TIM_IRQ               TIM6_IRQn

//定义了 TIM6 的中断服务函数（ISR）的名字。
#define            BASIC_TIM_IRQHandler        TIM6_IRQHandler

 // 使用基本定时器TIM7
#define            CONTROL_TIM                   TIM7
#define            CONTROL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            CONTROL_TIM_CLK               RCC_APB1Periph_TIM7
#define            CONTROL_TIM_Period            10000-1  // ARR=10000. 从而1s 触发一次中断
#define            CONTROL_TIM_Prescaler         7199         // 预分频，使 TIM7 计数周期: 0.1ms
#define            CONTROL_TIM_IRQ               TIM7_IRQn
#define            CONTROL_TIM_IRQHandler        TIM7_IRQHandler


/**************************函数声明********************************/

//void BASIC_TIM_Init(void);

void BasicTIM_Init(void);		
void ControlTIM_Config(void);

#endif
