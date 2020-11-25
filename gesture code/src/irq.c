/*
 * irq.c
 *
 *  Created on: 08-Sep-2020
 *      Author: Chirayu Thakur
 */

#include "irq.h"


uint32_t timestamp;




 void GPIO_ODD_IRQHandler()
  {
 	 GPIO_IntClear(1 << DR_pin);
 	 writeByteI2C(ZX_STATUS);
 	 readI2C();
 	 schedulerSetEventGestureIntReceived();
  }





