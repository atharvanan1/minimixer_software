
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


#include "em_acmp.h"
#include "em_assert.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_core.h"
#include "em_device.h"
#include "em_emu.h"
#include "em_gpio.h"
//#include "em_lesense.h"
//#include "em_pcnt.h"
#include "em_timer.h"
#include "em_prs.h"
//#include "em_vdac.h"

#include "capsense.h"


/** The current channel we are sensing. */
static volatile uint8_t currentChannel;

/** Flag for measurement completion. */
static volatile bool measurementComplete;

/**************************************************************************//**
 * @brief This vector stores the latest read values from the ACMP
 * @param ACMP_CHANNELS Vector of channels.
 *****************************************************************************/
static volatile uint32_t channelValues[ACMP_CHANNELS] = { 0 };

/**************************************************************************//**
 * @brief  This stores the maximum values seen by a channel
 * @param ACMP_CHANNELS Vector of channels.
 *****************************************************************************/
static volatile uint32_t channelMaxValues[ACMP_CHANNELS] = { 0 };


static const ACMP_Channel_TypeDef channelList[ACMP_CHANNELS] = CAPSENSE_CHANNELS;

void capsense_init()
{
      /* Use the default STK capacative sensing setup */
      ACMP_CapsenseInit_TypeDef capsenseInit = {
          false,                /* Don't use fullBias to lower power consumption */
          0x20,                 /* Using biasProg value of 0x20 (32) */
          1, /* Use hysteresis level 1 when ACMP output is 0 */
          1, /* Use hysteresis level 1 when ACMP output is 1 */
          1,        /* Use internal resistor value 1 */
          0x30,                 /* VDD level high */
          0x10,                 /* VDD level low */
          true                  /* Enable after init. */
        };

      CMU_ClockEnable(cmuClock_HFPER, true);
      CMU_ClockEnable(cmuClock_ACMP1, true);

      /* Set up ACMP1 in capsense mode */
      ACMP_CapsenseInit(ACMP_CAPSENSE, &capsenseInit);

      // This is all that is needed to setup PC8, or the left-most slider
      // i.e. no GPIO routes or GPIO clocks need to be configured
      ACMP_CapsenseChannelSet(ACMP_CAPSENSE, 32);  //ACMP_INPUTSEL_VASEL_APORT1XCH0

      // Enable the ACMP_CAPSENSE interrupt
      ACMP_IntEnable(ACMP_CAPSENSE, ACMP_IEN_EDGE);
      ACMP_CAPSENSE->CTRL = ACMP_CAPSENSE->CTRL | ACMP_CTRL_IRISE_ENABLED;

      // Wait until ACMP warms up
      while (!(ACMP_CAPSENSE->STATUS & ACMP_STATUS_ACMPACT)) ;

      CMU_ClockEnable(cmuClock_PRS, true);
      CMU_ClockEnable(cmuClock_TIMER1, true);

      // Use TIMER1 to count ACMP events (rising edges)
      // It will be clocked by the capture/compare feature
      TIMER_Init_TypeDef timer_settings = TIMER_INIT_DEFAULT;
      timer_settings.clkSel = timerClkSelCC1;
      timer_settings.prescale = timerPrescale1024;
      TIMER_Init(TIMER1, &timer_settings);
      TIMER1->TOP  = 0xFFFF;

      // Set up TIMER1's capture/compare feature, to act as the source clock
      TIMER_InitCC_TypeDef timer_cc_settings = TIMER_INITCC_DEFAULT;
      timer_cc_settings.mode = timerCCModeCapture;
      timer_cc_settings.prsInput = true;
      timer_cc_settings.prsSel = 0;			// Select channel 0
      timer_cc_settings.eventCtrl = timerEventRising;
      timer_cc_settings.edge = timerEdgeBoth;
      TIMER_InitCC(TIMER1, 1, &timer_cc_settings);

      // Set up PRS so that TIMER1 CC1 can observe the event produced by ACMP_CAPSENSE
      PRS_SourceSignalSet(0, PRS_CH_CTRL_SOURCESEL_ACMP1, PRS_CH_CTRL_SIGSEL_ACMP1OUT, prsEdgePos);

}

