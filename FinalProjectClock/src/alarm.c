#include "define.h"
extern int cur_mode;
extern int alarm_status;
extern int stopwatch_status;
extern int stopwatch_counter;

void RTC_IRQHandler() {
	if ((ILR & 1) == 1) {
		if (stopwatch_status) {
			stopwatch_counter++;
		}

		ILR |= (1 << 0);
	}
	else {
		cur_mode = ALARM;
		ILR |= (1 << 1);
	}
}

void toggle_alarm(int alarm) {
	alarm_status = !alarm_status;

	if (alarm_status) {
		AMR = 0xF8;
	}

	else {
		AMR = 0xFF;
	}
}

void alarm_init() {
	ALSEC = 0;
	ALMIN = 0;
	ALHOUR = 7;
	AMR = 0xFF;
	alarm_status = FALSE;
}
