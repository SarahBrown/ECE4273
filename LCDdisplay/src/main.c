#include "stdio.h"
#include "string.h"

// define LPC1769 I2C registers
#define PINSEL1 	(*(volatile unsigned int *)0x4002C004) // pin function select
#define PCLKSEL0 	(*(volatile unsigned int *)0x400FC1A8) // peripheral clock select
#define I2C_SCLH 	(*(volatile unsigned int *)0x4001C010) // sclh duty cycle
#define I2C_SCLL 	(*(volatile unsigned int *)0x4001C014) // scll duty cycle
#define I2C_CONSET 	(*(volatile unsigned int *)0x4001C000) // control set
#define I2C_CONCLR 	(*(volatile unsigned int *)0x4001C018) // control clear
#define I2C_DAT 	(*(volatile unsigned int *)0x4001C008) // data
#define I2C_STAT 	(*(volatile unsigned int *)0x4001C004) // status

// define clock/pll registers
#define CCLKCFG (*(volatile unsigned int *) 0x400FC104)
#define CLKSRCSEL (*(volatile unsigned int *) 0x400FC10C)
#define PLL0CFG (*(volatile unsigned int *) 0x400FC084)
#define PLL0CON (*(volatile unsigned int *) 0x400FC080)
#define PLL0FEED (*(volatile unsigned int *) 0x400FC08C)
#define PLL0STAT (*(volatile unsigned int *) 0x400FC088)
#define CLKOUTCFG (*(volatile unsigned int *) 0x400FC1C8)
#define SCS (*(volatile unsigned int *) 0x400FC1A0)

// define RIT/interupt registers
#define PCLKSEL1 (*(volatile unsigned int *) 0x400FC1AC)
#define PCONP (*(volatile unsigned int *) 0x400FC0C4)
#define RICOMP (*(volatile unsigned int *) 0x400B0000)
#define RICTRL (*(volatile unsigned int *) 0x400B0008)
#define RICOUNTER (*(volatile unsigned int *) 0x400B000C)
#define ISERO0 (*(volatile unsigned int *) 0xE000E100)
#define DACR (*(volatile unsigned int *) 0x4008C000)
#define PCONP (*(volatile unsigned int *) 0x400FC0C4)

// define pin registers
#define PINSEL3 (*(volatile unsigned int *) 0x4002C00C)
#define FIO0DIR (*(volatile unsigned int *)0x2009c000)
#define FIO0PIN (*(volatile unsigned int *)0x2009c014)

// I2C addresses
#define MCP_ADDR_W 0x40// MCP I2C ADDR write
#define MCP_ADDR_R 0x41// MCP I2C ADDR read
#define MCP_IODIRA 0x0 // MCP IO Pins DirA
#define MCP_IODIRB 0x1 // MCP IO Pins DirB
#define MCP_GPIOB 0x13 // GPIOB
#define MCP_GPIOA 0x12 // GPIOA
#define MCP_GPPUB 0x0D // pull-up resistor reg A

// RTC
#define CCR  (*(volatile unsigned int *) 0x40024008) // clock control reg
#define CIIR (*(volatile unsigned int *) 0x4002400C) // counter increment interrupt reg
#define AMR  (*(volatile unsigned int *) 0x40024010) // alarm mask reg
#define CTIME0 (*(volatile unsigned int *) 0x40024014)
#define SEC (*(volatile unsigned int *) 0x40024020)
#define MIN (*(volatile unsigned int *) 0x40024024)
#define HOUR (*(volatile unsigned int *) 0x40024028)

// frequencies
#define MATCH_FREQ_659 75873 // 100 MHz/(659Hz*2) (for 50% square)
#define MATCH_FREQ_523 95602

// macros
#define PLL_FEED() PLL0FEED = 0xAA; PLL0FEED = 0x55

#define K 2
#define M 24
#define N 1

#define TRUE 1
#define FALSE 0

#define COMMAND 0
#define DATA 1

#define PRESSED 0
#define UNPRESSED 1

#define CHIMES 0

#define MAIN_DISPLAY 0
#define SET_TIME 1
#define SET_TIMER 2

#define RED 0
#define BLUE 1
#define WHITE 2
#define GREEN 3

#define SET_SECOND BLUE
#define SET_MINUTE WHITE
#define SET_HOUR GREEN

