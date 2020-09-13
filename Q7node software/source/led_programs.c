#include "led_programs.h"
#include "ws2812b.h"

color led_strip[NUMBER_OF_LEDS] = {0};
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

void rainbow(void){
	rainbow_starting_pos += 10;
	rainbow_starting_pos %= NUMBER_OF_LEDS;
	
	color c = (color){0, 0, 0};
	
	for (uint16_t i = 0; i < NUMBER_OF_LEDS/5; i++)
	{
		uint16_t pos_on_rainbow = (i*5 + NUMBER_OF_LEDS - rainbow_starting_pos) % NUMBER_OF_LEDS;
		float hue = (float)pos_on_rainbow / (float)NUMBER_OF_LEDS;
		hslToRgb(hue, 1, 0.5f, &c);
		for (uint16_t j = 0; j < 5; j++) {
			led_strip[i*1 + j] = c;
		}
	}
	update_led_strip(led_strip, NUMBER_OF_LEDS);
	_delay_ms(100);
}
void smuth(void){
	color c;
	for (float hue = 0; hue <= 1; hue = hue + 0.0001) {
		hslToRgb(hue, 1, 0.5f, &c);
		led_strip_on(c.red, c.green, c.blue);
		update_led_strip(led_strip, NUMBER_OF_LEDS);
		_delay_ms(1);
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

void eplepsi(void)
{
	led_strip_on(100,0,0);
	update_led_strip(led_strip, NUMBER_OF_LEDS);
	_delay_ms(80);
	
	led_strip_on(0,100,0);
	update_led_strip(led_strip, NUMBER_OF_LEDS);
	_delay_ms(80);
	
	led_strip_on(0,0,100);
	update_led_strip(led_strip, NUMBER_OF_LEDS);
	_delay_ms(80);

}

void snake(void){
	color color_en = {0, 255, 255};
	color color_to = {255, 0, 255};
	color color_tre = {33, 170, 33};
	color color_fire = {255, 150, 0};

		
	for (int i = 0; i < 64; i++){
		led_strip[i] = (color)color_en;
		if (i == 15){
			i = 19;
			led_strip[16] = (color)color_en;
			led_strip[17] = (color)color_en;
			led_strip[18] = (color)color_en;
			led_strip[19] = (color)color_en;
		}
		
		
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
		led_strip[i] = (color)color_to;
		if (i == 15){
			i = 19;
			led_strip[16] = (color)color_to;
			led_strip[17] = (color)color_to;
			led_strip[18] = (color)color_to;
			led_strip[19] = (color)color_to;
		}
		
		
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
		led_strip[i] = (color)color_tre;
		if (i == 15){
			i = 19;
			led_strip[16] = (color)color_tre;
			led_strip[17] = (color)color_tre;
			led_strip[18] = (color)color_tre;
			led_strip[19] = (color)color_tre;
		}
		
		
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
		led_strip[i] = (color)color_fire;
		if (i == 15){
			i = 19;
			led_strip[16] = (color)color_fire;
			led_strip[17] = (color)color_fire;
			led_strip[18] = (color)color_fire;
			led_strip[19] = (color)color_fire;
		}
		
		
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

void on(void){
				led_strip[54] = (color){0, 255, 255};
				update_led_strip(led_strip, NUMBER_OF_LEDS);
}