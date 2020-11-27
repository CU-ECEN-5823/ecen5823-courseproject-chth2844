/**************************************************************************************************
 * @file      ble.c
 * @version   0.0.1
 * @brief     ble.c Function Implementations
 *
 * @author    Rucha Borwankar
 * @date     Oct 2, 2020
 *
 * @institution University of Colorado Boulder (UCB)
 * @course      ECEN 5823-001: IoT Embedded Firmware (Fall 2020)
 * @instructor  David Sluiter
 *
 * @assignment server code for final project
 * @due        Dec 3, 2020
 *
 * @resources  https://www.silabs.com/documents/public/reference-manuals/bluetooth-api-reference.pdf
 *				silicon labs example-soc thermometer
 *				https://docs.silabs.com/resources/bluetooth/code-examples/applications/thermometer-example-with-efr32-internal-temperature-sensor/source/app.c
 * 			   lecture 10-IoT
 *
 *
 * @copyright  All rights reserved. Distribution allowed only for the use of assignment grading.
 *       Use of code excerpts allowed at the discretion of author. Contact for permission.
 */

#include "ble.h"
#include "gecko_ble_errors.h"
#include <stdbool.h>
#include <stdbool.h>
uint8 connection;
int8_t rssi;
uint16 timeout;
bool toggle=1;
bool conn_done=0;

/* Function 	: handle_ble_events(struct gecko_cmd_packet *evt)
 * Description  :This state machine is used to handle the BLE events such as follows which are used to boot,setup connection by setting up advertising channels,
 * 				setup connection intervals and then connection closing functions.
 * 	Return		: No return value
*/

