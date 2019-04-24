/* microcontroller.h
 * Code to make AVR microcontrollers easier to use...
 * (C) Steve Verhagen 2012
*/

#ifndef MICROCONTROLLER_H
#define MICROCONTROLLER_H

// Mappings:
#ifdef CONFIG_attiny13a
	#define F_CPU 20000000
	#define avr_num_pb 6
	#define avr_num_pd 0
	#define avr_num_pa 0
#endif

#ifdef CONFIG_attiny44
	#define F_CPU 20000000
	#define avr_num_pb 6
	#define avr_num_pd 0
	#define avr_num_pa 6
#endif

// TODO:  modify names of functions to make more intuitive?

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include "adc.h"

// FUNCTION PROTOTYPES:
	//Functions to set register bit values:
	void sbit(volatile uint8_t *portdir, uint8_t pin);
	void cbit(volatile uint8_t *portdir, uint8_t pin);
	void tbit(volatile uint8_t *portdir, uint8_t pin);

	//Functions to set directional registers:
	void set_input(volatile uint8_t *portdir, uint8_t pin); // Sets a specific pin to input mode (default)
	void set_output(volatile uint8_t *portdir, uint8_t pin); // Sets a specific pin to output mode

	//Functions to control output pins:
	void output_high(volatile uint8_t *port, uint8_t pin); // Sets a pin high
	void output_low(volatile uint8_t *port, uint8_t pin); // Sets a pin low

	//Read from a specific input pin:
	char input_pin(volatile uint8_t *port, uint8_t pin);
//END FUNCTION PROTOTYPES

//set a bit=1 on a register or 'set' a bit
void sbit(volatile uint8_t *portdir, uint8_t pin)
{
	*portdir |= (1 << pin);
}

//set a bit=0 on a register or'clear' a bit
void cbit(volatile uint8_t *portdir, uint8_t pin)
{
	*portdir &= ~(1<<pin);
}

//toggle a bit on a register
void tbit(volatile uint8_t *portdir, uint8_t pin)
{
	*portdir ^= (1<<pin); 
}


void set_input(volatile uint8_t *portdir, uint8_t pin)
{
	*portdir &= ~(1<<pin);
}

void set_output(volatile uint8_t *portdir, uint8_t pin)
{
	*portdir |= (1<<pin);
}

void output_high(volatile uint8_t *port, uint8_t pin)
{
	*port |= (1<<pin);
}

void output_low(volatile uint8_t *port, uint8_t pin)
{
	*port &= ~(1<<pin);
}

char input_pin(volatile uint8_t *port, uint8_t pin)
{
	return ~*port & (1<<pin); 
}

char input_button(volatile uint8_t *port, uint8_t pin)
{
	char a = input_pin(port, pin);
	if ( a == 1)
	{
		while ( (input_pin(port, pin)) == 1 );
	}
	return a;
}

#endif
