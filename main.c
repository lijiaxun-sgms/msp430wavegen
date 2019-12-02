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
#include "AD9833\AD9833.h"
#include "clk_init.h"
#include "uart_init.h"

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
	__bis_SR_register(GIE);

	uart_print("CLS(13);DS12(0,0,'Loading...',1);\r\n");
    AD9833_Normsig();//Output [Sine,1KHz,1.68Vpp,0] wave
    uart_print("CLS(19);DS12(0,0,'Waveform Generator',1,0);DS12(0,15,'by Li Jiaxun',1,0);BOX(0,90,318,239,1);DS12(2,100,'Freq',1);DS12(2,112,'1 KHz',0);DS12(2,124,'Volt',1);DS12(2,136,'1.68 Vpp',0);DS12(2,148,'Wave',1);DS12(2,160,'Sinwave',0);DS12(2,172,'Phase',1);DS12(2,184,'0',0);\r\n");
    //GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    //GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
    //AD9833_WaveSeting(2000.0,0,SIN_WAVE,0 );//2KHz,FREQ0,SINE,PHS=0
    //AD9833_AmpSet(64);//Vpp=1.68v@3.3vMCU
}
