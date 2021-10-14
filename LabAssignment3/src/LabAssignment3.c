// define clock/pll registers
#define CCLKCFG (*(volatile unsigned int *) 0x400FC104)
#define CLKSRCSEL (*(volatile unsigned int *) 0x400FC10C)
#define PLL0CFG (*(volatile unsigned int *) 0x400FC084)
#define PLL0CON (*(volatile unsigned int *) 0x400FC080)
#define PLL0FEED (*(volatile unsigned int *) 0x400FC08C)
#define PLL0STAT (*(volatile unsigned int *) 0x400FC088)
#define CLKOUTCFG (*(volatile unsigned int *) 0x400FC1C8)
// define pin registers
#define PINSEL3 (*(volatile unsigned int *) 0x4002C00C)

// macros
#define PLL_FEED() PLL0FEED = 0xAA; PLL0FEED = 0x55

#define K 25
#define M 144

void setup() {
	PINSEL3 |= (01<<22); // configure clkout as output
}

int main(void) {
	// step 1 - disconnect PLL0 with one feed sequence
	PLL0CON &= ~(1 << 1);
	PLL_FEED();

	// step 2 - disable PLL0 with one feed sequence
	PLL0CON &= ~(1);
	PLL_FEED();

	// step 3 - change cpu clock divider to speed up operation without PLL0, if desired
	// step 4 - write to the clock source selection control register to change the clock source if needed
	CLKSRCSEL = 0;

	// step 5 - write to PLL0CFG and make it effective with one feed sequence (can only be updated when PLL0 disabled)
	PLL0CFG = 35; // bit 15 and bits 23:16 will be 0 by nature of decimal 35. Writing n at same time as m, but it is n=1-1=0
	PLL_FEED();

	// step 6 - enable PLL0 with one feed sequence
	PLL0CON |= (1 << 0);
	PLL_FEED();

	// step 8 - wait for PLL0 to achieve lock
	// step 8 is swapped with 7 per rec of powerpoint slides
	while (!(PLL0STAT >> 26)) {
	}

	// step 7 - change CPU clock divider setting for operation with PLL0
	CCLKCFG = K;

	// step 9 - connect PLL0 with one feed sequence
	PLL0CON |= (1 << 1);
	PLL_FEED();

	CLKOUTCFG = 0b0100000000;
	setup(); // setup pins


    while(1) {
    }
}
