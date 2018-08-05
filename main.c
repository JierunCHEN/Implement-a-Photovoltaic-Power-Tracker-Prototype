/*
 * Main.c
 *
 *  Created on: 2016-7-18
 *      Author: JierunChan
 */
#include <msp430.h> 
#include <Motor.h>
#include <ADS1118A.h>
#include<cry12864.h>
#include<key_event.h>
#include<clock.h>


int Mode=0;							//模式
int Hour_H=0,Hour_L=0,Mint_H=0,Mint_L=0,Sec_H=0,Sec_L=0;	//时钟标志
int Set_Times=0;		//设置次数标志
int State=0;			//启动1/停止0标志
float Power=0;			//显示发电功率
long Power_All=0;		//发电总量
int Pursuit_time[5]={0};	//跟踪时间
float Pre_Power=0;		//转换后发电功率
int Flash=50;			//光标标志位
int Flash_temp=0;		//光标标志位
float Integrate=0;
float differential=0;
float temp=0;
float temp2=0;
int Motor_Dir=0;
/*
 * main.c
 */
void main(void) {
	WDTCTL = WDTPW + WDTHOLD;	//关闭看门狗
	Motor_Init();			//初始化电机
	Ini_Lcd();			//初始化cry12864显示模块
	ADC_Init();			//初始化Internal_ADC
	ADS1118A_Init();		//初始化ADS1118A模块
	TA0CTL = TASSEL_1 + MC_1;   	//ACLK UP_MODE CLEAR
	TA0CCTL0 = CCIE;		//使能CCR0中断
	TA0CCR0 = 320;			//定时0.01s
	_enable_interrupts();		//使能总中断

	while(1){
		Display();				//LCD屏幕显示 
		key_change();				//键盘扫描 
		ADC12CTL0 |= ADC12SC;			//开始一次内部AD转换;
		if(Mode){				//判断处于发电模式还是跟踪模式 
			temp=ADS1118A_Act();		//读取光电检测结果并通过ADS1118转化 
			if(temp>=-0.03&&temp<=0.03)	//当光电检测结果小于偏差容限时，电机停止 
				Motor_Act(0);				
			else if(temp>=0.25)		//PD调节阀门，光电检测结果和目标值相差太多，电机全速运行 
				Motor_Act(80);
			else if(temp<=-0.25)
				Motor_Act(-80);
			else
				Motor_Act(Motor_Dir);	//当光电检测结果接近目标值时启动PD控制 
		}
	}
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void){
	clock_change();					//改变时钟显示值、并更改发电功率、发电总量显示值 
}

