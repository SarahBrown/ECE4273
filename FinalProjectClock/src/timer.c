#include "define.h"
extern int dac_waveup;
extern int wave_amp;
extern int counter;
extern int freq;


void clk_init() {
	// step 0 - enable main oscillator
	SCS |= (1 << 5);
	while (!(SCS & (1 << 6)));

	// step 1 - disconnect PLL0 with one feed sequence
	PLL0CON &= ~(1 << 1);
	PLL_FEED();

	// step 2 - disable PLL0 with one feed sequence
	PLL0CON &= ~(1);
	PLL_FEED();

	// step 3 - change cpu clock divider to speed up operation without PLL0, if desired
	// step 4 - write to the clock source selection control register to change the clock source if needed
	CLKSRCSEL = 1;

	// step 5 - write to PLL0CFG and make it effective with one feed sequence (can only be updated when PLL0 disabled)
	PLL0CFG = (N << 16) | M; // write MSEL0 to bits 14:0 and write NSEL0 to bits 23:16 in PLL0CFG
	PLL_FEED();

	// step 6 - enable PLL0 with one feed sequence
	PLL0CON |= (1 << 0);
	PLL_FEED();

	// step 8 - wait for PLL0 to achieve lock
	// step 8 is swapped with 7 per rec of powerpoint slides
	while (!(PLL0STAT & (1 << 26)));

	// step 7 - change CPU clock divider setting for operation with PLL0
	CCLKCFG = K;

	// step 9 - connect PLL0 with one feed sequence
	PLL0CON |= (1 << 1) | (1 << 0);
	PLL_FEED();
}

void rit_init() {
	PCLKSEL1 |= (01 << 26); //pclk = clk, page 59
	PCONP |= (1 << 16);
	RICOMP = freq;
	RICTRL |= (1<<3) | (1<<1);
	RICOUNTER = 0;
	ISER0 |= (1<<29);
}

void rit_enable() {
	ISER0 |= (1<<29);
}

void rit_disable() {
	ISER0 &= ~(1<<29);
}

void dac_init() {
	PINSEL1 = (PINSEL1 & ~(0b11 << DAC_PIN)) | (0b10 << DAC_PIN); // clears pins and then sets them to function 10
}

void RIT_IRQHandler() {
	dac_waveup = !dac_waveup;

	if (dac_waveup) {
		DACR = (wave_amp << 6);
	}

	else {
		DACR = 0;
	}

	if (counter == 5 && wave_amp > 10) {
		wave_amp -= 10;
		//wave_amp *= 0.99;
		counter = 0;
	}

	else if (wave_amp < 10 && wave_amp > 0) {
		wave_amp = 0;
	}

	counter++;
	RICTRL |= 1; // clears the interupt
}

void rtc_enable() {
	PCONP |= (1 << 9); // turns on power to rtc
	CCR &= ~(0 << 0); // time counter is disabled so it may be initialized
	SEC = 0;
	MIN = 0;
	HOUR = 0;
	CCR |= (1 << 0); // time counter is enabled so it may be initialized
	ISER0 |= (1 << 17); // enable rtc interupts, table 52
	CIIR = 1; // generate an interupt every second
}

/*
 * Function to waste time
 *
 * @param ticks - x variable in function
 * y = 2*10^-7*x+7*10^-7
 */
void wait_ticks(int ticks) {
	volatile int count;
	for (count=0; count<ticks; count++) {
		//do nothing
	}
}
