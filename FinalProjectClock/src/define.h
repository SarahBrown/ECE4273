/*
===============================================================================
 Name        : define.h
 Author      : Sarah Brown
 Description : Main header file for including other headers and constants
===============================================================================
*/

// other headers
#include "stdio.h"
#include "string.h"
#include "i2c.h"
#include "lcd.h"
#include "timer.h"
#include "button.h"
#include "alarm.h"
#include "display.h"
#include "melody.h"


// define LPC1769 I2C registers
#define PINSEL1 	(*(volatile unsigned int *)0x4002C004) // pin function select
#define PCLKSEL0 	(*(volatile unsigned int *)0x400FC1A8) // peripheral clock select
#define I2C_SCLH 	(*(volatile unsigned int *)0x4001C010) // sclh duty cycle
#define I2C_SCLL 	(*(volatile unsigned int *)0x4001C014) // scll duty cycle
#define I2C_CONSET 	(*(volatile unsigned int *)0x4001C000) // control set
#define I2C_CONCLR 	(*(volatile unsigned int *)0x4001C018) // control clear
#define I2C_DAT 	(*(volatile unsigned int *)0x4001C008) // data
#define I2C_STAT 	(*(volatile unsigned int *)0x4001C004) // status

// define clock/pll registers
#define CCLKCFG (*(volatile unsigned int *) 0x400FC104)
#define CLKSRCSEL (*(volatile unsigned int *) 0x400FC10C)
#define PLL0CFG (*(volatile unsigned int *) 0x400FC084)
#define PLL0CON (*(volatile unsigned int *) 0x400FC080)
#define PLL0FEED (*(volatile unsigned int *) 0x400FC08C)
#define PLL0STAT (*(volatile unsigned int *) 0x400FC088)
#define CLKOUTCFG (*(volatile unsigned int *) 0x400FC1C8)
#define SCS (*(volatile unsigned int *) 0x400FC1A0)

// define RIT/interupt registers
#define PCLKSEL1 (*(volatile unsigned int *) 0x400FC1AC)
#define PCONP (*(volatile unsigned int *) 0x400FC0C4)
#define RICOMP (*(volatile unsigned int *) 0x400B0000)
#define RICTRL (*(volatile unsigned int *) 0x400B0008)
#define RICOUNTER (*(volatile unsigned int *) 0x400B000C)
#define DACR (*(volatile unsigned int *) 0x4008C000)
#define PCONP (*(volatile unsigned int *) 0x400FC0C4)

// define pin registers
#define PINSEL3 (*(volatile unsigned int *) 0x4002C00C)
#define FIO0DIR (*(volatile unsigned int *)0x2009c000)
#define FIO0PIN (*(volatile unsigned int *)0x2009c014)

// RTC
#define CCR  (*(volatile unsigned int *) 0x40024008) // clock control reg
#define CIIR (*(volatile unsigned int *) 0x4002400C) // counter increment interrupt reg
#define AMR  (*(volatile unsigned int *) 0x40024010) // alarm mask reg. sec: bit 0, min: bit 1, hour: bit 2
#define SEC (*(volatile unsigned int *) 0x40024020)
#define MIN (*(volatile unsigned int *) 0x40024024)
#define HOUR (*(volatile unsigned int *) 0x40024028)
#define ISER0 (*(volatile unsigned int *) 0xE000E100)

// Alarm
#define ILR (*(volatile unsigned int *) 0x40024000)
#define ALSEC (*(volatile unsigned int *) 0x40024060)
#define ALMIN (*(volatile unsigned int *) 0x40024064)
#define ALHOUR (*(volatile unsigned int *) 0x40024068)

// I2C addresses
#define MCP_ADDR_W 0x40// MCP I2C ADDR write
#define MCP_ADDR_R 0x41// MCP I2C ADDR read
#define MCP_IODIRA 0x0 // MCP IO Pins DirA
#define MCP_IODIRB 0x1 // MCP IO Pins DirB
#define MCP_GPIOB 0x13 // GPIOB
#define MCP_GPIOA 0x12 // GPIOA
#define MCP_GPPUB 0x0D // pull-up resistor reg A

// macros
#define PLL_FEED() PLL0FEED = 0xAA; PLL0FEED = 0x55

// PLL variables for CLK init
#define K 2
#define M 24
#define N 1

// various magic numbers
#define TRUE 1
#define FALSE 0

#define COMMAND 0
#define DATA 1

#define PRESSED 0
#define UNPRESSED 1

#define MAIN_DISPLAY 0
#define SET_TIME 1
#define SET_ALARM 2
#define STOPWATCH 3
#define ALARM 4

#define RED 0
#define BLUE 1
#define WHITE 2
#define GREEN 3

#define SET_SECOND BLUE
#define SET_MINUTE WHITE
#define SET_HOUR GREEN

#define DAC_PIN 20 // pin p0.26
#define SPEAKER_EN_PIN 24 // pin p0.24
#define CLEAR 0x01

// frequencies for melody
// 100 MHz/(freq*2) (for 50% square)
#define FREQ_C 191110
#define FREQ_C_SHARP 180388
#define FREQ_D 170265
#define FREQ_D_SHARP 160710
#define FREQ_E 151685
#define FREQ_F 143172
#define FREQ_F_SHARP 135139
#define FREQ_G 127551
#define FREQ_G_SHARP 120395
#define FREQ_A 113636
#define FREQ_A_SHARP 88314
#define FREQ_B 101239

#define FREQ_C_OCT5 95557
#define FREQ_C_SHARP_OCT5 90192
#define FREQ_D_OCT5 85131
#define FREQ_D_SHARP_OCT5 80354
#define FREQ_E_OCT5 75844

#define EIGTH_NOTE 700
#define QUARTER_NOTE 500
#define HALF_NOTE 300
#define WHOLE_NOTE 1
