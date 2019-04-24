// ATtiny44 
// Two channel (PA4 and PA5) ADC conversion 

#include "microcontroller.h"
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h> 

/*ref:
number &= ~(1 << x);     //clear a bit
number |= 1 << x;        //set a bit
number ^= 1 << x;        //toggle a bit
bit = number & (1 << x); //get a bit*/

int main(void)
{ 
	
	char val;

	InitADC();
	set_output(&DDRB, PB1);
	while (1) 
	{
		val = Adc_Read();
		if (val > 150) {
			output_high(&PORTB, PB1);
		} else {
			output_low(&PORTB, PB1);
		}

		_delay_ms(25);
		//output_low(&PORTB, PB1);
		_delay_ms(1000);
	}
	
} 
