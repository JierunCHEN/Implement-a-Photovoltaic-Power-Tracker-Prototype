/*
 * Internal_ADC.c
 *
 *  Created on: 2016-7-22
 *      Author: JierunChan
 */
#include <msp430.h>
long Vout1,Vout2;
extern float Pre_Power;
void ADC_Init(){
	ADC12CTL0 &= ~ADC12ENC;			//关闭采样使能
	P6SEL |= BIT0 + BIT1;     		//选择P6.0,P6.1为模拟信号的输入端
	ADC12CTL0 |= ADC12ON + ADC12MSC + ADC12SHT0_2;		//开启ADC12，自动循环采样模式，设置采样保持时间为16个机器周期
	ADC12CTL0 |= REFMSTR + ADC12SREF_1 + ADC12REF2_5V + ADC12REFON; //使能电压管理，选择参考电压源，内部参考电压选择2.5V，打开内部参考电压
	ADC12CTL1 |= ADC12SHP + ADC12CONSEQ_1;		//使用采样定时器，单次顺序采样
	ADC12MCTL0 |= ADC12INCH_0; 			//选择采样通道A0
	ADC12MCTL1 |= ADC12INCH_1+ADC12EOS; 		//选择采样通道A1，设置采样顺序结点
	ADC12IE = 0x02; 				//使能ADC12IFG.1
	ADC12CTL0 |= ADC12ENC;    			//使能AD转换
}
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void) {
	Vout1 = ADC12MEM0;				//读取转换结果
	Vout2 = ADC12MEM1;
	Pre_Power = (1.0*Vout1*Vout2)/(25*625);
}




