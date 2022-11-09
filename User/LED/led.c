/**
************************************************************
* @file         
* @brief        
* @author    lqq
* @date       2022.10.3
* @version  ���V1.4 Ӳ��V1.4
* @note     ����GPIO�ĳ�ʼ����
*
*
*
***********************************************************/
#include "led.h"
#include "timer.h"
#include "delay.h"
#include "bsp_beep.h"
/**
* @brief ��ʼLED 
* @param 
* @return 
*/
void LED_Init(void)
{
 
/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd( LED1_GPIO_CLK | LED2_GPIO_CLK | LED3_GPIO_CLK, ENABLE);
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
		
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
		
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;

		/*���ÿ⺯������ʼ��GPIOF*/
		GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

		/* �ر�����led��	*/
		GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
		
		/* �ر�����led��	*/
		GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);	 
    
    /* �ر�����led��	*/
		GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
}
/**
* @brief ��ʼTB6612�� AIN1 AIN2 BIN1 BIN2
* @brief ����AIN1--PB13 AIN2-PB12
						 BIN1--PB1  BIN2--PB0
* @param 
* @return 
*/
void TB6612_GPIO_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	    		 //LED1-->PE.5 �˿�����, �������
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15); 						 //PE.5 ����� 
}



/**
* @brief ������SR04Trig GPIO��ʼ�� 
* @param 
* @return 
*/
void SR04_GPIO_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //PA0�˿�����, �������
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOA,GPIO_Pin_2);
}
/**
* @brief ��ǰ����ʱ
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
* @brief �������ʱ
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
* @brief ��������ʱ
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
* @brief ��������ʱ
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
* @brief ����ѭ���ĸ�GPIO�ĳ�ʼ��
* @param 
* @return 
*/
void TCRT5000_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��ʱ��
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //Ĭ������	  
	GPIO_Init(GPIOC, &GPIO_InitStructure);


}
 
void hongwai(void)  //����Թ�ѭ��
{
//�����ٹ�����ã��Ͳ�Ӱ����ˣ����Գ����ú�����װ
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
	
			//����Թ�ѭ��

//			else 
//			{
//				Forward();
//				TIM_SetCompare4(TIM8,800);
//			}


}

		
void sr04_bizhang(void)
{

		    //����������
					TIM_SetCompare3(TIM3,46);	 //�����ǰ ʹ��������ǰ�� 
					delay_ms(150);
					if(SR04_Distance()>35)// ǰ�����ϰ���
					{  
						  TIM_SetCompare4(TIM8,1000);
							Forward();
							delay_ms(350);
					}
					
					if(SR04_Distance()<35)  //��ǰ���ϰ���
					{     beep();
								TIM_SetCompare3(TIM3,18);	 //������ұ�ת��Լ30��
								delay_ms(200);
								if(SR04_Distance()>35)//�Ҳ����ϰ����ж�
								{  
									  TIM_SetCompare4(TIM8,1000);
										 Rightward();
									   LED2_ON	;
										delay_ms(700);
									LED2_OFF;    //��ת��ɫ
									delay_ms(300);
								}
								else {   //�ұ����ϰ���
										TIM_SetCompare3(TIM3,74); //��������ת��Լ30��
									
										delay_ms(200);
										if(SR04_Distance()>35)//������ϰ���
										{     TIM_SetCompare4(TIM8,1000);
													Leftward();
												  LED3_ON	;
													delay_ms(700);	
												  LED3_OFF;    //��ת��ɫ
						        			delay_ms(300);
										}
										else{
											    beep();
											    TIM_SetCompare4(TIM8,1000);
													Backward();//����
													delay_ms(700);
											TIM_SetCompare4(TIM8,1000);
													Rightward(); //��ת
											     LED2_ON	;
													delay_ms(700);
											   LED2_OFF	;
												delay_ms(300);
										     }											
							 }
					}								


}
