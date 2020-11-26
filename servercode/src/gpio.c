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
#define	LED0_port gpioPortF
#define LED0_pin  4
#define LED1_port gpioPortF
#define LED1_pin  5
#define ExtComin_port gpioPortD
#define ExtComin_pin  13
#define LUX_pin 7
#define LUX_port gpioPortC

void gpioInit()
{
	 //GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
	 GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	 GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);

	 //GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
	 GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	 GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, false);

	 GPIO_PinModeSet(RELAY_port, RELAY_pin, gpioModePushPull,false);
	 GPIO_PinModeSet(LUX_port,LUX_pin, gpioModePushPull, false);
}

void gpioLed0SetOn()
{
	GPIO_PinOutSet(LED0_port,LED0_pin);
}
void gpioLed0SetOff()
{
	GPIO_PinOutClear(LED0_port,LED0_pin);
}
void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED1_port,LED1_pin);
}
void gpioLed1SetOff()
{
	GPIO_PinOutClear(LED1_port,LED1_pin);
}

/****
 * Function:gpioSetDisplayExtcomin(bool high)
 * Description:This function is used to toggle extcomin pin in order to prevent charge build up.
 * this pin is toggled after every 1 sec as set by software timer and everytime this event triggered it enters displayUpdate
 * which calls the function and toggles the pin.
 */
void gpioSetDisplayExtcomin(bool flag)
{
	if(flag)
	{
		GPIO_PinOutSet(ExtComin_port, ExtComin_pin);
	}
	else
	{
		GPIO_PinOutClear(ExtComin_port, ExtComin_pin);
	}
}

void gpioSensorSetOn()
{
	GPIO_PinOutSet(LUX_port,LUX_pin);


}

void gpioSensorSetOff()
{
	GPIO_PinOutClear(LUX_port,LUX_pin);
}

