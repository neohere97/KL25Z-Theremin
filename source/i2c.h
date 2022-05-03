/***************************************************************************
 * PES Final Project
 * Author: Alexander Dean
 * Github: https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo/src
 ***************************************************************************/

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

//Function declarations, please check c file for function descriptions
void i2c_init(void);
void i2c_start(void);
void i2c_read_setup(uint8_t dev, uint8_t address);
uint8_t i2c_repeated_read(uint8_t);	
uint8_t i2c_read_byte(uint8_t dev, uint8_t address);
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data);


#endif