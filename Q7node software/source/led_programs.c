#include "led_programs.h"
#include "ws2812b.h"


color led_strip[NUMBER_OF_LEDS] = {0};
color list[65];


#define RB_CNT 64
static color rb_strip[RB_CNT];

static void pixelcopy(const color* src, color* dest);

	
int i = 0;
uint16_t rainbow_starting_pos = 0;

void led_strip_on(uint16_t red, uint16_t green, uint16_t blue)
{
	for (uint16_t i = 0; i < NUMBER_OF_LEDS; i++)
	{
		led_strip[i] = (color){red, green, blue};
	}
	update_led_strip(led_strip, NUMBER_OF_LEDS);
}

 void update_virituell_led_strip(void){
	
	for (int i = 0; i < 64; i++){
		pixelcopy(rb_strip + i, led_strip + i);
		if (i >= 20 && i <= 24)
			pixelcopy(rb_strip + i, led_strip + (114 - i));
		
		if (i >= 41 && i <= 54) {
			pixelcopy(rb_strip + i, led_strip + (130 - i));
			if (i == 54) {
				pixelcopy(rb_strip + i, led_strip + 74);
				pixelcopy(rb_strip + i, led_strip + 75);
				pixelcopy(rb_strip + i, led_strip + 76);
			}
		}
		if (i > 54) {
			pixelcopy(rb_strip + i, led_strip + (127 - i));
		}
	}
	update_led_strip(led_strip, NUMBER_OF_LEDS);
}


float hue2rgb(float p, float q, float t) {
	if(t < 0) t += 1;
	if(t > 1) t -= 1;
	if(t < 1/6.0) return p + (q - p) * 6 * t;
	if(t < 1/2.0) return q;
	if(t < 2/3.0) return p + (q - p) * (2/3.0 - t) * 6;
	return p;
}

void hslToRgb(float h, float s, float l, color *rgb) {
	float r, g, b;

	if (s == 0) {
		r = g = b = l; // achromatic
		} else {
		float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
		float p = 2 * l - q;
		r = hue2rgb(p, q, h + 1/3.0);
		g = hue2rgb(p, q, h);
		b = hue2rgb(p, q, h - 1/3.0);
	}

	rgb->red = r * 255;
	rgb->green = g * 255;
	rgb->blue = b * 255;
}

/****************************************************/

static void pixelcopy(const color* src, color* dest)
{
	dest->blue = src->blue;
	dest->green = src->green;
	dest->red = src->red;
}

void rb_extend(char base, color* data, char size)
{
	if (base >= size) {
		return;
	}
	
	color* src = data;
	color* dest = data + base;
	while (size--) {
		pixelcopy(src, dest);
		src++;
		dest++;
	}
}


#define REPEAT 32

void rb(const uint8_t* exit, const uint8_t* off, float inc)
{

	while (1) {
		for (float s = 0; s < 1; s += inc) {
			
			for (char i = 0; i < REPEAT; i++) {
				/* Get color from wheel */
				
				float hue = s + (float)i/(float)REPEAT;
				if (hue > 1.0) {
					hue -= 1.0;
				}
				color tmp;
				hslToRgb(hue, 1, 0.195f, &tmp);
				pixelcopy(&tmp, rb_strip + i);	
				
				if (*exit) return;
				if (*off == 1) return;
			}
			rb_extend(REPEAT, rb_strip, RB_CNT);
			
			update_virituell_led_strip();
		}
	}
}

/*******************************************************/

void rainbow(void){
	for (float hue = 0; hue <= 1; hue = hue + 0.05) {

		for (int i = 0; i < 16; i++){
			hslToRgb(hue, 1, 0.5, &list[i]);
			hue +=(float)1.0/16.0;
			if (hue >= 1.0){
				hue -= 1.0;
			}
		}

		color* dest = list + 16;
		color* src = list;
		
		while (dest != &list[64]){
			dest->red = src->red;
			dest->green = src->green;
			dest->blue = src->blue;
			
			dest++;
			src++;
		}
		PORTC.OUTTGL = (0b111 << 0);
		//update_virituell_led_strip();
	}
}


void smuth(const uint8_t* exit, const uint8_t* off){
	color c;
	for (float hue = 0; hue <= 1; hue = hue + 0.0001) {
		hslToRgb(hue, 1, 0.5f, &c);
		led_strip_on(c.red, c.green, c.blue);
		update_led_strip(led_strip, NUMBER_OF_LEDS);
		_delay_ms(1);
		if (*exit == 1) return;
		if (*off == 1) return;
	}
}

