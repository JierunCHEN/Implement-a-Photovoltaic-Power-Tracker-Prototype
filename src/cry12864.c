/*
 * cry12864.c
 *
 *  Created on: 2016-7-21
 *      Author: JierunChan
 */
#include <msp430.h>
typedef unsigned char uchar;
typedef unsigned int  uint;

extern int Mode,Hour_H,Hour_L,Mint_H,Mint_L,Sec_H,Sec_L;
extern float Power;
extern long Power_All;
extern int Pursuit_time[5];

#define LCD_DataIn    P8DIR=0x00    //数据口方向设置为输入
#define LCD_DataOut   P8DIR=0xff    //数据口方向设置为输出
#define LCD2MCU_Data  P8IN
#define MCU2LCD_Data  P8OUT
#define LCD_CMDOut    P3DIR|=(BIT0+BIT4+BIT5)     //P3口的相应三位设置为输出
#define LCD_RS_L            P3OUT&=~BIT0          //RS = P3.0
#define LCD_RS_H            P3OUT|=BIT0
#define LCD_RW_L            P3OUT&=~BIT5           //RW = P3.5
#define LCD_RW_H            P3OUT|=BIT5
#define LCD_EN_L            P3OUT&=~BIT4	       //EN = P3.4
#define LCD_EN_H            P3OUT|=BIT4


