/**************************************************************************************************
 * @file      main.c
 * @version   0.0.1
 * @brief     main.c Function Implementations
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
 * 			   lecture 10-IoT
 * 			   https://docs.silabs.com/mcu/5.6/efr32fg1/group-emlib-reference link
 * 				   2.lecture notes-Lecture 6
 * 				   3.https://www.silabs.com/community/mcu/32-bit/knowledge-base.entry.html/2018/08/23/how_to_perform_thei-6Kir
 * 				   4.https://www.silabs.com/community/software/simplicity-studio/forum.topic.html/efr32_-_getting_star-mnGn
 *
 * @copyright  All rights reserved. Distribution allowed only for the use of assignment grading.
 *       Use of code excerpts allowed at the discretion of author. Contact for permission.
 */

#include "src/main.h"


// DOS
#ifdef USE_SYSTICKS

// global variable
uint32_t    sysTicks = 0;   // # of ticks

// SysTick interrupt Handler. See startup file startup_LPC17xx.s for SysTick vector
// Linker picks up this label and inserts into the IRQ vector table
// CPU clears the NVIC IRQ pending bit when it fetches the first instruction of this routine
void SysTick_Handler(void) {

	sysTicks++;

} // SysTick_Handler()

#endif
/*reference:bluetooth example code=soc temperature example*/
#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];




int appMain(gecko_configuration_t *config)
{
	SLEEP_Init_t         sleepInitExData;
	gecko_init(config);
	gpioInit();				//Initialize the GPIO
	CMU_init();				//Initialize the CLOCK
	LETIMER_setup();		//Initialize the LETIMER0
	InitI2C();      //Initialize I2C peripheral

	//sleep function

	memset (&sleepInitExData, 0, sizeof(sleepInitExData));
    SLEEP_InitEx(&sleepInitExData);
	SLEEP_SleepBlockBegin(sleepEM3);
	displayInit();

	// DOS
#ifdef USE_SYSTICKS
	 uint32_t returnCode;

	  returnCode = SysTick_Config(SystemCoreClock / 1000);      /* Configure SysTick to generate an interrupt every millisecond */
	  if (returnCode != 0)  {                                   /* Check return code for errors */
		  LOG_ERROR ("SysTick_Config() returned a non-zero error code.\n");
	  }
#endif

	  while (1)
	  {

		  // DOS
		  if (!gecko_event_pending()) {
			  logFlush(); // flush the LOG before we sleep
			  //SLEEP_Sleep();
			  //__WFI();
		  }

		   evt = gecko_wait_event();	//check gecko ble events
		   handle_ble_event(evt);		//handle the ble events
		   state_machine(evt);			//temperature state machine to calculate the temp
#if DEVICE_IS_BLE_SERVER==0
		   char_event_sm(evt);
#endif
	  } // while(1)

} // appMain()
