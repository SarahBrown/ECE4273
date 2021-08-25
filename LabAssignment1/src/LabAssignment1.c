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

void wait_ticks(int ticks) {
	volatile int count;
	for (count=0; count<ticks; count++) {
		//do nothing
	}
}

int main(void) {
	FIO0DIR |= (1<<22); // configure port 0 bit 22 as output

    while(1) {
        FIO0PIN |= (1 << 22); // make port 0 bit 22 go high
        wait_ticks(30);
        FIO0PIN &= ~(1 << 22); // make port 0 bit 22 go low
        wait_ticks(30);
    }
    return 0 ;
}
