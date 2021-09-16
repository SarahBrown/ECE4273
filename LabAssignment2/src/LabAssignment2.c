/*
===============================================================================
 Name        : LabAssignment2.c
 Description : main definition
===============================================================================
*/

#define FIO0DIR ((volatile unsigned int *)0x2009c000)
#define FIO0PIN ((volatile unsigned int *)0x2009c014)
#define FIO2DIR ((volatile unsigned int *)0x2009c040)
#define FIO2PIN ((volatile unsigned int *)0x2009c054)

#define TRUE 1
#define FALSE 0
#define RED 0
#define GREEN 1
#define BLUE 2

#define PRESSED 0 // active low

#define NUM_RED_LEDS 4
#define NUM_GREEN_LEDS 4
#define NUM_BLUE_LEDS 4
#define NUM_SCORE_LEDS 2
#define NUM_P_SWITCHES 3

const unsigned char red_leds[] = {26,25,24,23}; //p0
const unsigned char green_leds[] = {17,18,1,0}; //p0
const unsigned char blue_leds[] = {6,7,8,9}; //p0
const unsigned char p1_score_leds[] = {8,7}; //p2
const unsigned char p2_score_leds[] = {6,5}; //p2

const unsigned char p1_switches[] = {12,11,10}; //p2.8, 2.7, 2.6
const unsigned char p2_switches[] = {4,3,2}; //p2.8, 2.7, 2.6

unsigned int leds_state = 0b1111111111110000; // rgbp1p2, leds start state so rgb are on, score is off

typedef struct {

} port_details;

//y = 2.727*10^-6 * x + 9.2*10^-6, y in usec, x = ticks
void wait_ticks(int ticks) {
	volatile int count;
	for (count=0; count<ticks; count++) {
		//do nothing
	}
}

void config_port(const unsigned char port[], int length, volatile unsigned int * reg_pointer, int is_high) {
	if (is_high) {
		for (int i = 0; i < length; i++) {
			*reg_pointer |= (1<<port[i]);
		}
	}

	else {
		for (int i = 0; i < length; i++) {
			*reg_pointer &= ~(1<<port[i]);
		}
	}
}

void change_led(const unsigned char led, volatile unsigned int * reg_pointer, int is_high) {
	if (is_high) {
			*reg_pointer |= (1<<led);
	}

	else {
			*reg_pointer &= ~(1<<led);
	}
}

void only_one(int led_count, int which_led, const unsigned char led, volatile unsigned int * reg_pointer, int is_high) {
	if (led_count == which_led) {
		change_led(led, reg_pointer, is_high);
	}

	else {
		change_led(led, reg_pointer, FALSE);
	}
}

unsigned int read_state(const unsigned char switches[], volatile unsigned int * reg_pointer, int color) {
	int state_one = *reg_pointer & (1 << switches[color]);
	wait_ticks(100);
	int state_two = *reg_pointer & (1 << switches[color]);
	return (state_one & state_two); // anded together for debounce
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
	config_port(red_leds, NUM_RED_LEDS, FIO0DIR, TRUE);
	config_port(green_leds, NUM_GREEN_LEDS, FIO0DIR, TRUE);
	config_port(blue_leds, NUM_BLUE_LEDS, FIO0DIR, TRUE);
	config_port(p1_score_leds, NUM_SCORE_LEDS, FIO2DIR, TRUE);
	config_port(p2_score_leds, NUM_SCORE_LEDS, FIO2DIR, TRUE);

	config_port(p1_switches, NUM_P_SWITCHES, FIO2DIR, FALSE);
	config_port(p2_switches, NUM_P_SWITCHES, FIO2DIR, FALSE);

	//turn all off
	config_port(red_leds, NUM_RED_LEDS, FIO0PIN, FALSE);
	config_port(green_leds, NUM_GREEN_LEDS, FIO0PIN, FALSE);
	config_port(blue_leds, NUM_BLUE_LEDS, FIO0PIN, FALSE);
	config_port(p1_score_leds, NUM_SCORE_LEDS, FIO2PIN, FALSE);
	config_port(p2_score_leds, NUM_SCORE_LEDS, FIO2PIN, FALSE);
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
