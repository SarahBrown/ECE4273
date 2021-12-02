#include "define.h"

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
