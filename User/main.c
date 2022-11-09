#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "rc522_config.h"
#include "rc522_function.h"
#include <stdbool.h>
#include "bsp_lcd.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "timer.h"
#include "exti.h"
#include "key.h"
#include "adc.h"
#include "bsp_ds18b20.h"
#include "bh1750.h"
#include "bsp_beep.h"
void LCD_test ( void );
void Printf_Charater(void);
extern u8  TIM5CH3_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM5CH3_CAPTURE_VAL;	//���벶��ֵ	

uint8_t KeyValue[]={0xFF ,0xFF, 0xFF, 0xFF, 0xFF, 0xFF};   // ��A��Կ
int Distance = 0;
int time = 0;
double temperature;
uint8_t  ucDs18b20Id [ 8 ];



/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main ( void )
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�	
	SysTick_Init ();     /*�δ�ʱ�ӳ�ʼ��*/
	EXTIX_Init();       //�ⲿ�ж� ��������	   
	USART_Config ();     /*USART ����ģʽΪ 115200 8-N-1���жϽ���*/
	RC522_Init ();      /*RC522ģ����������ĳ�ʼ������*/
	LCD_Init ();         /*LCD ��ʼ��*/
	DS18B20_Init();     //�¶ȴ�������ʼ��
	BH1750_Init();       //���մ�������ʼ��
	Adc_Init();		  	  //ADC��ʼ��	
	delay_init();	      //��ʱ������ʼ��	  
	LED_Init();		  	   //��ʼ����LED���ӵ�Ӳ���ӿ�
	TB6612_GPIO_Init();   //���������ʼ��
	SR04_GPIO_Init();     //������PA0
	TCRT5000_Init();       //����ѭ����ʼ��
	TIM8_PWM_Init(1999,359);     //�����PWM 
	TIM3_PWM_Init(999,1439);	    //���PWM 
  TIM2_Cap_Init(0XFFFF,72-1);	   //��1Mhz��Ƶ�ʼ��� 
	DS18B20_ReadId ( ucDs18b20Id );// ��ȡ DS18B20 �����к�
	TIM_SetCompare1(TIM3,46);	    //�����ǰ ʹ��������ǰ�� 
	//TIM_SetCompare1(TIM3,74);	   //��������45  ʹ����������
	//TIM_SetCompare1(TIM3,18);	   //������ҹ�45 ʹ����������
	 BEEP_GPIO_Config();
	PcdReset ();                  //��λRC522 
	M500PcdConfigISOType ( 'A' ); /*���ù�����ʽ*/
	ILI9341_Clear ( 0,   0, 240, 320, macBACKGROUND);	//��ILI9341��ʾ����ĳһ������ĳ����ɫ��������
  while ( 1 )
  {
		key_fun();
    /*lcd��ʾ����*/
    LCD_test ();   
		//hongwai();
    sr04_bizhang();

		
  }	    
}

/**
  * @brief  LCD��ʾ����
  * @param  ��
  * @retval ��
  */
