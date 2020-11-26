/*
 * gpio.h
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_
#include <stdbool.h>
#define RELAY_port gpioPortC
#define RELAY_pin  9
#define PB0_port gpioPortF
#define PB0_pin  6

void gpioInit();
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioLed1SetOn();
void gpioLed1SetOff();
void gpioSetDisplayExtcomin(bool high);

#define GPIO_SET_DISPLAY_EXT_COMIN_IMPLEMENTED (1)
//#define GPIO_DISPLAY_SUPPORT_IMPLEMENTED (1)
#endif /* SRC_GPIO_H_ */
