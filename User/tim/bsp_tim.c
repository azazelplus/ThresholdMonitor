

#include "bsp_tim.h"


//全局变量.
volatile uint8_t led_timer_count = 0; // LED保持亮的计数器
volatile uint8_t control_count = 0; // 模块工作停止开启的计数器




// TIM6的中断配置
static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


//TIM7的中断配置
static void CONTROL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = CONTROL_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}














//TIM6的初始化函数
static void BasicTIM_Config(void){
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启定时器时钟,即内部时钟CK_INT=72M
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
	
		// 设置自动重装载寄存器ARR的值，(右值见宏).累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;	

	  // 设置时钟预分频数Prescaler.同上
    TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler;
	
		// 时钟分频因子 ，基本定时器没有，不用管
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// 计数器计数模式，基本定时器只能向上计数，没有计数模式的设置
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// 重复计数器的值，基本定时器没有，不用管
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // 初始化定时器
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(BASIC_TIM, ENABLE);	
}


//TIM7的初始化
static void ControlTIM_Config(void){
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    BASIC_TIM_APBxClock_FUN(CONTROL_TIM_CLK, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = CONTROL_TIM_Period;	
    TIM_TimeBaseStructure.TIM_Prescaler= CONTROL_TIM_Prescaler;
	  // 初始化定时器
    TIM_TimeBaseInit(CONTROL_TIM, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(CONTROL_TIM, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(CONTROL_TIM,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(CONTROL_TIM, ENABLE);	
}


void BasicTIM_Init(void){
	//初始化TIM6和TIM7的NVIC中断
	BASIC_TIM_NVIC_Config();
	CONTROL_TIM_NVIC_Config();
	
	//初始化两个定时器
	BasicTIM_Config();
	ControlTIM_Config();
}




