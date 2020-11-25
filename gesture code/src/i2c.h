/*
 * i2c.h
 *
 *  Created on: 16-Sep-2020
 *      Author: chira
 */
#include "i2cspm.h"
#include "timers.h"
#include "gpio.h"
#include "log.h"


#define I2C_READ_TRANSFER   1
#define	I2C_WRITE_TRANSFER  2


//Function Prototypes
void InitI2C();
//void writeI2C();
void readI2C();
//void CalculateTemperature();
void writeByteI2C(uint8_t cmd);
void write_readi2c(uint8_t reg);
void writeDataByteI2C(uint8_t reg,uint8_t opcode);


