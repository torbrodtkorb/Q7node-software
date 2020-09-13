#include "timer.h"
#include <avr/interrupt.h>

void timer_config(void){
	// clk/2
	TCB0.CTRLA = (0x1 << 1);
	
	// enables interrupt on capture
	TCB0.INTCTRL = (1 << 0);
	TCB0.CCMP = 5000;
}

void timer_start(void){
	TCB0.CTRLA |= (1 << 0);
}

void timer_stop(void){
	TCB0.CTRLA &= ~(1 << 0);
	TCB0.CNT = 0;
}
