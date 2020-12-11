/*
 * System_Init.c
 *
 *  Created on: Nov 21, 2020
 *      Author: Jiabin Lin
 */

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_core.h"
#include "System_Init.h"


bool CurSts = POWERON;
void POWER_INIT(){
  CMU_ClockEnable(cmuClock_GPIO, true);
  //PC10 ---> ON2
   GPIO_PinOutSet(gpioPortC, 10);
   //PC8 ---> POWERON
   GPIO_PinOutSet(gpioPortC, 8);
   //Green LED
	GPIO_PinOutSet(gpioPortD, 14);
   CurSts = POWERON;
}


void POWER_DEINIT(){
	CMU_ClockEnable(cmuClock_GPIO, true);
	  //PC10 ---> ON2
	GPIO_PinOutClear(gpioPortC, 10);
	//PC8 ---> POWERON
	GPIO_PinOutClear(gpioPortC, 8);
	GPIO_PinOutClear(gpioPortD, 14);
	CurSts = POWEROFF;
}


void PBstatus(){
	GPIO_PinModeSet(PBSTAT_PORT, PBSTAT_PIN, gpioModeInput, false);
	GPIO_ExtIntConfig(PBSTAT_PORT, PBSTAT_PIN, PBSTAT_PIN, false,
				true, true);
}


void PB_Evt(uint32_t iflag) {
	if(CurSts == POWERON){
		POWER_DEINIT();
	}else
		POWER_INIT();
}


void AOstatus(){
	GPIO_PinModeSet(AO_PORT, AO_PIN, gpioModeInput, false);
	GPIO_ExtIntConfig(AO_PORT, AO_PIN, PBSTAT_PIN, false,
					true, true);
}

void Battery_status(){
	if(!GPIO_PinInGet(AO_PORT, AO_PIN)){
		for(int i =0;i<6;i++){
			GPIO_PinOutSet(gpioPortD, 14);
			for(int i =0;i<500000;i++)
				;
			GPIO_PinOutClear(gpioPortD, 14);
			for(int i =0;i<500000;i++)
				;
		}
		POWER_DEINIT();
	}
}