void handle_ble_event(struct gecko_cmd_packet *evt)
{
	switch (BGLIB_MSG_ID(evt->header))
	{
		case gecko_evt_system_boot_id:
		//display on lcd "advertising" till connection established
		displayPrintf(DISPLAY_ROW_NAME,"Server");
		//display on lcd the bt address using gecko_cmd_system_get_bt_address
		bd_addr add=gecko_cmd_system_get_bt_address()->address;
		displayPrintf(DISPLAY_ROW_BTADDR,"%x:%x:%x:%x:%x:%x",add.addr[5],add.addr[4],add.addr[3],add.addr[2],add.addr[1], add.addr[0]);
	   /*set advertising timing:parameters:1.Handle set for normal ble operation so 0 else for mesh 1
		* 2&3.The first two parameters are minimum and maximum advertising interval, both in units of (milliseconds * 1.6)(250*1.6=400)
		* 4.duration=0 means unlimited time till it disables
		* 5.max_Events=0 is no max number of advertising events.*/
		BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_set_advertise_timing(0, 400, 400, 0, 0));
		/* Start general advertising and enable connections-set scannable and discoverable parameters */
		BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable));
		displayPrintf(DISPLAY_ROW_CONNECTION,"Advertising");
		LOG_INFO("evt_sys_boot\n");
		start_sm_flag=0;
		break;

		case gecko_evt_le_connection_opened_id:
		/*get packet structure connection value from evt->data.evt_le_connection
		pass the connection variable,minimum interval=max interval=75ms(given)/1.25=60,slave latency=300(given),
		supervision timeout=(1+latency)(max_interval)*2=(1+3)(75*2)=600*/
		connection = evt->data.evt_le_connection_opened.connection;
		BTSTACK_CHECK_RESPONSE(gecko_cmd_le_connection_set_parameters(connection,60,60,3,600));
		//extra credit-flag set to start state machine only after connection done.
		//start_sm_flag=1;
		conn_done=1;
		start_sm_flag=1; //cst
		LOG_INFO("evt_le connection\n");
		//display on lcd "connected" when connection established with connect app
		displayPrintf(DISPLAY_ROW_CONNECTION,"connected");
		break;

		case gecko_evt_le_connection_parameters_id:
			/*to check paramters passed in connection_parameters in opened id state*/
			timeout=evt->data.evt_le_connection_parameters.timeout;
			LOG_INFO("gecko_evt_le_connection_parameters_id:timeout=%d\n",timeout);
			break;

		case gecko_evt_gatt_server_characteristic_status_id:
			/*When Indicate arrow button in the phone app turns blue one is sending an AP Write to the characteristic CCCD value in the server to enable the server
			 * to send indications for that characteristic. Tap the arrow again and you send a write to disable indications for that characteristic.
			 *  Then in the state machine when we need to compute the temperature from the values read from the 7021,
			 *  only send an indication for the temp characteristic if bool_flag is set.Thus sends the data through bitstream*/
			/*if ((evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_temperature_measurement)
			            && (evt->data.evt_gatt_server_characteristic_status.status_flags == 0x01))
			{
			          if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == 0x02)
			          {/*indication on
			            bool_flag=1;
			          } else if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == 0x00) {
			            /* Indications have been turned OFF
			            bool_flag=0;
			          }
			}*/
			//for relay service
			if ((evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_relay_state)
									            && (evt->data.evt_gatt_server_characteristic_status.status_flags == gatt_server_client_config))
			{
			          if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == gatt_indication)
			          {/*indication on*/
			        	  bool_relay_flag=1;
			          } else if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == gatt_disable) {
			            /* Indications have been turned OFF */
			       	   bool_relay_flag=0;
			          }
			}
			//for lux sensor service
						if ((evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_sensor_reading)
												            && (evt->data.evt_gatt_server_characteristic_status.status_flags == gatt_server_client_config))
						{
						          if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == gatt_indication)
						          {/*indication on*/
						        	  bool_sensor_flag=1;
						          } else if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == gatt_disable) {
						            /* Indications have been turned OFF */
						       	   bool_sensor_flag=0;
						          }
						}
		/*set global max tx power*/
		BTSTACK_CHECK_RESPONSE(gecko_cmd_le_connection_get_rssi(connection));
		LOG_INFO("evt_characteristics\n");
		break;

		case gecko_evt_le_connection_rssi_id:
			LOG_INFO("evt_rssi_id\n");
		gecko_cmd_system_halt(1);
		rssi=evt->data.evt_le_connection_rssi.rssi;
		if(rssi > -35)
	    {
		      gecko_cmd_system_set_tx_power(-260);
		}
		else if((rssi <= -35) && (rssi >= -45))
		{
		      gecko_cmd_system_set_tx_power(-200);
		}
		else if((rssi <= -45) && (rssi >= -55))
		{
		      gecko_cmd_system_set_tx_power(-150);
		}
		else if((rssi <= -55) && (rssi >= -65))
		{
		      gecko_cmd_system_set_tx_power(-50);
		}
		else if((rssi <= -65) && (rssi >= -75))
		{
		      gecko_cmd_system_set_tx_power(0);
		}
		else if((rssi <= -75) && (rssi >= -85))
		{
		      gecko_cmd_system_set_tx_power(50);
		}
		else if((rssi <= -85))
		{
		      gecko_cmd_system_set_tx_power(80);
		}
		else
		{
			  gecko_cmd_system_set_tx_power(0);
		}
		gecko_cmd_system_halt(0);
		break;

		case gecko_evt_system_external_signal_id:


		break;

		case gecko_evt_hardware_soft_timer_id:
					LOG_INFO("Software timer set\n");
					displayUpdate();
		break;

		case gecko_evt_le_connection_closed_id:
			LOG_INFO("evt_connection closed\n");
			//after disconnecting,it will show on lcd "advertising"
			displayPrintf(DISPLAY_ROW_CONNECTION,"Advertising");
			displayPrintf(DISPLAY_ROW_ACTION," ");
			displayPrintf(DISPLAY_ROW_TEMPVALUE," ");
			/*halt the system*/
			gecko_cmd_system_halt(1);
			/*set transmit power to 0*/
			gecko_cmd_system_set_tx_power(0);
			/* Start general advertising and enable connections. */
			BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable));
			gecko_cmd_system_halt(0);
			start_sm_flag=0;	/*once connection disconnects it will make state machine inaccesible*/
		break;
		} // switch

}
