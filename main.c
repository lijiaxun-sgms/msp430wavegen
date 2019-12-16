/*
 * MSP430F5529 Waveform Generator Main Program
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

#include "driverlib.h"
#include "msp430f5529.h"
#include "AD9833\AD9833.h"
#include "clk_init.h"
#include "uart_init.h"
#define FOSC ((double)12000000)
#define delay_us(x) __delay_cycles((long)(FOSC*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(FOSC*(double)x/1000.0))

void main(){
    WDT_A_hold(WDT_A_BASE);
    GPIO_setAsOutputPin(GPIO_PORT_P4,GPIO_PIN7);
    GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
    clock_init(12);//Set CPU Freq=12MHz,MCLK=SMCLK=12MHz
    uart_init();//Setup UART0 at P3
	ioset();//Setup SPI at P6
	//P6^0	FSYNC
	//P6^1	CLK
	//P6^2	DAT
	//P6^3	CS
	GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN7);

	P8REN = 6;
	P2REN = 104;
	P8OUT = 6;
	P2OUT = 104;

	__bis_SR_register(GIE);
	uart_print("CLS(13);DS12(0,0,'Loading...',1);\r\n");
    AD9833_Normsig();//Output [Sine,1KHz,1.68Vpp,0] wave
    AD9833_AmpSet(64);
    uart_print("CLS(19);DS12(0,0,'Waveform Generator',1,0);DS12(0,15,'by Li Jiaxun',1,0);BOX(0,90,318,239,1);DS12(2,100,'Freq',1);DS12(2,112,'1 KHz',0);DS12(2,124,'Volt',1);DS12(2,136,'1.68 Vpp',0);DS12(2,148,'Wave',1);DS12(2,160,'Sinwave',0);DS12(2,172,'Phase',1);DS12(2,184,'0',0);\r\n");
    //GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    //GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
    //AD9833_WaveSeting(2000.0,0,SIN_WAVE,0 );//2KHz,FREQ0,SINE,PHS=0
    //AD9833_AmpSet(64);//Vpp=1.68v@3.3vMCU
bigloop:
    	if(!(P8IN & BIT1)){
    		delay_ms(25);
    		if(!(P8IN & BIT1)){
    			AD9833_WaveSeting(2000.0,0,SIN_WAVE,0 );
    			AD9833_AmpSet(64);
    		    uart_print("CLS(19);DS12(0,0,'Waveform Generator',1,0);DS12(0,15,'by Li Jiaxun',1,0);BOX(0,90,318,239,1);DS12(2,100,'Freq',1);DS12(2,112,'2 KHz',0);DS12(2,124,'Volt',1);DS12(2,136,'1.68 Vpp',0);DS12(2,148,'Wave',1);DS12(2,160,'Sinwave',0);DS12(2,172,'Phase',1);DS12(2,184,'0',0);\r\n");
    		    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    		    GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
    		    delay_ms(300);
    		    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
    		}else{
    		}
    	}else if(!(P8IN & BIT2)){
    		delay_ms(25);
    		if(!(P8IN & BIT2)){
    			AD9833_WaveSeting(5000000,0,SIN_WAVE,0 );
    			AD9833_AmpSet(64);
    		    uart_print("CLS(19);DS12(0,0,'Waveform Generator',1,0);DS12(0,15,'by Li Jiaxun',1,0);BOX(0,90,318,239,1);DS12(2,100,'Freq',1);DS12(2,112,'5 MHz',0);DS12(2,124,'Volt',1);DS12(2,136,'500 mVpp',0);DS12(2,148,'Wave',1);DS12(2,160,'Sinwave',0);DS12(2,172,'Phase',1);DS12(2,184,'0',0);\r\n");
    		    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    		    GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
    		    delay_ms(300);
    		    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
    		}else{
    		}
    	}else if(!(P2IN & BIT3)){
    		delay_ms(25);
    		if(!(P2IN & BIT3)){
    			AD9833_WaveSeting(2000.0,0,SQU_WAVE,0 );
    		    uart_print("CLS(19);DS12(0,0,'Waveform Generator',1,0);DS12(0,15,'by Li Jiaxun',1,0);BOX(0,90,318,239,1);DS12(2,100,'Freq',1);DS12(2,112,'2 KHz',0);DS12(2,124,'Volt',1);DS12(2,136,'3.3 Vpp',0);DS12(2,148,'Wave',1);DS12(2,160,'Squwave',0);DS12(2,172,'Phase',1);DS12(2,184,'0',0);\r\n");
    		    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    		    GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
    		    delay_ms(300);
    		    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
    		}else{
    		}
    	}else if(!(P2IN & BIT5)){
    		delay_ms(25);
    		if(!(P2IN & BIT5)){
    			AD9833_WaveSeting(2000.0,0,TRI_WAVE,0 );
    			AD9833_AmpSet(64);
    		    uart_print("CLS(19);DS12(0,0,'Waveform Generator',1,0);DS12(0,15,'by Li Jiaxun',1,0);BOX(0,90,318,239,1);DS12(2,100,'Freq',1);DS12(2,112,'1 KHz',0);DS12(2,124,'Volt',1);DS12(2,136,'1.68 Vpp',0);DS12(2,148,'Wave',1);DS12(2,160,'Trianglewave',0);DS12(2,172,'Phase',1);DS12(2,184,'0',0);\r\n");
    		    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    		    GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
    		    delay_ms(300);
    		    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
    		}else{
    		}
    	}else if(!(P2IN & BIT6)){
    		delay_ms(25);
    		if(!(P2IN & BIT6)){
    			AD9833_WaveSeting(33000,0,SIN_WAVE,0 );
    			AD9833_AmpSet(32);
    		    uart_print("CLS(19);DS12(0,0,'Waveform Generator',1,0);DS12(0,15,'by Li Jiaxun',1,0);BOX(0,90,318,239,1);DS12(2,100,'Freq',1);DS12(2,112,'33 KHz',0);DS12(2,124,'Volt',1);DS12(2,136,'0.84 Vpp',0);DS12(2,148,'Wave',1);DS12(2,160,'Sinwave',0);DS12(2,172,'Phase',1);DS12(2,184,'0',0);\r\n");
    		    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    		    GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
    		    delay_ms(300);
    		    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
    		}else{
    		}
    	}
goto bigloop;
}