void led_strip_sparkle(uint16_t density, uint16_t speed)
{

	for (uint16_t i = 0; i < density; i++)
	{
		led_strip[rand() % NUMBER_OF_LEDS] = (color){ 100, 0, 80 };
	}
	
	for (int i = 0; i < NUMBER_OF_LEDS; i ++)
	{
		int16_t tmp = led_strip[i].red;
		tmp -= speed;
		
		if (tmp < 0)
		{
			tmp = 0;
		}
		led_strip[i].red = tmp;
		led_strip[i].blue = tmp;

	}
	update_led_strip(led_strip, NUMBER_OF_LEDS);
	_delay_ms(5);
}

void led_strip_sparkle_two(uint16_t density, uint16_t speed)
{

	for (uint16_t i = 0; i < density; i++)
	{
		led_strip[rand() % NUMBER_OF_LEDS] = (color){ 150, 150, 0 };
		led_strip[rand() % NUMBER_OF_LEDS] = (color){ 150, 0, 0 };
		led_strip[rand() % NUMBER_OF_LEDS] = (color){ 150, 0, 0 };
	}
	
	for (int i = 0; i < NUMBER_OF_LEDS; i ++)
	{
		int16_t tmp_red = led_strip[i].red;
		tmp_red -= speed;
		
		if (tmp_red < 0)
		{
			tmp_red = 0;
		}
		
		
		int16_t tmp_green = led_strip[i].green;
		tmp_green -= speed;
		
		if (tmp_green < 0)
		{
			tmp_green = 0;
		}
		
		
		int16_t tmp_blue = led_strip[i].blue;
		tmp_blue -= speed;
		
		if (tmp_blue < 0)
		{
			tmp_blue = 0;
		}
		
		
		led_strip[i].red = tmp_red;
		led_strip[i].blue = tmp_blue;
		led_strip[i].green = tmp_green;

	}
	update_led_strip(led_strip, NUMBER_OF_LEDS);
	_delay_ms(50);
}

void snake(const uint8_t* exit, const uint8_t* off){
	color color_en = {0, 255, 255};
	color color_to = {255, 0, 255};
	color color_tre = {33, 170, 33};
	color color_fire = {255, 150, 0};

	for (int i = 0; i < 64; i++){
		if (*exit == 1) return;
		if (*off == 1) return;
		led_strip[i] = (color)color_en;
		
		if (i > 19 && i < 25){
			led_strip[114-i] = (color)color_en;
		}
		
		if (i > 40 && i < 54){
			led_strip[130-i] = (color)color_en;
		}
		if (i > 53 && i < 64){
			led_strip[127-i] = (color)color_en;
		}
		if (i == 54){
			led_strip[74] = (color)color_en;
			led_strip[75] = (color)color_en;
			led_strip[76] = (color)color_en;
		}

		update_led_strip(led_strip, NUMBER_OF_LEDS);
		_delay_ms(50);	
	}
	
	
	
	for (int i = 0; i < 64; i++){
		if (*exit == 1) return;
		if (*off == 1) return;
		led_strip[i] = (color)color_to;
		
		if (i > 19 && i < 25){
			led_strip[114-i] = (color)color_to;
		}
		
		if (i > 40 && i < 54){
			led_strip[130-i] = (color)color_to;
		}
		if (i > 53 && i < 64){
			led_strip[127-i] = (color)color_to;
		}
		if (i == 54){
			led_strip[74] = (color)color_to;
			led_strip[75] = (color)color_to;
			led_strip[76] = (color)color_to;
		}

		update_led_strip(led_strip, NUMBER_OF_LEDS);
		_delay_ms(50);
	}
	
	
	
	for (int i = 0; i < 64; i++){
		if (*exit == 1) return;
		if (*off == 1) return;
		led_strip[i] = (color)color_tre;
		
		if (i > 19 && i < 25){
			led_strip[114-i] = (color)color_tre;
		}
		
		if (i > 40 && i < 54){
			led_strip[130-i] = (color)color_tre;
		}
		if (i > 53 && i < 64){
			led_strip[127-i] = (color)color_tre;
		}
		if (i == 54){
			led_strip[74] = (color)color_tre;
			led_strip[75] = (color)color_tre;
			led_strip[76] = (color)color_tre;
		}

		update_led_strip(led_strip, NUMBER_OF_LEDS);
		_delay_ms(50);
	}
	
	
	for (int i = 0; i < 64; i++){
		if (*exit == 1) return;
		if (*off == 1) return;
		led_strip[i] = (color)color_fire;
		
		if (i > 19 && i < 25){
			led_strip[114-i] = (color)color_fire;
		}
		
		if (i > 40 && i < 54){
			led_strip[130-i] = (color)color_fire;
		}
		if (i > 53 && i < 64){
			led_strip[127-i] = (color)color_fire;
		}
		if (i == 54){
			led_strip[74] = (color)color_fire;
			led_strip[75] = (color)color_fire;
			led_strip[76] = (color)color_fire;
		}

		update_led_strip(led_strip, NUMBER_OF_LEDS);
		_delay_ms(50);
	}
	
	
	
}