/*
===============================================================================
 Name        : LabAssignment2.c
 Description : main definition
===============================================================================
*/

#define FIO0DIR ((volatile unsigned int *)0x2009c000)
#define FIO0PIN ((volatile unsigned int *)0x2009c014)
#define FIO1DIR ((volatile unsigned int *)0x2009c020)
#define FIO1PIN ((volatile unsigned int *)0x2009c034)
#define FIO2DIR ((volatile unsigned int *)0x2009c040)
#define FIO2PIN ((volatile unsigned int *)0x2009c054)

#define TRUE 1
#define FALSE 0
#define PRESSED 0 // active low
#define GRID0 0; #define GRID1 1; #define GRID2 2; #define GRID3 3; #define GRID4 4; #define GRID5 5; #define GRID6 6; #define GRID7 7; #define GRID8 8 // grid squares

typedef struct {
	volatile unsigned int * direct_reg;
	volatile unsigned int * pin_reg;
	const unsigned int is_output; // boolean if pin is input or output
	const unsigned char pin_num; // pin number
} pin_details;

pin_details grid_leds_yellow[9] = {
		{FIO2DIR, FIO2PIN, FALSE, 5}, {FIO2DIR, FIO2PIN, FALSE, 4}, {FIO2DIR, FIO2PIN, FALSE, 3}, {FIO2DIR, FIO2PIN, FALSE, 12}, {FIO2DIR, FIO2PIN, FALSE, 11}, {FIO2DIR, FIO2PIN, FALSE, 10}, {FIO0DIR, FIO0PIN, FALSE, 21}, {FIO0DIR, FIO0PIN, FALSE, 3}, {FIO0DIR, FIO0PIN, FALSE, 2}
};

pin_details grid_leds_green[9] = {
		{FIO2DIR, FIO2PIN, FALSE, 2}, {FIO2DIR, FIO2PIN, FALSE, 1}, {FIO2DIR, FIO2PIN, FALSE, 0}, {FIO2DIR, FIO2PIN, FALSE, 8}, {FIO2DIR, FIO2PIN, FALSE, 7}, {FIO2DIR, FIO2PIN, FALSE, 6}, {FIO1DIR, FIO1PIN, FALSE, 31}, {FIO1DIR, FIO1PIN, FALSE, 30}, {FIO0DIR, FIO0PIN, FALSE, 26}
};

pin_details grid_switches[9] = {
		{FIO0DIR, FIO0PIN, TRUE, 11}, {FIO0DIR, FIO0PIN, TRUE, 10}, {FIO0DIR, FIO0PIN, TRUE, 5}, {FIO0DIR, FIO0PIN, TRUE, 16}, {FIO0DIR, FIO0PIN, TRUE, 15}, {FIO0DIR, FIO0PIN, TRUE, 17}, {FIO0DIR, FIO0PIN, TRUE, 25}, {FIO0DIR, FIO0PIN, TRUE, 24}, {FIO0DIR, FIO0PIN, TRUE, 23}
};

pin_details score_leds[6] = {
		{FIO0DIR, FIO0PIN, FALSE, 18}, {FIO0DIR, FIO0PIN, FALSE, 1}, {FIO0DIR, FIO0PIN, FALSE, 0}, {FIO0DIR, FIO0PIN, FALSE, 7}, {FIO0DIR, FIO0PIN, FALSE, 8}, {FIO0DIR, FIO0PIN, FALSE, 9}
};

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

/*
 * Function to configure pins as inputs/outputs
 *
 * @param pin_group[] - array of pins
 */
void config_port(pin_details pin_group[]) {
	for (int i = 0; i < sizeof(pin_group); i++) {
		if (pin_group[i].is_output) {
			*pin_group[i].direct_reg |= (1<<pin_group[i].pin_num);
		}

		else {
			*pin_group[i].direct_reg &= ~(1<<pin_group[i].pin_num);
		}
	}
}

