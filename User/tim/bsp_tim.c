

#include "bsp_tim.h"


//ȫ�ֱ���.
volatile uint8_t led_timer_count = 0; // LED�������ļ�����
volatile uint8_t control_count = 0; // ģ�鹤��ֹͣ�����ļ�����




// TIM6���ж�����
static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


//TIM7���ж�����
static void CONTROL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = CONTROL_TIM_IRQ ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}














//TIM6�ĳ�ʼ������
static void BasicTIM_Config(void){
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
	
		// �����Զ���װ�ؼĴ���ARR��ֵ��(��ֵ����).�ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;	

	  // ����ʱ��Ԥ��Ƶ��Prescaler.ͬ��
    TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler;
	
		// ʱ�ӷ�Ƶ���� ��������ʱ��û�У����ù�
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// ����������ģʽ��������ʱ��ֻ�����ϼ�����û�м���ģʽ������
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// �ظ���������ֵ��������ʱ��û�У����ù�
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // ��ʼ����ʱ��
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(BASIC_TIM, ENABLE);	
}


//TIM7�ĳ�ʼ��
static void ControlTIM_Config(void){
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    BASIC_TIM_APBxClock_FUN(CONTROL_TIM_CLK, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = CONTROL_TIM_Period;	
    TIM_TimeBaseStructure.TIM_Prescaler= CONTROL_TIM_Prescaler;
	  // ��ʼ����ʱ��
    TIM_TimeBaseInit(CONTROL_TIM, &TIM_TimeBaseStructure);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(CONTROL_TIM, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(CONTROL_TIM,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(CONTROL_TIM, ENABLE);	
}


void BasicTIM_Init(void){
	//��ʼ��TIM6��TIM7��NVIC�ж�
	BASIC_TIM_NVIC_Config();
	CONTROL_TIM_NVIC_Config();
	
	//��ʼ��������ʱ��
	BasicTIM_Config();
	ControlTIM_Config();
}




