#include "bsp_adc.h"


__IO uint16_t ADC_ConvertedValue;


//1.��ʼ��adcҪ�õ�gpio.
static void ADCx_GPIO_Config(void){

	
	//����һ����ʼ���ṹ�����.
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// �� ADC IO�˿�ʱ��
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );
	
	// ���� ADC IO ����ģʽ
	// ����Ϊģ������
	//GPIO.pin�ǽṹ��GPIO_InitTypeDef�ĳ�Ա. ��ʾҪ���õ�����. �˴�ADC_PIN��ͷ�ļ����ж���Ϊ`GPIO_Pin_1`.
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//ģ�����ģʽ
	
	// ��ʼ�� ADC IO. ADC_PORT�ں��ж���Ϊ`GPIOC`
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);		

}


/**
  * @brief  ����ADC����ģʽ
  * @param  ��
  * @retval ��
  */

//ADCģʽ����
static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;	
	
	//��DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	// ��ADCʱ��
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );

	
	//��λDMA������
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	/*-----------------����DMA��ʼ���ṹ��------------------------------*/
	
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStruct.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADC_x->DR ) );
	
	// �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
	
	// ����Դ��������
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// ��������СΪ1���������Ĵ�СӦ�õ��ڴ洢���Ĵ�С
	DMA_InitStruct.DMA_BufferSize = 1;
	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// �洢����ַ�̶�
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable; 
	
	// �������ݴ�СΪ���֣��������ֽ�
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// �洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// ѭ������ģʽ
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;	

	// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	
	// ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��P->M
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	
	// ��ʼ��DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStruct);
	
	// ʹ�� DMA ͨ��
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	
	
	
	
	/*-----------------����ADC��ʼ���ṹ��------------------------*/
	//�����ýṹ��ĵ�һ����Ա:ADC_Mode. adcģʽѡ��. ��������ֻʹ��һ��adc, ����ѡ��`����ģʽ`.(ȥ�̼���stm32f10x_adc.h, line94��. ����FWLB�ļ����ҵ�stm32f10x_adc.c,Ȼ������include��ǰ����ת����.
	//en, ��ʼ���ṹ���6����ԱҲҪ��stm32f10x_adc.h��鿴�ṹ��typedefŶ.
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	
	//�ڶ�����Ա��ADC_ScanConvMode(see stm32f10x_adc.h, line56). �Ƿ�ɨ��. ɨ������Զ�ͨ����. ѡ��DISABLE(how can i know the options?? see .h pls.).
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	
	//��������Ա��ADC_ContinuousConvMode, �Ƿ�����ת��, enable.
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	
	
	//4th member: ADC_ExternalTrigConv, adc����ģʽ. choose �������.(��ʹ���ⲿ����, �ⲿ�����Ƕ�ʱ��/�ⲿgpio)
	//ѡ��ο�.h�ļ�, line123~138. 
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	
	//5th: dataalign.
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	
	//6th: channel nember.
	ADC_InitStruct.ADC_NbrOfChannel = 1;


	//��ʼ��������, ����adc_init.�ú�����������: �����������úõĲ���, ȥ��д��������adc���ܵļĴ���, ʵ���������.
	//(.h, line429: `void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);`)
	ADC_Init(ADC_x, &ADC_InitStruct);



	//3.1����adcʱ��. ǰ���̼���stm32f10x_rcc.c, line766:`void RCC_ADCCLKConfig(uint32_t RCC_PCLK2)`
	//�������β�ѡ��: ��stm32f10x_rcc.c, line760~764��˵��. �˴�ѡ��8��֮һ��Ƶ.
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
		
	
	
	//3.2����`ͨ��ת��˳��`, `����ʱ��`
	//�鿴stm32f10x_adc.h, line442, �õ�����ADC_RegularChannelConfig.ÿ������...
	//1.�ĸ�adc, 
	//2.�ĸ�ͨ��, 
	//3.ת��˳��(cause ��������ֻ��һ��adcͨ����������rank����1��.)
	//4.����ʱ��. ��stm32f10x_adc.c, line579~588. ��������ѡ��55.5������.
	
	//ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);
	//���������!
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL, 1, ADC_SampleTime_28Cycles5);
	
	//DMA project doesnt need this
	#if 0
	//4.1ʹ��adcת������ж�, ����adc���ȼ�.
	//ȥadc.h���ҵ�IT(interrupt)�����ĺ���: void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState);
	//1.ѡ���ĸ�adc
	//2.�ж�����. ADC_IT_EOC, ����ͨ��.
	//3.enable/diable
	ADC_ITConfig(ADC_x, ADC_IT_EOC, ENABLE);
	#endif
	
	
	//4.ʹ��ADC��DMA����.
	ADC_DMACmd(ADC_x, ENABLE);
	
	
	//5.ʹ��adc(���Ǵ�adc), ׼����ʼת��.
	//(���ı�̷���: �Ѻ��������ai��, �����������β���������.)
	ADC_Cmd(ADC_x, ENABLE);
	
	
	//6.У׼adc.
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADC_x);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC_x));
	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADC_x);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC_x));
	
	
	//7.�������adc, ��ʼת��.
	// ����û�в����ⲿ����������ʹ���������ADCת�� 
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}

//DMA��Ŀ����Ҫ�ж�.
#if 0
//8.��д�жϷ�����, ��ȡADCת��������.
//������������������� ADC �� NVIC��Ƕ�������жϿ���������Ҳ���Ǹ��� STM32���� ADC �����ж�ʱ������δ���
static void ADC_NVIC_Config(void)
{
	////����һ�� NVIC_InitTypeDef �ṹ��������������� NVIC.
  NVIC_InitTypeDef NVIC_InitStructure;
	// ���ȼ�����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	
	//ѡ��Ҫ���õ��ж�Դ�������� ADC_IRQ��ADC ���жϣ���
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQ;
	
	//���� ��ռ���ȼ� = 1�������ȼ� = 1��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	//���� ADC �жϡ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	//���� STM32 �⺯�� NVIC_Init()�������������д�� NVIC��
  NVIC_Init(&NVIC_InitStructure);
}
#endif

//��װһ����������д�ĳ�ʼ������.
void ADCx_Init(void){
	
	//DMA��Ŀ����Ҫ����NVIC����
	//ADC_NVIC_Config();
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}
/*********************************************END OF FILE**********************/
