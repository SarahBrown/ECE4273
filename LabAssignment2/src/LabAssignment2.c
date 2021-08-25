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

#define PRESSED 0

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

unsigned int read_state(const unsigned char switches[], volatile unsigned int * reg_pointer, int color) {
	int state_one = *reg_pointer & (1 << switches[color]);
	wait_ticks(500); //about 1ms
	int state_two = *reg_pointer & (1 << switches[color]);
	wait_ticks(500);
	int state_three = *reg_pointer & (1 << switches[color]);
	return (state_one & state_two & state_three);
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

	// turn game leds on
	config_port(red_leds, NUM_RED_LEDS, FIO0PIN, TRUE);
	config_port(green_leds, NUM_GREEN_LEDS, FIO0PIN, TRUE);
	config_port(blue_leds, NUM_BLUE_LEDS, FIO0PIN, TRUE);

	// turn score leds off
	config_port(p1_score_leds, NUM_SCORE_LEDS, FIO2PIN, FALSE);
	config_port(p2_score_leds, NUM_SCORE_LEDS, FIO2PIN, FALSE);
}


int main(void) {
	setup();
	int p1_red = 1;
	int p1_green = 1;
	int p1_blue = 1;
	int p2_red = 1;
	int p2_green = 1;
	int p2_blue = 1;


    while(1) {
    	p1_red = read_state(p1_switches, FIO2PIN, RED);
    	p1_green = read_state(p1_switches, FIO2PIN, GREEN);
    	p1_blue = read_state(p1_switches, FIO2PIN, BLUE);
    	p2_red = read_state(p2_switches, FIO2PIN, RED);
    	p2_green = read_state(p2_switches, FIO2PIN, GREEN);
    	p2_blue = read_state(p2_switches, FIO2PIN, BLUE);

    	if (p1_red == PRESSED) {
    		config_port(red_leds, NUM_RED_LEDS, FIO0PIN, FALSE);
    	}
    	else {
    		config_port(red_leds, NUM_RED_LEDS, FIO0PIN, TRUE);
    	}

    	if (p1_green == PRESSED) {
    		config_port(green_leds, NUM_GREEN_LEDS, FIO0PIN, FALSE);
    	}
    	else {
    		config_port(green_leds, NUM_GREEN_LEDS, FIO0PIN, TRUE);
    	}

    	if (p1_blue == PRESSED) {
    		config_port(blue_leds, NUM_BLUE_LEDS, FIO0PIN, FALSE);
    	}
    	else {
    		config_port(blue_leds, NUM_BLUE_LEDS, FIO0PIN, TRUE);
    	}
    }
    return 0 ;
}
