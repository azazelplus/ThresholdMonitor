#ifndef __BSP
#define __BSP

//��Ϊ�ϵ�֮��, ��ţ�Ƶ�ͷ�ļ�
#include "stm32f10x.h"
#include "bsp_tim.h"



/********************������ʱ��TIM�������壬ֻ��TIM6��7************/

//��ʱ��Ӳ��ģ��ѡ��
#define            BASIC_TIM                   TIM6

//��ʱ����������ʱ����ʹ�ܺ���
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd

//���嶨ʱ�� TIM6 �����ص�ʱ�ӱ�ʶ���� TIM7 �� APB1 �����ϵ�ʱ�ӡ�
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM6

//�����˶�ʱ�����Զ���װ�ؼĴ��� (ARR)=999. ��ʱ����� 0 ������ 999���� 1000 ����������, ��1000*100us=100ms����һ���ж�.
#define            BASIC_TIM_Period            1000-1

//�����˶�ʱ����Ԥ��Ƶ����Prescaler����ֵ����ʱP=71, ��ʱ������ʱ��ΪCLK_APB1/(Prescaler+1)=72MHz/7200=10kHz, ��T=100us
#define            BASIC_TIM_Prescaler         7199

//������ TIM6 ���ж������IRQn
#define            BASIC_TIM_IRQ               TIM6_IRQn

//������ TIM6 ���жϷ�������ISR�������֡�
#define            BASIC_TIM_IRQHandler        TIM6_IRQHandler

 // ʹ�û�����ʱ��TIM7
#define            CONTROL_TIM                   TIM7
#define            CONTROL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            CONTROL_TIM_CLK               RCC_APB1Periph_TIM7
#define            CONTROL_TIM_Period            10000-1  // ARR=10000. �Ӷ�1s ����һ���ж�
#define            CONTROL_TIM_Prescaler         7199         // Ԥ��Ƶ��ʹ TIM7 ��������: 0.1ms
#define            CONTROL_TIM_IRQ               TIM7_IRQn
#define            CONTROL_TIM_IRQHandler        TIM7_IRQHandler


/**************************��������********************************/

//void BASIC_TIM_Init(void);

void BasicTIM_Init(void);		
void ControlTIM_Config(void);

#endif
