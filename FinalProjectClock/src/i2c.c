/*
===============================================================================
 Name        : i2c.c
 Author      : Sarah Brown
 Description : Various functions for use of i2c
===============================================================================
*/

#include "define.h"

// I2C Start
void i2c_start() {
	I2C_CONSET = (1 << 3); // set SI bit in i2c cont
	I2C_CONSET = (1 << 5); // set STA bit in i2c cont
    I2C_CONCLR = (1 << 3); // clear SI bit

    while (((I2C_CONSET >> 3) & 1) == 0) {} // wait for SI bit to return to 1

    I2C_CONCLR = (1 << 5); // clear STA bit
}

// I2C Write
void i2c_write(int data) {
	I2C_DAT = data;
    I2C_CONCLR = (1 << 3); // clear SI bit
    while (((I2C_CONSET >> 3) & 1) == 0) {} // wait for SI bit to return to 1

}

// I2C Read
int i2c_read(int stop) {
	// wait for data
    I2C_CONCLR = (1 << 3); // clear SI bit
    while (((I2C_CONSET >> 3) & 1) == 0) {} // wait for SI bit to return to 1

	int data = I2C_DAT; // save the data

	if (stop) { // send nack
        I2C_CONCLR = (1 << 2);
	}

	else { //send ack
        I2C_CONSET = (1 << 2);
	}

	return data;
}

// I2C Stop
void i2c_stop() {
	I2C_CONSET = (1 << 4); // set STO bit in i2c cont
    I2C_CONCLR = (1 << 3); // clear SI bit
    while (((I2C_CONSET >> 4) & 1) == 0) {} // wait for STO bit to return to 1
}

// I2C Init
void i2c_init() {
	PINSEL1 |= (1 << 22); PINSEL1 |= (1 << 24); // sets scl and sda

	// want i2c freq to be 100 kHz, clock is at 100 MHz
	// PCLK is CCLK/4 by default, divide by 250 to be 100 kHz
	I2C_SCLL = 125; I2C_SCLH = 125; // configs i2c freq, pg 459

	I2C_CONCLR = 0x38; I2C_CONSET = 0x40;  // inits i2c

	// setup mcp
	// set dir a to all output
	i2c_start();
	i2c_write(MCP_ADDR_W);
	i2c_write(MCP_IODIRA);
	i2c_write(0x00); // set a to output
	i2c_stop();

	// set all dir b to output and input
	i2c_start();
	i2c_write(MCP_ADDR_W);
	i2c_write(MCP_IODIRB);
	i2c_write(0x0F); // set b0-b3 to input, b4-b7 to output
	i2c_stop();

	// GPPU
	i2c_start();
	i2c_write(MCP_ADDR_W);
	i2c_write(MCP_GPPUB);
	i2c_write(0x0F); // set GPIO PULL-UP RESISTOR REGISTER for B
	i2c_stop();

}
