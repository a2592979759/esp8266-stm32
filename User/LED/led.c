/**
************************************************************
* @file         
* @brief        
* @author    lqq
* @date       2022.10.3
* @version  软件V1.4 硬件V1.4
* @note     关于GPIO的初始函数
*
*
*
***********************************************************/
#include "led.h"
#include "timer.h"
#include "delay.h"
#include "bsp_beep.h"
/**
* @brief 初始LED 
* @param 
* @return 
*/
void LED_Init(void)
{
 
/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd( LED1_GPIO_CLK | LED2_GPIO_CLK | LED3_GPIO_CLK, ENABLE);
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
		
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;

		/*调用库函数，初始化GPIO*/
		GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
		
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;

		/*调用库函数，初始化GPIOF*/
		GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

		/* 关闭所有led灯	*/
		GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
		
		/* 关闭所有led灯	*/
		GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);	 
    
    /* 关闭所有led灯	*/
		GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
}
/**
* @brief 初始TB6612的 AIN1 AIN2 BIN1 BIN2
* @brief 连接AIN1--PB13 AIN2-PB12
						 BIN1--PB1  BIN2--PB0
* @param 
* @return 
*/
void TB6612_GPIO_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15); 						 //PE.5 输出高 
}



/**
* @brief 超声波SR04Trig GPIO初始化 
* @param 
* @return 
*/
void SR04_GPIO_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //PA0端口配置, 推挽输出
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOA,GPIO_Pin_2);
}
/**
* @brief 向前无延时
* @param 
* @return 
*/
void Forward(void)
{
		AIN1 =0;
		AIN2 =1;
		BIN1 =0;
		BIN2 =1;
    //TIM_SetCompare4(TIM1,1200);

}	
/**
* @brief 向后无延时
* @param 
* @return 
*/
void Backward(void)
{
		AIN1 =1;
		AIN2 =0;
		BIN1 =1;
		BIN2 =0;
	 // TIM_SetCompare4(TIM8,1200);

}	
/**
* @brief 向右无延时
* @param 
* @return 
*/
void Rightward(void)
{
		AIN1 =0;
		AIN2 =1;
		BIN1 =1;
		BIN2 =0;
	 // TIM_SetCompare4(TIM8,1200);

}	
/**
* @brief 向左无延时
* @param 
* @return 
*/
void Leftward(void)
{
		AIN1 =1;
		AIN2 =0;
		BIN1 =0;
		BIN2 =1; 
	 // TIM_SetCompare4(TIM8,1200);

}	
void Stop(void)
{
		AIN1 =0;
		AIN2 =0;
		BIN1 =0;
		BIN2 =0; 
}	
 /**
* @brief 红外循迹四个GPIO的初始化
* @param 
* @return 
*/
void TCRT5000_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能时钟
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //默认下拉	  
	GPIO_Init(GPIOC, &GPIO_InitStructure);


}
 
void hongwai(void)  //红外对管循迹
{
//检测调速功能完好，就差加按键了，可以尝试用函数封装
			if(HW_1 == 0 && HW_2 == 0 && HW_3 == 0 && HW_4 == 0)
			{
				Forward();
				TIM_SetCompare4(TIM8,800);
				//delay_ms(50);
			}
			if(HW_1 == 0 && HW_2 == 1 && HW_3 == 0 && HW_4 == 0)
			{
				Leftward();
				TIM_SetCompare4(TIM8,800);
				//delay_ms(50);
			}
						else if(HW_1 == 1 && HW_2 == 0 && HW_3 == 0 && HW_4 == 0)
			{ 
				Leftward();
				TIM_SetCompare4(TIM8,800);
				//delay_ms(250);
			}
		else	if(HW_1 == 1 && HW_2 == 1 && HW_3 == 0 && HW_4 == 0)
			{
				Leftward();
				TIM_SetCompare4(TIM8,800);
				//delay_ms(300);
			}
			else if(HW_1 == 0 && HW_2 == 0 && HW_3 == 1 && HW_4 == 0)
			{
				Rightward();
				TIM_SetCompare4(TIM8,800);
			//	delay_ms(150);
			}
			else if(HW_1 == 0 && HW_2 == 0 && HW_3 == 0 && HW_4 == 1)
			{
				Rightward();
				TIM_SetCompare4(TIM8,800);
				//delay_ms(250);
			}
		else	if(HW_1 == 0 && HW_2 == 0 && HW_3 == 1 && HW_4 == 1)
			{
				Rightward();
				TIM_SetCompare4(TIM8,800);
				//delay_ms(300);
			}
				else	//(HW_1 == 1 && HW_2 == 1 && HW_3 == 1 && HW_4 == 1)
			{
						AIN1 =0;
						AIN2 =0;
						BIN1 =0;
						BIN2 =0; 
				//TIM_SetCompare4(TIM8,800);
				//delay_ms(300);
			}
	
			//红外对管循迹

//			else 
//			{
//				Forward();
//				TIM_SetCompare4(TIM8,800);
//			}


}

		
void sr04_bizhang(void)
{

		    //超声波避障
					TIM_SetCompare3(TIM3,46);	 //舵机向前 使超声波朝前方 
					delay_ms(150);
					if(SR04_Distance()>35)// 前方无障碍物
					{  
						  TIM_SetCompare4(TIM8,1000);
							Forward();
							delay_ms(350);
					}
					
					if(SR04_Distance()<35)  //向前有障碍物
					{     beep();
								TIM_SetCompare3(TIM3,18);	 //舵机向右边转大约30度
								delay_ms(200);
								if(SR04_Distance()>35)//右侧无障碍物判断
								{  
									  TIM_SetCompare4(TIM8,1000);
										 Rightward();
									   LED2_ON	;
										delay_ms(700);
									LED2_OFF;    //右转绿色
									delay_ms(300);
								}
								else {   //右边有障碍物
										TIM_SetCompare3(TIM3,74); //舵机向左边转大约30度
									
										delay_ms(200);
										if(SR04_Distance()>35)//左侧无障碍物
										{     TIM_SetCompare4(TIM8,1000);
													Leftward();
												  LED3_ON	;
													delay_ms(700);	
												  LED3_OFF;    //左转蓝色
						        			delay_ms(300);
										}
										else{
											    beep();
											    TIM_SetCompare4(TIM8,1000);
													Backward();//后退
													delay_ms(700);
											TIM_SetCompare4(TIM8,1000);
													Rightward(); //右转
											     LED2_ON	;
													delay_ms(700);
											   LED2_OFF	;
												delay_ms(300);
										     }											
							 }
					}								


}
