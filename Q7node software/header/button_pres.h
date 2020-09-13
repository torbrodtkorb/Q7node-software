#ifndef F_CPU
#define F_CPU 20000000
#endif

#ifndef BUTTON_PRES_H
#define BUTTON_PRES_H


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

extern volatile uint8_t button_pressed_before;
extern volatile uint8_t new_button_press;

void button_press_config(void);

void button_press_detect(void);




#endif