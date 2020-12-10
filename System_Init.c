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
#define SYSTEM_INIT 1
#if SYSTEM_INIT
void POWER_INIT(){
  CMU_ClockEnable(cmuClock_GPIO, true);
  //PC10 ---> ON2
   GPIO_PinOutSet(gpioPortC, 10);
   //PC8 ---> POWERON
   GPIO_PinOutSet(gpioPortC, 8);
}
#endif
