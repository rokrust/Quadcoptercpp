#include "NRF24L01.h"

#include <avr/io.h>
#include <stdio.h>

NRF24L01::NRF24L01(){
	DDRB |= (1 << COMMAND_PIN) | (1 << RST);
	PORTB |= (1 << COMMAND_PIN) | (1 << RST) | (1 << SS);

	//Power up radio controller
	transmitCommand(CONFIG);
	transmitData((1 << PWR_UP));
	
}

void NRF24L01::transmitCommand(char cData){
	PORTB &= ~(1 << COMMAND_PIN);
	transmitData(cData);
	PORTB |= (1 << COMMAND_PIN);
}

void NRF24L01::transmitData(char cData){
	PORTB &= ~(1 << SS);
	spi.transmit(cData);
	PORTB |= (1 << SS);
}
