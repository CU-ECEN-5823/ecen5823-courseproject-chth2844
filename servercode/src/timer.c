/**************************************************************************************************
 * @file      timer.c
 * @version   0.0.1
 *
 *
 * @author    Rucha Borwankar
 * @date
 *
 * @institution University of Colorado Boulder (UCB)
 * @course      ECEN 5823-001: IoT Embedded Firmware (Fall 2020)
 * @instructor  David Sluiter
 *
 * @assignment ecen5823-assignments-ruchaborwankar
 * @due        Sep 18, 2020
 *
 * @resources https://www.silabs.com/documents/public/reference-manuals/bluetooth-api-reference.pdf
 *				silicon labs example-soc thermometer
 *				https://docs.silabs.com/resources/bluetooth/code-examples/applications/thermometer-example-with-efr32-internal-temperature-sensor/source/app.c
 * 			   lecture 6 and 8 IOT
 * 			   https://docs.silabs.com/mcu/5.6/efr32fg1/group-emlib-reference link
 * 				   2.lecture notes-Lecture 6
 * 				   3.https://www.silabs.com/community/mcu/32-bit/knowledge-base.entry.html/2018/08/23/how_to_perform_thei-6Kir
 * 				   4.https://www.silabs.com/community/software/simplicity-studio/forum.topic.html/efr32_-_getting_star-mnGn
 *
 * @copyright  All rights reserved. Distribution allowed only for the use of assignment grading.
 *       Use of code excerpts allowed at the discretion of author. Contact for permission.
 */


//https://silabs-prod.adobecqms.net/community/mcu/32-bit/forum.topic.html/letimer0_comp0_notbeingloadedintocntonunderf-F9Hq
#include "timer.h"

/* Function 	: LETIMER_SETUP()
 * Description  : The initialization for the LETIMER0 which sets the load value for comp0 and comp1 register and sets up
 * 				 the Interrupt for respective flags.
 * */
void LETIMER_setup(void)
{

	// DOS
	uint32_t      temp, frequency;

	  const LETIMER_Init_TypeDef LETIMER_INITIAL_CONFIG = {
		//DOS .enable = true,               /* Start counting when init completed. */
		.enable = false,               /* Start counting when init completed. */
    	//DOS .debugRun = false,            /* Counter shall not keep running during debug halt. */
		.debugRun = true,            /* Counter shall not keep running during debug halt. */
    	.comp0Top = true,             /* Load COMP0 register into CNT when counter underflows. COMP0 is used as TOP */
    	.bufTop = false,              /* Don't load COMP1 into COMP0 when REP0 reaches 0. */
    	.out0Pol = 0,                 /* Idle value for output 0. */
    	.out1Pol = 0,                 /* Idle value for output 1. */
    	.ufoa0 = letimerUFOAPwm,      /* PWM output on output 0 */
    	.ufoa1 = letimerUFOAPwm,      /* PWM output on output 1*/
    	.repMode = letimerRepeatFree  /* Count until stopped */
  	  };

  	  /* Initialize LETIMER */
  	  LETIMER_Init(LETIMER0, &LETIMER_INITIAL_CONFIG);
  	 // timer_flag=0;
  	  //uint16_t actual_clk_freq=LFACLK_FREQ_HZ/LFACLK_PRESCALAR_DIVIDER_RATIO;
  	  //uint16_t actual_clk_period=1/actual_clk_freq;

  	  //printf("clk freq=%d clk period=%d\n\r",actual_clk_freq,actual_clk_period);

  	  comp0=(LETIMER_PERIOD_MS*actual_clk_freq)/1000;
  	  //comp1=(LETIMER_ON_PERIOD*actual_clk_freq)/1000;
  	  LOG_INFO("comp0 value is %d\n\r",comp0);


  	  LETIMER_CompareSet(LETIMER0, 0, comp0);   // set comp0/top value
      temp = LETIMER_CompareGet (LETIMER0, 0); // DOS sanity check, read it back

  	  // DOS LETIMER_TopSet(LETIMER0, comp0);

  	  // DOS clear any spurious/remaining/initial IF bits
  	  LETIMER_IntClear(LETIMER0, 0xFFFFFFFF);
  	  LETIMER_IntEnable(LETIMER0, LETIMER_IEN_UF);  //comp1 interrupt enable

  	  // DOS start the timer running
   	  LETIMER_Enable(LETIMER0, true);

	/* Clear any pending NVIC interrupts */
  	  NVIC_ClearPendingIRQ(LETIMER0_IRQn);

	/* Enable NVIC interrupts */
  	  NVIC_EnableIRQ(LETIMER0_IRQn);

  	  //LETIMER_Enable(LETIMER0, true);

    // check the LETIMER0 frequency
  	frequency = CMU_ClockFreqGet (cmuClock_LETIMER0);

  	// read it a few times to make sure it's running
  	temp = LETIMER_CounterGet (LETIMER0);
  	temp = LETIMER_CounterGet (LETIMER0);
  	temp = LETIMER_CounterGet (LETIMER0);

}
/* Function 	: timerWaitus()
 * Description  : This function is used to check the current counter on letimer and then depending on the millisecond delay required
 * 				  in between 3 sec range is then subtracted from the current counter of letimer.
 * */
void timerWaitus(uint32_t stab_time)
{
		//timer_flag=0;


		uint32_t       stab_time_cnt, current_cnt;
        //uint16_t       comp1; // DOS different var so we can see it in the debugger

		stab_time_cnt = (stab_time*actual_clk_freq)/1000000;    // calculate tick count for requested delay
		current_cnt   = LETIMER_CounterGet(LETIMER0);	        // get current value of counter
		uint32_t comp0_val=LETIMER_CompareGet(LETIMER0,0);
#ifdef DEBUG
		LOG_INFO("LETIMER_CompareGet(LETIMER0,0) %ld",comp0_val);
		LOG_INFO("stab_time_cnt,current_cnt %ld %ld",stab_time_cnt,current_cnt);
#endif
		if(current_cnt < stab_time_cnt)
		{

			comp1=comp0_val-(stab_time_cnt-current_cnt);
			//LOG_INFO("in current_cnt<comp1\n\r");
		}
		else
		{
			comp1= current_cnt - stab_time_cnt;            // comp1 is the value we delay to
#ifdef DEBUG
			LOG_INFO("in current_cnt normal condition");
#endif
		}
		// you're not handling the roll-over case, what happens if the CNT value 25 when this routine is called
		// your stab_time_cnt value is 80?
		LETIMER_CompareSet(LETIMER0, 1, comp1);
#ifdef DEBUG
		LOG_INFO("comp1=%d", comp1);
#endif
		LETIMER_IntEnable(LETIMER0, LETIMER_IEN_COMP1);         // Comp1 interrupt enable

}


