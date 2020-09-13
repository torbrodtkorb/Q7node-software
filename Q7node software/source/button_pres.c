#include "button_pres.h"

volatile uint8_t button_pressed_before = 1;
volatile uint8_t new_button_press = 0;

void button_press_config(void)
{
	
	// Set port A4 to input
	PORTA.DIRCLR = (1 << 4);
	
	// Enable pull up
	PORTA.PIN4CTRL |= (1 << PORT_PULLUPEN_bp);

}

void button_press_detect(void)
{
	if (!(PORTA.IN & (1 << 4)))
	{
		_delay_ms(35);
		
		if ((!(PORTA.IN & (1 << 4))) && (button_pressed_before == 1))
		{
			PORTB.OUTTGL = (1 << 4);
			new_button_press = 1;
			button_pressed_before = 0;
		}
	}
	else
	{
	button_pressed_before = 1;
	//new_button_press = 0;

	}
}