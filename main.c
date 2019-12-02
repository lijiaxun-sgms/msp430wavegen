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
    clock_init(48);//Set CPU Freq=48MHz
	ioset();
	//P6^0	FSYNC
	//P6^1	CLK
	//P6^2	DAT
	//P6^3	CS
    AD9833_Normsig();
    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
    //AD9833_WaveSeting(2000.0,0,SIN_WAVE,0 );//2KHz,FREQ0,SINE,PHS=0
    //AD9833_AmpSet(64);//Vpp=1.68v@3.3vMCU
}
