#include "NRF24L01.h"
#include "config.h"
#include <avr/io.h>
#include <stdio.h>


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
	_write_nrf(EN_AA, data);

	//Enable datapipe 0
	data[0] = (1 << ERX_P0);
	_write_nrf(EN_RXADDR, data);

	//Set pipe address width
	data[0] = _5_BYTES;
	_write_nrf(SETUP_AW, data);

	//Set pipe 0 address
	for(int i = 0; i < ADDRESS_WIDTH; i++){
		data[i] = P0_LSB_ADDRESS;
	}
	_write_nrf(RX_ADDR_P0, data, ADDRESS_WIDTH);
	_write_nrf(TX_ADDR, data, ADDRESS_WIDTH);
	
	//Set Payload width
	data[0] = PAYLOAD_WIDTH;
	_write_nrf(RX_PW_P0, data);

	//Set number of retransmits and interval between them
	data[0] = RETRANSMIT_3_250_US;
	_write_nrf(SETUP_RETR, data);
	
	//Power up radio controller, enable CRC and disable TX and MAX_RT interrupts
	//(leave RX interrupt active)
	data[0] = (1 << PWR_UP) | (1 << EN_CRC) | (1 << PRIM_RX) | 
			  (1 << MASK_TX_DS) | (1 << MASK_MAX_RT);
	_write_nrf(CONFIG, data);

	//Give NRF time to reach standby mode (1.5 ms)
	_delay_ms(10);
}

//The write flag is added to the command address during write operations
//unlike the read operations.
void NRF24L01::_write_nrf(uint8_t command, uint8_t* val, uint8_t nBytes){
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

void NRF24L01::_read_nrf(uint8_t command, uint8_t* val, uint8_t nBytes){
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

void NRF24L01::_set_rx_mode(){
	uint8_t configReg; 
	
	_read_nrf(CONFIG, &configReg);
	configReg |= (1 << PRIM_RX);
	_write_nrf(CONFIG, &configReg);
	
	_delay_us(10);
}

void NRF24L01::_set_tx_mode(){
	uint8_t configReg;

	_read_nrf(CONFIG, &configReg);
	configReg &= ~(1 << PRIM_RX);
	_write_nrf(CONFIG, &configReg);

	_delay_us(10);
}

//Must be called to after recieve/transmit to continue communication
void NRF24L01::reset(){
	//Clear status flags
	uint8_t data = (1 << RX_DR) | (1 << TX_DS) | (1 << MAX_RT);
	_write_nrf(STATUS, &data);
}


void NRF24L01::transmit(uint8_t* data){
	_write_nrf(FLUSH_TX, NULL, 0);
	_write_nrf(W_TX_PAYLOAD, data, PAYLOAD_WIDTH);
	_set_tx_mode();

	PORTB |= (1 << CE);
	_delay_us(20); //Should probably fix this
	PORTB &= ~(1 << CE);

	reset();
}

//Accept incoming messages. Does not block execution.
void NRF24L01::listen(){
	reset();

	_set_rx_mode();
	PORTB |= (1 << CE);
}

//Collect received message
void NRF24L01::receive(uint8_t* val){
	PORTB &= ~(1 << CE);
	_read_nrf(R_RX_PAYLOAD, val, PAYLOAD_WIDTH);
}

void NRF24L01::set_payload_width(uint8_t payload_width, uint8_t pipe){
	_write_nrf(RX_PW_P0 + pipe, &payload_width);
}