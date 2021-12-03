/*
===============================================================================
 Name        : main.c
 Author      : Sarah Brown
 Description : Main definition and control for clock
===============================================================================
*/

#include "define.h"
int cur_mode = MAIN_DISPLAY;
int alarm_status = FALSE;
int button=0xF; int prev_button=0xF;
int dac_waveup = 0;
int wave_amp = 0;
int counter = 0;
int freq = 0;
int stopwatch_status = FALSE;
int stopwatch_counter = 0;

// setup function to perform various inits
void setup() {
	clk_init();
	rit_init();
	dac_init();
	i2c_init();
	lcd_init();
	rtc_enable();
	alarm_init();
	FIO0DIR |= (1 << SPEAKER_EN_PIN); // set speaker enable pin to output
	disable_speaker(); // turns off speaker so it does not produce unwanted noise
	wait_ticks(500); // to ensure proper setup
}

int main(void) {
	setup(); // calls setup function

    while(1) {
    	button = button_press(); // updates button status
    	if (cur_mode == ALARM) { // if the alarm interrupt was triggered
    		alarm_display(); // go to alarm display
    	}

    	else {
			cur_mode = which_button(button, prev_button); // updates current mode based on button and prev button status

			switch(cur_mode){
				case MAIN_DISPLAY:
					main_display();
					break;
				case SET_TIME:
					set_time();
					break;
				case SET_ALARM:
					set_alarm();
					break;
				case STOPWATCH:
					stopwatch_display();
					break;
				case ALARM:
					alarm_display();
					break;
				default:
					main_display();
					break;
			}
    	}
    	prev_button = button; // updates prev button
    }
}
