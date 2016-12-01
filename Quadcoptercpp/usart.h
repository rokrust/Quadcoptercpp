#ifndef __USART_H__
#define __USART_H__

#define BAUD 9600
#define F_CPU 16000000
#define MYUBRR F_CPU/16/BAUD-1

//Error flags
//static int frameError = 0;
//static int dataOverrun = 0;
//static int parityError = 0;

void USART_init(unsigned int ubrr);


void USART_transmit(unsigned char data);
unsigned char USART_receive(void);

#endif