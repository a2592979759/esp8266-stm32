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
* @version  软件V1.4 硬件V1.4
* @note   按键的外部中断初始化
*
*
*
***********************************************************/
//外部中断 驱动代码	   
//外部中断0服务程序
void EXTIX_Init(void)
{
 
   	EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	按键端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟



   //GPIOA.12	  中断线以及中断初始化配置 下降沿触发 KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


   //GPIOA.7	  中断线以及中断初始化配置 上升沿触发 KEY2
 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13); 

  	EXTI_InitStructure.EXTI_Line=EXTI_Line13;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY1所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键KEY2所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

}
/**
* @brief 按键KEY1 PA0 中断处理函数
* @param 
* @return 
*/
void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY1==1)	 //按键KEY1 PA0
	{
		delay_ms(10);//消抖
		if(Mode == 4) Mode =1;     //加速按键
		else Mode = Mode +1;  
	}		 
	EXTI_ClearITPendingBit(EXTI_Line0);  //清除LINE4上的中断标志位  
}

/**
* @brief 按键KEY2 PA13 中断处理函数
* @param 
* @return 
*/
void EXTI15_10_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY2==1)	 //按键KEY2 PA13
	{
		delay_ms(10);//消抖
		if(Mode == 0) Mode = 4;     //减速速按键
		else Mode = Mode - 1;  
	}		 
	EXTI_ClearITPendingBit(EXTI_Line13);  //清除LINE4上的中断标志位  
}

