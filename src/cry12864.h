/*
 * cry12864.h
 *
 *  Created on: 2016-7-23
 *      Author: JierunChan
 */

#ifndef CRY12864_H_
#define CRY12864_H_

extern void Delay_1ms(void);
extern void Delay(unsigned int n);
extern void Write_Cmd(unsigned char cod);
extern void Write_Data(unsigned char dat);
extern void Ini_Lcd(void);
extern void Clear_GDRAM(void);
extern void Draw_TX(unsigned char Yaddr,unsigned char Xaddr,const unsigned char * dp);


#endif /* CRY12864_H_ */
