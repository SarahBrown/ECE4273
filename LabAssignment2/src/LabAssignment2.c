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

const unsigned char red_leds[] = {26,25,24,23}; //p0
const unsigned char green_leds[] = {17,18,1,0}; //p0
const unsigned char blue_leds[] = {6,7,8,9}; //p0
const unsigned char p1_score_leds[] = {8,7}; //p2
const unsigned char p2_score_leds[] = {6,5}; //p2

const unsigned char p1_switches[] = {12,11,10}; //p2.8, 2.7, 2.6
const unsigned char p2_switches[] = {4,3,2}; //p2.8, 2.7, 2.6


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

void setup() {
	// configure ports
	config_port(red_leds, 4, FIO0DIR, TRUE);
	config_port(green_leds, 4, FIO0DIR, TRUE);
	config_port(blue_leds, 4, FIO0DIR, TRUE);
	config_port(p1_score_leds, 2, FIO2DIR, TRUE);
	config_port(p2_score_leds, 2, FIO2DIR, TRUE);

	config_port(p1_switches, 3, FIO2DIR, FALSE);
	config_port(p2_switches, 3, FIO2DIR, FALSE);

	// turn game leds on
	config_port(red_leds, 4, FIO0PIN, TRUE);
	config_port(green_leds, 4, FIO0PIN, TRUE);
	config_port(blue_leds, 4, FIO0PIN, TRUE);

	// turn score leds off
	config_port(p1_score_leds, 2, FIO2PIN, FALSE);
	config_port(p2_score_leds, 2, FIO2PIN, FALSE);
}

int button_state(const unsigned char port[], int length, volatile unsigned int * reg_pointer) {
	int values = 0;
	return values;
}

int main(void) {
	setup();

    while(1) {
    	if (0==0) {
    		config_port(red_leds, 4, FIO0PIN, FALSE);
    	}
    	else {
    		config_port(red_leds, 4, FIO0PIN, TRUE);
    	}
    }
    return 0 ;
}
