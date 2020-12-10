/*
 * Encoder.h
 *
 *  Created on: Dec 6, 2020
 *      Author: Jiabin Lin
 */

#ifndef ENCODER_H_
#define ENCODER_H_
#include "em_gpio.h"

// different logic -> clockwise
// same logical -> counter clockwise
#define ENCODER1_A_PORT           gpioPortB
#define ENCODER1_A_PIN            13
#define ENCODER1_B_PORT           gpioPortB
#define ENCODER1_B_PIN            12


#define ENCODER2_A_PORT           gpioPortB
#define ENCODER2_A_PIN            11
#define ENCODER2_B_PORT           gpioPortA
#define ENCODER2_B_PIN            5

#define ENCODER3_A_PORT           gpioPortA
#define ENCODER3_A_PIN            4
#define ENCODER3_B_PORT           gpioPortA
#define ENCODER3_B_PIN            3

#define ERROR					  -1
#define YES						  1
#define NO						  0
#define DELAY_INTERVAL			  25000
#define INRANGE(x,n) (((x) >= 0 && (x) <= (n)) ? 1 : 0)
#define GREATER(x,n)  (((x) > (n))? 1 : 0)
#define LESSER(x,n)  (((x) < (n))? 1 : 0)


typedef struct EncoderPair{
	GPIO_Port_TypeDef EnPortB;
	GPIO_Port_TypeDef EnPortA;
	uint32_t EnPinB;
	uint32_t EnPinA;
}EncoderPair_t;

void setGpioCallback(void);
void EncoderInit();
bool isSamePhase(int ID);
#endif /* ENCODER_H_ */