/*******************************************
函数名称：Delay_1ms
功    能：延时约1ms的时间
参    数：无
返回值  ：无
********************************************/
void Delay_1ms(void)
{
	uchar i;

	for(i = 150;i > 0;i--)  _NOP();
}
/*******************************************
函数名称：Delay_Nms
功    能：延时N个1ms的时间
参    数：n--延时长度
返回值  ：无
********************************************/
void Delay_Nms(uint n)
{
    uint i;

    for(i = n;i > 0;i--)    Delay_1ms();
}
/*******************************************
函数名称：Write_Cmd
功    能：向液晶中写控制命令
参    数：cmd--控制命令
返回值  ：无
********************************************/
void Write_Cmd(uchar cmd)
{
    uchar lcdtemp = 0;

    LCD_RS_L;
    LCD_RW_H;
    LCD_DataIn;
    do                       //判忙
    {
        LCD_EN_H;
       	_NOP();
       	lcdtemp = LCD2MCU_Data;
       	LCD_EN_L;

    }
    while(lcdtemp & 0x80);

    LCD_DataOut;
    LCD_RW_L;
    MCU2LCD_Data = cmd;
    LCD_EN_H;
    _NOP();
    LCD_EN_L;
}
/*******************************************
函数名称：Write_Data
功    能：向液晶中写显示数据
参    数：dat--显示数据
返回值  ：无
********************************************/
void  Write_Data(uchar dat)
{
    uchar lcdtemp = 0;

    LCD_RS_L;
    LCD_RW_H;
    LCD_DataIn;
    do                       //判忙
    {
        LCD_EN_H;
        _NOP();
        lcdtemp = LCD2MCU_Data;
        LCD_EN_L;
    }
    while(lcdtemp & 0x80);

    LCD_DataOut;
    LCD_RS_H;
    LCD_RW_L;

    MCU2LCD_Data = dat;
    LCD_EN_H;
    _NOP();
    LCD_EN_L;
}
/*******************************************
函数名称：Ini_Lcd
功    能：初始化液晶模块
参    数：无
返回值  ：无
********************************************/
void Ini_Lcd(void)
{
    LCD_CMDOut;    //液晶控制端口设置为输出

   // Delay_Nms(500);
    Write_Cmd(0x30);   //基本指令集
    Delay_1ms();
    Write_Cmd(0x02);   // 地址归位
    Delay_1ms();
    Write_Cmd(0x0c);   //整体显示打开,游标关闭
    Delay_1ms();
    Write_Cmd(0x01);   //清除显示
    Delay_1ms();
    Write_Cmd(0x06);   //游标右移
    Delay_1ms();
    Write_Cmd(0x80);   //设定显示的起始地址
}
/*******************************************
函数名称:Clear_GDRAM
功    能:清除液晶GDRAM中的随机数据
参    数:无
返回值  :无
********************************************/
void Clear_GDRAM(void)
{
    uchar i,j,k;

	Write_Cmd(0x34);        //打开扩展指令集
	i = 0x80;
	for(j = 0;j < 32;j++)
	{
        Write_Cmd(i++);
        Write_Cmd(0x80);
  		for(k = 0;k < 16;k++)
  		{
  		    Write_Data(0x00);
  		}
	}
	i = 0x80;
 	for(j = 0;j < 32;j++)
	{
 	    Write_Cmd(i++);
        Write_Cmd(0x88);
  		for(k = 0;k < 16;k++)
  		{
   		    Write_Data(0x00);
   		}
	}
	Write_Cmd(0x30);        //回到基本指令集
}
/*******************************************
函数名称：Draw_TX
功    能：显示一个16*16大小的图形
参    数：Yaddr--Y地址
          Xaddr--X地址
          dp--指向图形数据存放地址的指针
返回值  ：无
********************************************/
void Draw_TX(uchar Yaddr,uchar Xaddr,const uchar * dp)
{
    uchar j;
    uchar k=0;

   // Write_Cmd(0x01);  //清屏,只能清除DDRAM
    Write_Cmd(0x34);  //使用扩展指令集，关闭绘图显示
    for(j=0;j<16;j++)
    {
        Write_Cmd(Yaddr++);       //Y地址
        Write_Cmd(Xaddr);    //X地址
  	Write_Data(dp[k++]);
  	Write_Data(dp[k++]);
    }
    Write_Cmd(0x36);  //打开绘图显示
  //  Write_Cmd(0x30);  //回到基本指令集模式
}
void Display(void){
	int i;
	char Chinese_0[5]="时间:";
	char Chinese_1[9]="瞬时功率:";
	char Chinese_2[7]="发电量:";
	char Chinese_3[8]="模拟跟踪";
	char Chinese_4[9]="跟踪时间:";
	float Power_temp = Power;
	long beishu=1;
	if(!Mode){
	    Write_Cmd(0x80);            //写第一行的显示地址
	    for(i = 0; i < 5; i++)
	    	Write_Data(Chinese_0[i]);     //显示时间:
	    Write_Data(0x30+Hour_H);
	    Write_Data(0x30+Hour_L);
	    Write_Data(':');
	    Write_Data(0x30+Mint_H);
	    Write_Data(0x30+Mint_L);
	    Write_Data(':');
	    Write_Data(0x30+Sec_H);
	    Write_Data(0x30+Sec_L);

	    Write_Cmd(0x90);            //写第二行的显示地址
	    for(i = 0; i < 9; i++)
	    	Write_Data(Chinese_1[i]);     //显示瞬时功率:

	    while(Power_temp>=beishu){
	    	beishu *= 10;
	    }
	    beishu /= 10;
	    while(beishu >= 1){
	    	Write_Data(0x30+(int)(Power_temp/beishu)%10);
	    	beishu/=10;
	    }
	    Write_Data('.');
	    Write_Data(0x30+((int)(Power*10))%10);
	    Write_Data('W');

	    Write_Cmd(0x88);            //写第三行的显示地址
	    for(i = 0; i < 7; i++)
	    	Write_Data(Chinese_2[i]);     //显示发电量:
	    beishu=1;
	    Power_temp=Power_All;
	    while(Power_temp>=beishu){
	    	beishu *= 10;
	    }
	    beishu /= 10;
	    while(beishu >= 1){
	    	Write_Data(0x30+(int)(Power_temp/beishu)%10);
	    	beishu/=10;
	    }

	    Write_Data('W');
	    Write_Data('S');
	}
	else{
	    Write_Cmd(0x92);            //写第二行的显示地址
	    for(i = 0; i < 8; i++)
	    	Write_Data(Chinese_3[i]);     //显示模拟跟踪

	    Write_Cmd(0x88);            //写第三行的显示地址
	    for(i = 0; i < 9; i++)
	    	Write_Data(Chinese_4[i]);     //显示跟踪时间:
	    if(Pursuit_time[4]==0){
	    	if(Pursuit_time[3]==0){
	    		if(Pursuit_time[2]==0){
	    			Write_Data(0x30+Pursuit_time[1]);
	    		}
	    		else{
	    			Write_Data(0x30+Pursuit_time[2]);
	    			Write_Data(0x30+Pursuit_time[1]);
	    		}
	    	}
	    	else{
	    		Write_Data(0x30+Pursuit_time[3]);
	    		Write_Data(0x30+Pursuit_time[2]);
    			Write_Data(0x30+Pursuit_time[1]);
	    	}
	    }
	    else{
	    	Write_Data(0x30+Pursuit_time[4]);
    		Write_Data(0x30+Pursuit_time[3]);
    		Write_Data(0x30+Pursuit_time[2]);
			Write_Data(0x30+Pursuit_time[1]);
	    }
	    Write_Data('.');
	    Write_Data(0x30+Pursuit_time[0]);
	    Write_Data('S');
	}
}


