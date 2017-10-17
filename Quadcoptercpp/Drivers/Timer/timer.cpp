#include "timer.h"
#include "Drivers/Sensors/adc.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

Timer::Timer(){
	//Set CTC mode
	TCCR0A |= (1 << WGM01);
	TCCR0A &= ~(1 << WGM00);
	TCCR0B &= ~(1 << WGM02);
	
	TIMSK0 |= (1 << OCIE0A) | (1 << TOIE0);
	
	OCR0A = 0xff; //compare value: timer frequency
	TCNT0 = 0; //Initialize counter to zero
	
}

Timer::Timer(uint8_t outputCmp){
	//Set CTC mode
	TCCR0A |= (1 << WGM01);
	TCCR0A &= ~(1 << WGM00);
	TCCR0B &= ~(1 << WGM02);
	
	TIMSK0 |= (1 << OCIE0A) | (1 << TOIE0);
	
	this->outputCmp = outputCmp;
	OCR0A = outputCmp; //compare value: timer frequency
	TCNT0 = 0; //Initialize counter to zero
	
}

void Timer::enable(){
	TCCR0B |= (1 << CS02) | (1 << CS00); //Set prescaler to 1024
	TCCR0B &= ~(1 << CS01);
}

void Timer::disable(){
	TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
}

ISR(TIMER0_COMPA_vect){
	printf("Hallo\n");
}