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

void gpioInit();
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioLed1SetOn();
void gpioLed1SetOff();
void gpioSetDisplayExtcomin(bool high);
void gpioSensorSetOn();
void gpioSensorSetOff();

#define GPIO_SET_DISPLAY_EXT_COMIN_IMPLEMENTED (1)
//#define GPIO_DISPLAY_SUPPORT_IMPLEMENTED (1)
#endif /* SRC_GPIO_H_ */
