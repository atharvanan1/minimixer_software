/*********************************
 * File: capsense.h
 * Project: MiniMixer
 * Description: Includes definitions used by capsense module
 *				Includes function prototypes
 *
 *
*********************************/
#ifndef CAPSENSE_H_
#define CAPSENSE_H_
#include <stdint.h>
#include <stdbool.h>

#include "em_acmp.h"



#define ACMP_CHANNELS 2
//								PA1, pin 26, Vol_Dec	PA2, pin 27, Vol_Inc
#define CAPSENSE_CHANNELS		{ acmpInputAPORT3YCH9,	acmpInputAPORT3XCH10 }


//#define ACMP_CHANNELS 1
////								PA2, pin 27, Vol_Inc
//#define CAPSENSE_CHANNELS		{acmpInputAPORT3XCH10}



#define ACMP_CAPSENSE ACMP1


#define ACMP_CAPSENSE_CMUCLOCK                  cmuClock_ACMP1
#define PRS_CH_CTRL_SOURCESEL_ACMP_CAPSENSE     PRS_CH_CTRL_SOURCESEL_ACMP1
#define PRS_CH_CTRL_SIGSEL_ACMPOUT_CAPSENSE     PRS_CH_CTRL_SIGSEL_ACMP1OUT


typedef enum {
	READY,
	MEASURE,
	COMPLETE
} capsense_states_t;


void CAPSENSE_Init(void);
uint32_t CAPSENSE_getVal(uint8_t channel);
uint32_t CAPSENSE_getNormalizedVal(uint8_t channel);
bool CAPSENSE_getPressed(uint8_t channel);
void CAPSENSE_Start_Measurement(ACMP_Channel_TypeDef channel);
//void CAPSENSE_Sense(void);


#endif