void LCD_test ( void )
{
	
	u16 adcx;
	float Light = 0;                   //���ն�
	float temp;  			               //TFT ��ʾADC����
	adcx=Get_Adc_Average(ADC_Channel_4,10);
	temp=(float)adcx*(3.3/4096);
	
	char cStr_ID [ 30 ];
	char dispBuff_ID[10];
	char dispBuff_volt[10];
	char dispBuff_cur[10];
	char dispBuff_tep[10];         //�����¶Ȼ�����
	char dispBuff_lig[10];        
	char dispBuff_pos_x[10];      //��������ʾ������˳��
	char dispBuff_pos_y[10];
	char dispBuff_distance[10];
	char dispBuff_Mspeed[10];
	
	uint32_t  cycle_ID=12345;
  uint8_t ucArray_ID [4];    /*�Ⱥ���IC�������ͺ�UID(IC�����к�)*/                                                                                         
	uint8_t ucStatusReturn;      /*����״̬*/ 
	uint32_t  MotorSpeed=2000;      //���ת��
	double cycle_volt=temp;
	double cycle_cur=cycle_volt/2;
	double temperature;
	float cycle_lig;
	uint32_t cycle_pos_x=55;
	uint32_t cycle_pos_y=33;
	uint32_t distance=SR04_Distance();
	
	
	
	/********��ʾ���г�ΨһID��*******/
	  sprintf ( dispBuff_ID, "The bicycle ID is:%d",cycle_ID );
    ILI9341_DispString_EN ( 0,0, dispBuff_ID, macBACKGROUND, macYELLOW );	
	/********************************/
	
	
   /***********���ID��************/
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )
    {
      /*��ʧ���ٴ�Ѱ��*/
			ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ); //PICC_REQALL   PICC_REQIDL
    }
		if ( ucStatusReturn == MI_OK  )
		{
      /*����ײ�����ж��ſ������д��������Χʱ������ͻ���ƻ������ѡ��һ�Ž��в�����*/
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   
			{
				  PcdSelect(ucArray_ID);			
			   	PcdAuthState( PICC_AUTHENT1A, 0x11, KeyValue, ucArray_ID );//У������    
				  sprintf ( cStr_ID, "The battery ID is:%02X%02X%02X%02X",ucArray_ID [0], ucArray_ID [1], ucArray_ID [2],ucArray_ID [3] );
          ILI9341_DispString_EN ( 0,16, cStr_ID, macBACKGROUND, macYELLOW );	
          PcdHalt();
			}				
		}		
	/********************************/	
	

		
	/******** ��ص�ѹ *******/		
	sprintf ( dispBuff_volt, "battery voltage:%lfV",cycle_volt );
  ILI9341_DispString_EN ( 0,32, dispBuff_volt, macBACKGROUND, macYELLOW );	
	/********************************/		
	
		
	   /******** ��س�ŵ���� *******/		
		sprintf ( dispBuff_cur, "discharge current:%lfA",cycle_cur );
    ILI9341_DispString_EN ( 0,48, dispBuff_cur, macBACKGROUND, macYELLOW );	
	/********************************/		
	
	
		
			/******** ��ع������� *******/
			temperature=DS18B20_GetTemp_MatchRom(ucDs18b20Id);
		sprintf ( dispBuff_tep, "around temperature:%lfC",temperature );
    ILI9341_DispString_EN ( 0,80, dispBuff_tep, macBACKGROUND, macYELLOW );	
	/********************************/	
	
	
	
  	/******** ��ǰ�������� *******/
			if (!i2c_CheckDevice(BH1750_Addr))
			{
				Light = LIght_Intensity();
				cycle_lig=Light;
			}
			if(cycle_lig<=15)
		  { 
		   	LED1_ON;
				
			 }
			else	
			{	
				LED1_OFF;
			}
		sprintf ( dispBuff_lig, "around light:%lfcd",cycle_lig );
    ILI9341_DispString_EN ( 0,64, dispBuff_lig, macBACKGROUND, macYELLOW );	
		/********************************/	
		
			
		
			/******** GPSλ����Ϣx *******/
	  sprintf ( dispBuff_pos_x, "Location information X:%d",cycle_pos_x );
    ILI9341_DispString_EN ( 0,96, dispBuff_pos_x, macBACKGROUND, macYELLOW );	
		/********************************/	
	
			
				/******** GPSλ����Ϣy *******/
		 sprintf ( dispBuff_pos_y, "Location information X:%d",cycle_pos_y );
    ILI9341_DispString_EN ( 0,112, dispBuff_pos_y, macBACKGROUND, macYELLOW );	
		/********************************/	
		
				/******** �ϰ������ *******/
		  sprintf ( dispBuff_distance, "Distance of object: %dCM",distance );
    ILI9341_DispString_EN ( 0,128, dispBuff_distance, macBACKGROUND, macYELLOW );	
	/********************************/	
	
					/********�����ת��*******/
		sprintf ( dispBuff_Mspeed, "Motor speed: %dr/s",MotorSpeed );
    ILI9341_DispString_EN ( 0,144, dispBuff_Mspeed, macBACKGROUND, macYELLOW );	
	//�Ƿ���Ҫ������ʱ��
	/********************************/	
		
		 
}
