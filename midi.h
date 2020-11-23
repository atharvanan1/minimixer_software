/*
 * @file midi.h
 *
 *  Created on: 2019.05.31.
 *      Author: baadamff
 */

#ifndef MIDI_H_
#define MIDI_H_

#ifdef __cplusplus
extern "C" {
#endif

void midi_init_buttons(void);
void midi_init_timer(void);
void midi_init_ble_connection(void);
void midi_ble_connected(uint8_t connection);
void midi_ble_disconnected(void);

#ifdef __cplusplus
}
#endif

#endif // MIDI_H_
