/**************************************************************************************************
 * @file      scheduler.c
 * @version   0.0.1
 * @brief     scheduler.c Function Implementations
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
 * @resources  https://www.silabs.com/documents/public/reference-manuals/bluetooth-api-reference.pdf
 *				silicon labs example-soc thermometer
 *				https://docs.silabs.com/resources/bluetooth/code-examples/applications/thermometer-example-with-efr32-internal-temperature-sensor/source/app.c
 * 			   lecture 10-IoT
 *
 * @copyright  All rights reserved. Distribution allowed only for the use of assignment grading.
 *       Use of code excerpts allowed at the discretion of author. Contact for permission.
 */

/**Scheduler file helps control the sequence of events.
 * */

#include "scheduler.h"
#include "sleep.h"
#include <stdbool.h>
#include "native_gecko.h"
#include "gecko_ble_errors.h"
#include "display.h"
float temp;


/* Function 	: schedulerSetEvent()
 * Description  : This event is used to increment the event_val global variable which indicates
 * 				  interrupt occuring after 3 sec.
 * */
void schedulerSetEvent(uint32_t var) {
 // set the event, this is a read-modify-write
	//LOG_INFO("In scheduler\n");
	// DOS this needs to be a read-modify-write called from ISR within CORE_ENTER_CRITICAL/CORE_EXIT_CRITICAL section
	//event_flag |= var; // or in the bits that are set
	gecko_external_signal(var);
}


/* Function 	: events_present()
 * Description  : This function is being checked in main function for event presence.
 * 				  After the schedulersetEvent entered from Interrupt Service Routine sets the global variable
 * 				  this function will set the flag accordingly and return.
 */
bool events_present() {
	if(event_flag>0){
		return 1;
	}
	else
		return 0;
}


/* Function 	: process_event
 * Description  : When event occurs the process_event will then be used to
 * 				  get temperature readings from the sensor.
 * */
void process_event(){
		   uint8_t write_buf[1]={0xF3};
					   		uint8_t read_buf[2]={0,0};
					   		float temp = 0.0;

					   //reset gloabl variable i.e flag to scheduler model
					   		//event_val=0;

					   		Sensor_enable();
					   		timerWaitus(80);
					   		while(timer_flag!=1);
					   		i2c_write(write_buf,1);
					   		timerWaitus(10);
					   		while(timer_flag!=1);
					   		temp=i2c_read(read_buf,2);
					   		LOG_INFO("Temp is %f",temp);
					   		Sensor_disable();		//disables all the gpio pins
	}

/* Function 	: get_event
 * Description  : It will store and clear the set bit and compare the bit with the flags to pass to event
 * */
uint32_t get_event(){
			uint32_t ret=0;
			CORE_DECLARE_IRQ_STATE;
			CORE_ENTER_CRITICAL();

			// you to select 1 event to return and clear that bit
			if (event_flag & timer_UF_flag) {

			    ret        =              timer_UF_flag;
			    event_flag = event_flag ^ timer_UF_flag; // clear the bit

			} else if (event_flag & timer_comp1_flag) {

				ret        =              timer_comp1_flag;
				event_flag = event_flag ^ timer_comp1_flag; // clear the bit

			} else if (event_flag & write_transfer_done) {

				ret        =              write_transfer_done;
				event_flag = event_flag ^ write_transfer_done; // clear the bit

			} else if (event_flag & read_transfer_done) {

				ret        =              read_transfer_done;
				event_flag = event_flag ^ read_transfer_done; // clear the bit

			} else {
				ret = null;
			}

			CORE_EXIT_CRITICAL();

	return ret;
}
/* Function 	: temp_read_ble(float temp)
 * Description  :This function takes the result temp value calculated from the sensor and then converts
 * 	Return		: No return value
 * 	Ref:silicon labs soc temperature example
*/
void temp_read_ble(float temp){
		uint8_t htmTempBuffer[5]; /* Stores the temperature data in the Health Thermometer (HTM) format. */
		uint32_t temperature;   /* Stores the temperature data read from the sensor in the correct format */
		uint8_t *p = htmTempBuffer; /* Pointer to HTM temperature buffer needed for converting values to bitstream. */
		//uint8_t flags = 0x00;   /* HTM flags set as 0 for Celsius, no time stamp and no temperature type. */

		/* Convert flags to bitstream and append them in the HTM temperature data buffer (htmTempBuffer) */
		UINT8_TO_BITSTREAM(p,0);
		//LOG_INFO("uint8 to bitstream done\n");
		/* Convert sensor data to correct temperature format */
		temperature = FLT_TO_UINT32(temp*1000, -3);
		//displayPrintf(DISPLAY_ROW_TEMPVALUE,temperature);
		/* Convert temperature to bitstream and place it in the HTM temperature data buffer (htmTempBuffer) */
		UINT32_TO_BITSTREAM(p, temperature);
		/* Send indication of the temperature in htmTempBuffer to all "listening" clients.
		* This enables the Health Thermometer in the Blue Gecko app to display the temperature.
		*  0xFF as connection ID will send indications to all connections. */
		//if the indicate is set,then it will send the notification and bitstream.
		displayPrintf(DISPLAY_ROW_TEMPVALUE,"Temp:%f",temp);
		if(bool_flag==1)
		{
				//BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_server_send_characteristic_notification(0xFF, gattdb_temperature_measurement, 5, htmTempBuffer));
		}
}

