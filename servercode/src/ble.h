/**************************************************************************************************
 * @file      ble.h
 * @version   0.0.1
 * @brief     ble.h Function Implementations
 *
 * @author    Rucha Borwankar
 * @date     Oct 2, 2020
 *
 * @institution University of Colorado Boulder (UCB)
 * @course      ECEN 5823-001: IoT Embedded Firmware (Fall 2020)
 * @instructor  David Sluiter
 *
 * @assignment ecen5823-assignment5-ruchaborwankar
 * @due        Sep 18, 2020
 *
 * @resources https://www.silabs.com/documents/public/reference-manuals/bluetooth-api-reference.pdf
 *				silicon labs example-soc thermometer
 *				https://docs.silabs.com/resources/bluetooth/code-examples/applications/thermometer-example-with-efr32-internal-temperature-sensor/source/app.c
 * 			   lecture 10-IoT
 *
 * @copyright  All rights reserved. Distribution allowed only for the use of assignment grading.
 *       Use of code excerpts allowed at the discretion of author. Contact for permission.
 */

#ifndef SRC_BLE_H_
#define SRC_BLE_H_
#include "native_gecko.h"
#include "gatt_db.h"

#include "main.h"
#include "em_emu.h"
#include "em_cmu.h"
#include "main.h"

bool bool_flag;
bool bool_relay_flag;
uint8_t start_sm_flag;
void handle_ble_event(struct gecko_cmd_packet *evt);
#endif /* SRC_BLE_H_ */
