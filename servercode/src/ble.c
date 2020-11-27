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
 *				assignment 6,7,8,9 for reference and soc example
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
uint8_t cli_connection;
uint8_t passkey_connection;
//to store connection opened evt value
uint8_t cli_connection;

//ref:soc client example
uint8_t tableIndex;
uint8_t* charValue;
uint16_t addrValue;
// Health Thermometer service UUID defined by Bluetooth SIG:ref-soc client example code
	const uint8_t thermoService[2] = { 0x09, 0x18 };
	// Temperature Measurement characteristic UUID defined by Bluetooth SIG-ref:soc client example code
	const uint8_t thermoChar[2] = { 0x1c, 0x2a };
	const uint8_t server_add[]=SERVER_BT_ADDRESS;	//use the hard coded value

// Flag for indicating DFU Reset must be performed
uint8_t bootToDfu = 0;
// Array for holding properties of multiple (parallel) connections
ConnProperties connProperties[MAX_CONNECTIONS];
// Counter of active connections
uint8_t activeConnectionsNum;
// State of the connection under establishment
ConnState connState;

uint8_t button_stat=0;
int bonding_success;
int passkey_flag;

/* Function 	: handle_ble_events(struct gecko_cmd_packet *evt)
 * Description  :This state machine is used to handle the BLE events such as follows which are used to boot,setup connection by setting up advertising channels,
 * 				setup connection intervals and then connection closing functions.
 * 	Return		: No return value
*/
#if DEVICE_IS_BLE_SERVER==1
void handle_ble_event(struct gecko_cmd_packet *evt)
{
	switch (BGLIB_MSG_ID(evt->header))
	{

		case gecko_evt_system_boot_id:
				//display on lcd "advertising" till connection established
				displayPrintf(DISPLAY_ROW_NAME,"Server");
				//display on lcd the bt address using gecko_cmd_system_get_bt_address
				bd_addr add=gecko_cmd_system_get_bt_address()->address;
				button_stat=0;
				displayPrintf(DISPLAY_ROW_BTADDR,"%x:%x:%x:%x:%x:%x",add.addr[5],add.addr[4],add.addr[3],add.addr[2],add.addr[1], add.addr[0]);
				bonding_success=0;
				passkey_flag=0;
				//here for cmd_sm_configure we pass MITM flag-bit 1 in 0X01
				//denotes Bonding requires MITM protection and sm_io_capab_displayyesno=dsiplay with yes/no buttons
				BTSTACK_CHECK_RESPONSE(gecko_cmd_sm_configure(0x0F, sm_io_capability_displayyesno));
				//useful to delete all the bondings to trigger and test the pairing processes.
				BTSTACK_CHECK_RESPONSE(gecko_cmd_sm_delete_bondings());
				//the application need to allow bonding.
				//gecko_cmd_sm_configure(flags, io_capabilities) called with flags parameter where bit 1 is set then the bonding enabled automatically.
				//BTSTACK_CHECK_RESPONSE(gecko_cmd_sm_set_bondable_mode(1));
				/*set advertising timing:parameters:1.Handle set for normal ble operation so 0 else for mesh 1
				* 2&3.The first two parameters are minimum and maximum advertising interval, both in units of (milliseconds * 1.6)(250*1.6=400)
				* 4.duration=0 means unlimited time till it disables
				* 5.max_Events=0 is no max number of advertising events.*/
				BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_set_advertise_timing(0, 400, 400, 0, 0));
				/* Start general advertising and enable connections-set scannable and discoverable parameters */
				BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable));
				displayPrintf(DISPLAY_ROW_CONNECTION,"Advertising");
				LOG_INFO("evt_sys_boot\n");
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
		case gecko_evt_sm_confirm_passkey_id:
			     LOG_INFO("Passkey confirm\r\n");
			     passkey_flag=1;

			     passkey_connection = evt->data.evt_sm_confirm_passkey.connection;
			     displayPrintf(DISPLAY_ROW_PASSKEY,"Passkey %lu",evt->data.evt_sm_confirm_passkey.passkey);
			     displayPrintf(DISPLAY_ROW_ACTION,"Confirm with PB0");
			     break;

			      case gecko_evt_sm_confirm_bonding_id:
			      LOG_INFO("Bonding confirm\r\n");
			      BTSTACK_CHECK_RESPONSE(gecko_cmd_sm_bonding_confirm(evt->data.evt_sm_confirm_bonding.connection,1));
			      break;

			     case gecko_evt_sm_bonded_id:
			     LOG_INFO("**************Bond success****************\r\n");
			     bonding_success=1;
			     displayPrintf(DISPLAY_ROW_CONNECTION,"Bonded");
			     displayPrintf(DISPLAY_ROW_PASSKEY," ");
			     displayPrintf(DISPLAY_ROW_ACTION," ");
			     break;

			     case gecko_evt_sm_bonding_failed_id:
			     LOG_INFO("Bonding failed\r\n");
			     displayPrintf(DISPLAY_ROW_PASSKEY,"");
			     displayPrintf(DISPLAY_ROW_ACTION,"");
			              //gecko_cmd_le_connection_close(evt->data.evt_sm_bonding_failed.connection);
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
#endif
//to switch to client make this define 0
#if DEVICE_IS_BLE_SERVER==0
void handle_ble_event(struct gecko_cmd_packet *evt)
{

			const uint8_t relay_uuid[16]={0x42,0x46,0xd6,0x2b,0xae,0xb1,0xb2,0xb9,0x7b,0x4d,0xab,0x29,0x0f,0x20,0xcd,0xb3};
			const uint8_t relay_uuid_service[16]={0x68,0xd9,0xa8,0x28,0x5c,0x19,0xc4,0x96,0x35,0x49,0xcb,0x87,0x77,0x88,0x6c,0xab};

			//const uint8_t sensor_uuid[16]={0x00,0x4a,0xd3,0x19,0xad,0xd6,0x49,0x83,0x71,0x42,0x1b,0x4a,0x99,0x64,0x1f,0xc4};
			//const uint8_t sensor_uuid_service[16]={};
			//initialize variables for the ble client state machine
			//static uint32 sensor_service_handler=0;	//to store the evt handler of service obtained,static to store the changes
		//	static uint32_t sensor_char_handler=0;		//to store the evt handler of characteristic,static to store the changes
			static uint32 relay_service_handler=0;	//to store the evt handler of service obtained,static to store the changes
			static uint32_t relay_char_handler=0;		//to store the evt handler of characteristic,static to store the changes
			//static float temperature;	//to store the float temperature value
			uint8_t *relay_ptr;

			switch (BGLIB_MSG_ID(evt->header))
			{
	/*Example code used-SOC temperature client code*/
				case gecko_evt_system_boot_id:
					LOG_INFO("evt boot id\n");
					displayPrintf(DISPLAY_ROW_NAME,"Client");
					//display on lcd the bt address using gecko_cmd_system_get_bt_address
					bd_addr add=gecko_cmd_system_get_bt_address()->address;
					displayPrintf(DISPLAY_ROW_BTADDR,"%x:%x:%x:%x:%x:%x",add.addr[5],add.addr[4],add.addr[3],add.addr[2],add.addr[1], add.addr[0]);
					// Set passive scanning on 1Mb PHY
					BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_set_discovery_type(le_gap_phy_1m, SCAN_PASSIVE));
					// Set scan interval and scan window
					BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_set_discovery_timing(le_gap_phy_1m, SCAN_INTERVAL, SCAN_WINDOW));
					// Set the default connection parameters for subsequent connections
					BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_set_conn_parameters(CONN_INTERVAL_MIN,CONN_INTERVAL_MAX,CONN_SLAVE_LATENCY,CONN_TIMEOUT));
					//useful to delete all the bondings to trigger and test the pairing processes.
					BTSTACK_CHECK_RESPONSE(gecko_cmd_sm_delete_bondings());
					displayPrintf(DISPLAY_ROW_CONNECTION,"Discovering");
					bonding_success=0;
					passkey_flag=0;
					//BTSTACK_CHECK_RESPONSE(gecko_cmd_sm_set_bondable_mode(1));
					BTSTACK_CHECK_RESPONSE(gecko_cmd_sm_configure(0x0F, sm_io_capability_displayyesno));
					// Start scanning
					BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_start_discovery(le_gap_phy_1m, le_gap_discover_generic));
					connState = scanning;
			    break;

			    //evt generated after confirm passkey cmd passed in external signal after pb0 pressed
				case gecko_evt_sm_confirm_passkey_id:
					LOG_INFO("Passkey confirm passkeyid entered**********\r\n");
			        passkey_flag=1;
					passkey_connection = evt->data.evt_sm_confirm_passkey.connection;
			        displayPrintf(DISPLAY_ROW_PASSKEY,"Passkey %lu",evt->data.evt_sm_confirm_passkey.passkey);
		            displayPrintf(DISPLAY_ROW_ACTION,"Confirm with PB0");
		        break;
	//if bonding can be done then this event checked
				case gecko_evt_sm_confirm_bonding_id:
					LOG_INFO("Bonding confirm************\r\n");
			        BTSTACK_CHECK_RESPONSE(gecko_cmd_sm_bonding_confirm(evt->data.evt_sm_confirm_bonding.connection,1));
		         break;
	//if successful
			    case gecko_evt_sm_bonded_id:
		            LOG_INFO("**************Bond success****************\r\n");
		            bonding_success=1;
					displayPrintf(DISPLAY_ROW_CONNECTION,"Bonded");
					displayPrintf(DISPLAY_ROW_PASSKEY," ");
					displayPrintf(DISPLAY_ROW_ACTION," ");
		        break;
	//if failed
				case gecko_evt_sm_bonding_failed_id:
		            LOG_INFO("Bonding failed*/////////////\r\n");
				    displayPrintf(DISPLAY_ROW_PASSKEY,"");
				    displayPrintf(DISPLAY_ROW_ACTION,"");
				 gecko_cmd_le_connection_close(evt->data.evt_sm_bonding_failed.connection);
		        break;


				/*Example code used-SOC temperature client code*/
				case gecko_evt_le_gap_scan_response_id:
					LOG_INFO("in scan response id**********************\n");
					// Parse advertisement packets
					if (evt->data.evt_le_gap_scan_response.packet_type == 0)
					{
				    // If a thermometer advertisement is found...
				    if (findServiceInAdvertisement(&(evt->data.evt_le_gap_scan_response.data.data[0]),evt->data.evt_le_gap_scan_response.data.len) != 0)
				    {
					// then stop scanning for a while
					     gecko_cmd_le_gap_end_procedure();
					     // and connect to that device
					            if (activeConnectionsNum < MAX_CONNECTIONS)
					            {
					            	BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_connect(evt->data.evt_le_gap_scan_response.address,evt->data.evt_le_gap_scan_response.address_type,le_gap_phy_1m));
					              connState = opening;
					            }
					}
					}
				break;

				//ref:server code
				case gecko_evt_le_connection_opened_id:
					LOG_INFO("*************CONNECTION OPENED id\n");
					displayPrintf(DISPLAY_ROW_CONNECTION,"Connected");
					displayPrintf(DISPLAY_ROW_BTADDR2,"%x:%x:%x:%x:%x:%x", server_add[5], server_add[4],server_add[3], server_add[2], server_add[1],server_add[0]);
					/*get packet structure connection value from evt->data.evt_le_connection
							pass the connection variable,minimum interval=max interval=75ms(given)/1.25=60,slave latency=300(given),
							supervision timeout=(1+latency)(max_interval)*2=(1+3)(75*2)=600*/
					cli_connection = evt->data.evt_le_connection_opened.connection;

					BTSTACK_CHECK_RESPONSE(gecko_cmd_le_connection_set_parameters(cli_connection,60,60,3,600));
					//passing the opened connection paramter with length and data of thermometer service uuid
					//BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_discover_primary_services_by_uuid(cli_connection, 2,(const uint8_t*)thermoService));
					BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_discover_primary_services(evt->data.evt_le_connection_opened.connection));
					//BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_discover_primary_services_by_uuid(evt->data.evt_le_connection_opened.connection, 16,sensor_uuid_service));
					connState = discoverServices;
				 break;

				 //if service then get the service handler
				case gecko_evt_gatt_service_id:
					LOG_INFO("*************service id\n");
						completion_evt = relay_service;
						LOG_INFO("get service handler for pb\n");
						relay_service_handler = evt->data.evt_gatt_service.service;

				break;

				//check characteristic and get it in character handler
				case gecko_evt_gatt_characteristic_id:
					LOG_INFO("*************characteristic id\n");
					completion_evt=relay_char;
					LOG_INFO("get char handler for pb\n");
					relay_char_handler=evt->data.evt_gatt_characteristic.characteristic;
				break;

				//ref:example code,if service set,call discover_char_by_uuid else if characterisitc then call characteristic notification
				case gecko_evt_gatt_procedure_completed_id:
					LOG_INFO("in procedure complete event:evt->data.evt_gatt_procedure_completed.result=%x\n",evt->data.evt_gatt_procedure_completed.result);
			    	  /*if(completion_evt == service)
			    	  {
						      BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_discover_characteristics_by_uuid(evt->data.evt_gatt_procedure_completed.connection,service_handler,2,(const uint8_t*)thermoChar));
						  	  completion_evt = clearevent;
						  	  connState = discoverCharacteristics;
			          }
			    	  if(completion_evt == characteristic)
			    	  {
			    		  	  LOG_INFO("in charactersitic completion event\n");
			    		  	  BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_set_characteristic_notification(evt->data.evt_gatt_procedure_completed.connection,char_handler,gatt_indication));
			    		  	  completion_evt = clearevent;
			    		  	  connState = enableIndication;
			    	  }*/
			    	  if(completion_evt == relay_char)
			    	  {
			    		  if(evt->data.evt_gatt_procedure_completed.result == 0)
			    		 {
			    			  BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_set_characteristic_notification(evt->data.evt_gatt_procedure_completed.connection,relay_char_handler,gatt_indication));
			    		 		completion_evt = clearevent;
			    	  }
			    	  }
			    	  if(completion_evt == relay_service)
			    	  {
			    		     BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_discover_characteristics_by_uuid(cli_connection, relay_service_handler,16,relay_uuid));
			    		  	completion_evt = clearevent;
			    	  }
			    	  //if encryption error occured then start the increase security bonding procedure
			    	  if(evt->data.evt_gatt_procedure_completed.result==0x040F)
			    	  {
			    		  LOG_INFO("increase secrity**********\n");
			    		  BTSTACK_CHECK_RESPONSE(gecko_cmd_sm_increase_security(cli_connection));
			    	  }
			    break;

			    //this evt then is used to calculate the temperature and converting uint32 to float using gattuint32tofloat function
			    //get rssi as well
				case gecko_evt_gatt_characteristic_value_id:
					LOG_INFO("IN CHAR VALUE ID*******************\n");
					LOG_INFO("pushed is %d",pushed);
					//when pb1 is pressed then only this event will pass the if part and give the status of the pushbutton
					  if(evt->data.evt_gatt_characteristic_value.characteristic == relay_char_handler)
					  {
							LOG_INFO("in char value id **********\n");
						    BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_send_characteristic_confirmation(evt->data.evt_gatt_procedure_completed.connection));
						    //displayPrintf(DISPLAY_ROW_CONNECTION,"Handling Indications");
						    //temperature = gattuint32tofloat(evt->data.evt_gatt_characteristic_value.value.data);
						    //displayPrintf(DISPLAY_ROW_TEMPVALUE,"Temperature:%.2f",temperature);
						    relay_ptr = evt->data.evt_gatt_characteristic_value.value.data;
						    LOG_INFO("*******Relay State is %u*******", *relay_ptr);
						 	if(*relay_ptr == 1)
						 	{
						 		displayPrintf(DISPLAY_ROW_ACTION,"RELAY ON");
						 	}
						 	else
						 	{
						 		displayPrintf(DISPLAY_ROW_ACTION,"RELAY OFF");
						    }
						    BTSTACK_CHECK_RESPONSE(gecko_cmd_le_connection_get_rssi(evt->data.evt_gatt_procedure_completed.connection));
					  }
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
				//checks the 3 sec irq-server code,checks the pb button presses and depending on pressed during passkey completion
				case gecko_evt_system_external_signal_id:
					LOG_INFO("external_system_signal_id reached\n");

					if(evt->data.evt_system_external_signal.extsignals==PB0_pressed && passkey_flag==1)
					{
						BTSTACK_CHECK_RESPONSE(gecko_cmd_sm_passkey_confirm(passkey_connection,1));
						passkey_flag=0;
					}
				break;
	//for extcom signal
				case gecko_evt_hardware_soft_timer_id:
					LOG_INFO("Software timer set\n");
					displayUpdate();
				break;

				//closing connection:start discovery again
				case gecko_evt_le_connection_closed_id:
					LOG_INFO("CONNECTION CLOSED ID\n");
					connState = scanning;
					bonding_success=0;
					LOG_INFO("delete bondings\n");
					//useful to delete all the bondings to trigger and test the pairing processes.
					BTSTACK_CHECK_RESPONSE(gecko_cmd_sm_delete_bondings());
					BTSTACK_CHECK_RESPONSE(gecko_cmd_le_gap_start_discovery(le_gap_phy_1m, le_gap_discover_generic));
					//after disconnection,show discovering again i.e check for new connections
					displayPrintf(DISPLAY_ROW_CONNECTION,"Discovering");
					displayPrintf(DISPLAY_ROW_ACTION,"");
					displayPrintf(DISPLAY_ROW_TEMPVALUE,"");
					connState = scanning;
				break;
				//ref:Example code
				// Check if the user-type OTA Control Characteristic was written.
				// If ota_control was written, boot the device into Device Firmware Upgrade (DFU) mode.
				case gecko_evt_gatt_server_user_write_request_id:
					if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control) {
						 // Set flag to enter to OTA mode
					     bootToDfu = 1;
						 // Send response to Write Request
						 gecko_cmd_gatt_server_send_user_write_response(
						 evt->data.evt_gatt_server_user_write_request.connection,gattdb_ota_control,bg_err_success);
						 // Close connection to enter to DFU OTA mode
						 BTSTACK_CHECK_RESPONSE(gecko_cmd_le_connection_close(evt->data.evt_gatt_server_user_write_request.connection));
				}
			    break;
			}
}
#endif

