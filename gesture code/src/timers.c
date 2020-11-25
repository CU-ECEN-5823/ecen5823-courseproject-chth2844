/*
 * timers.c
 *
 *  Created on: 07-Sep-2020
 *      Author: Chirayu Thakur
 */
#include "timers.h"
//extern int rollover;




/*
 * Function name: void initLETIMER0()
 *
 * Description: Initialize LETIMER0 peripheral
 * .
 * Parameters: None
 *
 *  Silab API's Used: CMU_ClockEnable(CMU_Clock_TypeDef clock, bool enable),
 *                    void LETIMER_Init(LETIMER_TypeDef *letimer, const LETIMER_Init_TypeDef *init)
 *                    void LETIMER_CompareSet(LETIMER_TypeDef *letimer,unsigned int comp, uint32_t value)
                      void LETIMER_IntEnable(LETIMER_TypeDef *letimer, uint32_t flags)
                      void LETIMER_Enable(LETIMER_TypeDef *letimer, bool enable)

 * Returns: None
 *
 */
void initLETIMER0()
{

	 CMU_ClockEnable(cmuClock_LFA,1);//enable LFA parent clock
	 CMU_ClockEnable(cmuClock_LETIMER0,1);//enable LETIMER0 clock

     #if LOWEST_ENERGY_MODE < 3

	     CMU_ClockDivSet(cmuClock_LETIMER0,cmuClkDiv_4);
    #else

	      CMU_ClockDivSet(cmuClock_LETIMER0,cmuClkDiv_1);
    #endif


	LETIMER_Init_TypeDef init=
	{

	.enable = false,/* Do not Start counting when init completed*/
	.debugRun = false, /* Counter shall not keep running during debug halt. */
	.comp0Top = true, /* Load COMP0 register into CNT when counter underflows. COMP is used as TOP */
	.bufTop = false, /* Don't load COMP1 into COMP0 when REP0 reaches 0. */
	.out0Pol = 0, /* Idle value for output 0. */
	.out1Pol = 0, /* Idle value for output 1. */
	.ufoa0 = letimerUFOANone, /* No output on output 0 */
	.ufoa1 = letimerUFOANone, /* No output on output 1*/
	.repMode = letimerRepeatFree /* Count while REP != 0 */

	};

	LETIMER_Init(LETIMER0,&init); //Initialize LETIMER0
	LETIMER_CompareSet(LETIMER0,0,COMP0); //Set Value of COMP0
	LETIMER_CompareSet(LETIMER0,1,1500); //Set Value of COMP1
	//LETIMER_IntEnable(LETIMER0,LETIMER_IF_COMP0); //Enable COMP0 Interrupt
	//LETIMER_IntEnable(LETIMER0,LETIMER_IF_COMP1); //Enable COMP1 Interrupt
	//NVIC_EnableIRQ(LETIMER0_IRQn); // config NVIC to take IRQs from LETIMER0
	LETIMER_Enable(LETIMER0,1);	//LETIMER enable

}

/*
 * Function name: void timerWaitUs(uint32_t us_wait)
 *
 * Description: Provide Interrupt based timing delay in uS
 * .
 * Parameters: uint32_t us_wait
 *
 *  Silab API's Used: void LETIMER_CompareSet(LETIMER_TypeDef *letimer,unsigned int comp, uint32_t value)
 *                    void LETIMER_IntEnable(LETIMER_TypeDef *letimer, uint32_t flags)
 * Returns: None
 *
 */
void timerWaitUs(uint32_t us_wait)
{

    if(us_wait>3000000)
    	{
    		us_wait=3000000; //Maximum possible delay
    	}

    	uint32_t interval,initial,final;

    	initial=LETIMER_CounterGet(LETIMER0);

    	interval=(us_wait*ACTUAL_CLK_FREQ_Hz)/1000000;

    	final=initial-interval;

    	if(final>COMP0)  //If result overflows
    	{
    		interval=65535-final;
    		final=COMP0-interval;
    	}

	LETIMER_CompareSet(LETIMER0,1,final);
	LETIMER_IntEnable(LETIMER0,LETIMER_IF_COMP1); //Enable COMP1 Interrupt

}










