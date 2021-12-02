/*
===============================================================================
 Name        : FinalProjectClock.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
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


void setup() {
	clk_init();
	//rit_init();
	//dac_init();
	i2c_init();
	lcd_init();
	rtc_enable();
	alarm_init();
	wait_ticks(500);
}

int main(void) {
	setup();

    while(1) {
    	button = button_press();
    	if (cur_mode == ALARM) {
    		alarm_display();
    	}

    	else {
			cur_mode = which_button(button, prev_button);

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
    	prev_button = button;
    }
}
