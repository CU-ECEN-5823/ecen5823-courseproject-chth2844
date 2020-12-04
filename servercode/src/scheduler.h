/**************************************************************************************************
 * @file      scheduler.h
 * @version   0.0.1
 * @brief     scheduler.h Function Implementations
 *
 * @author    Rucha Borwankar,Chirayu Thakur
 * @date     Oct 2, 2020
 *
 * @institution University of Colorado Boulder (UCB)
 * @course      ECEN 5823-001: IoT Embedded Firmware (Fall 2020)
 * @instructor  David Sluiter
 *
 * @assignment ecen5823-assignment3-ruchaborwankar
 * @due
 *
 * @resources  https://www.silabs.com/documents/public/reference-manuals/bluetooth-api-reference.pdf
 *				silicon labs example-soc thermometer
 *				https://docs.silabs.com/resources/bluetooth/code-examples/applications/thermometer-example-with-efr32-internal-temperature-sensor/source/app.c
 * 			   lecture 10-IoT
 *
 * @copyright  All rights reserved. Distribution allowed only for the use of assignment grading.
 *       Use of code excerpts allowed at the discretion of author. Contact for permission.
 */

/**Scheduler file helps control the sequence of events.
 * */


//header files
#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_
#include "timer.h"
#include "gpio.h"
#include "i2c.h"
#include "em_core.h"
#include "main.h"
#include "log.h"
#include <stdbool.h>
#include "sleep.h"
#include "irq.h"
#include "ble.h"
#include "gatt_db.h"
#include "infrastructure.h"
#include "native_gecko.h"

//defines
#define LUX_THRESHOLD  3

typedef enum uint32_t{
	null=0,
	timer_UF_flag=1,
	timer_comp1_flag=2,
	write_transfer_done=4,  // DOS define these as a single bit so we can set and clear individual bits
	read_transfer_done=8,
	PB0_pressed,
	PB0_released,
}temp_events;


typedef enum State
{
	STATE0_TIMER_WAIT,
	STATE_1_ENABLE_SENSOR,
	STATE2_READ_ADC,
	STATE3_REPORT
} State_t;



typedef enum lux_event
{
	LUX_POWER_OFF                      = 0x01,
	LUX_WAIT_FOR_POWER_UP              = 0x02,
	LUX_LETIMER0_SENSOR_ENABLE                 = 0x04,
	LUX_LETIMER0_READ                          = 0x08,
	LUX_I2C_TRANSFER_DONE              = 0x10
} Lux_Event_t;


uint32_t event_flag;

void schedulerSetEvent();
bool events_present();
void process_event();
uint32_t get_event();
void temp_read_ble(float temp);
void state_machine(struct gecko_cmd_packet *evt);
#endif /* SRC_SCHEDULER_H_ */
