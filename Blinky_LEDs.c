/*
 * Blinky_LEDs.c
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
/* Purpose: Test to flash for Mini-mixer
 * Input: None
 * Output: None
 * PD14---> Green LED
 * PD15---> BLUE LED
 */
void Blinky_LED(){
  CMU_ClockEnable(cmuClock_GPIO, true);
  GPIO_PinOutSet(gpioPortD, 14);
  GPIO_PinOutClear(gpioPortD, 15);
  for(int i=0; i<2000000;i++);
  GPIO_PinOutClear(gpioPortD, 14);
  GPIO_PinOutSet(gpioPortD, 15);
  for(int i=0; i<2000000;i++);
}
