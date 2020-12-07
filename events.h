/********************************************
 * File: events.h
 * Description: This file defines any external events related
 * 		to the MiniMixer board.
 *
 * 		This includes encoder and CapTouch events, as well as anything
 * 		not generated by the bluetooth module
 */


#ifndef EVENTS_H
#define EVENTS_H


// Board Hardware Generated Events
typedef enum{
	VOL_INC = 0x00000001,
	VOL_DEC = 0x0000002,
	ENC_0 = 0x00000004,
	ENC_1 = 0x00000008,
	ENC_3 = 0x00000010
} event_t;


#endif	//EVENTS_H
