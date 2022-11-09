#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "led.h"
//������ʼ������
/**
* @brief ������ʼ������
* @param 
* @return 
*/
int Mode=0;
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PA0

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó����룬Ĭ������	  
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��PC13

}


//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY_UP!!
//u8 KEY_Scan(u8 mode)
//{	 
//	static u8 key_up=1;//�������ɿ���־
//	if(mode)key_up=1;  //֧������		  
//	if(key_up&&(KEY1==1||KEY2==0))
//	{
//		delay_ms(10);//ȥ���� 
//		key_up=0;
//		return WKUP_PRES;
//	}else if(KEY1==0&&KEY2==1)return WKDN_PRES;; 	    
// 	return 0;// �ް�������
//}
void key_fun(void)

{
			if(KEY1==1)	 //����KEY1 PA0
	{
		delay_ms(10);//����
		if(Mode == 6) Mode =1;     //���ٰ���
		else Mode = Mode +1;  
	}		 
		if(KEY2==1)	 //����KEY2 PA13
	{
		delay_ms(10);//����
		if(Mode == 0) Mode = 4;     //�����ٰ���
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


//			if(KEY1==1)	 //����KEY1 PA0
//	{
//		delay_ms(10);//����
//  	TIM_SetCompare1(TIM3,74);	 //�������
//	}		
//				if(KEY2==1)	 //����KEY2 
//	{
//		delay_ms(10);//����
//  	TIM_SetCompare1(TIM3,18);	 //������� 
//	}	
