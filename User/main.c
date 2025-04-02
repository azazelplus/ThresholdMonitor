//2025.3.25.azazel
 //��������USART1�ӷ���Ŀ�ļ��еĻ����Ͻ��е�adc��ͨ���ж϶�ȡ��Ŀ�Ļ����ϵ���ֵ���.
 //�Ļ����ϵ�USART1_ADC_DMA.
 //�Ļ����ϵ�USART1_ADC_DMA_TIMled.
 

 
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "bsp_tim.h"


//����һ���ⲿ����ADC_ConvertedValue. 
// __IO��stm32�ٷ���(CMSIS�淶)�����һ����(����F12��һ��), ��#define __IO volatile.
//volatile �� C ���ԵĹؼ��֣����ڸ��߱������������������ֵ���ܻᱻ�ⲿ����Ӳ����DMA ���жϣ��޸ģ�����ÿ��ʹ������Ҫ���¶�ȡ���������Ż���
extern __IO uint16_t ADC_ConvertedValue;


extern uint8_t led_timer_count;
extern uint8_t control_count;


//�ֲ�����.��������ת�������ĵ�ѹֵ
float ADC_ConvertedValueLocal;


//����/����ռ�ձ�
#define A  		5				//����ʱ��
#define R  		10			//��Ϣʱ��
// ���õ�ѹ��ֵ
#define THRESHOLD_VOLTAGE 0.75f



void enter_sleep_mode(void)
{
    // �ر� LED
    GPIO_SetBits(GPIOB, GPIO_Pin_0);  

    // �ر� ADC �� DMA
    ADC_Cmd(ADC_x, DISABLE);
    DMA_Cmd(ADC_DMA_CHANNEL, DISABLE);

    // ����͹���ģʽ.
		//`__WFI();`����ͣ CPU��ֱ�����жϷ���
		//TIM7 �����жϺ�CPU �ָ����У��Ż����ִ�� WFI() ֮��Ĵ���
    __WFI();  

    // ���Ѻ����´� ADC �� DMA
    ADC_Cmd(ADC_x, ENABLE);
    DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);
}


//��ֵ�ȽϺ���. �Ƚϵ�ǰdma�е����ݺ��趨��ֵ. �����������, ����LED���Ҹ�LED������ʱ��+50.
void threshold_comp(){
					if (ADC_ConvertedValueLocal >= THRESHOLD_VOLTAGE)//�����������
				{
						// �� LED �õͣ����� LED
						GPIO_ResetBits(GPIOB, GPIO_Pin_0);
						// ���ü�ʱ��Ϊ 50�� 50 * �жϴ���Ƶ��(0.1s) = 5s��
						led_timer_count = 50;
				}	
}






// һ��������ʱ�õ���delay����.
// ��������Ǳ������Ż���for ѭ���Ļ����������Ҫ 4~5 �� CPU ʱ������ ִ��һ��ѭ����
// delay(k)->��Լ5k��cpu(��Ƶ 72MHz)���ں�Ž���һ��main
/*
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
} 
*/

	


// PB0 ��ɫ LED ���ú���
void LED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ���� GPIOB ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // ���� PB0 Ϊ�������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // Ĭ�Ϲر� LED
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

/**************************************************************************/



	
	
int main(void)
{	
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  USART_Config();
	printf("\r\n ----usart working fine.----\r\n");
	//��ʼ��adc������dma
	ADCx_Init();	
	//��ʼ��LED
	LED_GPIO_Config();
	//��ʼ��TIM
	BasicTIM_Init();	
	
  while(1)
	{		
			//��ѹֵ=�Ĵ���ֵ/4096*3.3
			ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; 
		
			
			//printf("\r\n The current AD value = 0x%04X \r\n", 
			//       ADC_ConvertedValue); 			
			//�ѵ�ѹֵ��%f(������)��ӡ����.
			printf("\r\n The current AD value = %f V \r\n",
						 ADC_ConvertedValueLocal); 

			
			/****************************�Ų�����ӡ**********************************/
	#if 0
			uint8_t led_status;
			uint8_t pin_input_status, pin_output_status;
			//��ӡ��ǰ LED ״̬

			led_status = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_0);
			printf("\r\n Current LED Status: %s \r\n", led_status ? "ON" : "OFF");
			if(ADC_ConvertedValueLocal >= THRESHOLD_VOLTAGE){
				printf("\r\n threshold: %s \r\n", "OVER!!!");
			}else{
				printf("\r\n threshold: %s \r\n", "SAFE.");
			}
		
			// ��ȡ GPIOB �˿ڵ���������״̬
			pin_input_status = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
			pin_output_status = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_0);
			printf("GPIOB Pin0 Input Status: %d, Output Status: %d\r\n", pin_input_status, pin_output_status);

			printf("led_timer_count is: %d\n", led_timer_count);
			
			printf("TIM6 CNT: %d\n", TIM6->CNT);
			
			printf("TIM->CR1 is: 0x%X\n",TIM6->CR1);//CR1�Ĵ������λΪ1��ʾTIM6�Ѿ�ʹ��
			printf("\r\n\r\n");
	#endif	
			
			printf("TIM7 CNT: %d\n", TIM7->CNT);//TIM7�ļ���?
			printf("TIM7->CR1 is: 0x%X\n",TIM7->CR1);//TIM7ʹ������
			printf("control_count is: 0x%X\n",control_count);//control_count��ֵ?
			
			/***************************�Ų�����ӡ����***************************/	
				
			if (control_count < A){  		
				threshold_comp();
			}else if(control_count <R){
				enter_sleep_mode();
			}else control_count = 0;	//���¼�ʱ
		}
	
}
/*********************************************END OF FILE**********************/
