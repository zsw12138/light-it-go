#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"


//ALIENTEK ̽����STM32F407������ ʵ��9
//PWM���ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

int main(void)
{ 
	u16 led1=2000,led2=500,led3=250;
  	
	u8 dir=1;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);//��ʼ�����ڲ�����Ϊ115200
 	 //TIM14_PWM_Init(500-1,84-1);	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.   
   TIM1_PWM_Init(4000-1,84-1);//168M-PA8����һ500hz
   //TIM8_PWM_Init(500-1,84-1);//168M-PC6
   //TIM9_PWM_Init(500-1,84-1);//168M-PA2
   //TIM10_PWM_Init(500-1,84-1);//168M-PB8
   //TIM11_PWM_Init(500-1,84-1);//168M-PB9
   TIM2_PWM_Init(1000-1,84-1);//84M-PA15���ƶ�1000hz
   TIM3_PWM_Init(500-1,84-1);//84M-PB4������2000hz
   //TIM4_PWM_Init(500-1,84-1);//84M-PB6
   //TIM5_PWM_Init(500-1,84-1);//84M-PA0//��
   //TIM12_PWM_Init(500-1,84-1);//84M-PB14
   //TIM13_PWM_Init(500-1,84-1);//84M-PA6
   //TIM14_PWM_Init(500-1,84-1);//84M-PA7  
   while(1) //ʵ�ֱȽ�ֵ��0-300��������300���300-0�ݼ���ѭ��
	{
 		TIM_SetCompare1(TIM1,led1);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		TIM_SetCompare1(TIM2,led2);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		TIM_SetCompare1(TIM3,led3);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	}
}
