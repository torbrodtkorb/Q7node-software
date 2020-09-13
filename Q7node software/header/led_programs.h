#ifndef LED_PROGRAMS_H
#define LED_PROGRAMS_H
#define NUMBER_OF_LEDS 295

#ifndef F_CPU
#define F_CPU 20000000
#endif

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include "ws2812b.h"



float hue2rgb(float p, float q, float t);

void hslToRgb(float h, float s, float l, color *rgb);

void led_strip_on(uint16_t red, uint16_t green, uint16_t blue);

void layer_on(uint16_t layer, uint16_t r, uint16_t g, uint16_t b);

//////////////////////////////////////////////////////////////////////////////////////////

void led_strip_sparkle(uint16_t density, uint16_t speed);

void led_strip_sparkle_two(uint16_t density, uint16_t speed);

void smuth(void);

void rainbow(void);

void eplepsi(void);

void snake(void);

void on(void);




#endif