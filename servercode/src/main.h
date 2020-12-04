/*
 * main.h
 *
 *  Created on: Sep 9, 2020
 *      Author: rucha & chirayu
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

#include "gecko_configuration.h"
#include "gpio.h"
#include "oscillators.h"

//assign 2-ref:lec slides
#include <stdio.h>		//printfs
#include "em_cmu.h"		//emlib header for clock management unit
#include "em_letimer.h"	//emlib header
#include "sleep.h"		//sleep control
#include "timer.h"		//timer capabilites
#include "irq.h"		//servicing interrupt service routines

//assign 3
#include "log.h"		//logging capabilities
#include "i2c.h"		//i2c functions for write and read and initialization
#include "scheduler.h"
//system defines

//assign 4
// DOS - only use systicks for debugging. make sure to comment out when you submit your code
#include "core_cm4.h"       // for sysTick
//#define USE_SYSTICKS
//#define DEBUG

//assign 5 headers
#include "ble.h"
#include "gecko_ble_errors.h"
#include "native_gecko.h"
#include "gatt_db.h"
#include "infrastructure.h"

//assign 6
#include "display.h"
#define LFXO_FREQ_HZ 	32768		//Low frequency mode
#define ULFRCO_FREQ_HZ	1000		//ultra low frequency mode freq

//energy modes and sleep

#define ENABLE_SLEEPING 	1		//enable sleep function

//#define LOWEST_ENERGY_MODE 0
//#define LOWEST_ENERGY_MODE 1
//#define LOWEST_ENERGY_MODE 2		//energy mode 2 is setup
#define LOWEST_ENERGY_MODE 3

struct gecko_cmd_packet *evt;
#if LOWEST_ENERGY_MODE == 0
#define LFACLK_FREQ_HZ						LFXO_FREQ_HZ
#define LFACLK_PRESCALAR_DIVIDER_RATIO		4
#endif

#if LOWEST_ENERGY_MODE == 1
#define LFACLK_FREQ_HZ						LFXO_FREQ_HZ
#define LFACLK_PRESCALAR_DIVIDER_RATIO		4
#endif

#if LOWEST_ENERGY_MODE == 2
#define LFACLK_FREQ_HZ						LFXO_FREQ_HZ
#define LFACLK_PRESCALAR_DIVIDER_RATIO		4
#endif

#if LOWEST_ENERGY_MODE == 3
#define LFACLK_FREQ_HZ						ULFRCO_FREQ_HZ
#define LFACLK_PRESCALAR_DIVIDER_RATIO		1
#endif


#endif /* SRC_MAIN_H_ */
