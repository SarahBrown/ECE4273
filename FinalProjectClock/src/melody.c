/*
===============================================================================
 Name        : melody.c
 Author      : Sarah Brown
 Description : Various functions to output songs from speaker for alarm
===============================================================================
*/

#include "define.h"

extern int dac_waveup;
extern int wave_amp;
extern int counter;
extern int freq;

// uses fet to disable speaker amp power
void enable_speaker() {
	FIO0PIN &= ~(1 << SPEAKER_EN_PIN); // 0 enables
}

// uses fet to disable speaker amp power
void disable_speaker(){
	FIO0PIN |= (1 << SPEAKER_EN_PIN); // 1 disables
}

// sets the note freq
void play_freq() {
	rit_disable();
	RICOMP = freq;
	wave_amp = 1023;
	counter = 0;
	RICOUNTER = 0;
	rit_enable();
}

// plays a note of a given frequency and size. or is silent for the length of a rest note
void play_note(int note_freq, int note_size, int rest) {
	if (rest) {
		wave_amp = 1023;
		if (note_size == WHOLE_NOTE) {
			while (wave_amp != 0) wave_amp--;
		}

		else if (note_size == HALF_NOTE) {
			while (wave_amp > HALF_NOTE) wave_amp--;
		}

		else if (note_size == QUARTER_NOTE) {
			while (wave_amp > QUARTER_NOTE) wave_amp--;
		}

		else if (note_size == EIGTH_NOTE) {
			while (wave_amp > EIGTH_NOTE) wave_amp--;
		}
	}

	else {
		freq = note_freq;
		play_freq();

		if (note_size == WHOLE_NOTE) {
			while (wave_amp != 0);
		}

		else if (note_size == HALF_NOTE) {
			while (wave_amp > HALF_NOTE);
		}

		else if (note_size == QUARTER_NOTE) {
			while (wave_amp > QUARTER_NOTE);
		}

		else if (note_size == EIGTH_NOTE) {
			while (wave_amp > EIGTH_NOTE);
		}
	}
}

// plays the song oklahoma
void play_song_oklahoma() {
	enable_speaker();
	play_note(FREQ_C, WHOLE_NOTE, FALSE); // OOOOk
	play_note(FREQ_G, QUARTER_NOTE, FALSE); // la
	play_note(FREQ_F, QUARTER_NOTE, FALSE); // hom
	play_note(FREQ_E, QUARTER_NOTE, FALSE); // a
	play_note(FREQ_D, QUARTER_NOTE, FALSE); // where
	play_note(FREQ_C, QUARTER_NOTE, FALSE); // the
	play_note(FREQ_D, HALF_NOTE, FALSE); // wind
	play_note(FREQ_G, HALF_NOTE, FALSE); // comes
	play_note(FREQ_G, QUARTER_NOTE, FALSE); // sweep
	play_note(FREQ_F_SHARP, QUARTER_NOTE, FALSE); // in'
	play_note(FREQ_G, QUARTER_NOTE, FALSE); // down
	play_note(FREQ_A, QUARTER_NOTE, FALSE); // the
	play_note(FREQ_G, WHOLE_NOTE, FALSE); // plain
	play_note(FREQ_G, QUARTER_NOTE, FALSE); // and
	play_note(FREQ_F, QUARTER_NOTE, FALSE); // the
	play_note(FREQ_E, HALF_NOTE, FALSE); // wav
	play_note(FREQ_G, HALF_NOTE, FALSE); // in'
	play_note(FREQ_G, HALF_NOTE, FALSE); // wheat
	play_note(FREQ_E, QUARTER_NOTE, FALSE); // can
	play_note(FREQ_D, HALF_NOTE, FALSE); // sure
	play_note(FREQ_F, HALF_NOTE, FALSE); // smell
	play_note(FREQ_F, HALF_NOTE, FALSE); // sweet
	play_note(FREQ_E, QUARTER_NOTE, FALSE); // when
	play_note(FREQ_D, QUARTER_NOTE, FALSE); // the
	play_note(FREQ_E, HALF_NOTE, FALSE); // wind
	play_note(FREQ_F, HALF_NOTE, FALSE); // comes
	play_note(FREQ_G, HALF_NOTE, FALSE); // right
	play_note(FREQ_A, QUARTER_NOTE, FALSE); // be
	play_note(FREQ_B, QUARTER_NOTE, FALSE); // hind
	play_note(FREQ_C_OCT5, QUARTER_NOTE, FALSE); // the
	play_note(FREQ_D_OCT5, WHOLE_NOTE, FALSE); // rain
	disable_speaker();
}

