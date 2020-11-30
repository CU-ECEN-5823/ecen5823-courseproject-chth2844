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
#include "gpio.h"
//float temp;
Lux_Event_t         lux_event;
State_t           currentState;
State_t    nextState = STATE0_TIMER_WAIT;
double lux;
uint32_t lux2;
double channel1;
double channel0;
uint8_t sensor_enable=0;


/* Function 	: schedulerSetEvent()
 * Description  : This event is used to increment the event_val global variable which indicates
 * 				  interrupt occuring after 3 sec.
 * */
void schedulerSetEvent(uint32_t var) {

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
//void process_event(){
//		   uint8_t write_buf[1]={0xF3};
//					   		uint8_t read_buf[2]={0,0};
//					   		float temp = 0.0;
//
//					   //reset gloabl variable i.e flag to scheduler model
//					   		//event_val=0;
//
//					   		Sensor_enable();
//					   		timerWaitus(80);
//					   		while(timer_flag!=1);
//					   		i2c_write(write_buf,1);
//					   		timerWaitus(10);
//					   		while(timer_flag!=1);
//					   		temp=i2c_read(read_buf,2);
//					   		LOG_INFO("Temp is %f",temp);
//					   		Sensor_disable();		//disables all the gpio pins
//	}

/* Function 	: get_event
 * Description  : It will store and clear the set bit and compare the bit with the flags to pass to event
 * */
uint32_t get_event(){
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_CRITICAL();

	if (lux_event & LUX_POWER_OFF) {
		lux_event = lux_event ^ LUX_POWER_OFF; // clear event, needs to be read-modify-write
		CORE_EXIT_CRITICAL();
		return (LUX_POWER_OFF);
	} else

		if (lux_event & LUX_WAIT_FOR_POWER_UP) {
			lux_event = lux_event ^ LUX_WAIT_FOR_POWER_UP; // clear event, needs to be read-modify-write
			CORE_EXIT_CRITICAL();
			return (LUX_WAIT_FOR_POWER_UP);
		} else

			if (lux_event & LUX_LETIMER0_READ) {
				lux_event = lux_event ^ LUX_LETIMER0_READ; // clear event, needs to be read-modify-write
				CORE_EXIT_CRITICAL();
				return (LUX_LETIMER0_READ);
			} else

				if (lux_event & LUX_LETIMER0_SENSOR_ENABLE) {
					lux_event = lux_event ^ LUX_LETIMER0_SENSOR_ENABLE; // clear event, needs to be read-modify-write
					CORE_EXIT_CRITICAL();
					return (LUX_LETIMER0_SENSOR_ENABLE);
				} else

					if (lux_event & LUX_I2C_TRANSFER_DONE) {
						lux_event = lux_event ^ LUX_I2C_TRANSFER_DONE; // clear event, needs to be read-modify-write
						CORE_EXIT_CRITICAL();
						return (LUX_I2C_TRANSFER_DONE);
					} else

						CORE_EXIT_CRITICAL();
	return (0);


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

void switch_relay_state(uint32_t lum)
{
	uint32_t op_state,op_state2;
	uint8_t buffer_op[1];
	uint8_t buffer_op2[1];
	uint8_t *ptr_op=buffer_op;
	uint8_t *ptr_op2=buffer_op2;
	//luminosity is less than threshold value switch on relay
	if(lum<LUX_THRESHOLD)
	{

		GPIO_PinOutSet(RELAY_port,RELAY_pin);
	    displayPrintf(DISPLAY_ROW_ACTION,"RELAY ON");
	    uint8_t op=GPIO_PinInGet(RELAY_port,RELAY_pin);
		LOG_INFO("relay on status with val %d************\n\r",op);
		op_state=FLT_TO_UINT32(op,-3);
		UINT32_TO_BITSTREAM(ptr_op,op_state);
	    BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_server_write_attribute_value(gattdb_relay_state,0,1,buffer_op));
	    if (bool_relay_flag==1){
	    	BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_server_send_characteristic_notification(0xFF,gattdb_relay_state,1,buffer_op));
	     }
	}


	//luminosity is greater than threshold value switch off relay
	else{

		GPIO_PinOutClear(RELAY_port,RELAY_pin);
		displayPrintf(DISPLAY_ROW_ACTION,"RELAY OFF");
		uint8_t op2=GPIO_PinInGet(RELAY_port,RELAY_pin);
		LOG_INFO("relay off status with val %d************\n\r",op2);
		BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_server_write_attribute_value(gattdb_relay_state,0,1,buffer_op2));
	    if (bool_relay_flag==1){
		   BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_server_send_characteristic_notification(0xFF,gattdb_relay_state,1,buffer_op2));
		}


	}
}