#define DAC_PIN 20 // pin p0.26
#define CLEAR 0x01

int cur_mode = SET_TIME;

/*
 * Function to waste time
 *
 * @param ticks - x variable in function
 * y = 2*10^-7*x+7*10^-7
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

int button_press() {
	int data;
	i2c_start();
	i2c_write(MCP_ADDR_W);
	i2c_write(MCP_GPIOB);
	i2c_start();
	i2c_write(MCP_ADDR_R);
	data = i2c_read(TRUE);
	i2c_stop();

	return (data & 0x0F);
}

void lcd_write(int command, int isData) {
	// update dbo-db7 to match command code
	i2c_start();
	i2c_write(MCP_ADDR_W);
	i2c_write(MCP_GPIOA);
	i2c_write(command);
	i2c_stop();

	// drive r/~w low (but its grounded so don't need to)

	if (isData) {
		// drive rs low to indicate this is a command
		i2c_start();
		i2c_write(MCP_ADDR_W);
		i2c_write(MCP_GPIOB);
		i2c_write(0x30);
		i2c_stop();
	}

	else {
		// drive rs low to indicate this is a command
		i2c_start();
		i2c_write(MCP_ADDR_W);
		i2c_write(MCP_GPIOB);
		i2c_write(0x20);
		i2c_stop();
	}

	// drive E high than low to generate the pulse
	// cpu is faster than 4MHz so might need a short delay after E is high to meet min pulse width req
	i2c_start();
	i2c_write(MCP_ADDR_W);
	i2c_write(MCP_GPIOB);
	i2c_write(0x00);
	i2c_stop();

	wait_ticks(500);
	i2c_start();
	i2c_write(MCP_ADDR_W);
	i2c_write(MCP_GPIOB);
	i2c_write(0x20);
	i2c_stop();
	// wait 100 us for command to complete
	wait_ticks(500);
}

/*
 * Initializes the clock to operate at 100 MHz
 */
void clk_init() {
	// step 0 - enable main oscillator
	SCS |= (1 << 5);
	while (!(SCS & (1 << 6)));

	// step 1 - disconnect PLL0 with one feed sequence
	PLL0CON &= ~(1 << 1);
	PLL_FEED();

	// step 2 - disable PLL0 with one feed sequence
	PLL0CON &= ~(1);
	PLL_FEED();

	// step 3 - change cpu clock divider to speed up operation without PLL0, if desired
	// step 4 - write to the clock source selection control register to change the clock source if needed
	CLKSRCSEL = 1;

	// step 5 - write to PLL0CFG and make it effective with one feed sequence (can only be updated when PLL0 disabled)
	PLL0CFG = (N << 16) | M; // write MSEL0 to bits 14:0 and write NSEL0 to bits 23:16 in PLL0CFG
	PLL_FEED();

	// step 6 - enable PLL0 with one feed sequence
	PLL0CON |= (1 << 0);
	PLL_FEED();

	// step 8 - wait for PLL0 to achieve lock
	// step 8 is swapped with 7 per rec of powerpoint slides
	while (!(PLL0STAT & (1 << 26)));

	// step 7 - change CPU clock divider setting for operation with PLL0
	CCLKCFG = K;

	// step 9 - connect PLL0 with one feed sequence
	PLL0CON |= (1 << 1) | (1 << 0);
	PLL_FEED();
}

/*
 * Initializes i2c settings
 */
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

void lcd_init() {
	wait_ticks(20000); // wait 4ms after control signals and data signals are configured (configured in i2c setup)
	lcd_write(0x38, COMMAND); // selects full 8-bit bus and 2 line display
	lcd_write(0x06, COMMAND); // sets cursor to auto advance to the right
	lcd_write(0x0c, COMMAND); // enables display only
	lcd_write(0x01, COMMAND); // clears display and moves cursor to upper left
	wait_ticks(20000); // wait 4ms
}

void rit_init() {
	PCLKSEL1 |= (01 << 26); //pclk = clk, page 59
	PCONP |= (1 << 16);
	RICOMP = MATCH_FREQ_659;
	RICTRL |= (1<<3) | (1<<1);
	RICOUNTER = 0;
	ISERO0 |= (1<<29);
}

