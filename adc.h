#include <avr/io.h>
#define vRef 1.1

/*uint8_t Adc_Read(){ 
    
   int i;
   uint8_t low_bit=0; 
   //ADMUX=(ADMUX & 0xf0)|adc_input; 
   _delay_us(10); 
    
ADCSRA|=(1<<ADSC);          
while(!(ADCSRA & (1<<ADIF)));    
low_bit=ADC;
   return(low_bit);    
} */

uint16_t adc_read(uint8_t ch)
{
  // select the corresponding channel 0~7
  // ANDing with ’7′ will always keep the value
  // of ‘ch’ between 0 and 7
  ch &= 0b00000111;  // AND operation with 7
  ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
 
	// start adc conversion
	ADCSRA |= (1 << ADSC);
 
	// wait for conversion to complete
	while(ADCSRA & (1<<ADSC));
 
  return (ADC);
}


void InitADC()
{
	//PRR &= ~(1 << PRADC); //Clear the PRADC in PRR to disable the power reduction register

	//ADCSRA = (1<<ADPS2) | (1<<ADPS1); // prescaler of 64 = 8MHz/64 = 125KHz. 
	//ADMUX |= (1 << ADLAR); // Left-align ADC value
	ADMUX=(1<<REFS0); // For Aref=AVcc;

	ADCSRA |= (0x01 << ADEN); //Turn on ADC
	

	//ADCSRA |= (1 << ADFR);  // Set ADC to Free-Running Mode
}

/*void StopADC()
{
	ADCSRA &= ~(0x01 << ADEN);
}
*/

char read_channel(char channel)			// Function for Reading ADC input
{
 
	ADCH=0x00;
	ADMUX=0x00;
	ADMUX |= 0x60 | channel; 				//Select channel
	ADCSRA |= (1<<ADSC);					//Start conversion by setting ADSC bit
	while( ADCSRA & (1<<ADSC) );			//Wait for conversion to end
	return (ADCH);							//return conversion result	
}

/*uint16_t ReadADC()
uint16_t ReadADC(uint8_t ch)
{
	//Select ADC Channel ch must be 0-7
	ch=ch&0b00000111;
	ADMUX|=ch;
	//sbit(ADMUX, MUX0);

	// Start conversion by setting bit ADSC or ADCSRA to 1:
	ADCSRA |= (0x01 << ADSC);
	
	// Wait for ADC to finish (ADSC will equal zero):
	while ( (ADCSRA & (0x01 << ADSC)) > 0)
	{ }
	
	return ADC;
}
*/
/* Old Code
 * 
void InitADC()
{
	ADSCRA |= (0x01 << ADEN)
	ADMUX=(1<<REFS0);                         // For Aref=AVcc;
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Rrescalar div factor =128
}

uint16_t ReadADC(uint8_t ch)
{
   //Select ADC Channel ch must be 0-7
   ch=ch&0b00000111;
   ADMUX|=ch;

   //Start Single conversion
   ADCSRA|=(1<<ADSC);

   //Wait for conversion to complete
   while(!(ADCSRA & (1<<ADIF)));

   //Clear ADIF by writing one to it
   //Note you may be wondering why we have write one to clear it
   //This is standard way of clearing bits in io as said in datasheets.
   //The code writes '1' but it result in setting bit to '0' !!!

   ADCSRA|=(1<<ADIF);

   return(ADC);
}
*/
