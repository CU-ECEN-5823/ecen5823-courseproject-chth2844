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
#include "ble_device_type.h"
#include <math.h>
#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif


// connection parameters
//ref:SOC temperature client code
#define CONN_INTERVAL_MIN             80   //100ms
#define CONN_INTERVAL_MAX             80   //100ms
#define CONN_SLAVE_LATENCY            0    //no latency
#define CONN_TIMEOUT                  100  //1000ms

#define SCAN_INTERVAL                 16   //10ms
#define SCAN_WINDOW                   16   //10ms
#define SCAN_PASSIVE                  0
#define TEMP_INVALID                  (uint32_t)0xFFFFFFFFu
#define RSSI_INVALID                  (int8_t)127
#define CONNECTION_HANDLE_INVALID     (uint8_t)0xFFu
#define SERVICE_HANDLE_INVALID        (uint32_t)0xFFFFFFFFu
#define CHARACTERISTIC_HANDLE_INVALID (uint16_t)0xFFFFu
#define TABLE_INDEX_INVALID           (uint8_t)0xFFu

#define EXT_SIGNAL_PRINT_RESULTS      0x01

typedef enum {
  scanning,
  opening,
  discoverServices,
  discoverCharacteristics,
  enableIndication,
  running
} ConnState;

typedef struct {
  uint8_t  connectionHandle;
  int8_t   rssi;
  uint16_t serverAddress;
  uint32_t thermometerServiceHandle;
  uint16_t thermometerCharacteristicHandle;
  uint32_t temperature;
} ConnProperties;

//setting up enum similar to example code for client events to check if service/characteristic obtained
typedef enum client_event
{
	clearevent=0,
	sensor_char=1,
	sensor_service=2,
	relay_char=3,
	relay_service=4
}Client_Evt;
typedef enum State_char
{

	discover_serv1,
	discover_char1,
	notify_char1,
	read_char1,
	discover_serv2,
	discover_char2,
	notify_char2,
	last_state,
} State_char_t;


Client_Evt completion_evt;

uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];
void char_event_sm(struct gecko_cmd_packet *evt);
static const gecko_configuration_t config ={
  .config_flags = 0,
#if defined(FEATURE_LFXO) || defined(PLFRCO_PRESENT)
  .sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
#else
  .sleep.flags = 0,
#endif
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap),
#if defined(FEATURE_LFXO)
  .bluetooth.sleep_clock_accuracy = 100, // ppm
#elif defined(PLFRCO_PRESENT)
  .bluetooth.sleep_clock_accuracy = 500, // ppm
#endif
  .gattdb = &bg_gattdb_data,
  .ota.flags = 0,
  .ota.device_name_len = 3,
  .ota.device_name_ptr = "OTA",
  .pa.config_enable = 1, // Set this to be a valid PA config
#if defined(FEATURE_PA_INPUT_FROM_VBAT)
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#else
  .pa.input = GECKO_RADIO_PA_INPUT_DCDC,
#endif // defined(FEATURE_PA_INPUT_FROM_VBAT)
  .rf.flags = GECKO_RF_CONFIG_ANTENNA,                 /* Enable antenna configuration. */
  .rf.antenna = GECKO_RF_ANTENNA,                      /* Select antenna path! */
};

bool bool_flag;
bool bool_relay_flag;
bool bool_sensor_flag;		//for sensor reading indication
uint8_t start_sm_flag;
uint8_t cli_connection;

void handle_ble_event(struct gecko_cmd_packet *evt);
float gattuint32tofloat(const uint8_t *value_start_little_endian);
uint8_t findIndexByConnectionHandle(uint8_t connection);			//ref:example code
uint8_t findServiceInAdvertisement(uint8_t *data, uint8_t len);		//ref:example code
#endif /* SRC_BLE_H_ */
