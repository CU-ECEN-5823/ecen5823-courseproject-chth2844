/*
 * gpio.c
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */
#include "gpio.h"
#include "em_gpio.h"
#include <string.h>


/**
 * TODO: define these.  See the radio board user guide at https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf
 * and GPIO documentation at https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__GPIO.html
 */
#define	LED0_port  gpioPortF
#define LED0_pin  4
#define LED1_port gpioPortF
#define LED1_pin  5


#define SENSOR_pin  15
#define SENSOR_port gpioPortD

void gpioInit()
{
//	//GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
//	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
//	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);

GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
//	//GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, false);

	//GPIO_DriveStrengthSet(SENSOR_port, gpioDriveStrengthWeakAlternateWeak);
	//GPIO_PinModeSet(SENSOR_port,SENSOR_pin, gpioModePushPull, false);


	GPIO_PinModeSet(DR_port,DR_pin, gpioModeInputPullFilter , true);


}

//void gpioLed0SetOn()
//{
//	GPIO_PinOutSet(LED0_port,LED0_pin);
//}
//void gpioLed0SetOff()
//{
//	GPIO_PinOutClear(LED0_port,LED0_pin);
//}
void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED1_port,LED1_pin);
}
//void gpioLed1SetOff()
//{
//	GPIO_PinOutClear(LED1_port,LED1_pin);
//}


void setGestureInterrupt()
{
	GPIO_ExtIntConfig (DR_port, DR_pin, DR_pin, false, true, true);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
}

void gpioSensorSetOn()
{
	GPIO_PinOutSet(SENSOR_port,SENSOR_pin);


}

void gpioSensorSetOff()
{
	GPIO_PinOutClear(SENSOR_port,SENSOR_pin);
}