/*
 * Function to one toggle led in a pin group to on/off
 *
 * @param pin_group[] - array of pins
 * @param led_grid_num - stores which led num in array is being toggled
 * @param is_high - boolean value stores whether led is on or offf
 */
void change_led(pin_details pin_group[], int led_num, int is_high) {
	if (is_high) {
			*pin_group[led_num].pin_reg |= (1<<pin_group[led_num].pin_num);
	}

	else {
			*pin_group[led_num].pin_reg &= ~(1<<pin_group[led_num].pin_num);
	}
}

/*
 * Function to toggle multiple leds in a pin group to on/off
 *
 * @param pin_group[] - array of pins
 * @param on_off_values[] - array of led values
 */
void change_leds(pin_details pin_group[], int on_off_values[]) {
	for (int i = 0; i < sizeof(on_off_values); i++) {
		if (on_off_values) {
			*pin_group[i].pin_reg |= (1<<pin_group[i].pin_num);
		}

		else {
			*pin_group[i].pin_reg &= ~(1<<pin_group[i].pin_num);
		}
	}
}

unsigned int read_state(const unsigned char switches[], volatile unsigned int * reg_pointer, int color) {
	int state_one = *reg_pointer & (1 << switches[color]);
	wait_ticks(100);
	int state_two = *reg_pointer & (1 << switches[color]);
	return (state_one & state_two); // anded together for debounce
}

void only_one(int led_count, int which_led, const unsigned char led, volatile unsigned int * reg_pointer, int is_high) {
	if (led_count == which_led) {
		change_led(led, reg_pointer, is_high);
	}

	else {
		change_led(led, reg_pointer, FALSE);
	}
}

void change_leds(int led_count) {
	only_one(led_count,  0, p1_score_leds[0], FIO2PIN, leds_state & (1 << 0));
	only_one(led_count,  1, p1_score_leds[1], FIO2PIN, leds_state & (1 << 1));
	only_one(led_count,  2, p2_score_leds[0], FIO2PIN, leds_state & (1 << 2));
	only_one(led_count,  3, p2_score_leds[1], FIO2PIN, leds_state & (1 << 3));
	only_one(led_count,  4, blue_leds[0], FIO0PIN, leds_state & (1 << 4));
	only_one(led_count,  5, blue_leds[1], FIO0PIN, leds_state & (1 << 5));
	only_one(led_count,  6, blue_leds[2], FIO0PIN, leds_state & (1 << 6));
	only_one(led_count,  7, blue_leds[3], FIO0PIN, leds_state & (1 << 7));
	only_one(led_count,  8, green_leds[0], FIO0PIN, leds_state & (1 << 8));
	only_one(led_count,  9, green_leds[1], FIO0PIN, leds_state & (1 << 9));
	only_one(led_count, 10, green_leds[2], FIO0PIN, leds_state & (1 << 10));
	only_one(led_count, 11, green_leds[3], FIO0PIN, leds_state & (1 << 11));
	only_one(led_count, 12, red_leds[0], FIO0PIN, leds_state & (1 << 12));
	only_one(led_count, 13, red_leds[1], FIO0PIN, leds_state & (1 << 13));
	only_one(led_count, 14, red_leds[2], FIO0PIN, leds_state & (1 << 14));
	only_one(led_count, 15, red_leds[3], FIO0PIN, leds_state & (1 << 15));
}

void setup() {
	// configure ports
	config_port(grid_leds_yellow);
	config_port(grid_leds_green);
	config_port(grid_switches);
	config_port(score_leds);
}


int main(void) {
	setup();
	int p1_red = 1; int p1_green = 1; int p1_blue = 1;
	int p2_red = 1; int p2_green = 1; int p2_blue = 1;
	int led_count = 0; // to switch which led is on

    while(1) {
    	change_leds(led_count);
    	led_count++;


    	if(led_count > 15) {
    		led_count = 0;
    	}
    }
    return 0 ;
}
