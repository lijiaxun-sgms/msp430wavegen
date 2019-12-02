/*
 * MSP430F5529 Overclocking
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
#include "clk_init.h"
#include "driverlib.h"
#define UCS_XT1_CRYSTAL_FREQUENCY 32768
#define UCS_XT2_CRYSTAL_FREQUENCY 4000000
#define UCS_XT1_TIMEOUT 50000
#define UCS_XT2_TIMEOUT 50000
void clock_init(unsigned char FOSC){
	//Enable XT1+XT2
	UCS_setExternalClockSource(UCS_XT1_CRYSTAL_FREQUENCY,UCS_XT2_CRYSTAL_FREQUENCY);
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,GPIO_PIN4 + GPIO_PIN5);
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,GPIO_PIN2 + GPIO_PIN3);
	UCS_turnOnLFXT1WithTimeout(UCS_XT1_DRIVE_0,UCS_XCAP_3,UCS_XT1_TIMEOUT);
	UCS_turnOnXT2WithTimeout(UCS_XT2_DRIVE_4MHZ_8MHZ,UCS_XT2_TIMEOUT);
    //Hack of Vcore
    PMMCTL0_H = 0xA5;//PMM on
    SVSMLCTL |= SVSMLRRL_1 + SVMLE;//SVML config
    PMM_setVCore(PMM_CORE_LEVEL_3);//Vcore highest
    while((PMMIFG & SVSMLDLYIFG ) == 0);
    PMMIFG &= ~(SVMLVLRIFG + SVMLIFG + SVSMLDLYIFG);
    if((PMMIFG & SVMLIFG) == 1)
         while((PMMIFG & SVMLVLRIFG) == 0);
    SVSMLCTL &= ~SVMLE;//SVML config
    PMMCTL0_H = 0X00;
    //Hack end
    __bis_SR_register(SCG0);
    UCSCTL0 = 0;
    UCSCTL6 = (UCSCTL6&(~(XT2OFF|XT1OFF))|XCAP_3|XT2DRIVE_0);
    UCSCTL3 = (5<<4)|(2<<0);
    if(FOSC< 5)
        UCSCTL1 = DCORSEL_2;
    else if(FOSC<15)
        UCSCTL1 = DCORSEL_4;
    else
        UCSCTL1 = DCORSEL_7;
    UCSCTL2 = (FOSC-1);
    __bic_SR_register(SCG0);
    __delay_cycles(782000);
    while(SFRIFG1 & OFIFG){
      UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
      SFRIFG1 &= ~OFIFG;
    }
    UCSCTL4 = UCSCTL4&(~(SELS_7|SELM_7))|SELS_3|SELM_3;
}
