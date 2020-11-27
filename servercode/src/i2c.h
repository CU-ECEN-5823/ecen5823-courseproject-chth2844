/*
 * i2c.h
 *
 *  Created on: Sep 19, 2020
 *      Author: rucha
 */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

#include "em_i2c.h"
#include "em_gpio.h"
#include "i2cspm.h"
#include "timer.h"
#include "main.h"

//I2C GPIO Ports
#define I2C0_SCL_PORT gpioPortC
#define I2C0_SCL_PIN  10
#define I2C0_SDA_PORT gpioPortC
#define I2C0_SDA_PIN  11

#define TSL2561_ADDR   0x39 // default address

// TSL2561 registers

#define TSL2561_CMD           0x80
#define TSL2561_CMD_CLEAR     0xC0
#define	TSL2561_REG_CONTROL   0x00
#define	TSL2561_REG_TIMING    0x01
#define	TSL2561_REG_THRESH_L  0x02
#define	TSL2561_REG_THRESH_H  0x04
#define	TSL2561_REG_INTCTL    0x06
#define	TSL2561_REG_ID        0x0A
#define	TSL2561_REG_DATA_0    0x0C
#define	TSL2561_REG_DATA_1    0x0E


//Function Prototypes
void InitI2C();
void writeI2C();
void readI2C();
void CalculateTemperature();
void i2c_write_command(uint8_t command,uint8_t opcode);
void get_ADC_Channel_values(double* ch1,double* ch0);
void calculate_Lux(double ch1,double ch0,double* result);
void DisableI2C();


#endif /* SRC_I2C_H_ */
