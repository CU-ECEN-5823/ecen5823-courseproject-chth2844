/*
 * timer.h
 *
 *  Created on: 07-Sep-2020
 *      Author: Chirayu Thakur
 */

#ifndef MY_TIMERS_H_
#define MY_TIMERS_H_
#include "gecko_configuration.h"
#include "native_gecko.h"
#include"em_letimer.h"
#include"em_cmu.h"
#include"main.h"
#include"gpio.h"
#include "sleep.h"
#include "timers.h"
#include "main.h"
//defines
#define  PERIOD_TIME_MS            3000
//DOS 10.8ms, #define CONVERISON_TIME_US         10000
#define CONVERISON_TIME_US         10800
#define POWERUP_TIME_US            80000
#define ACTUAL_CLK_FREQ_Hz     LFACLK_FREQ_HZ  /PRESCALER_DIVIDE_RATIO
#define COMP0  ((PERIOD_TIME_MS*ACTUAL_CLK_FREQ_Hz) /1000)
//#define COMP1  COMP0-((POWERUP_TIME_MS*ACTUAL_CLK_FREQ_Hz) /1000)




//function prototypes
void initLETIMER0();
void timerWaitUs(uint32_t us_wait);

#endif /* MY_TIMERS_H_ */