// plays the pokemon theme song
void play_song_pokemon(){
	enable_speaker();
	play_note(FREQ_A, EIGTH_NOTE, FALSE); // i wanna be the very best like no one ever was
	play_note(FREQ_A, EIGTH_NOTE, FALSE); // wan
	play_note(FREQ_A, EIGTH_NOTE, FALSE); //na
	play_note(FREQ_A, QUARTER_NOTE, FALSE); //be
	play_note(FREQ_A, QUARTER_NOTE, FALSE); // the
	play_note(FREQ_G, QUARTER_NOTE, FALSE);// ver
	play_note(FREQ_E, QUARTER_NOTE, FALSE); //y
	play_note(FREQ_C, QUARTER_NOTE, FALSE); // best
	play_note(FREQ_C, QUARTER_NOTE, FALSE); // like
	play_note(FREQ_A, QUARTER_NOTE, FALSE); // no
	play_note(FREQ_A, QUARTER_NOTE, FALSE); // one
	play_note(FREQ_G, QUARTER_NOTE, FALSE); // ev
	play_note(FREQ_F, QUARTER_NOTE, FALSE); // er
	play_note(FREQ_G, HALF_NOTE, FALSE); // was
	play_note(FREQ_G, WHOLE_NOTE, TRUE); // REST
	play_note(FREQ_F, QUARTER_NOTE, FALSE); // to
	play_note(FREQ_A_SHARP, QUARTER_NOTE, FALSE); // catch
	play_note(FREQ_B, QUARTER_NOTE, FALSE); // them
	play_note(FREQ_B, QUARTER_NOTE, FALSE); // is
	play_note(FREQ_A, QUARTER_NOTE, FALSE); // my
	play_note(FREQ_G, QUARTER_NOTE, FALSE); // real
	play_note(FREQ_F, QUARTER_NOTE, FALSE); // test
	play_note(FREQ_F, QUARTER_NOTE, FALSE); // to
	play_note(FREQ_A, QUARTER_NOTE, FALSE); // train
	play_note(FREQ_A, QUARTER_NOTE, FALSE); // them
	play_note(FREQ_G, QUARTER_NOTE, FALSE); // is
	play_note(FREQ_F, QUARTER_NOTE, FALSE); // my
	play_note(FREQ_A, WHOLE_NOTE, FALSE); // cause
	play_note(FREQ_A, HALF_NOTE, TRUE); // REST
	play_note(FREQ_A, EIGTH_NOTE, FALSE); // po
	play_note(FREQ_C_OCT5, EIGTH_NOTE, FALSE); // ke
	play_note(FREQ_D_OCT5, EIGTH_NOTE, FALSE); // mon
	play_note(FREQ_A, EIGTH_NOTE, TRUE); // REST
	play_note(FREQ_A, QUARTER_NOTE, FALSE); // got
	play_note(FREQ_A, QUARTER_NOTE, FALSE); // ta
	play_note(FREQ_B, QUARTER_NOTE, FALSE); // catch
	play_note(FREQ_D_OCT5, QUARTER_NOTE, FALSE); // em
	play_note(FREQ_D_OCT5, QUARTER_NOTE, FALSE); // all
	disable_speaker();
}
