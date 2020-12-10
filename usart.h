/*
 * usart.h
 *
 *  Created on: Nov 13, 2020
 *      Author: Jiabin Lin
 */

#ifndef SRC_USART_H_
#define SRC_USART_H_
//#define MAX_NOTE_DELAY 3000000
//#define MIN_WU_RESPONSE_TIME   3500
void initUSART2 (void);
void initUSART0_MINI (void);
void MIDI_Command(uint8_t data0, uint8_t data1, uint8_t data2);
void USART_TX_EnableINT();
#endif /* SRC_USART_H_ */
