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
#include "math.h"
#define INCLUDE_LOG_DEBUG 1
uint8_t device_address=0x39; //Address of LUX Sensor
uint8_t write_command[2];
uint16_t read_buffer[2];         //Read value from Sensor is stored here
I2C_TransferReturn_TypeDef transferstatus;
I2C_TransferSeq_TypeDef seq;
int transfer;



/*
 * Function Name:void InitI2C()
 *
 * Description: Initializes I2C peripheral
 * .
 * Parameters: None
 *
 *  Silab API's Used: void I2CSPM_Init(I2CSPM_Init_TypeDef *init);
 *
 * Returns: None
 *
 */
void InitI2C()
{

I2CSPM_Init_TypeDef init;
init.port= I2C0;                      /* Use I2C instance 0 */
init.sclPort= gpioPortC;                  /* SCL port */
init.sclPin= 10;                         /* SCL pin */
init.sdaPort= gpioPortC;                  /* SDA port */
init.sdaPin=11;                         /* SDA pin */
init.portLocationScl=14;                         /* Location of SCL */
init.portLocationSda=16;                         /* Location of SDA */
init.i2cRefFreq=0;                          /* Use currently configured reference clock */
init.i2cMaxFreq=I2C_FREQ_STANDARD_MAX;      /* Set to standard rate  */
init.i2cClhr=i2cClockHLRStandard;       /* Set to use 4:4 low/high duty cycle */
I2CSPM_Init(&init);
NVIC_EnableIRQ(I2C0_IRQn);
}

/*
 * Function Name: void writeI2C()
 *
 * Description:Performs I2C Write Operation
 * .
 * Parameters: None
 *
 *  Silab API's Used:I2C_TransferReturn_TypeDef I2CSPM_Transfer(I2C_TypeDef *i2c, I2C_TransferSeq_TypeDef *seq)
 *
 * Returns: None
 *
 */
void writeI2C()
{



 seq.addr= device_address << 1; //Perform a write to device at I2C address 0x40
 seq.flags=I2C_FLAG_WRITE;
 seq.buf[0].data=write_command;  //Write command for temperature measurement
 seq.buf[0].len=1; //length of Write command (bytes)
 transferstatus=I2C_TransferInit(I2C0,&seq);

 }





void i2c_write_command(uint8_t command,uint8_t opcode)
{


	write_command[0]                  = command; // command register address
	write_command[1]                  = opcode; // command opcode


	seq.addr	=	device_address << 1;
	seq.flags	=	I2C_FLAG_WRITE_WRITE;
	seq.buf[0].data = &write_command[0];
	seq.buf[1].data = &write_command[1];
	seq.buf[0].len	=	1;
	seq.buf[1].len	=	1;

	transferstatus = I2CSPM_Transfer (I2C0, &seq);


}

void i2c_read(uint8_t command)
{

	write_command[0]                  = command; // data high register address

	seq.addr        = device_address << 1;
	seq.flags       = I2C_FLAG_WRITE_READ;  // performs a write followed by a read
	seq.buf[0].data = (uint8_t*) &write_command[0];
	seq.buf[0].len  = 1; // write 1 byte,
	seq.buf[1].data = (uint8_t*) &read_buffer;
	seq.buf[1].len  = 2; // read 2 bytes back

	transferstatus = I2CSPM_Transfer (I2C0, &seq);

}

void i2c_write_byte(uint8_t byte)
{
	write_command[0]=byte;
	seq.addr= device_address << 1; //Perform a write to device at I2C address 0x39
	seq.flags=I2C_FLAG_WRITE;
	seq.buf[0].data=&write_command[0];  //Write command for temperature measurement
	seq.buf[0].len=1; //length of Write command (bytes)
	transferstatus=I2C_TransferInit(I2C0,&seq);

}

void get_ADC_Channel_values(double* ch1,double* ch0)
{

	i2c_read(TSL2561_CMD|(TSL2561_REG_DATA_1&0x0F));
	*ch1=read_buffer[0]+read_buffer[1]*256;

    i2c_read(TSL2561_CMD|(TSL2561_REG_DATA_0&0x0F));
    *ch0=read_buffer[0]+read_buffer[1]*256;
}

void calculate_Lux(double ch1,double ch0,double* result)
{
	double ratio;

	if ((ch1 != 0) && (ch0 != 0)) //Avoid divide by zero error
	{
		 ratio = ch1/ ch0;


		if ((ratio <= 0.5) && (ratio > 0.0))
		{
			*result = (0.0304 * ch0) - ((0.062 * ch0) * (pow((ch1/ch0), 1.4)));
		}
		else if ((ratio <= 0.61) && (ratio > 0.5))
		{
			*result = (0.0224 * ch0) - (0.031 * ch1);
		}
		else if ((ratio <= 0.80) && (ratio > 0.61))
		{
			*result = (0.0128 * ch0) - (0.0153 * ch1);
		}
		else if ((ratio <= 1.3) && (ratio > 0.8))
		{
			*result = (0.00146 * ch0) - (0.00112*ch1);
		}
		else
			*result = 0.0;
	}
}

