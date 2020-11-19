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
#define SEN_port gpioPortD
#define SEN_pin 15
#define SCL_pin	10
#define SDA_pin	11

bool write,read;
//initialize i2c structure
void I2C_init();

//write command to the i2c
void i2c_write(uint8_t *data, uint8_t len);
uint16_t i2c_write_interrupt();
//read data from i2c sensor and convert value to degree celsius
float i2c_read(uint8_t *data, uint8_t len);
uint16_t i2c_read_interrupt();
//enable sensor at PD 15 for load power management
void Sensor_enable();

//disable sensor for load power management at PD15
void Sensor_disable();


float temp_calc();
#endif /* SRC_I2C_H_ */
