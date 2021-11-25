/*
===============================================================================
 Name        : LabAssignment1.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#define FIO0DIR (*(volatile unsigned int *)0x2009c000)
#define FIO0PIN (*(volatile unsigned int *)0x2009c014)

// define clock/pll registers
#define CCLKCFG (*(volatile unsigned int *) 0x400FC104)
#define CLKSRCSEL (*(volatile unsigned int *) 0x400FC10C)
#define PLL0CFG (*(volatile unsigned int *) 0x400FC084)
#define PLL0CON (*(volatile unsigned int *) 0x400FC080)
#define PLL0FEED (*(volatile unsigned int *) 0x400FC08C)
#define PLL0STAT (*(volatile unsigned int *) 0x400FC088)
#define CLKOUTCFG (*(volatile unsigned int *) 0x400FC1C8)
#define SCS (*(volatile unsigned int *) 0x400FC1A0)

// macros
#define PLL_FEED() PLL0FEED = 0xAA; PLL0FEED = 0x55

#define K 2
#define M 24
#define N 1

// sets clock to 100 MHz
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

void wait_ticks(int ticks) {
	volatile int count;
	for (count=0; count<ticks; count++) {
		//do nothing
	}
}

int main(void) {
	clk_init();

	FIO0DIR |= (1<<22); // configure port 0 bit 22 as output

    while(1) {
        FIO0PIN |= (1 << 22); // make port 0 bit 22 go high
        wait_ticks(30);
        FIO0PIN &= ~(1 << 22); // make port 0 bit 22 go low
        wait_ticks(30);
    }
    return 0 ;
}