/**************************************************************************//**
 * @brief Get the current normalized channelValue for a channel
 * @param channel The channel.
 * @return The channel value in range (0-256).
 *****************************************************************************/
uint32_t CAPSENSE_getVal(uint8_t channel)
{
  return channelValues[channel];
}



/**************************************************************************//**
 * @brief Get the current normalized channelValue for a channel
 * @param channel The channel.
 * @return The channel value in range (0-256).
 *****************************************************************************/
uint32_t CAPSENSE_getNormalizedVal(uint8_t channel)
{
  uint32_t max = channelMaxValues[channel];
  return (channelValues[channel] << 8) / max;
}

/**************************************************************************//**
 * @brief Get the state of the Gecko Button
 * @param channel The channel.
 * @return true if the button is "pressed"
 *         false otherwise.
 *****************************************************************************/
bool CAPSENSE_getPressed(uint8_t channel)
{
  uint32_t treshold;
  /* Treshold is set to 12.5% below the maximum value */
  /* This calculation is performed in two steps because channelMaxValues is
   * volatile. */
  treshold  = channelMaxValues[channel];
  treshold -= channelMaxValues[channel] >> 2;

  if (channelValues[channel] < treshold) {
    return true;
  }
  return false;
}


/**************************************************************************//**
 * @brief
 *   Start a capsense measurement of a specific channel and waits for
 *   it to complete.
 *****************************************************************************/
static void CAPSENSE_Measure(ACMP_Channel_TypeDef channel)
{
  /* Set up this channel in the ACMP. */
  ACMP_CapsenseChannelSet(ACMP_CAPSENSE, channel);

  /* Reset timers */
  TIMER0->CNT = 0;
  TIMER1->CNT = 0;

  measurementComplete = false;

  /* Start timers */
  TIMER0->CMD = TIMER_CMD_START;
  TIMER1->CMD = TIMER_CMD_START;

  /* Wait for measurement to complete */
  while ( measurementComplete == false ) {
    EMU_EnterEM1();
  }
}


void CAPSENSE_Sense(void)
{
  /* Use the default STK capacative sensing setup and enable it */
  ACMP_Enable(ACMP_CAPSENSE);

#if defined(CAPSENSE_CHANNELS)
  /* Iterate through only the channels in the channelList */
  for (currentChannel = 0; currentChannel < ACMP_CHANNELS; currentChannel++) {
    CAPSENSE_Measure(channelList[currentChannel]);
  }
#else
  /* Iterate through all channels and check which channel is in use */
  for (currentChannel = 0; currentChannel < ACMP_CHANNELS; currentChannel++) {
    /* If this channel is not in use, skip to the next one */
    if (!channelsInUse[currentChannel]) {
      continue;
    }

    CAPSENSE_Measure((ACMP_Channel_TypeDef) currentChannel);
  }
#endif

  /* Disable ACMP while not sensing to reduce power consumption */
  ACMP_Disable(ACMP_CAPSENSE);
}



/**************************************************************************//**
 * @brief
 *   TIMER0 interrupt handler.
 *
 * @details
 *   When TIMER0 expires the number of pulses on TIMER1 is inserted into
 *   channelValues. If this values is bigger than what is recorded in
 *   channelMaxValues, channelMaxValues is updated.
 *   Finally, the next ACMP channel is selected.
 *****************************************************************************/
void TIMER0_IRQHandler(void)
{
  uint32_t count;

  /* Stop timers */
  TIMER0->CMD = TIMER_CMD_STOP;
  TIMER1->CMD = TIMER_CMD_STOP;

  /* Clear interrupt flag */
  TIMER0->IFC = TIMER_IFC_OF;

  /* Read out value of TIMER1 */
  count = TIMER1->CNT;

  /* Store value in channelValues */
  channelValues[currentChannel] = count;

  /* Update channelMaxValues */
  if (count > channelMaxValues[currentChannel]) {
    channelMaxValues[currentChannel] = count;
  }

  measurementComplete = true;
}
