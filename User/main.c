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
extern u8  TIM5CH3_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM5CH3_CAPTURE_VAL;	//输入捕获值	

uint8_t KeyValue[]={0xFF ,0xFF, 0xFF, 0xFF, 0xFF, 0xFF};   // 卡A密钥
int Distance = 0;
int time = 0;
double temperature;
uint8_t  ucDs18b20Id [ 8 ];



/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main ( void )
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级	
	SysTick_Init ();     /*滴答时钟初始化*/
	EXTIX_Init();       //外部中断 驱动代码	   
	USART_Config ();     /*USART 配置模式为 115200 8-N-1，中断接收*/
	RC522_Init ();      /*RC522模块所需外设的初始化配置*/
	LCD_Init ();         /*LCD 初始化*/
	DS18B20_Init();     //温度传感器初始化
	BH1750_Init();       //光照传感器初始化
	Adc_Init();		  	  //ADC初始化	
	delay_init();	      //延时函数初始化	  
	LED_Init();		  	   //初始化与LED连接的硬件接口
	TB6612_GPIO_Init();   //电机驱动初始化
	SR04_GPIO_Init();     //超声波PA0
	TCRT5000_Init();       //红外循迹初始化
	TIM8_PWM_Init(1999,359);     //电机的PWM 
	TIM3_PWM_Init(999,1439);	    //舵机PWM 
  TIM2_Cap_Init(0XFFFF,72-1);	   //以1Mhz的频率计数 
	DS18B20_ReadId ( ucDs18b20Id );// 读取 DS18B20 的序列号
	TIM_SetCompare1(TIM3,46);	    //舵机向前 使超声波朝前方 
	//TIM_SetCompare1(TIM3,74);	   //舵机向左拐45  使超声波朝左
	//TIM_SetCompare1(TIM3,18);	   //舵机向右拐45 使超声波朝右
	 BEEP_GPIO_Config();
	PcdReset ();                  //复位RC522 
	M500PcdConfigISOType ( 'A' ); /*设置工作方式*/
	ILI9341_Clear ( 0,   0, 240, 320, macBACKGROUND);	//对ILI9341显示器的某一窗口以某种颜色进行清屏
  while ( 1 )
  {
		key_fun();
    /*lcd显示测试*/
    LCD_test ();   
		//hongwai();
    sr04_bizhang();

		
  }	    
}

/**
  * @brief  LCD显示函数
  * @param  无
  * @retval 无
  */
void LCD_test ( void )
{
	
	u16 adcx;
	float Light = 0;                   //光照度
	float temp;  			               //TFT 显示ADC测量
	adcx=Get_Adc_Average(ADC_Channel_4,10);
	temp=(float)adcx*(3.3/4096);
	
	char cStr_ID [ 30 ];
	char dispBuff_ID[10];
	char dispBuff_volt[10];
	char dispBuff_cur[10];
	char dispBuff_tep[10];         //测量温度缓冲区
	char dispBuff_lig[10];        
	char dispBuff_pos_x[10];      //代表着显示行排列顺序
	char dispBuff_pos_y[10];
	char dispBuff_distance[10];
	char dispBuff_Mspeed[10];
	
	uint32_t  cycle_ID=12345;
  uint8_t ucArray_ID [4];    /*先后存放IC卡的类型和UID(IC卡序列号)*/                                                                                         
	uint8_t ucStatusReturn;      /*返回状态*/ 
	uint32_t  MotorSpeed=2000;      //电机转速
	double cycle_volt=temp;
	double cycle_cur=cycle_volt/2;
	double temperature;
	float cycle_lig;
	uint32_t cycle_pos_x=55;
	uint32_t cycle_pos_y=33;
	uint32_t distance=SR04_Distance();
	
	
	
	/********显示自行车唯一ID号*******/
	  sprintf ( dispBuff_ID, "The bicycle ID is:%d",cycle_ID );
    ILI9341_DispString_EN ( 0,0, dispBuff_ID, macBACKGROUND, macYELLOW );	
	/********************************/
	
	
   /***********电池ID号************/
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )
    {
      /*若失败再次寻卡*/
			ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ); //PICC_REQALL   PICC_REQIDL
    }
		if ( ucStatusReturn == MI_OK  )
		{
      /*防冲撞（当有多张卡进入读写器操作范围时，防冲突机制会从其中选择一张进行操作）*/
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   
			{
				  PcdSelect(ucArray_ID);			
			   	PcdAuthState( PICC_AUTHENT1A, 0x11, KeyValue, ucArray_ID );//校验密码    
				  sprintf ( cStr_ID, "The battery ID is:%02X%02X%02X%02X",ucArray_ID [0], ucArray_ID [1], ucArray_ID [2],ucArray_ID [3] );
          ILI9341_DispString_EN ( 0,16, cStr_ID, macBACKGROUND, macYELLOW );	
          PcdHalt();
			}				
		}		
	/********************************/	
	

		
	/******** 电池电压 *******/		
	sprintf ( dispBuff_volt, "battery voltage:%lfV",cycle_volt );
  ILI9341_DispString_EN ( 0,32, dispBuff_volt, macBACKGROUND, macYELLOW );	
	/********************************/		
	
		
	   /******** 电池充放电电流 *******/		
		sprintf ( dispBuff_cur, "discharge current:%lfA",cycle_cur );
    ILI9341_DispString_EN ( 0,48, dispBuff_cur, macBACKGROUND, macYELLOW );	
	/********************************/		
	
	
		
			/******** 电池工作温升 *******/
			temperature=DS18B20_GetTemp_MatchRom(ucDs18b20Id);
		sprintf ( dispBuff_tep, "around temperature:%lfC",temperature );
    ILI9341_DispString_EN ( 0,80, dispBuff_tep, macBACKGROUND, macYELLOW );	
	/********************************/	
	
	
	
  	/******** 当前环境亮度 *******/
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
		
			
		
			/******** GPS位置信息x *******/
	  sprintf ( dispBuff_pos_x, "Location information X:%d",cycle_pos_x );
    ILI9341_DispString_EN ( 0,96, dispBuff_pos_x, macBACKGROUND, macYELLOW );	
		/********************************/	
	
			
				/******** GPS位置信息y *******/
		 sprintf ( dispBuff_pos_y, "Location information X:%d",cycle_pos_y );
    ILI9341_DispString_EN ( 0,112, dispBuff_pos_y, macBACKGROUND, macYELLOW );	
		/********************************/	
		
				/******** 障碍物距离 *******/
		  sprintf ( dispBuff_distance, "Distance of object: %dCM",distance );
    ILI9341_DispString_EN ( 0,128, dispBuff_distance, macBACKGROUND, macYELLOW );	
	/********************************/	
	
					/********电机的转速*******/
		sprintf ( dispBuff_Mspeed, "Motor speed: %dr/s",MotorSpeed );
    ILI9341_DispString_EN ( 0,144, dispBuff_Mspeed, macBACKGROUND, macYELLOW );	
	//是否需要加入延时？
	/********************************/	
		
		 
}
