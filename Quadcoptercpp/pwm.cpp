#include "pwm.h"
#include "adc.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

void Pwm::initializePWM0A(){
	DDRD |= (1 << PORTD6);

	//Clear compare match (non-inverted mode)
	TCCR0A |= (1 << COM0A1);
	TCCR0A &= ~(1 << COM0A0);

	//Set to fast-PWM mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	TCCR0B &= ~(1 << WGM02);
}

void Pwm::initializePWM0B(){
	DDRD |= (1 << PORTD5);

	//Clear compare match (non-inverted mode)
	TCCR0A |= (1 << COM0B1);
	TCCR0A &= ~(1 << COM0B0);

	//Set to fast-PWM mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	TCCR0B &= ~(1 << WGM02);
}

void Pwm::initializePWM2A(){
	DDRB |= (1 << PORTB3);

	//Clear compare match (non-inverted mode)
	TCCR2A |= (1 << COM2A1);
	TCCR2A &= ~(1 << COM2A0);

	//Set to fast-PWM mode
	TCCR2A |= (1 << WGM21) | (1 << WGM20);
	TCCR2B &= ~(1 << WGM22);
}

void Pwm::initializeTimer2A(){
	//Change timer to normal mode
	TCCR2A &= ~( (1 << WGM21) | (1 << WGM20) );
	TCCR2B &= ~(1 << WGM22);

	//Enable output compare interrupt
	TIMSK2 |= (1 << OCIE2A) | (1 << TOIE2);

	//Set to output
	DDRD |= (1 << PORTD4);

}

void Pwm::initializePWM2B(){
	DDRD |= (1 << PORTD3);

	//Clear compare match (non-inverted mode)
	TCCR2A |= (1 << COM2B1);
	TCCR2A &= ~(1 << COM2B0);

	//Set to fast-PWM mode
	TCCR2A |= (1 << WGM21) | (1 << WGM20);
	TCCR2B &= ~(1 << WGM22);
}


//Timer 2 overwrites the MOSI pin and can therefore not be used with SPI.
Pwm::Pwm(int timerNumber): timerNumber(timerNumber){
	switch(timerNumber){
		case 0:
			initializePWM0A();
			outputCmpRegister = &(OCR0A);
			break;
		
		case 1:
			initializePWM0B();
			outputCmpRegister = &(OCR0B);
			break;
		
		case 2:
			initializePWM2A();
			outputCmpRegister = &(OCR2A);
			
			//If SPI is used, change to interrupt driven timer
			//and implement PWM in ISR
			if(PORTB & (1 << PORTB3)){
				initializeTimer2A();
			}

			break;

		case 3:
			initializePWM2B();
			outputCmpRegister = &(OCR2B);
			break;

	}
}


Pwm::Pwm(int timerNumber, uint8_t outputCmp): Pwm(timerNumber){
	setCompare(outputCmp);
}

void Pwm::setCompare(uint8_t cmp){
	*outputCmpRegister = cmp;
}

void Pwm::enable(){
	//Enable timer 0 with 256 prescaler
	if(timerNumber == 0 || timerNumber == 1){
		TCCR0B |= (1 << CS02);
		TCCR0B &= ~((1 < CS01) | (1 << CS00));
	}

	//Enable timer 2 with 256 prescaler
	if(timerNumber == 2 || timerNumber == 3){
		TCCR2B |= (1 << CS22) | (1 << CS21);
		TCCR2B &= ~(1 << CS20);

		//In case SPI is used
		if( timerNumber == 2 && (PORTB & (1 << PORTB3)) ){
			PORTD |= (1 << PORTD4);
		}
	}

}

void Pwm::disable(){
	if(timerNumber == 0 || timerNumber == 1){
		TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
	}

	if(timerNumber == 2 || timerNumber == 3){
		TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));

		//In case SPI is used
		if(timerNumber == 2 && PORTB & (1 << PORTB3)){
			PORTD &= ~(1 << PORTD4);
		}
	}

}

//Emulate PWM
//Hopefully this won't take up too much of the CPU.
ISR(TIMER2_COMPA_vect){
	PORTD &= ~(1 << PORTD4);
}

ISR(TIMER2_OVF_vect){
	PORTD |= (1 << PORTD4);
}