void lux_read_ble(uint32_t lux)
{


	uint8_t lux_buffer[1];
	//uint8_t flags = 0x00;
	//uint32_t lux_val;
    uint8_t *ptr =  lux_buffer;
  //  UINT8_TO_BITSTREAM(p,0);


//    lux_val=FLT_TO_UINT32(lux,0);

	uint32_t lux_val;
	 lux_val=FLT_TO_UINT32(lux,-3);
    /* Convert temperature to bitstream and place it in the HTM temperature data buffer (htmTempBuffer) */
    UINT32_TO_BITSTREAM(ptr, lux_val);

    BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_server_send_characteristic_notification(0xFF,gattdb_sensor_reading, 1, lux_buffer));


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

	uint16_t             ret;
	if (BGLIB_MSG_ID(evt->header) != gecko_evt_system_external_signal_id) {
			return;
		}

	currentState = nextState;
	//LOG_INFO("state is %d",current_state);

	if(start_sm_flag==1)
	{
		switch(currentState)
		{
		case STATE0_TIMER_WAIT:
			nextState = STATE0_TIMER_WAIT; // default
			if ((evt->data.evt_system_external_signal.extsignals & LUX_WAIT_FOR_POWER_UP)!=0 )
			{
#ifdef DEBUG
				LOG_INFO("0");
#endif

				gpioSensorSetOn();
				timerWaitus(50000);
				nextState = STATE_1_ENABLE_SENSOR;


			}

			break;

		case STATE_1_ENABLE_SENSOR:
			nextState = STATE_1_ENABLE_SENSOR;


			if ((evt->data.evt_system_external_signal.extsignals & LUX_LETIMER0_SENSOR_ENABLE)!=0 )
			{
#ifdef DEBUG
				LOG_INFO("1");

#endif
				InitI2C();      //Initialize I2C peripheral
				sensor_enable=1;
				i2c_write_command(TSL2561_REG_CONTROL,0x03);
				timerWaitus(420000);
				nextState=STATE2_READ_ADC;
			}

			break;

		case STATE2_READ_ADC:
			nextState = STATE2_READ_ADC; // default


			if ((evt->data.evt_system_external_signal.extsignals & LUX_LETIMER0_READ)!=0 )
			{
#ifdef DEBUG
				LOG_INFO("2");
#endif
//				for (int i = 0; i < 1750000; ) {
//						  i=i+1;
//					}
				SLEEP_SleepBlockBegin(sleepEM2);
				get_ADC_Channel_values(&channel1,&channel0);
				nextState = STATE3_REPORT;

			}

			break;

		case STATE3_REPORT:
			nextState = STATE3_REPORT; // default
			if ((evt->data.evt_system_external_signal.extsignals & LUX_I2C_TRANSFER_DONE)!=0 )
			{
#ifdef DEBUG
				LOG_INFO("3");
#endif
				SLEEP_SleepBlockEnd(sleepEM2);
				calculate_Lux(channel1,channel0,&lux); //calculate luminosity value
				lux2=(uint32_t)lux;
				displayPrintf(DISPLAY_ROW_TEMPVALUE,"Luminosity=%d",lux2); //display luminosity
				lux_read_ble(lux2); //Display and Send BLE Value
				switch_relay_state(lux2); //control relay state


                i2c_write_command(TSL2561_REG_CONTROL,0x00); //power down sensor
				sensor_enable=0; //sensor disabled
				gpioSensorSetOff(); //turn sensor off
				DisableI2C(); //Disable i2c clock and gpio pins
				nextState=STATE0_TIMER_WAIT;


			}

			break;


		} // switch
	}//extra credit
}



