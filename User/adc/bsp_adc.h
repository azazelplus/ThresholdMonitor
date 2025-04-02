#ifndef __BSP_ADC_H
#define	__BSP_ADC_H


#include "stm32f10x.h"

//������������Լ�д���������������Լ���.
//���ǵ�ͷ�ļ�Ӧ���͹̼���(stm32f10x_adc.h)�ж���ĺ����������ֿ�!
//��.c�ļ�����Ҫ���ô����̼��⺯���������úͳ�ʼ��, �����Լ��ĺ겻��Ӱ��.




// ADC ���ѡ��(ѡadc1����adc2����adc3)
// ������ ADC1/2�����ʹ��ADC3���ж���ص�Ҫ�ĳ�ADC3��
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd

//���ǵ�ͷ�ļ�Ӧ���͹̼���(stm32f10x_adc.h)�ж���ĺ����������ֿ�! ADCx�ڹ̼������ж���, �������������һ��_
#define    ADC_x                          ADC1		//ѡ��adc1
#define    ADC_CLK                       RCC_APB2Periph_ADC1	//adc1������APB2���ߡ�



// ADC GPIO�궨��
// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC

//������ʵ����ѡ����PC1����. ���Ҫ�ñ��adc��������, Ҫ�޸�GPIOCΪGPIOX(X=A,B,C), GPIO_PIN_1�޸�Ϊ��Ӧ���.
#define    ADC_PORT                      GPIOC
#define    ADC_PIN                       GPIO_Pin_1


// ADC ͨ���궨��
#define    ADC_CHANNEL                   ADC_Channel_11		//pc1��Ӧͨ��11.���ѯADC����ͨ����pin�ı��,



//�й�DMA�Ķ���
										
#define    ADC_DMA_CHANNEL               DMA1_Channel1


// ADC �ж���غ궨��(ע��������adc1,2��д��, Ҫ��adc3��Ҫ�޸�)
// ADC1_2_IRQn				��STM32 ����� ADC1/ADC2 ������жϺš�
// ADC1_2_IRQHandler	��stm32�ٷ����adc1,2�Ĺ�ͬ�жϴ�����.
//#define    ADC_IRQ                       ADC1_2_IRQn
//#define    ADC_IRQHandler                ADC1_2_IRQHandler

//#define    ADC_IRQ                       ADC3_IRQn
//#define    ADC_IRQHandler                ADC3_IRQHandler




void ADCx_Init(void);




#endif