// Init connection properties
void initProperties(void)
{
  uint8_t i;
  activeConnectionsNum = 0;

  for (i = 0; i < MAX_CONNECTIONS; i++) {
    connProperties[i].connectionHandle = CONNECTION_HANDLE_INVALID;
    connProperties[i].thermometerServiceHandle = SERVICE_HANDLE_INVALID;
    connProperties[i].thermometerCharacteristicHandle = CHARACTERISTIC_HANDLE_INVALID;
    connProperties[i].temperature = TEMP_INVALID;
    connProperties[i].rssi = RSSI_INVALID;
  }
}

// Parse advertisements looking for advertised Health Thermometer service
uint8_t findServiceInAdvertisement(uint8_t *data, uint8_t len)
{
  uint8_t adFieldLength;
  uint8_t adFieldType;
  uint8_t i = 0;
  // Parse advertisement packet
  while (i < len) {
    adFieldLength = data[i];
    adFieldType = data[i + 1];
    // Partial ($02) or complete ($03) list of 16-bit UUIDs
    if (adFieldType == 0x02 || adFieldType == 0x03) {
      // compare UUID to Health Thermometer service UUID
      if (memcmp(&data[i + 2], thermoService, 2) == 0) {
        return 1;
      }
    }
    // advance to the next AD struct
    i = i + adFieldLength + 1;
  }
  return 0;
}

