/***************************************************************************//**
 * @file
 * @brief init_board.c
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#include "hal-config.h"
#else
#include "bspconfig.h"
#endif

#include "board_features.h"
#include "em_cmu.h"
#include "capsense.h"
#include "letimer.h"

#include "retargetserial.h"

void initBoard(void)
{
  // Enable clock for USART0
  CMU_ClockEnable(cmuClock_USART0, true);
  // Enable GPIO clock source
  CMU_ClockEnable(cmuClock_GPIO, true);
  // Place custom board initialization code here.
  //PC10 ---> ON2
   GPIO_PinOutSet(gpioPortC, 10);
   //PC8 ---> POWERON
   GPIO_PinOutSet(gpioPortC, 8);
   //Green LED On indicate system powered
   GPIO_PinOutSet(gpioPortD, 14);


   CAPSENSE_Init();
   LETIMERinit();
}


void initVcomEnable(void)
{

	void RETARGET_SerialInit(void);


}
