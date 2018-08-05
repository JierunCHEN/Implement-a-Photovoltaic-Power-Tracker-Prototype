/*
 * key_event.c
 *
 *  Created on: 2016-7-21
 *      Author: JierunChan
 */

#include <msp430.h>
#define key_H_OUT     P7DIR|=0xF0   //键盘四行设为输出状态
#define key_L_IN      P2DIR=0x00   //键盘四列设为输入状态

extern int Mode,Hour_H,Hour_L,Mint_H,Mint_L;
extern int Set_Times;
extern int State;
extern int Flash;
extern int Flash_temp;
//*********************//
//名称：key_event
//功能：进行一次按键扫描，检测是否有按键摁下
//入口参数：无
//出口参数：keycode
//**********************//

unsigned int  key_event(void)
{
    unsigned int key_num;    		//当前扫描得到的按键的位置
	static unsigned int key_past;   //局部静态变量存放上次按键的位置
	static unsigned int get_newkey; //局部静态变量指示是否为新按键
    unsigned int key;              	//扫描得到的按键值



	key_H_OUT;
	key_L_IN;
    key_num=0;
	P7OUT=0x7F;        //第一行输出低电平
	if(0xE0==(P2IN&0xF0))
	{
		_delay_cycles(10000);
		if(0xE0==(P2IN&0xF0)){
		key_num=1;
		}
	}
	if(0xD0==(P2IN&0xF0))
		{
		_delay_cycles(10000);
		if(0xD0==(P2IN&0xF0)){
			key_num=2;
		}
		}
	if(0xB0==(P2IN&0xF0))
		{
		_delay_cycles(10000);
		if(0xB0==(P2IN&0xF0)){
			key_num=3;
		}
		}
	if(0x70==(P2IN&0xF0))
		{
		_delay_cycles(10000);
		if(0x70==(P2IN&0xF0)){
			key_num=4;
		}}


    P7OUT=0xBF;        //第二行输出低电平
    	if(0xE0==(P2IN&0xF0))
    	{
    		_delay_cycles(10000);
    		if(0xE0==(P2IN&0xF0)){
    		key_num=5;
    	}}
    	if(0xD0==(P2IN&0xF0))
    		{
    		_delay_cycles(10000);
    		if(0xD0==(P2IN&0xF0)){
    			key_num=6;
    		}}
    	if(0xB0==(P2IN&0xF0))
    		{
    		_delay_cycles(10000);
    		if(0xB0==(P2IN&0xF0)){
    			key_num=7;
    		}}
    	if(0x70==(P2IN&0xF0))
    		{
    		_delay_cycles(10000);
    		if(0x70==(P2IN&0xF0)){
    			key_num=8;
    		}}


        P7OUT=0xDF;        //第三行输出低电平
        	if(0xE0==(P2IN&0xF0))
        	{
        		_delay_cycles(10000);
        		if(0xE0==(P2IN&0xF0)){
        		key_num=9;
        	}}
        	if(0xD0==(P2IN&0xF0))
        		{
        		_delay_cycles(10000);
        		if(0xD0==(P2IN&0xF0)){
        			key_num=10;
        		}}
        	if(0xB0==(P2IN&0xF0))
        		{
        		_delay_cycles(10000);
        		if(0xB0==(P2IN&0xF0)){
        			key_num=11;
        		}}
        	if(0x70==(P2IN&0xF0))
        		{
        		_delay_cycles(10000);
        		if(0x70==(P2IN&0xF0)){
        			key_num=12;
        		}}


          P7OUT=0xEF;        //第四行输出低电平
            if(0xE0==(P2IN&0xF0))
            	{
            	_delay_cycles(10000);
            	if(0xE0==(P2IN&0xF0)){
            		key_num=13;
            	}}
            	if(0xD0==(P2IN&0xF0))
            		{
            		_delay_cycles(10000);
            		if(0xD0==(P2IN&0xF0)){
            			key_num=14;
            		}}
            	if(0xB0==(P2IN&0xF0))
            		{
            		_delay_cycles(10000);
            		if(0xB0==(P2IN&0xF0)){
            			key_num=15;
            		}}
            	if(0x70==(P2IN&0xF0))
            		{
            		_delay_cycles(10000);
            		if(0x70==(P2IN&0xF0)){
            			key_num=16;
            		}}


      //判断扫描得到摁键的情况
       if(key_num!=key_past)      //如果扫描得到的按键和上次不相同，则有一个新的按键摁下，经过一次中断去抖
       {
    	   get_newkey=1;
    	   key_past=key_num;
    	   key=0;             		// 本次keycode值为无效值
       }
       else{                       	//如果扫描得到的按键与上次相同，则判断是否为新按键
    	   if(get_newkey==1){
    		   get_newkey=0;        //如果为新按键，则清零get_newkey标志
    		   key=key_num;        	//新按键的键码赋值至keycode
    	   }
    	   else{
    		   key=0;               //如果是同一次按键，则keycode值无效
    	   }

       }
       return key;

}

void key_change(void){
	switch(key_event()){
	case 1:				//改变运行模式
		if(!Mode&&!State){
		    Write_Cmd(0x01);   	//清除显示
		    Delay_1ms();
			Mode=1;
		}
		else{
		    Write_Cmd(0x01);   	//清除显示
		    Delay_1ms();
			Mode=0;
			TBCCR3 = 0;
			TBCCR4 = 0;
		}
		break;
	case 2:
		if(!Mode){		//设置按键使能
			switch(Set_Times){
			case 1:
				Hour_H=Flash_temp;
				break;
			case 2:
				Hour_L=Flash_temp;
				break;
			case 3:
				Mint_H=Flash_temp ;
				break;
			case 4:
				Mint_L=Flash_temp;
				break;
			default:
				break;
			}

			Set_Times++;
			if(Set_Times==5)
				Set_Times=1;
			Flash=0;
			switch(Set_Times){
			case 1:
				Flash_temp = Hour_H;
				break;
			case 2:
				Flash_temp = Hour_L;
				break;
			case 3:
				Flash_temp = Mint_H;
				break;
			case 4:
				Flash_temp = Mint_L;
				break;
			default:
				break;
			}
		}
		break;
	case 3:
		if(!Mode&&(Set_Times!=0)){	//增加按键使能
			Flash=0;
			Flash_temp++;
			if(Flash_temp==10)
					Flash_temp=0;
		}
		break;
	case 4:
		if(!Mode&&(Set_Times!=0)){	//减小按键使能
			Flash=0;
			if(Flash_temp==0)
				Flash_temp=10;
			Flash_temp--;
		}
		break;
	case 5:
		State=1;
		Flash=50;
		switch(Set_Times){
		case 1:
			Hour_H=Flash_temp;
			break;
		case 2:
			Hour_L=Flash_temp;
			break;
		case 3:
			Mint_H=Flash_temp ;
			break;
		case 4:
			Mint_L=Flash_temp;
			break;
		default:
			break;
		}
		break;
	case 6:
		State=0;
		break;
	default:
		break;
	}
}





