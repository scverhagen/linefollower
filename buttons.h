#ifndef BUTTONS_H
#define BUTTONS_H

#include <avr/io.h>
#include "microcontroller.h"

uint8_t old_button_states;

void initbuttons(void)
{
	old_button_states = PIND;
}

uint8_t read_button_down(pin)
{
	//detect whether this is a change or just repetition
	if ( (input_pin(&old_button_states, pin) == 0)  && (input_pin(&PIND, pin) == 1) )
	{
		//set old_button_states
		old_button_states = PIND;
		
		//button_up event is true
		return 1;
		
	} else {
		//set old_button_states
		old_button_states = PIND;
		
		//button_up event is false
		return 0;
	}
}

uint8_t read_button_up(pin)
{
	//detect whether this is a change or just repetition
	if ( (input_pin(&old_button_states, pin) == 1)  && (input_pin(&PIND, pin) == 0) )
	{
		//set old_button_states
		old_button_states = PIND;
		
		//button_up event is true
		return 1;
		
	} else {
		//set old_button_states
		old_button_states = PIND;
		
		//button_up event is false
		return 0;
	}
}

uint8_t read_button_pressed(pin)
{
	return input_pin(&PIND, pin);
}	

#endif
