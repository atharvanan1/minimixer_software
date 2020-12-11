

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "capsense_interrupt.h"

#include "em_csen.h"
#include "em_cmu.h"


static void Capsense_HFClockOn(void){

	CMU_ClockEnable(cmuClock_CSEN_HF, true);

}


static void Capsense_HFClockOff(void){
	// Disable Capsense Clock
	CMU_ClockEnable(cmuClock_CSEN_HF, true);

}

void Capsense_Init(void)
{
	/* Using CSEN module, Create Initialization Struct */

	CSEN_Init_TypeDef capsense_init = CSEN_INIT_DEFAULT;

	/* Need to enable HFPER only on register access*/
	CMU_ClockEnable(cmuClock_HFPER, true);
	Capsense_HFClockOn();

	/* Need to enable LFB_Clk */

	//CMU_ClockEnable()


	capsense_init.keepWarm = 1;
	/* Can do software triggered conversions for test */




	/* Low Energy Clock for demo */





	/* Select the input pin and initialize the conversion mode. */
	CSEN_InitMode_TypeDef csenInitMode = CSEN_INITMODE_DEFAULT;



	csenInitMode.singleSel = csenSingleSelAPORT1XCH0;
	CSEN_InitMode(CSEN, &csenInitMode);

}






void Capsense_Example(void)
{
	/* Using CSEN module, Create Initialization Struct */

	CSEN_Init_TypeDef capsense_init = CSEN_INIT_DEFAULT;

	/* Need to enable HFPER (on register access?)*/
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_CSEN_HF, true);


	/* Select the input pin and initialize the conversion mode. */
	CSEN_InitMode_TypeDef csenInitMode = CSEN_INITMODE_DEFAULT;



	csenInitMode.singleSel = csenSingleSelAPORT1XCH0;
	CSEN_InitMode(CSEN, &csenInitMode);


	/* Enable CSEN and manually start the conversion. */
	CSEN_Enable(CSEN);
	CSEN_Start(CSEN);
}




