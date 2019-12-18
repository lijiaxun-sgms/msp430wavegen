/*
 * AD9833 Driver for MSP430
 * Copyright (C) 2019  Li Jiaxun<febrieac@outlook.com>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "ad9833.h"
#include "driverlib.h"
#define FSYNC_0()	GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN0)
#define FSYNC_1()	GPIO_setOutputHighOnPin(GPIO_PORT_P6,GPIO_PIN0)
#define SCK_0()		GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN1)
#define SCK_1()		GPIO_setOutputHighOnPin(GPIO_PORT_P6,GPIO_PIN1)
#define DAT_0()		GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN2)
#define DAT_1()		GPIO_setOutputHighOnPin(GPIO_PORT_P6,GPIO_PIN2)
#define CS_0()		GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN3)
#define CS_1()		GPIO_setOutputHighOnPin(GPIO_PORT_P6,GPIO_PIN3)

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function			: ioset
 * Description		: Set SPI pins
 * Parameter		: N/A
 * Return			: N/A
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
void ioset(){
	/*
	 * Port		Function
	 * P6^0		 FSYNC
	 * P6^1	      CLK
	 * P6^2	      DAT
	 * P6^3	      CS
	 */
	GPIO_setAsOutputPin(GPIO_PORT_P6,GPIO_PIN0);
	GPIO_setAsOutputPin(GPIO_PORT_P6,GPIO_PIN1);
	GPIO_setAsOutputPin(GPIO_PORT_P6,GPIO_PIN2);
	GPIO_setAsOutputPin(GPIO_PORT_P6,GPIO_PIN3);
}

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function			: AD9833_Delay
 * Description		: Software delay, already re-written for map430
 * Parameter		: N/A
 * Return			: N/A
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void AD9833_Delay(){
	__delay_cycles(5);
}

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function			: AD9833_Write
 * Description		: Write SFR of AD9833 using software 3-wire 16-bit SPI
 * Parameter		: TxData (16-bit)
 * Return			: N/A
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
void AD9833_Write(unsigned int TxData){
	unsigned char i;
	SCK_1();
	AD9833_Delay();
	FSYNC_1();
	AD9833_Delay();
	FSYNC_0();
	AD9833_Delay();
	for(i = 0; i < 16; i++){
		if(TxData&0x8000){
			DAT_1();
		}
		else{
			DAT_0();
		}
		AD9833_Delay();
		SCK_0();
		AD9833_Delay();		
		SCK_1();
		TxData <<= 1;//Shift to next bit
	}
	FSYNC_1();
} 

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function			: AD9833_AmpSet
 * Description		: Set the output voltage
 * Parameter		: amp (8-bit)
 * Return			: N/A
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
void AD9833_AmpSet(unsigned char amp){
	unsigned char i;
	unsigned int temp;
	CS_0();//Choose device MCP41010
	temp	=	0x1100|amp;
	for(i=0;i<16;i++){
		SCK_0();
		if(temp&0x8000){
			DAT_1();
		}
		else{
			DAT_0();
		}
		temp <<= 1;
	    SCK_1();
	    AD9833_Delay();
	}
   	CS_1();//Choose device AD9833
}

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function			: AD9833_WaveSeting
 * Description		: Set AD9833 with waveform info
 * Parameter		: 1.Freq: 		0.1 hz - 12Mhz
			  	  	  2.Freq_SFR: 	0/1
			  	  	  3.WaveMode: 	TRI_WAVE,SIN_WAVE,SQU_WAVE
			  	  	  4.Phase
 * Return			: N/A
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
void AD9833_WaveSeting(	double Freq,
						unsigned int Freq_SFR,
						unsigned int WaveMode,
						unsigned int Phase
						){
	int freq_LSB,freq_MSB,Phs_data;
	double freq_mid,freq_DATA;
	long int freq_hex;
	/*
	 * AND 0x3FFF => Erase D15 and D14
	 * OR  0xC000 => Set Bit D15&D14 As High
	 */
	freq_mid	=	268435456/25;//for 25MHZ OSC
	freq_DATA	=	Freq;
	freq_DATA	=	freq_DATA/1000000;
	freq_DATA	=	freq_DATA*freq_mid;
	freq_hex	=	freq_DATA;
	freq_LSB	=	freq_hex;
	freq_LSB	=	freq_LSB&0x3FFF;
	freq_MSB	=	freq_hex >> 14;
	freq_MSB	=	freq_MSB&0x3FFF;
	/*
	 *D15 D14 D13 D12 D11... D0        MEANS
	 * 1   1   0   X   DATA12	    USE PHASE0 REG
	 * 1   1   1   X   DATA12	    USE PHASE1 REG
	 */
	Phs_data	=	Phase|0xC000;//D15=1 D14=1 D13=0 D12=X
	/*************************************************/
	AD9833_Write(0x0100); //AD9833SFR RESET=1
	//B28=1 means load two data directly two write of SFR
	AD9833_Write(0x2100); //AD9833SFR B28=1 RESET=1
	/*
	 *D15 D14 D13 ... D0        MEANS
	 * 0   1    DATA14	    USE FREQ0 REG
	 * 1   0    DATA14	    USE FREQ1 REG
	 *
	 * OR 0x4000 => Set Bit D14 As High
	 * OR 0x8000 => Set Bit D15 As High
	 */
	if(Freq_SFR==0){//USE FREQ0 REG
	 	freq_LSB=freq_LSB|0x4000;
	 	freq_MSB=freq_MSB|0x4000;
		AD9833_Write(freq_LSB); //Load LSB
		AD9833_Write(freq_MSB); //Load MSB
		AD9833_Write(Phs_data);	//Set phase
    }
	if(Freq_SFR==1){//USE FREQ1 REG
		freq_LSB=freq_LSB|0x8000;
		freq_MSB=freq_MSB|0x8000;
		AD9833_Write(freq_LSB);
		AD9833_Write(freq_MSB);
		AD9833_Write(Phs_data);
	}
	if(WaveMode==TRI_WAVE){
		AD9833_Write(0x2002);
	}
	if(WaveMode==SQU_WAVE){
		AD9833_Write(0x2028);
	}
	if(WaveMode==SIN_WAVE){
		AD9833_Write(0x2000);
	}
}

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function			: AD9833_Normsig
 * Description		: Output a normal signal 1KHz Sine Half Vpp
 * Parameter		: amp (8-bit)
 * Return			: N/A
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
void AD9833_Normsig(){
	AD9833_Write(0x0100);
	AD9833_Delay();
	AD9833_WaveSeting(1000,0,SIN_WAVE,0);
	AD9833_AmpSet(64);
}
