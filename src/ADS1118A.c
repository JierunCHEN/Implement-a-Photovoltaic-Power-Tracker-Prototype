/*
 * ADS1118A.c
 *
 *  Created on: 2016-7-22
 *      Author: JierunChan
 */
#include <msp430.h>

volatile signed int dummy;

float Voltage_ch1;
float Voltage_ch2;

void ADS1118A_Init(void)
{
	P1OUT |= 0x02;                         	// Set P1.1 for CS
	P1DIR |= 0x02;                          // Set P1.1 to output direction
	P3SEL |= 0x80;                          // P3.7 option select
	P5SEL |= 0x30;				// P5.4,5 option select

	UCB1CTL1 |= UCSWRST;                    // **Put state machine in reset**
	UCB1CTL0 |= UCMST+UCSYNC+UCMSB;  	// 3-pin, 8-bit SPI master
	                                        // Clock polarity high, MSB
	UCB1CTL1 |= UCSSEL_2;                   // SMCLK
	UCB1BR0 = 0x05;                         // /2
	UCB1BR1 = 0;                            //
	UCB1CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**

	__delay_cycles(100);                    // Wait for slave to initialize
}
/*
 * Mode 0: Only write config register to ADS1118
 * Mode 1: Write config register to ADS1118 as well as read data from ADS1118
 */
signed int WriteSPI(unsigned int config, unsigned char mode)
{
	signed int msb;
	unsigned int temp;
	//volatile signed int dummy;

	temp = config;
	if (mode==1) temp = 0;
		//temp = (config | 0x8000)&(0xfffD);


	while(!(UCB1IFG&UCTXIFG));
	UCB1TXBUF = (temp >> 8 );			// Write MSB of Config
	while(!(UCB1IFG&UCRXIFG));
	msb = UCB1RXBUF;				// Read MSB of Data

	while(!(UCB1IFG&UCTXIFG));
	UCB1TXBUF = (temp & 0xff);			// Write LSB of Config
	while(!(UCB1IFG&UCRXIFG));
	msb = (msb << 8) | UCB1RXBUF;			// Read LSB of Data

	while(!(UCB1IFG&UCTXIFG));
	UCB1TXBUF = (temp >> 8 );			// Write MSB of Config
	while(!(UCB1IFG&UCRXIFG));
	dummy = UCB1RXBUF;				// Read MSB of Config


	while(!(UCB1IFG&UCTXIFG));
	UCB1TXBUF= (temp & 0xff);			// Write LSB of Config
	while(!(UCB1IFG&UCRXIFG));
	dummy = (dummy <<8) | UCB1RXBUF;		// Read LSB of Config

	__delay_cycles(100);

	return msb;
}

void ADS_Config(signed int temp_config_value)
{
	signed int Config_Value;

	Config_Value = temp_config_value;
	//Config_Value = 0x8583;		// Initial Config Register
	// ADS1118 configuration AIN0/AIN1, FS=+/-2.048, DR=128sps, PULLUP on DOUT

	P1OUT &=~ 0x02;				// Set CS low
	__delay_cycles(100);                    // Wait for slave to initialize

	WriteSPI(Config_Value,0);		// Write configuration to ADS1118

	__delay_cycles(100);                    // Wait for slave to initialize

	P1OUT |= 0x02;				// Set CS high
}

int ADS_Read(void)
{
	unsigned int Data, Config_Value;

	//Config_Value = 0x058B;
	Config_Value = 0;
	// ADS1118 configuration AIN0/AIN1, FS=+/-2.048, DR=128sps, PULLUP on DOUT

	P1OUT &=~ 0x02;					// Set CS low
	Data = WriteSPI(Config_Value,1);		// Read data from ADS1118
	P1OUT |= 0x02;					// Set CS high

	return Data;
}


float ADS1118A_Act(){
	volatile int ADC_Result;

	ADS_Config(0xB583);                         	//config ch2
    ADC_Result = ADS_Read();			     	// Read data from ch2,the last time result
	Voltage_ch1 = ADC_Result*1.0/32768*2.048;
    __delay_cycles(10000);                      	//need to wait time until end of convert

	ADS_Config(0x8583);                          	//config ch1
    ADC_Result = ADS_Read();			        // Read data from ch1,the last time result
    Voltage_ch2 = ADC_Result*1.0/32768*2.048;
   __delay_cycles(10000);
   return (Voltage_ch1-Voltage_ch2);
}


