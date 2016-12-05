#include "pwm.h"
#include "adc.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

Pwm::Pwm(){
	DDRD |= (1 << PORTD6);
	
	//Output compare value
	OCR0A = 0xff;

	//Clear OC0A on compare (non-inverted mode)
	TCCR0A |= (1 << COM0A0);
	TCCR0A &= ~(1 << COM0A1);

	//Set to CTC mode (clear timer on compare)
	TCCR0A &= ~(1 << WGM00);
	TCCR0A |= (1 << WGM01);
	TCCR0B &= ~(1 << WGM02);

}


Pwm::Pwm(uint8_t outputCmp): Pwm(){
	setCompare(outputCmp);
}

void Pwm::setCompare(uint8_t cmp){
	OCR0A = cmp;
}

void Pwm::enable(){
	//Enable with 256 prescaler
	TCCR0B |= (1 << CS02) | (1 << CS00);
	TCCR0B &= ~(1 < CS01);

}

void Pwm::disable(){
	TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
}