// Find the index of a given connection in the connection_properties array
uint8_t findIndexByConnectionHandle(uint8_t connection)
{
  for (uint8_t i = 0; i < activeConnectionsNum; i++) {
    if (connProperties[i].connectionHandle == connection) {
      return i;
    }
  }
  return TABLE_INDEX_INVALID;
}

// Add a new connection to the connection_properties array
void addConnection(uint8_t connection, uint16_t address)
{
  connProperties[activeConnectionsNum].connectionHandle = connection;
  connProperties[activeConnectionsNum].serverAddress    = address;
  activeConnectionsNum++;
}

// Remove a connection from the connection_properties array
void removeConnection(uint8_t connection)
{
  uint8_t i;
  uint8_t table_index = findIndexByConnectionHandle(connection);

  if (activeConnectionsNum > 0) {
    activeConnectionsNum--;
  }
  // Shift entries after the removed connection toward 0 index
  for (i = table_index; i < activeConnectionsNum; i++) {
    connProperties[i] = connProperties[i + 1];
  }
  // Clear the slots we've just removed so no junk values appear
  for (i = activeConnectionsNum; i < MAX_CONNECTIONS; i++) {
    connProperties[i].connectionHandle = CONNECTION_HANDLE_INVALID;
    connProperties[i].thermometerServiceHandle = SERVICE_HANDLE_INVALID;
    connProperties[i].thermometerCharacteristicHandle = CHARACTERISTIC_HANDLE_INVALID;
    connProperties[i].temperature = TEMP_INVALID;
    connProperties[i].rssi = RSSI_INVALID;
  }
}

