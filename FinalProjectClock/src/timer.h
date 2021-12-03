/*
===============================================================================
 Name        : timer.h
 Author      : Sarah Brown
 Description : Header for rit and rtc functions
===============================================================================
*/

void clk_init();
void rit_init();
void rit_enable();
void rit_disable();
void dac_init();
void RIT_IRQHandler();
void rtc_enable();
void wait_ticks(int ticks);
