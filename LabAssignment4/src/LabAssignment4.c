// define LPC1769 I2C registers
#define PINSEL1 	(*(volatile unsigned int *)0x4002C004) // pin function select
#define PCLKSEL0 	(*(volatile unsigned int *)0x400FC1A8) // peripheral clock select
#define I2C_SCLH 	(*(volatile unsigned int *)0x4001C010) // sclh duty cycle
#define I2C_SCLL 	(*(volatile unsigned int *)0x4001C014) // scll duty cycle
#define I2C_CONSET 	(*(volatile unsigned int *)0x4001C000) // control set
#define I2C_CONCLR 	(*(volatile unsigned int *)0x4001C018) // control clear
#define I2C_DAT 	(*(volatile unsigned int *)0x4001C008) // data
#define I2C_STAT 	(*(volatile unsigned int *)0x4001C004) // status

// define logic to be logical
#define TRUE 1
#define FALSE 0

// I2C addresses
#define TC74_ADDR_W 0x90 // temp sensor I2C ADDR write
#define TC74_ADDR_R 0x91 // temp sensor I2C ADDR read
#define MCP_ADDR_W 0x40// MCP I2C ADDR write
#define MCP_ADDR_R 0x41// MCP I2C ADDR read
#define MCP_IODIRA 0x0 // MCP IO Pins DirA
#define MCP_IODIRB 0x1 // MCP IO Pins DirB
#define LEFTSIDE 0x13 // GPIOB
#define RIGHTSIDE 0x12 // GPIOA


// constants
#define CEL 0
#define FAH 1

// global variables
int cur_mode = CEL;

/*
 * Function to waste time
 *
 * @param ticks - x variable in function
 * y = 2.73x + 8.5. units: y - usec, x - ticks
 */
void wait_ticks(int ticks) {
	volatile int count;
	for (count=0; count<ticks; count++) {
		//do nothing
	}
}

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

// Write Single Digit Num
void write_num(int num, int side) {
	// i2c and mcp stuff
	i2c_start();
	i2c_write(MCP_ADDR_W);
	i2c_write(side);

	switch (num) {
		default:
			i2c_write(0x40); break;
		case 0:
			i2c_write(0x40); break;
		case 1:
			i2c_write(0x79); break;
		case 2:
			i2c_write(0x24); break;
		case 3:
			i2c_write(0x30); break;
		case 4:
			i2c_write(0x19); break;
		case 5:
			i2c_write(0x12); break;
		case 6:
			i2c_write(0x03); break;
		case 7:
			i2c_write(0x78); break;
		case 8:
			i2c_write(0x00); break;
		case 9:
			i2c_write(0x18); break;
	}
	i2c_stop();
}

// Write Two Digit Num
void write_two_digit(int temp) {
	int tens_place = temp/10;
	write_num(tens_place, LEFTSIDE);

	int ones_place = temp%10;
	write_num(ones_place, RIGHTSIDE);
}

// Read Temp
int read_temp() {
	int temp = 0;
	// read value
	wait_ticks(100);
	i2c_start(); // start i2c
	i2c_write(TC74_ADDR_W); // select temp
	i2c_write(0); // temp reg 0
	wait_ticks(100);
	i2c_start(); // start read
	i2c_write(TC74_ADDR_R); // temp reg 0

	temp = i2c_read(TRUE);
	wait_ticks(100);
	i2c_stop();

	// if negative, undo twos complement
	if ((temp & 0x80) != 0) {
		temp = -(~temp+1);
	}

	if (cur_mode == FAH) {
		temp = ((9.0/5.0)*temp)+32.0;
	}

	return temp;
}

int button_press() {
	int data;
	i2c_start();
	i2c_write(MCP_ADDR_W);
	i2c_write(RIGHTSIDE);
	i2c_start();
	i2c_write(MCP_ADDR_R);
	data = i2c_read(TRUE);
	i2c_stop();

	return data>>7;
}

void setup() {
	PINSEL1 |= (1 << 22); PINSEL1 |= (1 << 24); // sets scl and sda

	I2C_SCLL = 5; I2C_SCLH = 5; // configs i2c freq

	I2C_CONCLR = 0x38; I2C_CONSET = 0x40;  // inits i2c

    // setup mcp
    // set dir a to output and one input
	i2c_start();
	i2c_write(MCP_ADDR_W);
	i2c_write(MCP_IODIRA);
	i2c_write(0x80); // set a7 to input, rest to output
	i2c_stop();

    // set all dir b to output
	i2c_start();
	i2c_write(MCP_ADDR_W);
	i2c_write(MCP_IODIRB);
	i2c_write(0x00); // set all b to output
	i2c_stop();
}

int main(void) {
	setup();
	int temper = 0;
	int button=0; int prev_button=0; int avg_times = 100.0;
    while(1) {
    	for (int i = 0; i < avg_times; i++) {
    		temper += read_temp();
    	}
    	temper /= avg_times;
    	button = button_press();

    	//temper = temper/num_avg;
    	write_two_digit(temper);
    	// write temp
    	// change temp mode. toggle between when pressed
    	if (button ==1 && prev_button == 0) {
    		cur_mode = !cur_mode;
    	}
    	prev_button=button;

    }
}
