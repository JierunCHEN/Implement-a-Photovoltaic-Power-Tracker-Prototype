#include <msp430.h>
/*
 * Motor.c
 *
 *  Created on: 2016-7-21
 *      Author: JierunChan
 */
void Motor_Init(){
	P4DIR |= 0X18;			//选择TB0.3&TB.4口输出
	P4SEL |= 0X18;
	P7DIR |= BIT3;              	//低电平复位H桥PWM波dvr8412模块
	P7OUT &=~BIT3;
	P9DIR = 0x0E;			// 设置M0\M1\M2为第一种模式
	P9OUT &=~0x0E;
	P7DIR &=~BIT2;			// 错误保护标志/FAULT
	P2DIR &=~BIT1;			// 过热保护标志位/OTW
	P7OUT |=BIT3;			// 将/RESET拉高防止重复复位
	TBCCR0 = 100;			//设定周期为100个机器周期
	TBCTL = TBSSEL_1 + MC_1 + TBCLR;//选用32kHz，不分频，增计数
	TBCCTL3 |= OUTMOD_7;	    	//选择TB0.3为模式7
	TBCCTL4 |= OUTMOD_7;	    	//选择TB0.4为模式7
	return;
}
void Motor_Act(float Dir){		//Dir为光电检测模块两差模通道ch1和ch2的插值转化后的结果 
	if(Dir==0){			//若返回值0，电机停止运转 
		TBCCR3 = 0;
		TBCCR4 = 0;
	}
	else if(Dir>0){			//若返回值大于0，电机正转 
		if(Dir>70)
			Dir=70;
		TBCCR3 = 10+Dir;
		TBCCR4 = 0;
	}
	else {				//若返回值小于0，电机反转 
		if(Dir<-70)
			Dir=-70;
		TBCCR3 = 0;
		TBCCR4 = 10-Dir;
	}
	return;
}



