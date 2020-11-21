/*
 * scheduler.c
 *
 *  Created on: 17-Sep-2020
 *      Author: Chirayu Thakur
 */
#include "scheduler.h"
GEST_Event_t      temp_event;

State_t           currentState;

State_t    nextState = STATE0_INTERRUPT_WAIT;
uint8_t gesture;


/*
 * Function Name: void state_machine (uint32_t event)
 *
 * Description: State Machine to perform i2c transfer to sensor
 * .
 * Parameters: uint32_t event
 *
 *  Silab API's Used: None
 *
 * Returns: None
 *
 */



// Our events:
//   TEMP_POWER_OFF                      = 0x01,
//   TEMP_WAIT_FOR_POWER_UP              = 0x02, // DOS, this happens on a COMP0 IRQ (no UF in this design, which is ok)
//   TEMP_LETIMER0_COMP1                 = 0x04, // DOS added for COMP1 for waitTimerus()
//   TEMP_I2C_TRANSFER_DONE              = 0x08  // DOS added for I2C transfer Done

void state_machine (uint32_t event)
{

	currentState = nextState; // DOS, missing assignment

switch (currentState)
{

case STATE0_INTERRUPT_WAIT:
	nextState=STATE0_INTERRUPT_WAIT;
	if (event == GESTURE_DR_INTERRUPT_RECEIVED)
	{
		if(gesture_available())
		{
			gesture=interpret_gesture();
			LOG_INFO("State 0");

		}

		nextState = STATE1_DISPLAY_GESTURE;

	}

	break;

case STATE1_DISPLAY_GESTURE:

	nextState=STATE1_DISPLAY_GESTURE;
	if (event == GESTURE_DISPLAY_GESTURE)
	{
		switch(gesture)
		{
		case RIGHT_SWIPE:
#ifdef DEBUG
			LOG_INFO("RIGHT_SWIPE");
#endif
			break;

		case LEFT_SWIPE:
#ifdef DEBUG
			LOG_INFO("LEFT_SWIPE");
#endif
			break;

		case UP_SWIPE:
#ifdef DEBUG
			LOG_INFO("UP_SWIPE");
#endif
			break;

		default:
#ifdef DEBUG
			LOG_INFO("NO_GESTURE");
#endif
			break;


		}

		nextState=STATE0_INTERRUPT_WAIT;
		schedulerSetEventDisplayGestureUpdated();



	}

	break;

default:

	break;

} // switch

} // state_machine()




void schedulerSetEventGestureIntReceived()
{
	temp_event|=GESTURE_DR_INTERRUPT_RECEIVED;
}

void schedulerSetEventDisplayGesture()
{
	temp_event|=GESTURE_DISPLAY_GESTURE;
}

void schedulerSetEventDisplayGestureUpdated()
{
	temp_event|=GESTURE_DISPLAY_UPDATED;
}



// DOS
// Have to select one event to return AND clear from our data structure

uint32_t get_event()
{
	CORE_DECLARE_IRQ_STATE;

   //uint32_t theEvent;
   // determine 1 event to return
   //theEvent = temp_event;
  // temp_event^=temp_event; //clear event

	CORE_ENTER_CRITICAL();

   if (temp_event & GESTURE_DR_INTERRUPT_RECEIVED) {
	   temp_event = temp_event ^ GESTURE_DR_INTERRUPT_RECEIVED; // clear event, needs to be read-modify-write
	   CORE_EXIT_CRITICAL();
	   return (GESTURE_DR_INTERRUPT_RECEIVED);
   } else

   if (temp_event & GESTURE_DISPLAY_GESTURE) {
	   temp_event = temp_event ^ GESTURE_DISPLAY_GESTURE; // clear event, needs to be read-modify-write
	   CORE_EXIT_CRITICAL();
	   return (GESTURE_DISPLAY_GESTURE);
   } else

   if (temp_event & GESTURE_DISPLAY_UPDATED) {
	   temp_event = temp_event ^ GESTURE_DISPLAY_UPDATED; // clear event, needs to be read-modify-write
	   CORE_EXIT_CRITICAL();
	   return (GESTURE_DISPLAY_UPDATED);
   } else



   CORE_EXIT_CRITICAL();
   return (0); //DOS, no event


} // get_event()






/*
 * Function name: bool events_present()
 *
 * Description: Returns TRUE if event present,False if not.
 * .
 * Parameters: None
 *
 *  Silab API's Used:None
 *
 * Returns: TRUE or FALSE
 *
 */
bool events_present()
{
    // DOS don't need a function call, you can look at the variable directly
	// if(get_event()!=0)
	if (temp_event)
  {
	return true;// return true if any events pending
  }

  else
  {
	return false; //else return false
  }

} // events_present()






