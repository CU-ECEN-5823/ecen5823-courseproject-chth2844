/*
 * i2c.c
 *
 *  Created on: 26-Sep-2020
 *      Author: chira
 */
#include "i2c.h"
#define INCLUDE_LOG_DEBUG 1
uint8_t device_address=0x10; //Address of ZX Gesture Sensor
uint8_t write_command[1]={0x10}; //Command to Perform Temperature Measurement
uint16_t read_buffer[2];         //Read value from Sensor is stored here
uint16_t result;             //16 bit Temperature value from Sensor
float temperature; //Final Temperature Measurement
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
init.sclPort= gpioPortF;                  /* SCL port */
init.sclPin= 2;                         /* SCL pin */
init.sdaPort= gpioPortF;                  /* SDA port */
init.sdaPin=3;                         /* SDA pin */
init.portLocationScl=25;                         /* Location of SCL */
init.portLocationSda=27;                         /* Location of SDA */
init.i2cRefFreq=0;                          /* Use currently configured reference clock */
init.i2cMaxFreq=I2C_FREQ_STANDARD_MAX;      /* Set to standard rate  */
init.i2cClhr=i2cClockHLRStandard;       /* Set to use 4:4 low/high duty cycle */
I2CSPM_Init(&init);
//NVIC_EnableIRQ(I2C0_IRQn);
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
void writeByteI2C(uint8_t cmd)
{

uint8_t write[1];
write[0]=cmd;

 transfer=I2C_WRITE_TRANSFER;
 seq.addr= device_address << 1; //Perform a write to device at I2C address 0x10
 seq.flags=I2C_FLAG_WRITE;
 seq.buf[0].data=write;  //Write command for gesture measurement
 seq.buf[0].len=1; //length of Write command (bytes)
 transferstatus = I2CSPM_Transfer (I2C0, &seq);

 	if (transferstatus != i2cTransferDone) {
 	gpioLed1SetOn();
 	LOG_ERROR ("I2CSPM_Transfer: I2C bus write  failed\n");
 	    }

 }



void writeDataByteI2C(uint8_t reg,uint8_t opcode)
{

	uint8_t                        cmd_data[2];


	cmd_data[0]                  = reg; // command register address
	cmd_data[1]                  = opcode; // command opcode

	seq.addr        = device_address << 1;
	seq.flags       = I2C_FLAG_WRITE;
	seq.buf[0].data = (uint8_t*)&cmd_data[0];
	seq.buf[0].len  = 2; // write 2 bytes, command register address + opcode
	seq.buf[1].data = 0;
	seq.buf[1].len  = 0;

	transferstatus = I2CSPM_Transfer (I2C0,&seq);

	if (transferstatus != i2cTransferDone) {

	LOG_ERROR ("I2CSPM_Transfer: I2C bus write failed\n");
    }
}


/*
 * Function Name: void readI2C()
 *
 * Function Description: Performs I2C Read Operation
 * .
 * Parameters: None
 *
 *  Silab API's Used:I2C_TransferReturn_TypeDef I2CSPM_Transfer(I2C_TypeDef *i2c, I2C_TransferSeq_TypeDef *seq)
 *
 * Returns: None
 *
 */


void readI2C()
{
	transfer=I2C_READ_TRANSFER;
	seq.addr=device_address<<1; //Perform a read to device at I2C address 0x10
	seq.flags=I2C_FLAG_READ;
	seq.buf[0].data=(uint8_t*)read_buffer; //Load MSB of read value into read_buffer[0]
	seq.buf[0].len=1;
	//transferstatus=I2C_TransferInit(I2C0,&seq);
	transferstatus = I2CSPM_Transfer (I2C0, &seq);

		if (transferstatus != i2cTransferDone) {
		gpioLed1SetOn();
		LOG_ERROR ("I2CSPM_Transfer: I2C bus  read failed\n");
		    }


}


void write_readi2c(uint8_t reg)
{
	uint8_t cmd_data[0];

	cmd_data[0]                  = reg; // data high register address

	seq.addr        = device_address << 1;
	seq.flags       = I2C_FLAG_WRITE_READ;  // performs a write followed by a read
    seq.buf[0].data = (uint8_t*) &cmd_data;
	seq.buf[0].len  = 1; // write 1 byte,
	seq.buf[1].data = (uint8_t*) &read_buffer[0];
	seq.buf[1].len  = 1; // read 1 byte back
	transferstatus = I2CSPM_Transfer (I2C0, &seq);

	if (transferstatus != i2cTransferDone) {
	gpioLed1SetOn();
	LOG_ERROR ("I2CSPM_Transfer: I2C bus write read failed\n");
	    }

}


