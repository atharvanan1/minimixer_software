/*
 * MIDI_API.h
 *
 *  Created on: Dec 1, 2020
 *      Author: Jiabin Lin
 */

#ifndef SRC_MIDI_API_H_
#define SRC_MIDI_API_H_
#include <stdint.h>
#define MASK(x)                 (1UL<<(x))
#define CHAN_NUM                16
#define MAX_NOTE_DELAY          3000000
#define MIN_WU_RESPONSE_TIME    3500
#define MIDI_BAUD_RATE          (31250)

typedef struct BTVal
{
  uint8_t NoteStatus;        //0 OFF, 1 ON
  uint8_t Pitch;
  uint8_t Vel;
} BTVal_t;

/* 1st bit BT ready
 * 2nd bit Encoder Instruments ready
 * 3rd bit Encoder Reverb ready
 * 4th bit Encoder Delay ready
 * 5th bit Capacitor sensor ready
 * 6th bit Pitch bend ready
 */
#define BT_READY              MASK(0)
#define ENCODER_INST_READY    MASK(1)
#define ENCODER_REVERB_READY  MASK(2)
#define ENCODER_DELAY_READY   MASK(3)
#define CAP_SENSE_READY       MASK(4)
#define PITCH_BEND_READY      MASK(5)
void ResetCodec ();
void WakeUp_Test ();
void MIDI_ChannelBank (uint8_t chan);
void MIDI_SetInstrument (uint8_t Enco_Inst, uint8_t chan);
void MIDI_NoteOnOff (BTVal_t BTV, uint8_t chan);
void MIDI_SetVolumn (uint8_t CapSense_Vol, uint8_t chan);
void MIDI_Reverb (uint8_t Enco_Reverb, uint8_t chan);
void MIDI_PitchBend (uint16_t BendVal, uint8_t chan);
void MIDI_Commands_API (uint8_t chan);

#endif /* SRC_MIDI_API_H_ */
