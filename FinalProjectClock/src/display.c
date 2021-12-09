/*
===============================================================================
 Name        : display.c
 Author      : Sarah Brown
 Description : Various functions to control lcd display output
===============================================================================
*/

#include "define.h"
extern int cur_mode;
extern int alarm_status;
extern int button; extern int prev_button;
extern int stopwatch_status ;
extern int stopwatch_counter;

// main clock display
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
	display_string("B: Time     W: Alarm");
	lcd_write(0xD4, COMMAND); // move cursor to line 4
	display_string("G: Stopwatch");
}

// display for setting time
void set_time(){
	lcd_write(0x01, COMMAND); // clear display
	CCR &= ~(0 << 0); // time counter is disabled so it may be initialized
	int cur_sec = SEC; int cur_min = MIN; int cur_hour = HOUR;

	while (((button >> SET_TIME) & 1) == PRESSED) { // waits for button to be released from entering function
		button = button_press();
	}

	int increment_time = which_button(button, prev_button);

	while (increment_time != RED) { // red button to exit back to main display
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
				if (cur_hour > 23) cur_hour=0;
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
	lcd_write(0x01, COMMAND); // clear display
}

void set_alarm() {
	lcd_write(0x01, COMMAND); // clear display
	int alarm_min = ALMIN; int alarm_hour = ALHOUR;

	while (((button >> SET_ALARM) & 1) == PRESSED) { // waits for button to be released from entering function
		button = button_press();
	}

	int increment_time = which_button(button, prev_button);

	while (increment_time != RED) {  // red button to exit back to main display
		switch (increment_time) {
			case BLUE:
				toggle_alarm();
				break;
			case SET_MINUTE: // white
				alarm_min++;
				if (alarm_min > 59) alarm_min=0;
				break;
			case SET_HOUR: // green
				alarm_hour++;
				if (alarm_hour > 23) alarm_hour=0;
				break;
		}
		lcd_write(0x80, COMMAND); // move cursor to line 1
		display_string("TIME: ");
		display_digits(alarm_hour);
		display_char(':');
		display_digits(alarm_min);
		lcd_write(0xC0, COMMAND); // move cursor to line 2
		if (alarm_status) display_string("Alarm: On ");
		else display_string("Alarm: Off");

		prev_button = button;
		button = button_press();
		increment_time = which_button(button, prev_button);
	}

	cur_mode = MAIN_DISPLAY;
	ALMIN = alarm_min;
	ALHOUR = alarm_hour;
	lcd_write(0x01, COMMAND); // clear display
}

void alarm_display() {
	lcd_write(0x01, COMMAND); // clear display
	lcd_write(0x80, COMMAND); // move cursor to line 1
	display_string("ALARM ALARM ALARM");
	//play_song_pokemon();
	play_song_oklahoma();

	prev_button = button;
	button = button_press();

	int check_button = which_button(button, prev_button);

	while (check_button != RED) {  // red button to exit back to main display
		lcd_write(0x80, COMMAND); // move cursor to line 1
		display_string("ALARM ALARM ALARM");

		prev_button = button;
		button = button_press();
		check_button = which_button(button, prev_button);
	}

	cur_mode = MAIN_DISPLAY;
	lcd_write(0x01, COMMAND); // clear display
}

void stopwatch_display() {
	lcd_write(0x01, COMMAND); // clear display
	int stopwatch_min = 0; int stopwatch_hour = 0;

	while (((button >> STOPWATCH) & 1) == PRESSED) { // waits for button to be released from entering function
		button = button_press();
	}

	int check_button = which_button(button, prev_button);

	while (check_button != RED) {  // red button to exit back to main display
		switch (check_button) {
			case BLUE:
				stopwatch_status = !stopwatch_status;
				break;
			case WHITE:
				stopwatch_counter = 0;
				stopwatch_min = 0;
				stopwatch_hour = 0;
				break;
		}

		if (stopwatch_counter > 59) {
			stopwatch_counter = 0;
			stopwatch_min++;
		}

		if (stopwatch_min > 59) {
			stopwatch_min = 0;
			stopwatch_hour++;
		}

		if (stopwatch_hour > 23) {
			stopwatch_hour = 0;
		}

		lcd_write(0x80, COMMAND); // move cursor to line 1
		display_string("TIME: ");
		display_digits(stopwatch_hour);
		display_char(':');
		display_digits(stopwatch_min);
		display_char(':');
		display_digits(stopwatch_counter);

		prev_button = button;
		button = button_press();
		check_button = which_button(button, prev_button);
	}

	cur_mode = MAIN_DISPLAY;
	lcd_write(0x01, COMMAND); // clear display
}
