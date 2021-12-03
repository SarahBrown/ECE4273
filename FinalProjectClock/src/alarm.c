/*
===============================================================================
 Name        : alarm.c
 Author      : Sarah Brown
 Description : Various functions for controlling the alarm
===============================================================================
*/

#include "define.h"
extern int cur_mode;
extern int alarm_status;
extern int stopwatch_status;
extern int stopwatch_counter;

void RTC_IRQHandler() {
	if ((ILR & 1) == 1) {
		if (stopwatch_status) { // if stopwatch is unpaused
			stopwatch_counter++; // increment
		}

		ILR |= (1 << 0); // resets interrupt
	}
	else { // otherwise interrupt is alarm
		cur_mode = ALARM;
		ILR |= (1 << 1); // resets interrupt
	}
}

void toggle_alarm(int alarm) {
	alarm_status = !alarm_status;

	if (alarm_status) {
		AMR = 0xF8; // sets mask to only check hour, min, sec
	}

	else {
		AMR = 0xFF; // turns off alarm
	}
}

void alarm_init() {
	ALSEC = 0;
	ALMIN = 0;
	ALHOUR = 7; // initializes alarm to 7am
	AMR = 0xFF; // initializes alarm to off
	alarm_status = FALSE;
}
