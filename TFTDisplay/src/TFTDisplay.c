// general LPC1769 Registers
#define PCONP
#define PCLKSEL0
#define PINSEL0
#define PINSEL1
#define PINMODE


// define LPC1769 SPI Registers
#define S0SPCR 	(*(volatile unsigned int *) 0x40020000) // SPI Control Register
#define S0SPSR 	(*(volatile unsigned int *) 0x40020004) // SPI Status Register
#define S0SPDR 	(*(volatile unsigned int *) 0x40020008) // SPI Data Register
#define S0SPCCR (*(volatile unsigned int *) 0x4002000C) // SPI Clock Counter Register
#define S0SPINT (*(volatile unsigned int *) 0x4002001C) // SPI Interrupt Flag

#define SCK // SPI clock signal
#define SSEL // S select
#define MISO // M in S out, transfer serial data from SPI s to SPI m
#define MOSI // M out S in, transfer serial data from SPI m to SPI s

// Adafruit_ST7789, 2.0" TFT with ST7789, 320x240
#define TFT_WIDTH 240
#define TFT_HEIGHT 320

// set up SPI
void setup() {
	// set bit PCSPI in PCONP register (table 46), PCSPI = 1

	// set bit PCLK_SPI in the PCLKSEL0 register (table 40), in master mode the clock must be an even number >= 8 (sect 17.7.4)

	// configure SPI pins
	// PINSEL0 (table 80), PINSEL0[31:30] is used to configure the SPI CLK pin
	// PINSEL1 (table 81), PINSEL1[1:0], PINSEL1[3:2] and PINSEL1[5:4] are used to configure the pins SSEL, MISO and MOSI
	// PINMODE (sect 8.4)

	// interrupts
	// SPI interrupt flag is enabled using S0SPINT[0] bit (sect 17.7.7)
	// SPI interrupt flag enabled in the NVIC (table 50)
}
