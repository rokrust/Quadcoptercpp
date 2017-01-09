#include "NRF24L01.h"

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#define F_CPU 16000000UL


NRF24L01::NRF24L01(){
	uint8_t data[10];

	DDRB |= (1 << CE) | (1 << RST);
	PORTB |= (1 << CE) | (1 << RST) | (1 << SS);
	
	//Enable datapipe 0
	data[0] = (1 << ERX_P0);
	writeNrf(EN_RXADDR, data, 1);

	//Enable auto ACK
	data[0] = (1 << ENAA_P0);
	writeNrf(EN_AA, data, 1);

	//Set pipe address width
	data[0] = (1 << _5_BYTES);
	writeNrf(SETUP_AW, data, 1);

	//Set pipe 0 5 byte address
	for(int i = 0; i < ADDRESS_WIDTH; i++){
		data[i] = P0_LSB_ADDRESS;
	}
	writeNrf(RX_ADDR_P0, data, ADDRESS_WIDTH);
	writeNrf(TX_ADDR, data, ADDRESS_WIDTH);
	

	//Set Payload width
	data[0] = PAYLOAD_WIDTH;
	writeNrf(RX_PW_P0, data, 1);

	//Set number of retransmits and interval between them
	data[0] = RETRANSMIT_3_250_US;
	writeNrf(SETUP_RETR, data, 1);
	

	//Power up radio controller, enable CRC and empty FIFO'S
	data[0] = (1 << PWR_UP) | (1 << EN_CRC);
	writeNrf(CONFIG, data, 1);

	//Give NRF time to reach standby mode (1.5 ms)
	_delay_ms(10);
}

void NRF24L01::writeNrf(uint8_t command, uint8_t* val, uint8_t nBytes){
		PORTB &= ~(1 << SS);

		spi.transmit(command);

		for(int i = 0; i < nBytes; i++){
			spi.transmit(val[i]);
		}

		PORTB |= (1 << SS);


}

void NRF24L01::readNrf(uint8_t command, uint8_t* val, uint8_t nBytes){
	PORTB &= ~(1 << SS);

	spi.transmit(command);

	for(int i = 0; i < nBytes; i++){
		val[i] = spi.transmit(NOP);
	}

	PORTB |= (1 << SS);
}

void NRF24L01::setRxMode(){
	PORTB &= ~(1 << SS);

	uint8_t configReg = spi.transmit(CONFIG);
	configReg |= (1 << PRIM_RX);
	spi.transmit(configReg);

	PORTB |= (1 << SS);
}

void NRF24L01::setTxMode(){
	PORTB &= ~(1 << SS);
	
	uint8_t configReg = spi.transmit(CONFIG);
	configReg &= ~(1 << PRIM_RX);
	spi.transmit(configReg);

	PORTB |= (1 << SS);
}

void NRF24L01::transmit(uint8_t* data){
	writeNrf(FLUSH_TX, NULL, 0);
	writeNrf(W_TX_PAYLOAD, data, PAYLOAD_WIDTH);
	setTxMode();

	PORTB |= (1 << CE);
	_delay_us(20); //Should probably fix this
	PORTB &= ~(1 << CE);
}

void NRF24L01::listen(){
	writeNrf(FLUSH_RX, NULL, 0);
	setRxMode();

	PORTB |= (1 << CE);
}

void NRF24L01::recieve(uint8_t* val){
	writeNrf(R_RX_PAYLOAD, NULL, 0);
	for(int i = 0; i < PAYLOAD_WIDTH; i++){
		val[i] = spi.transmit(NOP);	
	}

	PORTB &= ~(1 << CE);
}