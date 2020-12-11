/**********************************
 *
 *
 *
 *********************************/




#include "em_letimer.h"
#include "em_cmu.h"


#include "letimer.h"
#include "events.h"
#include "native_gecko.h"


void LETIMERinit(void){

  // Enable clock to LETIMER0
  CMU_ClockEnable(cmuClock_LETIMER0, true);

  // Select LFXO for the LETIMER
  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

	LETIMER_Init_TypeDef letimerInit = LETIMER_INIT_DEFAULT;

	// Calculate the top value (frequency) based on clock source
	uint32_t topValue = CMU_ClockFreqGet(cmuClock_LETIMER0) / OUT_FREQ;

	// Reload top on underflow, pulse output, and run in free mode
	letimerInit.enable = false;
	letimerInit.comp0Top = true;
	letimerInit.topValue = topValue;
	letimerInit.repMode = letimerRepeatFree;

	LETIMER0->IEN = LETIMER_IEN_UF;



	// Initialize LETIMER
	LETIMER_Init(LETIMER0, &letimerInit);
	NVIC_EnableIRQ(LETIMER0_IRQn);

}



void LETIMERstart(void){

	LETIMER0->CNT = LETIMER0->COMP0;

	LETIMER_Enable(LETIMER0, 1);

}



void LETIMER0_IRQHandler(void){
	// read and clear the interrupts that have been set (UF only)
	uint32_t int_flag = LETIMER0->IF & LETIMER0->IEN;

	// Clear interrupt
	LETIMER_IntClear(LETIMER0, int_flag);

	// Disable LETIMER
	LETIMER_Enable(LETIMER0, 0);

	// Send signal to app SM
	gecko_external_signal(CAP_MEASURE_START);

}
