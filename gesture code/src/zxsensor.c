/*
 * zxgesturesensor.c
 *
 *  Created on: 14-Nov-2020
 *      Author: chira
 */

#include "zxsensor.h"

extern uint16_t read_buffer[2];         //Read value from Sensor is stored here
uint16_t interrupt_set;



void enable_gesture_interrupt()
{

	 //write_readi2c(ZX_MODEL); //Set Gesture Sense Interrupt

	//writeByteI2C(ZX_DRE); //ge
	writeDataByteI2C(ZX_DRE,0x02);
   //interrupt_set=read_buffer[0];
    //writeDataByteI2C(ZX_DRCFG,0x00); //Set Active Low Interrupt
 	//setGestureInterrupt(); //Set GPIO Interrupt


}

int interpret_gesture()
{

	writeByteI2C(ZX_GESTURE);
	readI2C();
	schedulerSetEventDisplayGesture();
	 switch(read_buffer[0])
	 {
	    case RIGHT_SWIPE:
	             return RIGHT_SWIPE;

	  case LEFT_SWIPE:
	             return LEFT_SWIPE;

	   case UP_SWIPE:
	             return UP_SWIPE;

	   default:
	          return NO_GESTURE;


	 }



}

bool gesture_available()
{
	writeByteI2C(ZX_STATUS);
	readI2C();
	read_buffer[0]&=0b00011100;
	if (read_buffer[0]) {
	        return true;
	    }

	    return false;


}

