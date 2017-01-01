#include "spi.h"

#define R_RX_PAYLOAD 0x61
#define W_TX_PAYLOAD 0xA0
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2
#define ACTIVATE 0x50
#define NOP 0xff

#define CONFIG 0x00
	#define MASK_RX_DR 6	//Mask interrupt from RX_DR
	#define MASK_TX_DS 5	//Mask interrupt from TX_DS
	#define MASK_MAX_RT 4	//Mask interrupt from MAX_RT
	#define EN_CRC 3		//Enable CRC
	#define CRCO 2			//CRC encoding scheme
	#define PWR_UP 1		//0: pwr_dwn, 1: pwr_up
	#define PRIM_RX 0		//0: PTX, 1: PRX


#define SS 2
#define COMMAND_PIN 0
#define RST 1


class NRF24L01{
private:
	Spi spi;

public:
	NRF24L01();
	void transmitCommand(char cData);
	void transmitData(char cData);

};

