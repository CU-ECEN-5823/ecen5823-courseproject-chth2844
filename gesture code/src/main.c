/*
 * main.c
 *
 *  Updated on: 18-Sep-2020
 *      Author: Chirayu Thakur
 */

#include "main.h"
#include "log.h"

//extern I2C_TransferReturn_TypeDef transferstatus;
SLEEP_EnergyMode_t energy_Mode_Entered; //Contains Energy Mode entered during sleep
uint32_t evt; //Event to be processed

/*
 * Function name: int appMain(gecko_configuration_t *config)
 *
 * Description: Initialize peripherals, process events and enter low power mode
 * .
 * Parameters: gecko_configuration_t *config
 *
 *  Silab API's Used: errorcode_t gecko_init(const gecko_configuration_t *config),
 *                    void SLEEP_SleepBlockBegin(SLEEP_EnergyMode_t eMode),
 *                    SLEEP_EnergyMode_t SLEEP_Sleep(void)
 *
 *  Returns: None
 *
 */



int appMain(gecko_configuration_t *config)
{


	  gecko_init(config);// Initialize stack
	  logInit();
	  gpioInit(); //Initialize gpio
	  init_oscillators(); //Initialize oscillators
	  initLETIMER0(); //Initialize LETIMER0 as well as it's interrupts
	  InitI2C();      //Initialize I2C peripheral
	  enable_gesture_interrupt();
	  SLEEP_SleepBlockBegin(sleepEM4);//Block Energy modes equal to and greater than EM2



  /* Infinite loop */
	while (1) {


		if (!events_present()) { // no more events

		logFlush(); // flush the LOG before we sleep
		SLEEP_Sleep();	 // go to lowest energy level
		// wait for IRQ
		}

		else
		{
		evt = get_event(); // get 1 event
		//sequence through states driven by events
		state_machine(evt);
		}


      }//while(1)

}//appMain()

