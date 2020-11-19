/**************************************************************************************************
 * @file      i2c.c
 * @version   0.0.1
 * @brief     i2c.c Function Implementations
 *
 * @author    Rucha Borwankar
 * @date     Oct 2, 2020
 *
 * @institution University of Colorado Boulder (UCB)
 * @course      ECEN 5823-001: IoT Embedded Firmware (Fall 2020)
 * @instructor  David Sluiter
 *
 * @assignment ecen5823-assignment3-ruchaborwankar
 * @due        Sep 18, 2020
 *
 * @resources  Utilized Silicon Labs' EMLIB peripheral libraries to implement functionality.
 *         - i2cspm.h used for i2c and Si7021 communication
 *
 * @copyright  All rights reserved. Distribution allowed only for the use of assignment grading.
 *       Use of code excerpts allowed at the discretion of author. Contact for permission.
 */

#include "i2c.h"

/*
 * I2c read,write buffers and read and write transferSeq for interrupt based i2c interrupt based functions.
 */
uint8_t read_buf[2]={0,0};
uint8_t write_buf[1]={0xF3};
I2C_TransferSeq_TypeDef i2c_read_seq=
{
		  .addr = (0x40)<<1,
		  .flags = I2C_FLAG_READ,
		  .buf[0].data = read_buf,
		  .buf[0].len	= 2,
};

I2C_TransferSeq_TypeDef i2c_write_seq=
{
     	  .addr = (0x40)<<1,
		  .flags = I2C_FLAG_WRITE,
		  .buf[0].data=write_buf,
		  .buf[0].len=1
};


/* Function 	: I2C_init()
 * Description  : The initialization for the I2C for temperature sensor using the
 *				  I2CSPM_Init_Typedef structure that consists of initialization for
 *				  port,pins,location of respective pins,configuring the clock,the type for pins
 *				  is open drain GPIO type with High frequency clock.
 * 	Return		: No return value
*/
void I2C_init(){
//header file
	I2CSPM_Init_TypeDef I2C_init =
	{ 	I2C0,                       /* Use I2C instance 0 */                       \
		gpioPortC,                  /* SCL port */                                 \
	    SCL_pin,                   /* SCL pin */                                  \
		gpioPortC,                  /* SDA port */                                 \
	    SDA_pin,                         /* SDA pin */                                  \
	    14,                         /* Location of SCL */                          \
	    16,                         /* Location of SDA */                          \
	    0,                          /* Use currently configured reference clock */ \
	    I2C_FREQ_STANDARD_MAX,      /* Set to standard rate  */                    \
	    i2cClockHLRStandard,        /* Set to use 4:4 low/high duty cycle */
	 };
	I2CSPM_Init(&I2C_init);
	//uint32_t i2c_bus_frequency = I2C_BusFreqGet (I2C0);
#ifdef DEBUG
	LOG_INFO("i2c clock value=%ld",i2c_bus_frequency);
#endif
}


/* Function 	: I2C_write(uint8_t data,uint8_t len)
 * Parameters	: Data and length
 * Description  : The write function for the I2C consists of the I2C_TransferSeq_Typedef which consists of
 * 				  parameters which take command i.e 0xF3 which puts it in the No hold master mode.
 * 				  The flag here is set as I2C_FLAG_WRITE and seq.addr takes the device address which in case of SI7021 is 0x40
 * Return value : No return value but the function checks the I2C_Transfer function that
 * 				  returns the TransferDone value i.e 0 on successful write.
*/
void i2c_write(uint8_t *write_data, uint8_t len)
{
	I2C_TransferSeq_TypeDef write_Init;
	I2C_TransferReturn_TypeDef ret = 0;
	write_Init.addr = (0x40 << 1);
	write_Init.flags = I2C_FLAG_WRITE;
	write_Init.buf[0].data = write_data;
	write_Init.buf[0].len = len;
	ret = I2CSPM_Transfer(I2C0,&write_Init);
	LOG_INFO("%d=ret from write",ret);
}

