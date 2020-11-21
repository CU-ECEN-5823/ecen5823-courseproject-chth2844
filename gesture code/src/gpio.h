/*
 * gpio.h
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_
#include <stdbool.h>

#define DR_port gpioPortA
#define DR_pin   5

//Function Prototypes
void gpioInit();
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioLed1SetOn();
void gpioLed1SetOff();
void gpioSensorSetOn();
void gpioSensorSetOff();
void setGestureInterrupt();
#endif /* SRC_GPIO_H_ */
