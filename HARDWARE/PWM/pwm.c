#include "pwm.h"
#include "led.h"
#include "usart.h"
 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ��PWM ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


//TIM14 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM14_PWM_Init(u32 arr,u32 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//ʹ��PORTFʱ��	
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); //GPIOF9����Ϊ��ʱ��14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //��ʼ��PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//��ʼ����ʱ��14
	
	//��ʼ��TIM14 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1

	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM14, ENABLE);  //ʹ��TIM14
 
										  
}  
void TIM1_PWM_Init(u32 arr,u32 psc)
{		 					 
   //�˲������ֶ��޸�IO������
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_Init(GPIOA,&GPIO_InitStructure);

   GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);

   //ʱ����ʼ��
   TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //���������á�
   TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //����������
   TIM_TimeBaseInitStructure.TIM_Prescaler = psc;   //Timer clock = sysclock /(TIM_Prescaler+1) = 168M
   TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInitStructure.TIM_Period = arr;    //Period = (TIM counter clock / TIM output clock) - 1 = 20K
   TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);

   //��ʼ��TIM14 Channel1 PWMģʽ	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
   TIM_OC1Init(TIM1,&TIM_OCInitStructure);
   TIM_Cmd(TIM1,ENABLE);
   TIM_CtrlPWMOutputs(TIM1,ENABLE);
} 

//TIM13 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM8_PWM_Init(u32 arr,u32 psc)
{		 					 
   //�˲������ֶ��޸�IO������
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_Init(GPIOC,&GPIO_InitStructure);

   GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
   //ʱ����ʼ��
   TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //���������á�
   TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //����������
   TIM_TimeBaseInitStructure.TIM_Prescaler = psc;   //Timer clock = sysclock /(TIM_Prescaler+1) = 168M
   TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInitStructure.TIM_Period = arr;    //Period = (TIM counter clock / TIM output clock) - 1 = 20K
   TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStructure);

   //��ʼ��TIM14 Channel1 PWMģʽ	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
   TIM_OC1Init(TIM8,&TIM_OCInitStructure);
   TIM_Cmd(TIM8,ENABLE);
   TIM_CtrlPWMOutputs(TIM8,ENABLE);
} 

//TIM13 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM9_PWM_Init(u32 arr,u32 psc)
{		 					 
   //�˲������ֶ��޸�IO������
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_Init(GPIOA,&GPIO_InitStructure);

   GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM9);

   //ʱ����ʼ��
   TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //���������á�
   TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //����������
   TIM_TimeBaseInitStructure.TIM_Prescaler = psc;   //Timer clock = sysclock /(TIM_Prescaler+1) = 168M
   TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInitStructure.TIM_Period = arr;    //Period = (TIM counter clock / TIM output clock) - 1 = 20K
   TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);

   //��ʼ��TIM14 Channel1 PWMģʽ	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
   TIM_OC1Init(TIM9,&TIM_OCInitStructure);
   TIM_Cmd(TIM9,ENABLE);
   TIM_CtrlPWMOutputs(TIM9,ENABLE);
} 
//TIM13 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM10_PWM_Init(u32 arr,u32 psc)
{		 					 
   //�˲������ֶ��޸�IO������
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_Init(GPIOB,&GPIO_InitStructure);

   GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM10);

   //ʱ����ʼ��
   TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //���������á�
   TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //����������
   TIM_TimeBaseInitStructure.TIM_Prescaler = psc;   //Timer clock = sysclock /(TIM_Prescaler+1) = 168M
   TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInitStructure.TIM_Period = arr;    //Period = (TIM counter clock / TIM output clock) - 1 = 20K
   TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStructure);

   //��ʼ��TIM14 Channel1 PWMģʽ	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
   TIM_OC1Init(TIM10,&TIM_OCInitStructure);
   TIM_Cmd(TIM10,ENABLE);
   TIM_CtrlPWMOutputs(TIM10,ENABLE);
} 
//TIM13 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM11_PWM_Init(u32 arr,u32 psc)
{		 					 
   //�˲������ֶ��޸�IO������
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_Init(GPIOB,&GPIO_InitStructure);

   GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM11);

   //ʱ����ʼ��
   TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //���������á�
   TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //����������
   TIM_TimeBaseInitStructure.TIM_Prescaler = psc;   //Timer clock = sysclock /(TIM_Prescaler+1) = 168M
   TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInitStructure.TIM_Period = arr;    //Period = (TIM counter clock / TIM output clock) - 1 = 20K
   TIM_TimeBaseInit(TIM11,&TIM_TimeBaseInitStructure);

   //��ʼ��TIM14 Channel1 PWMģʽ	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
   TIM_OC1Init(TIM11,&TIM_OCInitStructure);
   TIM_Cmd(TIM11,ENABLE);
   TIM_CtrlPWMOutputs(TIM11,ENABLE);
} 





