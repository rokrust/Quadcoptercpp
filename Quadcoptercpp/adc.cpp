#include "adc.h"
#include <avr/io.h>

Adc::Adc(){
	DDRC &= ~(1 << DDC0); //Might not be needed
	
	//Set referance to AVcc
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
	ADMUX &= ~(1 << REFS1);
	
	ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	
	//ADCSRB//
	//Set timer_A compare to trigger
	ADCSRB |= (1 << ADTS1) | (1 << ADTS0);
	ADCSRB &= ~(1 << ADTS2);
}

char Adc::read(char ch){
	//Start conversion
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC)){;}
	
	//Read from channel ch
	ADMUX |= ch;
	char adc_value = ADCH;
	ADMUX ^= ch;

	
	return adc_value;
}