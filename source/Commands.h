/*
 * Commands.h
 *
 *  Created on: Nov 26, 2020
 *      Author: Jiabin Lin
 */

#ifndef SRC_COMMANDS_H_
#define SRC_COMMANDS_H_

// See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 32 for more!

#endif /* SRC_COMMANDS_H_ */
#define MIDI_BAUDRATE       31250
#define MIDI_CHAN_MSG       0xB0
#define MIDI_CHAN_PROGRAM   0xC0
/* Volume message type
 * Message type: MIDI_CHAN_MSG | channel
 *               MIDI_CHAN_VOLUME
 *               Volume
 *Volume range from 0 to 127
 */
#define MIDI_CHAN_VOLUME    0x07
#define MIDI_MIN_VOLUMN     0
#define MIDI_MAX_VOLUMN     127
#define MIDI_MAX_VELOCITY   127
#define MIDI_MIN_VELOCITY   0
#define MIDI_MAX_REVERB     127
#define MIDI_MIN_REVERB     0
#define MIDI_FIRST_INS      1
#define MIDI_LAST_INS       128

/* Channel bank selection - Set to 0x79 to change the instruments
 * Message type: MIDI_CHAN_MSG | channel
 *               MIDI_CHAN_BANK
 *               BANK
 */
#define MIDI_CHAN_BANK      0x00
#define MIDI_BANK_MELODY    0x79
#define MIDI_BANK_DRUMS1    0x78
#define MIDI_BANK_DRUMS2    0x7F
#define MIDI_MIN_PITCHBEND  0x00
#define MIDI_MAX_PITCHBEND  0x3FF
/* Note on and off
 * Message type: MIDI_NOTE_ON | channel
 *               PITCH
 *               VELOCITY
 */
#define MIDI_NOTE_ON        0x90
#define MIDI_NOTE_OFF       0x80

/* Reverb message
 * UART command:  (MIDI_CHAN_MSG | channel) |
 *                (MIDI_REVERB_GLOBAL) |
 *                (Effect Range)
 * Effect range from 0 to 0xFF
 */
#define MIDI_REVERB_GLOBAL    0x0C
#define MIDE_REVERB_MAX_VAL	  0xFF
/* Pitch bend
 * Message type: MIDI_PITCH_WHEEL | channel
 *               byte one - bend range
 *               byte two - bend range
 * Range 0 - 0x3FF
 */
#define MIDI_PITCH_WHEEL      0xE0







/* Change/Set instruments
 * Command type:
 *             (MIDI_CHAN_PROGRAM | channel) | (Instruments)
 *              delay
 * Ins range from 1 - 128
 */
//No define, see data sheet for all instruments
#define MAX_INTS_VAL	127



#define CHANNEL                   0

#define VS1053_BANK_DEFAULT       0x00
#define VS1053_BANK_DRUMS1        0x78
#define VS1053_BANK_DRUMS2        0x7F
//Default to MELODY
#define VS1053_BANK_MELODY        0x79
//Reverb Command
#define GLOBAL_REVERB_COMMAND     0x0c
//Pitch Bend
#define MIDI_PITCH_WHEEL          0xE0
#define LOW_BEND_MASK             0xFF
