#include "pwm.h"
#include "adc.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

Pwm::Pwm(){
	//Activates all 8 bit timers. Can not be used with SPI
	//Will be called every time a pwm-object is made but that's okay.
	DDRD |= (1 << PORTD3) | (1 << PORTD5) | (1 << PORTD6);
	DDRB |= (1 << PORTB3);

	//Timer 0
	//Clear compare match (non-inverted mode)
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
	TCCR0A &= ~((1 << COM0A0) | (1 << COM0B0));

	//Set to fast-PWM mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	TCCR0B &= ~(1 << WGM02);
	
	//Timer 2
	//Clear compare match (non-inverted mode)
	TCCR2A |= (1 << COM2A1) | (1 << COM2B1);
	TCCR2A &= ~((1 << COM2A0) | (1 << COM2B0));

	//Set to fast-PWM mode
	TCCR2A |= (1 << WGM21) | (1 << WGM20);
	TCCR2B &= ~(1 << WGM22);

	setOutputCompareRegister(0);
}


Pwm::Pwm(uint8_t outputCmp, int timerNumber): Pwm(){
	setOutputCompareRegister(timerNumber);
	setCompare(outputCmp);
}

void Pwm::setCompare(uint8_t cmp){
	*outputCmpRegister = cmp;
}

void Pwm::enable(){
	//Enable timer 0 with 256 prescaler
	TCCR0B |= (1 << CS02);
	TCCR0B &= ~((1 < CS01) | (1 << CS00));

	//Enable timer 2 with 256 prescaler
	TCCR2B |= (1 << CS22) | (1 << CS21);
	TCCR2B &= ~(1 << CS20);


}

void Pwm::disable(){
	TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
	TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
}

void Pwm::setOutputCompareRegister(int timerNumber){
	//Should work. If not cast (volatile char*)registerAddress
	switch(timerNumber){
		case 0:
		outputCmpRegister = &(OCR0A);
		break;

		case 1:
		outputCmpRegister = &(OCR0B);
		break;

		case 2:
		outputCmpRegister = &(OCR2A);
		break;

		case 3:
		outputCmpRegister = &(OCR2B);
		break;
	}
}
