#include "NRF24L01.h"

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>


//Change to active mode only happens if CE is set high
//RX: PWR_UP 1, PRIM_RX 1, CE high
//TX: PWR_UP 1, PRIM_RX 0, payload i TX FIFO, CE high

NRF24L01::NRF24L01(){
	_delay_ms(100);

	//Stores data to be written to registers
	uint8_t data[10];

	DDRB |= (1 << CE);
	PORTB |= (1 << SS); //Deactivate SPI bus
	PORTB &= ~(1 << CE); //Set in standby/pwr_dwn mode
	
	//Enable external interrupt pin INT0
	EIMSK |= (1 << INT0);
		
	//Enable auto ACK
	data[0] = (1 << ENAA_P0);
	writeNrf(EN_AA, data);

	//Enable datapipe 0
	data[0] = (1 << ERX_P0);
	writeNrf(EN_RXADDR, data);

	//Set pipe address width
	data[0] = _5_BYTES;
	writeNrf(SETUP_AW, data);

	//Set pipe 0 address
	for(int i = 0; i < ADDRESS_WIDTH; i++){
		data[i] = P0_LSB_ADDRESS;
	}
	writeNrf(RX_ADDR_P0, data, ADDRESS_WIDTH);
	writeNrf(TX_ADDR, data, ADDRESS_WIDTH);
	
	//Set Payload width
	data[0] = PAYLOAD_WIDTH;//PAYLOAD_WIDTH;
	writeNrf(RX_PW_P0, data);

	//Set number of retransmits and interval between them
	data[0] = RETRANSMIT_3_250_US;
	writeNrf(SETUP_RETR, data);
	
	//Power up radio controller, enable CRC and disable TX and MAX_RT interrupts
	//(leave RX interrupt active)
	data[0] = (1 << PWR_UP) | (1 << EN_CRC) | (1 << PRIM_RX) | 
			  (1 << MASK_TX_DS) | (1 << MASK_MAX_RT);
	writeNrf(CONFIG, data);

	//Give NRF time to reach standby mode (1.5 ms)
	_delay_ms(10);
}

//The write flag is added to the command address during write operations
//unlike the read operations.
void NRF24L01::writeNrf(uint8_t command, uint8_t* val, uint8_t nBytes){
	PORTB &= ~(1 << SS);
	_delay_us(10);
	
	spi.transmit(command + NRF_WRITE_FLAG);
	_delay_us(10);

	for(int i = 0; i < nBytes; i++){
		spi.transmit(val[i]);
		_delay_us(10);
	}
	
	PORTB |= (1 << SS);
}

void NRF24L01::readNrf(uint8_t command, uint8_t* val, uint8_t nBytes){
	PORTB &= ~(1 << SS);
	_delay_us(10);
	
	spi.transmit(command);
	_delay_us(10);

	for(int i = 0; i < nBytes; i++){
		val[i] = spi.transmit(NOP);
		_delay_us(10);
	}
	
	PORTB |= (1 << SS);
}

void NRF24L01::setRxMode(){
	uint8_t configReg; 
	
	readNrf(CONFIG, &configReg);
	configReg |= (1 << PRIM_RX);
	writeNrf(CONFIG, &configReg);
	
	_delay_us(10);
}

void NRF24L01::setTxMode(){
	uint8_t configReg;

	readNrf(CONFIG, &configReg);
	configReg &= ~(1 << PRIM_RX);
	writeNrf(CONFIG, &configReg);

	_delay_us(10);
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
	setRxMode();
	PORTB |= (1 << CE);
}

void NRF24L01::recieve(uint8_t* val){
	PORTB &= ~(1 << CE);
	readNrf(R_RX_PAYLOAD, val, PAYLOAD_WIDTH);
}