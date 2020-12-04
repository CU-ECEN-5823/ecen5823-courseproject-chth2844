/**************************************************************************************************
 * @file      irq.c
 * @version   0.0.1
 *
 *
 * @author    Rucha Borwankar,chirayu thakur
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

#include "src/irq.h"
#include "timer.h"

extern I2C_TransferReturn_TypeDef transferstatus;

//int rollover;
extern uint8_t sensor_enable;
//uint32_t timestamp;

uint32_t      myMilliseconds = 0; // DOS
/* Function Name: void LETIMER0_IRQHandler()
 *
 * Description: Interrupt Handler for LETIMER0 peripheral. Schedules Temperature Measure  event  every 3s.
 *
 * Parameters: None
 *
 * Silab API's used:void LETIMER_IntClear(LETIMER_TypeDef *letimer, uint32_t flags),
 *                  uint32_t LETIMER_IntGet(LETIMER_TypeDef *letimer)
 *
 * Returns:None
 *
 */

void LETIMER0_IRQHandler()
{
	CORE_DECLARE_IRQ_STATE;

	uint32_t    flag;

		 flag = LETIMER_IntGet(LETIMER0);  // get IF bits, IF bits are often = 0x07 -> UF, COMP1 and COMP0
		 LETIMER_IntClear(LETIMER0, flag); // punch down all IF bits in LETIMER0

		 // DOS
		 // Your bug was not punching down COMP0 IF bit. You had calls to IntClear for UF and COMP1, but COMP0
		 // remained set. This caused an immediate return from interrupt when you called SLEEP_Sleep(), this
		 // is why it appeared you were in EM0 for 90ms


		 CORE_ENTER_CRITICAL();

		 if (flag & LETIMER_IF_UF)
		{

			 myMilliseconds += 3000; // DOS should probably have a #define value to replace 3000

			 //DOS LOG_WARN("******Irq handler UF*******\n\r");
#ifdef DEBUG
			 LOG_INFO("*UF"); // way fewer chars to transfer: faster
#endif

		 	 //CORE_ENTER_CRITICAL();
			 schedulerSetEvent(LUX_WAIT_FOR_POWER_UP);
		 	 //CORE_EXIT_CRITICAL();

		 	 roll_over=roll_over+1;
		 }

		 //DOS*********  else if (flag & LETIMER_IF_COMP1)
	     if (flag & LETIMER_IF_COMP1)

		 {

			 //DOS LOG_WARN("******Irq handler COMP1*******\n\r");
#ifdef DEBUG
			 LOG_INFO("*COMP1"); // way fewer chars to transfer: faster
#endif

		 	 //CORE_ENTER_CRITICAL();
			 if(sensor_enable==1)
			{
			 schedulerSetEvent(LUX_LETIMER0_READ);
			}
			 else{

					schedulerSetEvent(LUX_LETIMER0_SENSOR_ENABLE);

				}

		 	 //CORE_EXIT_CRITICAL();

		 	LETIMER_IntDisable(LETIMER0, LETIMER_IEN_COMP1);

		 }

	     CORE_EXIT_CRITICAL();

} // LETIMER0_IRQHandler()
/*
void LETIMER0_IRQHandler()
{
	uint32_t reason =  LETIMER_IntGet(LETIMER0); //determine pending interrupts
	//static int counter=0;
	LETIMER_IntClear(LETIMER0,reason);

	CORE_DECLARE_IRQ_STATE;

	// DOS
	//if(reason==(LETIMER_IF_COMP0|LETIMER_IF_UF|LETIMER_IF_COMP1))//3s Interval Interrupt
	if (reason & LETIMER_IF_COMP0)
	{

		rollover++;

		CORE_ENTER_CRITICAL();
		schedulerSetEvent(LUX_WAIT_FOR_POWER_UP);
		CORE_EXIT_CRITICAL();

		uint32_t count = (rollover*comp0)-LETIMER_CounterGet (LETIMER0); //Get timer count

		timestamp=(count*1000)/actual_clk_freq; //Return Timestamp in ms
	}


	if (reason & LETIMER_IF_COMP1)
	{

		CORE_ENTER_CRITICAL();
		if(sensor_enable==1)
		{
			schedulerSetEvent(LUX_LETIMER0_READ);

		}

		else{

			schedulerSetEvent(LUX_LETIMER0_SENSOR_ENABLE);

		}
		CORE_EXIT_CRITICAL();



		LETIMER_IntDisable(LETIMER0,LETIMER_IF_COMP1);


	}
}//LETIMER0_IRQHandler()
*/
uint32_t getMyMilliseconds() {
	return (myMilliseconds);
} // getMyMilliseconds()

void I2C0_IRQHandler(void){

	CORE_DECLARE_IRQ_STATE;
	transferstatus=I2C_Transfer(I2C0); //shepherds the transfer
	// LOG_INFO("Transfer Value %d, Transfer Status %d\n",transfer,transferstatus);


	if (transferstatus==i2cTransferDone) {
		CORE_ENTER_CRITICAL();
		schedulerSetEvent(LUX_I2C_TRANSFER_DONE);
		CORE_EXIT_CRITICAL();
	}


} // I2C0_IRQHandler()


