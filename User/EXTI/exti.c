#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"

/**
************************************************************
* @file         
* @brief        
* @author      lqq
* @date       2022.10.12
* @version  ���V1.4 Ӳ��V1.4
* @note   �������ⲿ�жϳ�ʼ��
*
*
*
***********************************************************/
//�ⲿ�ж� ��������	   
//�ⲿ�ж�0�������
void EXTIX_Init(void)
{
 
   	EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	�����˿ڳ�ʼ��

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��



   //GPIOA.12	  �ж����Լ��жϳ�ʼ������ �½��ش��� KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


   //GPIOA.7	  �ж����Լ��жϳ�ʼ������ �����ش��� KEY2
 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13); 

  	EXTI_InitStructure.EXTI_Line=EXTI_Line13;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

}
/**
* @brief ����KEY1 PA0 �жϴ�����
* @param 
* @return 
*/
void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY1==1)	 //����KEY1 PA0
	{
		delay_ms(10);//����
		if(Mode == 4) Mode =1;     //���ٰ���
		else Mode = Mode +1;  
	}		 
	EXTI_ClearITPendingBit(EXTI_Line0);  //���LINE4�ϵ��жϱ�־λ  
}

/**
* @brief ����KEY2 PA13 �жϴ�����
* @param 
* @return 
*/
void EXTI15_10_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY2==1)	 //����KEY2 PA13
	{
		delay_ms(10);//����
		if(Mode == 0) Mode = 4;     //�����ٰ���
		else Mode = Mode - 1;  
	}		 
	EXTI_ClearITPendingBit(EXTI_Line13);  //���LINE4�ϵ��жϱ�־λ  
}

