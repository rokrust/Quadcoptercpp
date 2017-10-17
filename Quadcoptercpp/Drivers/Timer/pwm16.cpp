#include "pwm16.h"

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//The top value of this 16bit can be changed to yield different frequencies
//(Good for speakers hopefully)
Pwm16::Pwm16(uint16_t topVal, uint16_t outputCmp): topVal(topVal), outputCmp(outputCmp){
	DDRB |= 0x7;

	//Set fast pwm-mode
	TCCR1A &= ~(1 << WGM10);
	TCCR1A |=  (1 << WGM11);
	TCCR1B |=  (1 << WGM13) | (1 << WGM12);

	//Clear PWM pin on compare match
	TCCR1A &= ~(1 << COM1A0);
	TCCR1A |=  (1 << COM1A1);

	ICR1 = topVal;
	OCR1A = outputCmp; //Compare match value
	TCNT1 = 0; //Counter register
}

Pwm16::Pwm16(): Pwm16(0xffff, 0xffff){
	
}


void Pwm16::enable(){
	TCCR1B |= (1 << CS12);
	TCCR1B &= ~((1 << CS11) | (1 << CS10));
}

void Pwm16::disable(){
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

/*
ISR(TIMER1_COMPA_vect){
	
	
}
*/