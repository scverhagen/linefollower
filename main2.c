// ATtiny44 
// Two channel (PA4 and PA5) ADC conversion 

#include <avr/io.h> 
#include <avr/interrupt.h> 

uint8_t result, active_channel; 

void init(void){ 

   ADMUX |= (1 << MUX2); 
   ADCSRB |= (1 << ADLAR); 
   ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADEN) | (1 << ADSC) | (1 << ADIE); 
   DDRA &= ~(1 << PA4) & ~(1 << PA5); 
   sei();                           //enable interrupt 
} 

ISR(ADC_vect){ 
      result = ADCH; 
      if (active_channel == 0){ 
 
         /* CODE HERE */ 

         active_channel=1; 
         ADMUX |= (1 << MUX0); 
      } 
      else if (active_channel == 1){ 

         /* CODE HERE */          

         active_channel=0; 
         ADMUX &= ~(1 << MUX0); 
      } 
      ADCSRA |= (1 << ADSC); 
} 

int main(void){ 
   init(); 
   active_channel=0; 

   for(;;){ 
   } 

} 
