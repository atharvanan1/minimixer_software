/*
 * usart.c
 *
 *  Created on: Nov 13, 2020
 *      Author: Jiabin Lin
 */


/**************************************************************************//**
 * @main_series1_PG1_EFR.c
 * @brief Demonstrates USART1 as SPI master.
 * @version 0.0.1
 ******************************************************************************
 * @section License
 * <b>Copyright 2018 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_core.h"
#include "MIDI_API.h"
#define  UART_IN_MINI   1
#if !UART_IN_MINI
#define  UART_MIDI          USART2
#elif UART_IN_MINI
#define  UART_MIDI          USART0
#endif

/**************************************************************************//**
 * @brief Initialize USART2
 * Set baud rate to 31250
 * TX pin PA6
 * RX pin PA7
 *****************************************************************************/
#if !UART_IN_MINI
void initUSART2 (void)
{
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_USART2, true);
    CMU_ClockEnable(cmuClock_GPIO, true);
    GPIO_PinModeSet(gpioPortA, 6, gpioModePushPull, 1);

    //Default Configuration with MIDI baud rate
    USART_InitAsync_TypeDef initAsync = USART_INITASYNC_DEFAULT;
    initAsync.baudrate = MIDI_BAUD_RATE;
    USART_InitAsync(USART2, &initAsync);

    // Set USART pin locations
    USART2->ROUTELOC0 = (USART2->ROUTELOC0 & ~(_USART_ROUTELOC0_TXLOC_MASK) )
                        |  (USART_ROUTELOC0_TXLOC_LOC1);
    USART2->ROUTEPEN = USART_ROUTEPEN_TXPEN;
    // Enable USART pins, Disable Loopback
    USART2->CTRL = USART2->CTRL | !USART_CTRL_LOOPBK;
    USART2->CMD =  USART_CMD_TXEN;
    //Enable USART2
    USART_Enable(USART2, usartEnable);
}
#endif


#if UART_IN_MINI
void initUSART0_MINI (void)
{
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_USART0, true);
    CMU_ClockEnable(cmuClock_GPIO, true);
    GPIO_PinModeSet(gpioPortF, 2, gpioModePushPull, 1);

    //Default Configuration with MIDI baud rate
    USART_InitAsync_TypeDef initAsync = USART_INITASYNC_DEFAULT;
    initAsync.baudrate = MIDI_BAUD_RATE;
    USART_InitAsync(USART0, &initAsync);

    // Set USART pin locations
    USART0->ROUTELOC0 = (USART0->ROUTELOC0 & ~(_USART_ROUTELOC0_TXLOC_MASK) )
                        |  (USART_ROUTELOC0_TXLOC_LOC26);
    USART0->ROUTEPEN = USART_ROUTEPEN_TXPEN;
    // Enable USART pins, Disable Loopback
    USART0->CTRL = USART0->CTRL | !USART_CTRL_LOOPBK;
    USART0->CMD =  USART_CMD_TXEN;
    //Enable USART0
    USART_Enable(USART0, usartEnable);
}
#endif

//Takes the 960us for 3 bytes of transmission
void MIDI_Command(uint8_t data0, uint8_t data1, uint8_t data2){
  USART_Tx(UART_MIDI, data0);
  USART_Tx(UART_MIDI, data1);
  USART_Tx(UART_MIDI, data2);
}