/* Function 	: I2C_write_interrupt
 * Parameters	: Data and length
 * Description  : The write function for the I2C interrupt based will enable i2c and the initialize transfer init
 * Return value : checks and returns the the TransferDone value i.e 0 on successful write.
*/
uint16_t i2c_write_interrupt(){
		I2C_TransferReturn_TypeDef i2c_ret_status = 0;
		write=1;
#ifdef DEBUG
		LOG_INFO("IN write interrupt i2c func\n");
#endif
		NVIC_EnableIRQ(I2C0_IRQn);
		I2C_Enable(I2C0,true);
		i2c_ret_status = I2C_TransferInit(I2C0,&i2c_write_seq);
		return i2c_ret_status;

}

/* Function 	: I2C_read()
 * Description  : The read function for the I2C consists of the I2C_TransferSeq_Typedef which consists of
 * 				  parameters like data and length are passed which help store the received value from I2C transfer.
 * 				  The flag here is set as I2C_FLAG_READ and seq.addr takes the device address which in case of SI7021 is 0x40.
 * 				  The received data is stored in a buffer which is then shifted and stored in a 16 bit integer variable then used to
 * 				  calculate temperature in degree Celsius.The value is stored in float variable temp.
 * Return value : Returns the float temperature value in degree celsius.
*/
float i2c_read(uint8_t *read_data, uint8_t len)
{
	I2C_TransferSeq_TypeDef read_Init;
	I2C_TransferReturn_TypeDef ret = 0;
	read_Init.addr = (0x40 << 1);
	read_Init.flags = I2C_FLAG_READ;
	read_Init.buf[0].data = read_data;
	read_Init.buf[0].len = len;
	ret = I2CSPM_Transfer(I2C0,&read_Init);
	LOG_INFO("Read is %d",ret);
	float temp;
		int16_t comb_data = (read_data[0]<<8) + (read_data[1]);
		temp = (float)((175.72*(comb_data)) / 65536 ) - 46.85;
		return temp;
}

/* Function 	: temp_calc
 * Description  : In this,converts the read value into float value for temperature according to the data sheet.
 * Return value : Returns the float temperature value in degree celsius.
*/
float temp_calc(){
	float temp;
	int16_t comb_data = (read_buf[0]<<8) + (read_buf[1]);
	temp = (float)((175.72*(comb_data)) / 65536 ) - 46.85;
#ifdef DEBUG
	LOG_INFO("%f=temperature",temp);
#endif
	return temp;
}

/* Function 	: I2C_read_interrupt()
 * Description  : The read function initializes the irq to receive irq and then status is checked of transfer init function.
 * Return value : Returns read transfer done value.
*/

uint16_t i2c_read_interrupt()
{
	I2C_TransferReturn_TypeDef i2c_status = 0;
	read=1;
#ifdef DEBUG
	LOG_INFO("IN read interrupt i2c func\n");
#endif
	NVIC_EnableIRQ(I2C0_IRQn);
	i2c_status = I2C_TransferInit(I2C0,&i2c_read_seq);
	return i2c_status;
}

/* Function 	: Sensor_enable()
 * Description  : Function enables GPIO PD15 which is Sensor enable pin which is used for load power management
*/
void Sensor_enable(){
#ifdef DEBUG
	LOG_INFO("REACHED HERE\n\r");
#endif
	GPIO_PinModeSet(gpioPortD,SEN_pin,gpioModePushPull,true);
	GPIO_PinOutSet(SEN_port,SEN_pin);
}

/* Function 	: Sensor_disable()
 * Description  : Function disables GPIO PD15 which is Sensor enable pin which is used for load power management
*/
void Sensor_disable(){
GPIO_PinOutClear(SEN_port,SEN_pin);
GPIO_PinModeSet(SEN_port, SEN_pin, gpioModeDisabled, false);
}


