#include "usart.h"
#include <avr/io.h>

void USART_init(unsigned int ubrr){
	//Set baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	//Enable receiver and transmitter
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
	
	//Set frame format: 8data, 1stop bit
	UCSR0C |= (3<<UCSZ00);
}

void USART_transmit(unsigned char data){
	/*Wait for empty transmit buffer*/
	while( !(UCSR0A & (1<<UDRE0)) );
	
	/*Put data into buffer, sends the data*/
	UDR0 = data;
}

unsigned char USART_receive(void){
	
	/*Wait for data to be received*/
	while( !(UCSR0A & (1<<RXC0)) );
	
	
	/*Get and return received data from buffer*/
	return UDR0;
}

void USART_transmit_string(const char* s){
	for(int i = 0; s[i] != '\0'; i++){
		USART_transmit(s[i]);
	}
	USART_transmit('\0');
}

unsigned char* USART_recieve_string(unsigned char* s){
	
	for(int i = 0; s[i] != '\0'; i++){
		s[i] = USART_receive();
	}

	return s;
}