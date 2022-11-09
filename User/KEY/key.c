#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "led.h"
//按键初始化函数
/**
* @brief 按键初始化函数
* @param 
* @return 
*/
int Mode=0;
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化PA0

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成输入，默认下拉	  
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化PC13

}


//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY_UP!!
//u8 KEY_Scan(u8 mode)
//{	 
//	static u8 key_up=1;//按键按松开标志
//	if(mode)key_up=1;  //支持连按		  
//	if(key_up&&(KEY1==1||KEY2==0))
//	{
//		delay_ms(10);//去抖动 
//		key_up=0;
//		return WKUP_PRES;
//	}else if(KEY1==0&&KEY2==1)return WKDN_PRES;; 	    
// 	return 0;// 无按键按下
//}
void key_fun(void)

{
			if(KEY1==1)	 //按键KEY1 PA0
	{
		delay_ms(10);//消抖
		if(Mode == 6) Mode =1;     //加速按键
		else Mode = Mode +1;  
	}		 
		if(KEY2==1)	 //按键KEY2 PA13
	{
		delay_ms(10);//消抖
		if(Mode == 0) Mode = 4;     //减速速按键
		else Mode = Mode - 1;  
	}		
	
	switch(Mode)
	{
		
	case 0:AIN1 =0;AIN2 =0;BIN1 =0;BIN2 =0;break;
	case 1:Forward();TIM_SetCompare4(TIM8,1500);break;
	case 2:Forward();TIM_SetCompare4(TIM8,1200);break;
	case 3:Forward();TIM_SetCompare4(TIM8,900);break;
	case 4:Forward(); TIM_SetCompare4(TIM8,600);break;
	case 5:Forward();TIM_SetCompare4(TIM8,300);break;
	default:Forward(); TIM_SetCompare4(TIM8,1000);break;
		
	}
}


//			if(KEY1==1)	 //按键KEY1 PA0
//	{
//		delay_ms(10);//消抖
//  	TIM_SetCompare1(TIM3,74);	 //舵机向左
//	}		
//				if(KEY2==1)	 //按键KEY2 
//	{
//		delay_ms(10);//消抖
//  	TIM_SetCompare1(TIM3,18);	 //舵机向右 
//	}	
