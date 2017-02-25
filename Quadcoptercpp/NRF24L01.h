#include "spi.h"
#include <stdint.h>
#include <avr/interrupt.h>

#define PAYLOAD_WIDTH 2 //Number of bytes in payload
#define ADDRESS_WIDTH 5 //Number of bytes in address
#define P0_LSB_ADDRESS 0x12

#define R_RX_PAYLOAD 0x61 //Read
#define W_TX_PAYLOAD 0xA0 //Write
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2
#define ACTIVATE 0x50
#define TX_ADDR 0x10
#define NOP 0xff

#define CONFIG 0x00
	#define MASK_RX_DR 6	//Mask interrupt from RX_DR
	#define MASK_TX_DS 5	//Mask interrupt from TX_DS
	#define MASK_MAX_RT 4	//Mask interrupt from MAX_RT
	#define EN_CRC 3		//Enable CRC
	#define CRCO 2			//CRC encoding scheme
	#define PWR_UP 1		//0: pwr_dwn, 1: pwr_up
	#define PRIM_RX 0		//0: PTX, 1: PRX

#define EN_AA 0x01
	#define ENAA_P5 5
	#define ENAA_P4 4
	#define ENAA_P3 3
	#define ENAA_P2 2
	#define ENAA_P1 1
	#define ENAA_P0 0

#define EN_RXADDR 0x02
	#define ERX_P5 5
	#define ERX_P4 4
	#define ERX_P3 3
	#define ERX_P2 2
	#define ERX_P1 1
	#define ERX_P0 0

#define SETUP_AW 0x03
	#define _3_BYTES 0x1
	#define _4_BYTES 0x2
	#define _5_BYTES 0x3

#define SETUP_RETR 0x04
	#define RETRANSMIT_3_250_US 0x03

#define RX_PW_P0 0x11
#define RX_ADDR_P0 0x0A

#define SS 2 //Also known as CSN
#define CE 0
#define RST 1 //Unused

//USES STATIC PAYLOAD LENGTH
class NRF24L01{
private:
	Spi spi;
	uint8_t config_register;

	void readNrf(uint8_t reg, uint8_t* val, uint8_t nBytes);
	void writeNrf(uint8_t reg, uint8_t* val, uint8_t nBytes = 1);
	void setRxMode();
	void setTxMode();

public:
	NRF24L01();

	void listen(); //can't transmit while listening. Run receive to stop listening
	void recieve(uint8_t* data);
	void transmit(uint8_t* data);

};
