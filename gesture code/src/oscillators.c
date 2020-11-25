/*
 * oscillators.c
 *
 *  Created on: 10-Sep-2020
 *      Author: Chirayu Thakur
 */
#include "oscillators.h"


/*
 *  Function Name: void init_oscillators()
 *
 *  Description: Initialize clock oscillators
 * .
 * Parameters: None
 *
 *  Silab API's Used:None
 *
 * Returns: None
 *
 */

void init_oscillators()
{

	// Enable ULFRCO oscillator, and wait for it to be stable
	CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);
	CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_ULFRCO);


}
