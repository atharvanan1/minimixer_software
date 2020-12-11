/*
 * MIDI_API.c
 *
 *  Created on: Dec 1, 2020
 *      Author: Jiabin Lin
 */

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_core.h"
#include "Commands.h"
#include <stdint.h>
#include "usart.h"
#include "stdbool.h"
#include "MIDI_API.h"

/*Change all these values in sensor interrupt*/
volatile uint8_t Enco_Inst = 79;
volatile uint8_t Enco_Reverb = 0xFF;
volatile uint32_t Enco_NtDly = MAX_NOTE_DELAY;
volatile uint8_t CapSense_Vol = 0;
volatile uint8_t BendVal = 0;


volatile uint8_t MIDI_Flag = 0;

void ResetCodec ()
{
  CMU_ClockEnable (cmuClock_GPIO, true);
  GPIO_PinOutClear (gpioPortF, 3);
  GPIO_PinOutSet (gpioPortF, 3);
  for (int i = 0; i < 10000; i++)
    ;
}

//The wake up Codec test takes 3.25ms with maximum delay and minimum response time
void WakeUp_Test ()
{
  CMU_ClockEnable (cmuClock_GPIO, true);
  GPIO_PinOutClear (gpioPortD, 11);
  GPIO_PinOutClear (gpioPortC, 10);
  GPIO_PinOutSet (gpioPortC, 10);
  for (int i = 0; i < MIN_WU_RESPONSE_TIME; i++)
    ;

  MIDI_Command (0x90 | 0, 60, 127);
  MIDI_Command (0x90 | 0, 61, 127);
  for (int i = 0; i < MAX_NOTE_DELAY; i++)
    ;

  GPIO_PinOutClear (gpioPortC, 10);
  GPIO_PinOutSet (gpioPortD, 11);
}

void MIDI_NoteOnOff (BTVal_t BTV, uint8_t chan)
{

  if (BTV.Pitch > 127 || BTV.Vel > 127 || chan > 15)
    return;

  MIDI_Command (BTV.NoteStatus & ~(0x7), BTV.Pitch, BTV.Vel);
}

void MIDI_ChannelBank (uint8_t chan)
{
  if (chan > 15)
    return;
  MIDI_Command (MIDI_CHAN_MSG | chan, MIDI_CHAN_BANK, VS1053_BANK_MELODY); /*Chose Melody bank to select instruments*/
}

void MIDI_SetInstrument (uint8_t Enco_Inst, uint8_t chan)
{
  if (Enco_Inst > 127 || chan > 15)
    return;
  Enco_Inst++; /*Offset by one to compensate the 1-128*/
  MIDI_Command (MIDI_CHAN_PROGRAM | chan, 0, Enco_Inst);
}

void MIDI_SetVolumn (uint8_t CapSense_Vol, uint8_t chan)
{
  if (chan > 15 || CapSense_Vol > 127)
    return;
  MIDI_Command (MIDI_CHAN_MSG | chan, 0, CapSense_Vol);
}

void MIDI_Reverb (uint8_t Enco_Reverb, uint8_t chan)
{
  if (chan > 15 || Enco_Reverb > 0xFF)
    return;
  MIDI_Command (MIDI_CHAN_MSG | chan, GLOBAL_REVERB_COMMAND, Enco_Reverb);
}

void MIDI_PitchBend (uint16_t BendVal, uint8_t chan)
{
  if (chan > 15)
    return;
  MIDI_Command (MIDI_PITCH_WHEEL | chan, (BendVal>>8)&0x3, LOW_BEND_MASK & BendVal);
}

