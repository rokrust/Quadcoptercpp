﻿#include "spi.h"
#include "usart.h"

#include <avr/io.h>
#include <stdio.h>

Spi::Spi()
{
	/* Set MOSI and SCK output, all others input */
	DDRB = (1<<PORTB3)|(1<<PORTB5)| (1 << PORTB2);
	/* Enable SPI, Master, set clock rate fck/16 */
	/* Sample at rising edge					 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

char Spi::transmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));

	return SPDR;
}

void Spi::error_status(){
	if(SPSR & (1 << WCOL)){//Data collision detected
		printf("ERROR: Data collision detected\n");
	}
}