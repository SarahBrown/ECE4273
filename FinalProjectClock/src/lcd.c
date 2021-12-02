#include "define.h"

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

void lcd_init() {
	wait_ticks(20000); // wait 4ms after control signals and data signals are configured (configured in i2c setup)
	lcd_write(0x38, COMMAND); // selects full 8-bit bus and 2 line display
	lcd_write(0x06, COMMAND); // sets cursor to auto advance to the right
	lcd_write(0x0c, COMMAND); // enables display only
	lcd_write(0x01, COMMAND); // clears display and moves cursor to upper left
	wait_ticks(20000); // wait 4ms
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

