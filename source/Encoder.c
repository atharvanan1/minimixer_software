/*
 * Encoder.c
 *
 *  Created on: Dec 6, 2020
 *      Author: Jiabin Lin
 */
#include "em_gpio.h"
#include <string.h>
#include "gpiointerrupt.h"
#include "Encoder.h"
#include "Commands.h"
#include "MIDI_API.h"
#include <assert.h>
#include "events.h"
#include "native_gecko.h"


/* Purpose: set up Encoder GPIO and interrupt
 * Input: None
 * Output: None
 *
 */
void EncoderInit() {
	/*Left Encoder*/
	GPIO_PinModeSet(ENCODER1_A_PORT, ENCODER1_A_PIN, gpioModeInput, false);
	GPIO_ExtIntConfig(ENCODER1_A_PORT, ENCODER1_A_PIN, ENCODER1_A_PIN, false,
			true, true);

	GPIO_PinModeSet(ENCODER1_B_PORT, ENCODER1_B_PIN, gpioModeInput, false);
	GPIO_ExtIntConfig(ENCODER1_B_PORT, ENCODER1_B_PIN, ENCODER1_B_PIN, false,
			true, true);

	/*Middle Encoder*/
	GPIO_PinModeSet(ENCODER2_A_PORT, ENCODER2_A_PIN, gpioModeInput, false);
	GPIO_ExtIntConfig(ENCODER2_A_PORT, ENCODER2_A_PIN, ENCODER2_A_PIN, false,
			true, true);

	GPIO_PinModeSet(ENCODER2_B_PORT, ENCODER2_B_PIN, gpioModeInput, false);
	GPIO_ExtIntConfig(ENCODER2_B_PORT, ENCODER2_B_PIN, ENCODER2_B_PIN, false,
			true, true);

	/*Right Encoder*/
	GPIO_PinModeSet(ENCODER3_A_PORT, ENCODER3_A_PIN, gpioModeInput, false);
	GPIO_ExtIntConfig(ENCODER3_A_PORT, ENCODER3_A_PIN, ENCODER3_A_PIN, false,
			true, true);

	GPIO_PinModeSet(ENCODER3_B_PORT, ENCODER3_B_PIN, gpioModeInput, false);
	GPIO_ExtIntConfig(ENCODER3_B_PORT, ENCODER3_B_PIN, ENCODER3_B_PIN, false,
			true, true);

	/*GPIO Encoder Interrupt*/
	NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);

}

bool isSamePhase(int ID) {
	if (ID == 1) {
		return (GPIO_PinInGet(ENCODER1_A_PORT, ENCODER1_A_PIN)
				== GPIO_PinInGet(ENCODER1_B_PORT, ENCODER1_B_PIN));
	}
	if (ID == 2) {
		return (GPIO_PinInGet(ENCODER2_A_PORT, ENCODER2_A_PIN)
				== GPIO_PinInGet(ENCODER2_B_PORT, ENCODER2_B_PIN));
	}
	if (ID == 3) {
		return (GPIO_PinInGet(ENCODER3_A_PORT, ENCODER3_A_PIN)
				== GPIO_PinInGet(ENCODER3_B_PORT, ENCODER3_B_PIN));
	}
	return -1;
}

/*Callback event*/
void Encoder1_Evt(uint32_t iflag) {

	gecko_external_signal(ENC_1);

}
/*Callback event*/
void Encoder2_Evt(uint32_t iflag) {

	gecko_external_signal(ENC_2);

}
/*Callback event*/
void Encoder3_Evt(uint32_t iflag) {

	gecko_external_signal(ENC_3);

}

/* Register Callback functions */
void setGpioCallback(void) {
	GPIOINT_CallbackRegister(ENCODER1_A_PIN, Encoder1_Evt);
	GPIOINT_CallbackRegister(ENCODER2_A_PIN, Encoder2_Evt);
	GPIOINT_CallbackRegister(ENCODER3_A_PIN, Encoder3_Evt);
}
