#include "timer16.h"
#include "adc.h"
#include "MPU6050.h"

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

Timer16::Timer16(){
	//Set CTC mode
	TCCR1A &= (1 << WGM11) | (1 << WGM10);
	TCCR1B |= (1 << WGM12);
	TCCR1B &= ~(1 << WGM13);
	
	//Enable overflow and output compare interrupts
	TIMSK1 |= (1 << OCIE0A) | (1 << TOIE1);
	
	OCR1A = 0xffff;
	TCNT1 = 0;
	
	sei();
}

Timer16::Timer16(uint16_t outputCmp){
	//Set CTC mode
	TCCR1A &= (1 << WGM11) | (1 << WGM10);
	TCCR1B |= (1 << WGM12);
	TCCR1B &= ~(1 << WGM13);
	
	//Enable overflow and output compare interrupts
	TIMSK1 |= (1 << OCIE0A) | (1 << TOIE1);
	
	this->outputCmp = outputCmp;
	OCR1A = outputCmp;
	TCNT1 = 0;
	
	sei();
}

void enable(){
	TCCR1B |= (1 << CS12) | (1 << CS10);
	TCCR1B &= ~(1 << CS11);
}

void disable(){
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

ISR(TIMER1_COMPA_vect){
	
	
}