//TIM13 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM2_PWM_Init(u32 arr,u32 psc)
{		 					 
   //�˲������ֶ��޸�IO������
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//TIM14ʱ��ʹ��    
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTFʱ��	

   GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2); //GPIOF9����Ϊ��ʱ��14

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;           //GPIOF9
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
   GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PF9

   TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
   TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

   TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//��ʼ����ʱ��14

   //��ʼ��TIM14 Channel1 PWMģʽ	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1; 
   TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
   TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
  
   TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPEʹ�� 

   TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM14
} 
//TIM13 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u32 arr,u32 psc)
{		 					 
   //�˲������ֶ��޸�IO������
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM14ʱ��ʹ��    
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTFʱ��	

   GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3); //GPIOF9����Ϊ��ʱ��14

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;           //GPIOF9
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
   GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PF9

   TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
   TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

   TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ����ʱ��14

   //��ʼ��TIM14 Channel1 PWMģʽ	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1; 
   TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
   TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
  
   TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPEʹ�� 

   TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM14
} 
//TIM13 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM4_PWM_Init(u32 arr,u32 psc)
{		 					 
   //�˲������ֶ��޸�IO������
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM14ʱ��ʹ��    
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTFʱ��	

   GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOF9����Ϊ��ʱ��14

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOF9
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
   GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PF9

   TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
   TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

   TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//��ʼ����ʱ��14

   //��ʼ��TIM14 Channel1 PWMģʽ	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1; 
   TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
   TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
  
   TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ�� 

   TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM14
}  
//TIM13 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM5_PWM_Init(u32 arr,u32 psc)
{		 					 
   //�˲������ֶ��޸�IO������
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM14ʱ��ʹ��    
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTFʱ��	

   GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //GPIOF9����Ϊ��ʱ��14

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOF9
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
   GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PF9

   TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
   TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

   TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//��ʼ����ʱ��14

   //��ʼ��TIM14 Channel1 PWMģʽ	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1; 
   TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
   TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
  
   TIM_ARRPreloadConfig(TIM5,ENABLE);//ARPEʹ�� 

   TIM_Cmd(TIM5, ENABLE);  //ʹ��TIM14
}  
//TIM13 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM12_PWM_Init(u32 arr,u32 psc)
{		 					 
   //�˲������ֶ��޸�IO������
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  	//TIM14ʱ��ʹ��    
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTFʱ��	

   GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM12); //GPIOF9����Ϊ��ʱ��14

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;           //GPIOF9
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
   GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PF9

   TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
   TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

   TIM_TimeBaseInit(TIM12,&TIM_TimeBaseStructure);//��ʼ����ʱ��14

   //��ʼ��TIM14 Channel1 PWMģʽ	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1; 
   TIM_OC1Init(TIM12, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
   TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
  
   TIM_ARRPreloadConfig(TIM12,ENABLE);//ARPEʹ�� 

   TIM_Cmd(TIM12, ENABLE);  //ʹ��TIM14
}  
//TIM13 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM13_PWM_Init(u32 arr,u32 psc)
{		 					 
   //�˲������ֶ��޸�IO������
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  	//TIM14ʱ��ʹ��    
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTFʱ��	

   GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM13); //GPIOF9����Ϊ��ʱ��14

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOF9
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
   GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PF9

   TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
   TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

   TIM_TimeBaseInit(TIM13,&TIM_TimeBaseStructure);//��ʼ����ʱ��14

   //��ʼ��TIM14 Channel1 PWMģʽ	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1; 
   TIM_OC1Init(TIM13, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
   TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
  
   TIM_ARRPreloadConfig(TIM13,ENABLE);//ARPEʹ�� 

   TIM_Cmd(TIM13, ENABLE);  //ʹ��TIM14
}  


//TIM14 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��

