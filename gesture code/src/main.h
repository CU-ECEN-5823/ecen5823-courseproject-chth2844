/*
 * main.h
 *
 *  Created on: 09-Sep-2020
 *      Author: Chirayu Thakur
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_
#include "gecko_configuration.h"
#include "gpio.h"
#include "native_gecko.h"
#include"em_cmu.h"
#include "sleep.h"      //For sleepmode related routines
#include "timers.h"     //For letimer related functions
#include "oscillators.h"//For oscillator related functions
#include "i2c.h"        //For i2c related functions
#include "scheduler.h"  //For i2c scheduler
#include "em_letimer.h"
#include "zxsensor.h"

// DOS enables LOG_*** calls
#define     DEBUG


//System defines
#define LFXO_FREQ_HZ  32768
#define ULFRCO_FREQ_HZ 1000
//Define this to enable sleeping in the main while(1) loop
//#define ENABLE_SLEEPING 1
//Energy Modes
//#define LOWEST_ENERGY_MODE 0
//#define LOWEST_ENERGY_MODE 1
//#define LOWEST_ENERGY_MODE 2
#define LOWEST_ENERGY_MODE 3
//Clock Frequency and Prescaler Value depends upon Energy Mode
//#if LOWEST_ENERGY_MODE==0
//   #define LFACLK_FREQ_HZ          LFXO_FREQ_HZ
//   #define PRESCALER_DIVIDE_RATIO (cmuClkDiv_4)
//#endif

//#if LOWEST_ENERGY_MODE==1
   //#define LFACLK_FREQ_HZ          LFXO_FREQ_HZ
   //#define PRESCALER_DIVIDE_RATIO (cmuClkDiv_4)
//#endif

//#if LOWEST_ENERGY_MODE==2
//   #define LFACLK_FREQ_HZ          LFXO_FREQ_HZ
//   #define PRESCALER_DIVIDE_RATIO (cmuClkDiv_4)
//#endif

#if LOWEST_ENERGY_MODE==3
   #define LFACLK_FREQ_HZ          ULFRCO_FREQ_HZ
   #define PRESCALER_DIVIDE_RATIO (cmuClkDiv_1)
#endif



#endif /* SRC_MAIN_H_ */
