/*
===============================================================================
 Name        : i2c.h
 Author      : Sarah Brown
 Description : Header for i2c functions
===============================================================================
*/

void i2c_start();
void i2c_write(int data);
int i2c_read(int stop);
void i2c_stop();
void i2c_init();