void rit_enable() {
	ISERO0 |= (1<<29);
}

void rit_disable() {
	ISERO0 &= ~(1<<29);
}

void dac_init() {
	PINSEL1 = (PINSEL1 & ~(0b11 << DAC_PIN)) | (0b10 << DAC_PIN); // clears pins and then sets them to function 10
}

void RIT_IRQHandler() {
	RICTRL |= 1; // clears the interupt
}

void rtc_enable() {
	PCONP |= (1 << 9); // turns on power to rtc
	CCR &= ~(0 << 0); // time counter is disabled so it may be initialized
	SEC = 0;
	MIN = 0;
	HOUR = 0;
	CCR |= (1 << 0); // time counter is enabled so it may be initialized
}

void setup() {
	clk_init();
	rit_init();
	dac_init();
	i2c_init();
	lcd_init();
	rtc_enable();
	wait_ticks(500);
}

void display_string(char *str) {
	int len = strlen(str);

	for (int i = 0; i < len; i++) {
		char cur_char = str[i];
		lcd_write(cur_char, DATA);
	}
}

void display_char(char c) {
	lcd_write(c, DATA);
}

void display_digits(int time_unit) {
	display_char(time_unit/10 + '0');
	display_char(time_unit%10 + '0');
}

int which_button(int button, int prev_button) {
	if ((button & 0b1) == PRESSED && (prev_button & 0b1) == UNPRESSED) {
		return RED;
	}

	else if (((button>>1) & 0b1) == PRESSED && ((prev_button>>1) & 0b1) == UNPRESSED) {
		return BLUE;
	}

	else if (((button>>2) & 0b1) == PRESSED && ((prev_button>>2) & 0b1) == UNPRESSED) {
		return WHITE;
	}

	else if (((button>>3) & 0b1) == PRESSED && ((prev_button>>3) & 0b1) == UNPRESSED) {
		return GREEN;
	}

	return -1;
}

void main_display() {
	int cur_sec = SEC;
	int cur_min = MIN;
	int cur_hour = HOUR;

	lcd_write(0x80, COMMAND); // move cursor to line 1
	display_string("TIME: ");
	display_digits(cur_hour);
	display_char(':');
	display_digits(cur_min);
	display_char(':');
	display_digits(cur_sec);

	lcd_write(0x94, COMMAND); // move cursor to line 3
	display_string("Time:  B1 Alarm:  B2");
	lcd_write(0xD4, COMMAND); // move cursor to line 3
	display_string("Timer: B3 Chimes: B4");
}

void set_time(){
	lcd_write(0x01, COMMAND); // clear display
	CCR &= ~(0 << 0); // time counter is disabled so it may be initialized
	int button=0xF; int prev_button=0xF;
	int cur_sec = SEC; int cur_min = MIN; int cur_hour = HOUR;

	button = button_press();
	int increment_time = which_button(button, prev_button);

	while (increment_time != RED) {
		switch (increment_time) {
			case SET_SECOND:
				cur_sec++;
				if (cur_sec > 59) cur_sec=0;
				break;
			case SET_MINUTE:
				cur_min++;
				if (cur_min > 59) cur_min=0;
				break;
			case SET_HOUR:
				cur_hour++;
				if (cur_hour > 59) cur_hour=0;
				break;
		}
		lcd_write(0x80, COMMAND); // move cursor to line 1
		display_string("TIME: ");
		display_digits(cur_hour);
		display_char(':');
		display_digits(cur_min);
		display_char(':');
		display_digits(cur_sec);

		prev_button = button;
		button = button_press();
		increment_time = which_button(button, prev_button);
	}

	cur_mode = MAIN_DISPLAY;
	SEC = cur_sec;
	MIN = cur_min;
	HOUR = cur_hour;
	CCR |= (1 << 0); // time counter is enabled so it may be initialized
}

int main(void) {
	setup();
	int button=0xF; int prev_button=0xF; int new_mode = cur_mode;

    while(1) {
    	button = button_press();
    	cur_mode = which_button(button, prev_button);

    	switch(cur_mode){
    		case MAIN_DISPLAY:
    			main_display();
    			break;
    		case SET_TIME:
    			set_time();
    			break;
    		default:
    			main_display();
    			break;
    	}
    	prev_button = button;
    }
}
