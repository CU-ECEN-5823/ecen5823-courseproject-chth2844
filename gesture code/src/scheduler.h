/*
 * scheduler.h
 *
 *  Created on: 17-Sep-2020
 *      Author: chira
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_
#include "stdbool.h"
#include "i2c.h"
#define INCLUDE_LOG_DEBUG 1
#include "log.h"
#include "em_core.h"
#include "zxsensor.h"
//States
typedef enum State
{
//	STATE0_TIMER_WAIT,
//	STATE1_WARMUP,
//	STATE2_TEMP_WAIT,
//	STATE3_READ_WAIT,
//	STATE4_REPORT,
//	MY_NUM_STATES
	STATE0_INTERRUPT_WAIT,
	STATE1_DISPLAY_GESTURE,


} State_t;

// DOS - yikes these values are 0,1,2,3,4,5 and therefore cannot be or'ed together !!!
/*
typedef enum uint32_t
{
	TEMP_POWER_OFF,
	TEMP_WAIT_FOR_POWER_UP,
	TEMP_WAIT_FOR_I2C_WRITE_COMPLETE,
	TEMP_WAIT_FOR_CONVERSION,
	TEMP_WAIT_FOR_I2C_READ_START,
	TEMP_WAIT_FOR_I2C_READ_COMPLETE

} Temp_Event_t;
*/

// DOS need to be individual bits
typedef enum uint32_t
{
//	TEMP_POWER_OFF                      = 0x01,
//	TEMP_WAIT_FOR_POWER_UP              = 0x02, // DOS, this happens on a COMP0 IRQ (no UF in this design, which is ok)
//	TEMP_LETIMER0_COMP1                 = 0x04, // DOS added for COMP1 for waitTimerus()
//	TEMP_I2C_TRANSFER_DONE              = 0x08  // DOS added for I2C transfer Done
//	TEMP_WAIT_FOR_I2C_WRITE_COMPLETE    = 0x08,
//	TEMP_WAIT_FOR_CONVERSION            = 0x10,
//	TEMP_WAIT_FOR_I2C_READ_START        = 0x20,
//	TEMP_WAIT_FOR_I2C_READ_COMPLETE     = 0x40
	GESTURE_DR_INTERRUPT_RECEIVED       = 0x01,
	GESTURE_DISPLAY_GESTURE             = 0x02,
	GESTURE_DISPLAY_UPDATED             = 0x04


} GEST_Event_t;


//Function Prototypes
void schedulerSetEventTempWaitPowerUp();
// DOS added
void schedulerSetEventComp1(void);
// DOS added
void schedulerSetEventI2CTransferDone(void);
void schedulerSetEventTempWaitWriteComplete();
void schedulerSetEventTempWaitForConversion();
void schedulerSetEventTempWaitReadStart();
void schedulerSetEventTempWaitReadComplete();
void schedulerSetEventPowerOff();
bool events_present();
void I2CEM1BlockStart();
void I2CEM1BlockEnd();
void EM3BlockStart();
void EM3BlockEnd();
void state_machine (uint32_t event);
void schedulerSetEventGestureIntReceived();
void schedulerSetEventDisplayGesture();
void schedulerSetEventDisplayGestureUpdated();
uint32_t get_event();

#endif /* SRC_SCHEDULER_H_ */
