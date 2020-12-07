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

#define ACMP_CAPSENSE ACMP1
// This needs to be filled in by the ports on our cap sensors
//#define CAPSENSE_CHANNELS       { acmpInputAPORT1XCH0, acmpInputAPORT2XCH1, acmpInputAPORT1XCH2, acmpInputAPORT2XCH3 }

//								PA1, pin 26, Vol_Inc	PA2, pin 27, Vol_Dec
#define CAPSENSE_CHANNELS		{ acmpInputAPORT3YCH9, acmpInputAPORT3XCH10}


#define ACMP_CAPSENSE_CMUCLOCK                  cmuClock_ACMP1
#define PRS_CH_CTRL_SOURCESEL_ACMP_CAPSENSE     PRS_CH_CTRL_SOURCESEL_ACMP1
#define PRS_CH_CTRL_SIGSEL_ACMPOUT_CAPSENSE     PRS_CH_CTRL_SIGSEL_ACMP1OUT


void CAPSENSE_Init(void);
uint32_t CAPSENSE_getVal(uint8_t channel);
uint32_t CAPSENSE_getNormalizedVal(uint8_t channel);
bool CAPSENSE_getPressed(uint8_t channel);
//static void CAPSENSE_Measure(ACMP_Channel_TypeDef channel);
void CAPSENSE_Sense(void);


#endif
