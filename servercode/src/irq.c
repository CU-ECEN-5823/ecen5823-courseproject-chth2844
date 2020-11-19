/**************************************************************************************************
 * @file      irq.c
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

#include "src/irq.h"

/*Function:LETIMER0_IRQHANDLER
 * Description:When interrupt occurs on letimer0 this function is serviced.
 * 				In this function,we call scheduler function in critical section as there is change in global variable.
 *
 * **/

uint32_t      myMilliseconds = 0; // DOS

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
			 schedulerSetEvent(timer_UF_flag);
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
		 	 schedulerSetEvent(timer_comp1_flag);
		 	 timer_flag=1;
		 	 //CORE_EXIT_CRITICAL();

		 	LETIMER_IntDisable(LETIMER0, LETIMER_IEN_COMP1);

		 }

	     CORE_EXIT_CRITICAL();

} // LETIMER0_IRQHandler()

uint32_t getMyMilliseconds() {
	return (myMilliseconds);
} // getMyMilliseconds()

void I2C0_IRQHandler(void){

	 CORE_DECLARE_IRQ_STATE;
	 uint16_t transferStatus;
	 uint16_t flag=I2C0->IF & I2C0->IEN;

	 // get the IRQ flags and'ed with what's enabled,
	 // see why we got here

	 //DOS LOG_WARN("******I2C handler*******\n\r");
#ifdef DEBUG
	 LOG_WARN("I2C"); // way less chars to send
#endif

	 transferStatus = I2C_Transfer(I2C0);
	 if (transferStatus==i2cTransferDone) {

	     CORE_ENTER_CRITICAL();

	     if (read==1) {
	         schedulerSetEvent(read_transfer_done);
	         read=0;
	     }
	     else if (write==1) {
		     schedulerSetEvent(write_transfer_done);
		     write=0;
		     // LOG_WARN("WRITE SUCCESSFUL\n");
	     }

	     CORE_EXIT_CRITICAL();

	 } // if

} // I2C0_IRQHandler()