//change accordingly
void lux_read_ble(float temp){
		uint8_t htmTempBuffer[5]; /* Stores the temperature data in the Health Thermometer (HTM) format. */
		uint32_t temperature;   /* Stores the temperature data read from the sensor in the correct format */
		uint8_t *p = htmTempBuffer; /* Pointer to HTM temperature buffer needed for converting values to bitstream. */
		//uint8_t flags = 0x00;   /* HTM flags set as 0 for Celsius, no time stamp and no temperature type. */

		//typecast to int
		uint8_t lux_int=(uint8_t)temp;
		/* Convert flags to bitstream and append them in the HTM temperature data buffer (htmTempBuffer) */
		UINT8_TO_BITSTREAM(p,0);
		//LOG_INFO("uint8 to bitstream done\n");
		/* Convert sensor data to correct temperature format */
		//temperature = FLT_TO_UINT32(temp*1000, -3);
		//displayPrintf(DISPLAY_ROW_TEMPVALUE,temperature);
		/* Convert temperature to bitstream and place it in the HTM temperature data buffer (htmTempBuffer) */
		UINT32_TO_BITSTREAM(p, temperature);
		/* Send indication of the temperature in htmTempBuffer to all "listening" clients.
		* This enables the Health Thermometer in the Blue Gecko app to display the temperature.
		*  0xFF as connection ID will send indications to all connections. */
		//if the indicate is set,then it will send the notification and bitstream.
		displayPrintf(DISPLAY_ROW_TEMPVALUE,"Temp:%f",temp);
		if(bool_sensor_flag==1)
		{
				BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_server_send_characteristic_notification(0xFF, gattdb_sensor_reading, 5, htmTempBuffer));
		}
}
/* Function 	:state_machine(struct gecko_cmd_packet *evt)
 * Description  :This state machine is used to calculate the temperature wherein:
 * 				state 1:stateidle checks for the UF flag to set after every 3 sec,if set it initiates i2c,
 * 				sensor is enabled and waits for power up 80ms
 * 				state2:once the comp1 flag is set because of 80 ms timer up,it will intiate write seq
 * 				state3:once the write seq is completed wait for 10.8ms
 * 				state4:after comp1 flag set after 10.8 msec it will read the data
 * 				state 5:after read transfer done,it will calculate temp(send thrgh ble)and then disable sensor and irq
 * 	Return		: No return value
 * 	Ref:silicon labs soc temperature example
*/
void state_machine(struct gecko_cmd_packet *evt) {
	temp_states          current_state;
	//DOS static temp_states   next_state=wait_for_power_up;
	static temp_states   next_state=stateIdle; // DOS
	uint16_t             ret;
	if (BGLIB_MSG_ID(evt->header) != gecko_evt_system_external_signal_id) {
			return;
		}
	current_state = next_state;
	LOG_INFO("state is %d",current_state);
	if(start_sm_flag==1)
	{
	switch (current_state) {

	                    case stateIdle:
		                next_state=stateIdle;
		                //LOG_INFO("ext signal=%ld\n",evt->data.evt_system_external_signal.extsignals);
						if(evt->data.evt_system_external_signal.extsignals & timer_UF_flag)
						{
						 //evt=0;
						LOG_INFO("state 0:idle state initiates sensor and power up\n");
						  // start a measurement sequence: init I2C, power up 7021, wait 80ms
						  I2C_init();
					      Sensor_enable();
					      timerWaitus(80000);
						  next_state = wait_for_power_up;

						}
		               break;

				   	    case wait_for_power_up:
				   	    	next_state=wait_for_power_up;
				   	        if(evt->data.evt_system_external_signal.extsignals & timer_comp1_flag) // times up
				   		  	  {
				   			//evt=0;
#ifdef DEBUG
				   			LOG_INFO("1.IN wait_for_power_up state \n\r");
#endif
				   			LOG_INFO("1");
				   			// initiate an I2C transfer to send F3 command to 7021
				   			// block sleep to EM2
				   			SLEEP_SleepBlockBegin(sleepEM2);
				   			ret=i2c_write_interrupt();
				   			next_state = wait_for_i2c_write_complete;
				   		  	  }

				   		break;


				   	    case wait_for_i2c_write_complete:
				   	    	next_state=wait_for_i2c_write_complete;
				   		  //DOS if(evt==timer_comp1_flag)
				   	     //LOG_INFO("ext signal=%ld\n",evt->data.evt_system_external_signal.extsignals);
				   	      if(evt->data.evt_system_external_signal.extsignals & write_transfer_done)
				   		  {
				   			  //evt=0;
#ifdef DEBUG
				   			LOG_INFO("2.IN wait_for_i2c_write_complete state \n\r");
#endif
				   			  	  //timer_flag=0;

				   			LOG_INFO("2");
				   			  SLEEP_SleepBlockEnd(sleepEM2); // end sleep block
				   			  // now wait 10.8 ms for 7021 to take the measurement
				   			 timerWaitus(10800);
				   			 next_state=wait_for_i2c_read_start;
				   		  	  }
				   		break;



				   	    case wait_for_i2c_read_start:
				   	    	next_state=wait_for_i2c_read_start;
				   	     //LOG_INFO("ext signal=%ld\n",evt->data.evt_system_external_signal.extsignals);
				   		if(evt->data.evt_system_external_signal.extsignals & timer_comp1_flag) // times up
				   			{
				   			//evt=0;
#ifdef DEBUG
				   				LOG_INFO("3.IN wait_for_i2c_read_start \n\r");
#endif

				   				LOG_INFO("3");
				   				//NVIC_DisableIRQ(I2C0_IRQn);
								//SLEEP_SleepBlockEnd(sleepEM2);
								//timerWaitus(10000);

				   				// initiate an I2C transfer to read the temp measurement from the 7021
				   				// block sleep to EM2
				   				SLEEP_SleepBlockBegin(sleepEM2);
				   				ret=i2c_read_interrupt();
				   				next_state=wait_for_i2c_read_complete;
				   		    }
				   		break;



				   	    case wait_for_i2c_read_complete:
				   	    	next_state=wait_for_i2c_read_complete;
				   	     //LOG_INFO("ext signal=%ld\n",evt->data.evt_system_external_signal.extsignals);
				   		if(evt->data.evt_system_external_signal.extsignals & read_transfer_done)
				   			{
				   			//evt=0;
#ifdef DEBUG
				   			LOG_INFO("4.In wait for i2c read complete state \n\r");
#endif
				   			LOG_INFO("4");
				   			SLEEP_SleepBlockEnd(sleepEM2);
				   			temp=temp_calc();
							LOG_INFO("Temp=%f",temp);

							temp_read_ble(temp);
							//Sensor_disable();
							NVIC_DisableIRQ(I2C0_IRQn);
							next_state=stateIdle;

				   			}
				   		break;
	     } // switch
	}//extra credit
}



