#define F_CPU 20000000

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>

#include "led_programs.h"
#include "button_pres.h"
#include "ws2812b.h"
#include "timer.h"

enum press_type {
	PRESS_SHORT,
	PRESS_LONG
};

static volatile uint16_t tick = 0;

void led_config(void)
{
	// Set port A6 og 7 to output
	PORTB.DIRSET = (0b11 << 4);
	PORTC.DIRSET = (0b111 << 0);
	
	// turn on leds
	PORTB.OUTCLR = (0b11 << 4);
	PORTC.OUTCLR = (0b111 << 0);
	
	CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB = 0;
}

void interrupt_config()
{
	// Setup interrupt
	PORTA.PIN4CTRL |= (1 << 3) | (0x1 << 0);
}

volatile uint8_t interrupt_capture = 1;
volatile uint8_t program_counter = 0;
volatile uint8_t off = 0;
uint8_t top_value = 6;


char color1 [3];
char color2 [3];
char color3 [3];
char color4 [3];
char color5 [3];
char color6 [3];
char color7 [3];
char color8 [3];
char color9 [3];

float hue = 0;

/* Put your code here */
static void press_available(enum press_type type) {
	if (type == PRESS_LONG) {
		//PORTB.OUTTGL = (0b1 << 4);
		off = 1;
		interrupt_capture = 0;
		program_counter = 0;
		
		
		} else if (type == PRESS_SHORT) {
		//PORTC.OUTTGL = (0b1 << 2);
		interrupt_capture = 1;
	}
}


int main(void)
{
	sei();
	interrupt_config();
	led_config();
	timer_config();
	led_strip_on(0,0,0);
	
	while (!interrupt_capture);
	interrupt_capture = 0;

	while (1)
	{
		if (off == 1){
			off = 0;
			led_strip_on(0,0,0);
		}
		
		if (interrupt_capture == 1)
		{
			program_counter += 1;
			interrupt_capture = 0;
			
			if (program_counter == (top_value + 1)) {
				program_counter = 1;
			}
			
		}
		
		switch (program_counter)
		{
			case 1:
			rb((uint8_t *)&interrupt_capture, (uint8_t *)&off, 0.007);
			break;
			
			case 2:
			rb((uint8_t *)&interrupt_capture, (uint8_t *)&off, 0.0015);
			break;
			
			case 3:
			smuth((uint8_t *)&interrupt_capture, (uint8_t *)&off);
			break;
			
			case 4:
			led_strip_sparkle(5,5);
			break;
			
			case 5:
			led_strip_sparkle_two(1,3);
			break;
			
			case 6:
			snake((uint8_t *)&interrupt_capture, (uint8_t *)&off);
			break;
		}
	}
}

#define LONG_PRESS_MS 500
#define DEBOUNCE_MS 10

ISR (PORTA_PORT_vect)
{
	// Clear interrupt flag
	PORTA.INTFLAGS |= (1 << 4);
	
	if (PORTA.IN & (1 << 4)) {
		/* Button released */
		if (tick > DEBOUNCE_MS) {
			/* Event is handled - button is released */
			timer_stop();
			if (tick > LONG_PRESS_MS) {
				press_available(PRESS_LONG);
				} else {
				press_available(PRESS_SHORT);
			}
			tick = 0;
			} else {
			//PORTC.OUTTGL = (0b111 << 0);
			timer_stop();
			tick = 0;
		}
		} else {
		/* Button pressed */
		tick = 0;
		
		timer_start();
	}
}

ISR (TCB0_INT_vect){
	//hvis button press
	TCB0.INTFLAGS = (1 << 0);
	tick++;
}
