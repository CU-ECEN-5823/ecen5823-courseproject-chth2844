/**************************************************************************************************
 * @file      scheduler.c
 * @version   0.0.1
 * @brief     scheduler.c Function Implementations
 *
 * @author    Chirayu Thakur
 * @date     Oct 2, 2020
 *
 * @institution University of Colorado Boulder (UCB)
 * @course      ECEN 5823-001: IoT Embedded Firmware (Fall 2020)
 * @instructor  David Sluiter
 *
 * @assignment ecen5823-assignment5-ruchaborwankar/chirayu
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
uint8_t op=0,op2=0;

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

void switch_relay_state(uint32_t lum)
{

	if(lum<LUX_THRESHOLD){
	GPIO_PinOutSet(RELAY_port,RELAY_pin);
	displayPrintf(DISPLAY_ROW_ACTION,"RELAY ON");
	op=1;
	}
	else
	{
		GPIO_PinOutClear(RELAY_port,RELAY_pin);
		displayPrintf(DISPLAY_ROW_ACTION,"RELAY OFF");
		op=0;
	}

		//op = GPIO_PinInGet(PB0_port, PB0_pin);
		LOG_INFO("relay Status = %d\n", op);
		BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_server_write_attribute_value(gattdb_relay_state,0,1,&op));
		BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_server_send_characteristic_notification(0xFF,gattdb_relay_state,1,&op));
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

