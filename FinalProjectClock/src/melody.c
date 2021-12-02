#include "define.h"

extern int dac_waveup;
extern int wave_amp;
extern int counter;
extern int freq;

void play_note(int freq) {
	rit_disable();
	RICOMP = freq;	//100 MHz/(freq*2) (for 50% square)
	wave_amp = 1023;
	counter = 0;
	RICOUNTER = 0;
	rit_enable();
}

void play_song() {

}
