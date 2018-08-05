/*
 * clock.c
 *
 *  Created on: 2016-7-24
 *      Author: JierunChan
 */
extern int Mode,Hour_H,Hour_L,Mint_H,Mint_L,Sec_H,Sec_L;
extern int State;
extern float Power;
extern float Pre_Power;
extern long Power_All;
extern int Pursuit_time[5];
static int Inter_times=0;
extern int Flash;
extern int Set_Times;
extern int Flash_temp;

extern int Motor_Dir;

int ceshi_i=0;
extern float temp;
extern float temp2;
float ceshi[100]={0};
int clock_i=0;
extern float Integrate;
extern float differential;

void clock_change(void){
	int Hour_H_temp,Hour_L_temp,Mint_H_temp,Mint_L_temp,Sec_H_temp,Sec_L_temp;
	differential=70*(temp-temp2);
	temp2=temp;
//	Integrate+=(0.01887*temp2);
	Motor_Dir=(int)(20*(temp2+Integrate+differential));
	if((++clock_i)==10){
		clock_i=0;
		if(Flash!=50){
			Flash++;
			if(Flash==5||Flash==15||Flash==25||Flash==35||Flash==45){
				switch(Set_Times){
				case 1:
					Hour_H=(int)' '-0x30;
					break;
				case 2:
					Hour_L=(int)' '-0x30;
					break;
				case 3:
					Mint_H=(int)' '-0x30;
					break;
				case 4:
					Mint_L=(int)' '-0x30;
					break;
				default:
					break;
				}
			}
			else if(Flash==10||Flash==20||Flash==30||Flash==40||Flash==50){
				switch(Set_Times){
				case 1:
					Hour_H=Flash_temp;
					break;
				case 2:
					Hour_L=Flash_temp;
					break;
				case 3:
					Mint_H=Flash_temp;
					break;
				case 4:
					Mint_L=Flash_temp;
					break;
				default:
					break;
				}
			}
		}
		if(!Mode&&State){
			 Inter_times++;
			 if(Inter_times==10){
				 Inter_times=0;

				 Hour_H_temp=Hour_H;
				 Hour_L_temp=Hour_L;
				 Mint_H_temp=Mint_H;
				 Mint_L_temp=Mint_L;
				 Sec_H_temp=Sec_H;
				 Sec_L_temp=Sec_L;

					if((Hour_H!=0||Hour_L!=0||Mint_H!=0||Mint_L!=0||Sec_H!=0)&&Sec_L==0)//秒低位设置
						Sec_L_temp=9;
					else if(Hour_H==0&&Hour_L==0&&Mint_H==0&&Mint_L==0&&Sec_H==0&&Sec_L==0)
						Sec_L_temp=0;
					else
						Sec_L_temp--;

					if((Hour_H!=0||Hour_L!=0||Mint_H!=0||Mint_L!=0)&&Sec_H==0&&Sec_L==0)//秒高位设置
						Sec_H_temp=5;
					else if(Hour_H==0&&Hour_L==0&&Mint_H==0&&Mint_L==0&&Sec_H==0)
						Sec_H_temp=0;
					else if(Sec_H!=0&&Sec_L==0)
						Sec_H_temp--;

					if((Hour_H!=0||Hour_L!=0||Mint_H!=0)&&Mint_L==0&&Sec_H==0&&Sec_L==0)//分钟低位设置
						Mint_L_temp=9;
					else if(Hour_H==0&&Hour_L==0&&Mint_H==0&&Mint_L==0)
						Mint_L_temp=0;
					else if(Mint_L!=0&&Sec_H==0&&Sec_L==0)
						Mint_L_temp--;

					if((Hour_H!=0||Hour_L!=0)&&Mint_H==0&&Mint_L==0&&Sec_H==0&&Sec_L==0)//分钟高位设置
						Mint_H_temp=5;
					else if(Hour_H==0&&Hour_L==0&&Mint_H==0)
						Mint_H_temp=0;
					else if(Mint_H!=0&&Mint_L==0&&Sec_H==0&&Sec_L==0)
						Mint_H_temp--;

					if(Hour_H!=0&&Hour_L==0&&Mint_H==0&&Mint_L==0&&Sec_H==0&&Sec_L==0)//小时低位设置
						Hour_L_temp=9;
					else if(Hour_H==0&&Hour_L==0)
						Hour_L_temp=0;
					else if(Hour_L!=0&&Mint_H==0&&Mint_L==0&&Sec_H==0&&Sec_L==0)
						Hour_L_temp--;

					if(Hour_H!=0&&Hour_L==0&&Mint_H==0&&Mint_L==0&&Sec_H==0&&Sec_L==0)//小时高位设置
						Hour_H_temp--;

					 Hour_H=Hour_H_temp;
					 Hour_L=Hour_L_temp;
					 Mint_H=Mint_H_temp;
					 Mint_L=Mint_L_temp;
					 Sec_H=Sec_H_temp;
					 Sec_L=Sec_L_temp;


					if((Hour_H!=0||Hour_L!=0||Mint_H!=0||Mint_L!=0||Sec_H!=0||Sec_L!=0)){
						Power=Pre_Power;			//将采样功率赋值给显示功率
						Power_All+=(long)Power;
					}
			 }

		}
		else if(Mode){
			if(ceshi_i<99)
			ceshi[ceshi_i++]=temp;
			if(Pursuit_time[0]==9&&Pursuit_time[1]==9&&Pursuit_time[2]==9&&Pursuit_time[3]==9){	//跟踪时间千位
				if(Pursuit_time[4]==9)
					Pursuit_time[4]=0;
				else
					Pursuit_time[4]++;
			}

			if(Pursuit_time[0]==9&&Pursuit_time[1]==9&&Pursuit_time[2]==9){		//跟踪时间百位
				if(Pursuit_time[3]==9)
					Pursuit_time[3]=0;
				else
					Pursuit_time[3]++;
			}

			if(Pursuit_time[0]==9&&Pursuit_time[1]==9){				//跟踪时间十位
				if(Pursuit_time[2]==9)
					Pursuit_time[2]=0;
				else
					Pursuit_time[2]++;
			}

			if(Pursuit_time[0]==9){				//跟踪时间个位
				if(Pursuit_time[1]==9)
					Pursuit_time[1]=0;
				else
					Pursuit_time[1]++;
			}

			if(Pursuit_time[0]==9)				//跟踪时间小数点后一位
				Pursuit_time[0]=0;
			else
				Pursuit_time[0]++;
		}
	}

}



