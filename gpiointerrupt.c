/***************************************************************************//**
 * @file
 * @brief GPIOINT API implementation
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include "em_gpio.h"
#include "em_core.h"
#include "gpiointerrupt.h"
#include "em_assert.h"
#include "em_common.h"

/*******************************************************************************
 ********************************   MACROS   ***********************************
 ******************************************************************************/



/*******************************************************************************
 ********************************   GLOBALS   **********************************
 ******************************************************************************/

/* Array of user callbacks. One for each pin interrupt number. */
static GPIOINT_IrqCallbackPtr_t gpioCallbacks[16] = { 0 };

/*******************************************************************************
 ******************************   PROTOTYPES   *********************************
 ******************************************************************************/
static void GPIOINT_IRQDispatcher(uint32_t iflags);


/***************************************************************************//**
 * @brief
 *   Registers user callback for given pin interrupt number.
 *
 * @details
 *   Use this function to register a callback which shall be called upon
 *   interrupt generated for a given pin interrupt number.
 *   Interrupt itself must be configured externally. Function overwrites previously
 *   registered callback.
 *
 * @param[in] pin
 *   Pin number for the callback.
 * @param[in] callbackPtr
 *   A pointer to callback function.
 ******************************************************************************/
void GPIOINT_CallbackRegister(uint8_t intNo, GPIOINT_IrqCallbackPtr_t callbackPtr)
{
  CORE_ATOMIC_SECTION(
    /* Dispatcher is used */
    gpioCallbacks[intNo] = callbackPtr;
    )
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/***************************************************************************//**
 * @brief
 *   Function calls users callback for registered pin interrupts.
 *
 * @details
 *   This function is called when GPIO interrupts are handled by the dispatcher.
 *   Function gets even or odd interrupt flags and calls user callback
 *   registered for that pin. Function iterates on flags starting from MSB.
 *
 * @param iflags
 *  Interrupt flags which shall be handled by the dispatcher.
 *
 ******************************************************************************/
static void GPIOINT_IRQDispatcher(uint32_t iflags)
{
  uint32_t irqIdx;
  GPIOINT_IrqCallbackPtr_t callback;

  /* check for all flags set in IF register */
  while (iflags != 0U) {
    irqIdx = SL_CTZ(iflags);

    /* clear flag*/
    iflags &= ~(1 << irqIdx);

    callback = gpioCallbacks[irqIdx];
    if (callback) {
      /* call user callback */
      callback(irqIdx);
    }
  }
}

/***************************************************************************//**
 * @brief
 *   GPIO EVEN interrupt handler. Interrupt handler clears all IF even flags and
 *   call the dispatcher passing the flags which triggered the interrupt.
 *
 ******************************************************************************/
void GPIO_EVEN_IRQHandler(void)
{
  uint32_t iflags;

  /* Get all even interrupts. */
  iflags = GPIO_IntGetEnabled() & EVEN_INT_MASK;

  /* Clean only even interrupts. */
  GPIO_IntClear(iflags);

  GPIOINT_IRQDispatcher(iflags);
}

/***************************************************************************//**
 * @brief
 *   GPIO ODD interrupt handler. Interrupt handler clears all IF odd flags and
 *   call the dispatcher passing the flags which triggered the interrupt.
 *
 ******************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
  uint32_t iflags;

  /* Get all odd interrupts. */
  iflags = GPIO_IntGetEnabled() & ODD_INT_MASK;

  /* Clean only odd interrupts. */
  GPIO_IntClear(iflags);

  GPIOINT_IRQDispatcher(iflags);
}
