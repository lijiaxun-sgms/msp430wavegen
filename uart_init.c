/*
 * MSP430F5529 UART init
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
#include "uart_init.h"
#include "driverlib.h"
void myuart_init(){
	//P3.3,4 = USCI_A0 TXD/RXD
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,GPIO_PIN3 + GPIO_PIN4);
	//Baudrate = 9600, clock freq = 4MHz
	//UCBRx = 26, UCBRFx = 1, UCBRSx = 0, UCOS16 = 1
	USCI_A_UART_initParam param = {0};
	param.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK;
	param.clockPrescalar = 26;
	param.firstModReg = 1;
	param.secondModReg = 0;
	param.parity = USCI_A_UART_NO_PARITY;
	param.msborLsbFirst = USCI_A_UART_LSB_FIRST;
	param.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
	param.uartMode = USCI_A_UART_MODE;
	param.overSampling = USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;
	if (STATUS_FAIL == USCI_A_UART_init(USCI_A0_BASE, &param)){
		return;
	}
	//Enable UART module for operation
	USCI_A_UART_enable(USCI_A0_BASE);
	//Enable Receive Interrupt
	USCI_A_UART_clearInterrupt(USCI_A0_BASE,USCI_A_UART_RECEIVE_INTERRUPT);
	USCI_A_UART_enableInterrupt(USCI_A0_BASE,USCI_A_UART_RECEIVE_INTERRUPT);
}

