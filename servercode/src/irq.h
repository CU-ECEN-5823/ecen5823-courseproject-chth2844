/**************************************************************************************************
 * @file      irq.h
 * @version   0.0.1
 *
 *
 * @author    Rucha Borwankar
 * @date
 *
 * @institution University of Colorado Boulder (UCB)
 * @course      ECEN 5823-001: IoT Embedded Firmware (Fall 2020)
 * @instructor  David Sluiter
 *
 * @assignment ecen5823-assignments-ruchaborwankar
 * @due        Sep 18, 2020
 *
 * @resources https://www.silabs.com/documents/public/reference-manuals/bluetooth-api-reference.pdf
 *				silicon labs example-soc thermometer
 *				https://docs.silabs.com/resources/bluetooth/code-examples/applications/thermometer-example-with-efr32-internal-temperature-sensor/source/app.c
 * 			   lecture 6 and 8 IOT
 * 			   https://docs.silabs.com/mcu/5.6/efr32fg1/group-emlib-reference link
 * 				   2.lecture notes-Lecture 6
 * 				   3.https://www.silabs.com/community/mcu/32-bit/knowledge-base.entry.html/2018/08/23/how_to_perform_thei-6Kir
 * 				   4.https://www.silabs.com/community/software/simplicity-studio/forum.topic.html/efr32_-_getting_star-mnGn
 *
 * @copyright  All rights reserved. Distribution allowed only for the use of assignment grading.
 *       Use of code excerpts allowed at the discretion of author. Contact for permission.
 */


#ifndef SRC_IRQ_H_
#define SRC_IRQ_H_
#include "timer.h"
#include "main.h"
#include "em_letimer.h"
#include "gpio.h"
#include "em_core.h"
#include "oscillators.h"
#include "log.h"
//assign 3
#include "i2c.h"
#include "scheduler.h"

//function prototypes
void LETIMER0_IRQHandler();

#endif /* SRC_IRQ_H_ */
