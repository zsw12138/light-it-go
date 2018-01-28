#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"


//ALIENTEK 探索者STM32F407开发板 实验9
//PWM输出实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

int main(void)
{ 
	u16 led1=2000,led2=500,led3=250;
  	
	u8 dir=1;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);//初始化串口波特率为115200
 	 //TIM14_PWM_Init(500-1,84-1);	//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.   
   TIM1_PWM_Init(4000-1,84-1);//168M-PA8，灯一500hz
   //TIM8_PWM_Init(500-1,84-1);//168M-PC6
   //TIM9_PWM_Init(500-1,84-1);//168M-PA2
   //TIM10_PWM_Init(500-1,84-1);//168M-PB8
   //TIM11_PWM_Init(500-1,84-1);//168M-PB9
   TIM2_PWM_Init(1000-1,84-1);//84M-PA15，灯二1000hz
   TIM3_PWM_Init(500-1,84-1);//84M-PB4，灯三2000hz
   //TIM4_PWM_Init(500-1,84-1);//84M-PB6
   //TIM5_PWM_Init(500-1,84-1);//84M-PA0//丑
   //TIM12_PWM_Init(500-1,84-1);//84M-PB14
   //TIM13_PWM_Init(500-1,84-1);//84M-PA6
   //TIM14_PWM_Init(500-1,84-1);//84M-PA7  
   while(1) //实现比较值从0-300递增，到300后从300-0递减，循环
	{
 		TIM_SetCompare1(TIM1,led1);	//修改比较值，修改占空比
		TIM_SetCompare1(TIM2,led2);	//修改比较值，修改占空比
		TIM_SetCompare1(TIM3,led3);	//修改比较值，修改占空比
	}
}
