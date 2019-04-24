#include "microcontroller.h"

#define led1 PB0

#define motor_right PB2
#define motor_left PA7

uint16_t adc_left_val;
uint16_t adc_left_chan = 0;
uint16_t adc_right_val;
uint16_t adc_right_chan = 1;
uint8_t adc_tolerance = 10;//20;

int direction;
#define FORWARD 1
#define LEFT 2
#define RIGHT 3

void init_leds(void)
{
	set_output(&DDRB, led1);
	//output_high(&PORTB, led1);
	//_delay_ms(1);
	output_low(&PORTB, led1);
	//_delay_ms(10);
}

void init_motors(void)
{
	set_output(&DDRB, motor_right);
	output_low(&PORTB, motor_right);
	
	set_output(&DDRA, motor_left);
	output_low(&PORTA, motor_left);
}

init_adc(void)
{
	// set up prescaler
	//ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 

	// use vcc as analog reference
	ADMUX |= (0 << REFS1) | (0 << REFS0); 

	// left-align data
	//ADMUX |= (1 << ADLAR);

	// enable adc
	ADCSRA |= (1 << ADEN);

}

int main(void)
{
	init_leds();
	init_motors();
	init_adc();

	// for make-shift pwm
	//int pwmval = 0;

	while (1)
	{
		direction = FORWARD;

		// read sensor values:
		adc_left_val = adc_read(adc_left_chan);
		adc_right_val = adc_read(adc_right_chan);

		// sensor logic:
		if( adc_left_val < ( adc_right_val - adc_tolerance ) ) {
			direction = LEFT;
		}
		
		if( adc_left_val > ( adc_right_val + adc_tolerance) ) {
			direction = RIGHT;
		}

		if ( direction == FORWARD )
		{ 
			output_high(&PORTB, motor_right);
			output_high(&PORTA, motor_left);
		}

		if ( direction == LEFT )
		{ 
			output_high(&PORTB, motor_right);
			output_low(&PORTA, motor_left);
		}
		
		if ( direction == RIGHT )
		{ 
			output_low(&PORTB, motor_right);
			output_high(&PORTA, motor_left);
		}
	}	

}
