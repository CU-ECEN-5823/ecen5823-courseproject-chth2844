/*
 * irq.h
 *
 *  Created on: 08-Sep-2020
 *      Author: chira
 */

#ifndef SRC_IRQ_H_
#define SRC_IRQ_H_
#include "em_letimer.h"
#include "gpio.h"
#include "timers.h"
#include "i2c.h"
#include "em_core.h"
#include "scheduler.h"
#include "log.h"
#include "em_core.h"
#include "scheduler.h"
#include "zxsensor.h"
//function prototypes
void LETIMER0_IRQHandler();
#endif /* SRC_IRQ_H_ */
