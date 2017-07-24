#include "timer16.h"
#include "config.h"

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t prescalerValues[6] = {0, 1, 8, 64, 256, 1024};


Timer16::Timer16(){
	//Set CTC mode
	TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
	TCCR1B |= (1 << WGM12);
	TCCR1B &= ~(1 << WGM13);
	
	//Disable output compare pins
	TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0) | (COM1B1) | (COM1B0));
	
	OCR1A = 0xffff;
	TCNT1 = 0;
	
	//No prescaler by default
	prescalerIndex = 1;
}


Timer16::Timer16(uint16_t timerFreq) : Timer16(){
	setToInterruptMode(timerFreq);
}

void Timer16::enable(){
	TCCR1B |= prescalerIndex;

	TCNT1 = 0;
}

void Timer16::disable(){
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

void Timer16::setFrequency(uint16_t timerFreq){

	//Value is possibly too large for the timer's counter register
	uint32_t outputCmp = F_CPU/timerFreq;
	
	//Increase prescaler until overflow is gone or max prescaler is reached
	while(outputCmp / prescalerValues[prescalerIndex] > 0xffff && prescalerIndex < 6){
		prescalerIndex++;
	}
	
	outputCmp /= prescalerValues[prescalerIndex];
	OCR1A = outputCmp;
}

void Timer16::setToInterruptMode(uint16_t timerFreq){
	//Enable overflow and output compare interrupts
	TIMSK1 |= (1 << OCIE0A) | (1 << TOIE1);

	setFrequency(timerFreq);
}

void Timer16::setToTickMode(){
	TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
	TCCR1B &= ~((1 << WGM13) | (1 << WGM12));

	TIMSK1 &= ~((1 << OCIE0A) | (1 << TOIE1));

	prescalerIndex = 1;
}

uint32_t Timer16::currentTime(){
	uint16_t currentCount = TCNT1;

	return (F_CPU / prescalerValues[prescalerIndex]) / currentCount;
}