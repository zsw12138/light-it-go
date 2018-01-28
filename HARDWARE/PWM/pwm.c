#include "pwm.h"
#include "led.h"
#include "usart.h"
 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器PWM 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


//TIM14 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM14_PWM_Init(u32 arr,u32 psc)
{		 					 
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); //GPIOF9复用为定时器14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //初始化PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//初始化定时器14
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM14, ENABLE);  //使能TIM14
 
										  
}  
void TIM1_PWM_Init(u32 arr,u32 psc)
{		 					 
   //此部分需手动修改IO口设置
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

   //时基初始化
   TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //死区控制用。
   TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数器方向
   TIM_TimeBaseInitStructure.TIM_Prescaler = psc;   //Timer clock = sysclock /(TIM_Prescaler+1) = 168M
   TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInitStructure.TIM_Period = arr;    //Period = (TIM counter clock / TIM output clock) - 1 = 20K
   TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);

   //初始化TIM14 Channel1 PWM模式	 
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

//TIM13 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM8_PWM_Init(u32 arr,u32 psc)
{		 					 
   //此部分需手动修改IO口设置
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
   //时基初始化
   TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //死区控制用。
   TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数器方向
   TIM_TimeBaseInitStructure.TIM_Prescaler = psc;   //Timer clock = sysclock /(TIM_Prescaler+1) = 168M
   TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInitStructure.TIM_Period = arr;    //Period = (TIM counter clock / TIM output clock) - 1 = 20K
   TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStructure);

   //初始化TIM14 Channel1 PWM模式	 
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

//TIM13 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM9_PWM_Init(u32 arr,u32 psc)
{		 					 
   //此部分需手动修改IO口设置
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

   //时基初始化
   TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //死区控制用。
   TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数器方向
   TIM_TimeBaseInitStructure.TIM_Prescaler = psc;   //Timer clock = sysclock /(TIM_Prescaler+1) = 168M
   TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInitStructure.TIM_Period = arr;    //Period = (TIM counter clock / TIM output clock) - 1 = 20K
   TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);

   //初始化TIM14 Channel1 PWM模式	 
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
//TIM13 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM10_PWM_Init(u32 arr,u32 psc)
{		 					 
   //此部分需手动修改IO口设置
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

   //时基初始化
   TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //死区控制用。
   TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数器方向
   TIM_TimeBaseInitStructure.TIM_Prescaler = psc;   //Timer clock = sysclock /(TIM_Prescaler+1) = 168M
   TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInitStructure.TIM_Period = arr;    //Period = (TIM counter clock / TIM output clock) - 1 = 20K
   TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStructure);

   //初始化TIM14 Channel1 PWM模式	 
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
//TIM13 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM11_PWM_Init(u32 arr,u32 psc)
{		 					 
   //此部分需手动修改IO口设置
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

   //时基初始化
   TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //死区控制用。
   TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数器方向
   TIM_TimeBaseInitStructure.TIM_Prescaler = psc;   //Timer clock = sysclock /(TIM_Prescaler+1) = 168M
   TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInitStructure.TIM_Period = arr;    //Period = (TIM counter clock / TIM output clock) - 1 = 20K
   TIM_TimeBaseInit(TIM11,&TIM_TimeBaseInitStructure);

   //初始化TIM14 Channel1 PWM模式	 
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





//TIM13 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM2_PWM_Init(u32 arr,u32 psc)
{		 					 
   //此部分需手动修改IO口设置
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//TIM14时钟使能    
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTF时钟	

   GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2); //GPIOF9复用为定时器14

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;           //GPIOF9
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
   GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PF9

   TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
   TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

   TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//初始化定时器14

   //初始化TIM14 Channel1 PWM模式	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1; 
   TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
   TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
  
   TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPE使能 

   TIM_Cmd(TIM2, ENABLE);  //使能TIM14
} 
//TIM13 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u32 arr,u32 psc)
{		 					 
   //此部分需手动修改IO口设置
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM14时钟使能    
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTF时钟	

   GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3); //GPIOF9复用为定时器14

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;           //GPIOF9
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
   GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PF9

   TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
   TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

   TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化定时器14

   //初始化TIM14 Channel1 PWM模式	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1; 
   TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
   TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
  
   TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 

   TIM_Cmd(TIM3, ENABLE);  //使能TIM14
} 
//TIM13 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM4_PWM_Init(u32 arr,u32 psc)
{		 					 
   //此部分需手动修改IO口设置
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM14时钟使能    
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTF时钟	

   GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOF9复用为定时器14

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOF9
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
   GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PF9

   TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
   TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

   TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化定时器14

   //初始化TIM14 Channel1 PWM模式	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1; 
   TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
   TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
  
   TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能 

   TIM_Cmd(TIM4, ENABLE);  //使能TIM14
}  
//TIM13 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM5_PWM_Init(u32 arr,u32 psc)
{		 					 
   //此部分需手动修改IO口设置
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM14时钟使能    
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTF时钟	

   GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //GPIOF9复用为定时器14

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOF9
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
   GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PF9

   TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
   TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

   TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//初始化定时器14

   //初始化TIM14 Channel1 PWM模式	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1; 
   TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
   TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
  
   TIM_ARRPreloadConfig(TIM5,ENABLE);//ARPE使能 

   TIM_Cmd(TIM5, ENABLE);  //使能TIM14
}  
//TIM13 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM12_PWM_Init(u32 arr,u32 psc)
{		 					 
   //此部分需手动修改IO口设置
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  	//TIM14时钟使能    
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTF时钟	

   GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM12); //GPIOF9复用为定时器14

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;           //GPIOF9
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
   GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PF9

   TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
   TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

   TIM_TimeBaseInit(TIM12,&TIM_TimeBaseStructure);//初始化定时器14

   //初始化TIM14 Channel1 PWM模式	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1; 
   TIM_OC1Init(TIM12, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
   TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
  
   TIM_ARRPreloadConfig(TIM12,ENABLE);//ARPE使能 

   TIM_Cmd(TIM12, ENABLE);  //使能TIM14
}  
//TIM13 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM13_PWM_Init(u32 arr,u32 psc)
{		 					 
   //此部分需手动修改IO口设置
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  	//TIM14时钟使能    
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTF时钟	

   GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM13); //GPIOF9复用为定时器14

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOF9
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
   GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PF9

   TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
   TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

   TIM_TimeBaseInit(TIM13,&TIM_TimeBaseStructure);//初始化定时器14

   //初始化TIM14 Channel1 PWM模式	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1; 
   TIM_OC1Init(TIM13, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
   TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
  
   TIM_ARRPreloadConfig(TIM13,ENABLE);//ARPE使能 

   TIM_Cmd(TIM13, ENABLE);  //使能TIM14
}  


//TIM14 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数

