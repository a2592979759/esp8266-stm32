#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//ALIENTEKս��STM32������
//��ʱ�� ��������	   		  
//********************************************************************************
extern 	int time ;
extern int Distance;
void TIM8_PWM_Init(u16 arr,u16 pac);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM2_Cap_Init(u16 arr,u16 psc);
int SR04_Distance(void);
#endif
