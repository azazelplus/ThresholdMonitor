/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_usart.h"
#include "bsp_tim.h"



extern uint8_t led_timer_count;
extern uint8_t control_count;

//DMA模块不需要这部分
#if 0
#include "bsp_adc.h"

//定义`ADC_ConvertedValue`. 这个变量用来存储 ADC 采样的数值.
//__IO宏等于volatile, 关键字volatile 表示硬件寄存器的值可能会变,不许优化.
__IO uint16_t ADC_ConvertedValue;
#endif

    

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}



//下面是自己写的代码


//// 串口中断服务函数
//void DEBUG_USART_IRQHandler(void)
//{
//  uint8_t ucTemp;
//	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
//	{		
//		ucTemp = USART_ReceiveData(DEBUG_USARTx);
//    USART_SendData(DEBUG_USARTx,ucTemp);    
//	}	 
//}




//在DMA项目中不需要adc中断函数ADC_IRQHandler
//编写adc中断服务函数. 函数名stm32有要求. 
//这里的ADC_IRQHandler是我们自己的宏定义(bsp_adc.h), 即`ADC1_2_IRQHandler`.
//它是stm32规定的adc1,2的共同中断处理函数.
//void ADC_IRQHandler(void){
//	//先用ADC_GetITStatus(查询:stm32f10x_adc.h, line462)这个库函数判断中断有没有来.
//	if(
//	ADC_GetITStatus(ADC_x, ADC_IT_EOC) == SET
//	){
//		//if里是中断里要进行的操作.
//		
//		//调用固件库函数(adc.h,line444)ADC_GetConversionValue
//		ADC_ConvertedValue = ADC_GetConversionValue(ADC_x);
//	}
//	//用ADC_ClearITPendingBit(查询:stm32f10x_adc.h, line463)函数清楚中断标志位, 防止cpu一直在中断里不出来.
//	ADC_ClearITPendingBit(ADC_x, ADC_IT_EOC);
//}




/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
void TIM6_IRQHandler(void)//每次TIM6 RESET时触发.
{
    // 检查是否是 BASIC_TIM 更新中断
    if (TIM_GetITStatus(BASIC_TIM, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(BASIC_TIM, TIM_IT_Update);  // 清除中断标志

        if (led_timer_count > 0)
        {
            led_timer_count--;  // 递减计数器
            if (led_timer_count == 0)
            {
                // 计时结束，关闭 LED
                GPIO_SetBits(GPIOB, GPIO_Pin_0);
            }
        }
    }
}

void TIM7_IRQHandler(void)//每次TIM7 RESET时触发.
{
    // 检查是否是 TIM7 更新中断
    if (TIM_GetITStatus(CONTROL_TIM, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(CONTROL_TIM, TIM_IT_Update);  // 清除中断标志
				control_count++;               // 计数 +1                    
    }
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
