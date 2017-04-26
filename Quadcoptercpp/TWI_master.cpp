﻿/*****************************************************************************
*
* Atmel Corporation
*
* File              : TWI_Master.c
* Compiler          : IAR EWAAVR 2.28a/3.10c
* Revision          : $Revision: 1.13 $
* Date              : $Date: 24. mai 2004 11:31:20 $
* Updated by        : $Author: ltwa $
*
* Support mail      : avr@atmel.com
*
* Supported devices : All devices with a TWI module can be used.
*                     The example is written for the ATmega16
*
* AppNote           : AVR315 - TWI Master Implementation
*
* Description       : This is a sample driver for the TWI hardware modules.
*                     It is interrupt driveren. All functionality is controlled through
*                     passing information to and from functions. Se main.c for samples
*                     of how to use the driver.
*
*
****************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include "TWI_Master.h"

static unsigned char TWI_buf[ TWI_BUFFER_SIZE ];    // Transceiver buffer
static unsigned char TWI_msgSize;                   // Number of bytes to be transmitted.
static unsigned char TWI_state = TWI_NO_STATE;      // State byte. Default set to TWI_NO_STATE.

union TWI_statusReg TWI_statusReg = {0};            // TWI_statusReg is defined in TWI_Master.h

/****************************************************************************
Call this function to set up the TWI master to its initial standby state.
Remember to enable interrupts from the main application after initializing the TWI.
****************************************************************************/
TWI::TWI(void)
{
	TWBR = TWI_TWBR;                                  // Set bit rate register (Baudrate). Defined in header file.
	// TWSR = TWI_TWPS;                                  // Not used. Driver presumes prescaler to be 00.
	TWDR = 0xFF;                                      // Default content = SDA released.
	TWCR = (1<<TWEN)|                                 // Enable TWI-interface and release TWI pins.
	(0<<TWIE)|(0<<TWINT)|                      // Disable Interupt.
	(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // No Signal requests.
	(0<<TWWC);		                           //
	sei();
}

/****************************************************************************
Call this function to test if the TWI_ISR is busy transmitting.
****************************************************************************/
unsigned char TWI::transceiver_busy( void )
{
	return ( TWCR & (1<<TWIE) );                  // IF TWI Interrupt is enabled then the Transceiver is busy
}

/****************************************************************************
Call this function to fetch the state information of the previous operation. The function will hold execution (loop)
until the TWI_ISR has completed with the previous operation. If there was an error, then the function
will return the TWI State code.
****************************************************************************/
unsigned char TWI::get_state_info( void )
{
	while ( TWI::transceiver_busy() );             // Wait until TWI has completed the transmission.
	return ( TWI_state );                         // Return error state.
}


/****************************************************************************
Call this function to send a prepared message. The first byte must contain the slave address and the
read/write bit. Consecutive bytes contain the data to be sent, or empty locations for data to be read
from the slave. Also include how many bytes that should be sent/read including the address byte.
The function will hold execution (loop) until the TWI_ISR has completed with the previous operation,
then initialize the next operation and return.
****************************************************************************/
void TWI::start_transceiver_with_data( unsigned char *msg, unsigned char msgSize )
{
	unsigned char temp;

	while ( transceiver_busy() );	              // Wait until TWI is ready for next transmission.
	TWI_msgSize = msgSize;                        // Number of data to transmit.
	TWI_buf[0]  = msg[0];                         // Store slave address with R/W setting.
	if (!( msg[0] & (TRUE<<TWI_READ_BIT) ))       // If it is a write operation, then also copy data.
	{
		for ( temp = 1; temp < msgSize; temp++ )
		TWI_buf[ temp ] = msg[ temp ];
	}
	TWI_statusReg.all = 0;
	TWI_state         = TWI_NO_STATE ;
	TWCR = (1<<TWEN)|                             // TWI Interface enabled.
	(1<<TWIE)|(1<<TWINT)|                  // Enable TWI Interupt and clear the flag.
	(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|       // Initiate a START condition.
	(0<<TWWC);                             //
}

/****************************************************************************
Call this function to resend the last message. The driver will reuse the data previously put in the transceiver buffers.
The function will hold execution (loop) until the TWI_ISR has completed with the previous operation,
then initialize the next operation and return.
****************************************************************************/
void TWI::start_transceiver( void )
{
	while ( transceiver_busy() );             // Wait until TWI is ready for next transmission.
	TWI_statusReg.all = 0;
	TWI_state         = TWI_NO_STATE ;
	TWCR = (1<<TWEN)|                             // TWI Interface enabled.
	(1<<TWIE)|(1<<TWINT)|                  // Enable TWI Interupt and clear the flag.
	(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|       // Initiate a START condition.
	(0<<TWWC);                             //
}

/****************************************************************************
Call this function to read out the requested data from the TWI transceiver buffer. I.e. first call
TWI_Start_Transceiver to send a request for data to the slave. Then Run this function to collect the
data when they have arrived. Include a pointer to where to place the data and the number of bytes
requested (including the address field) in the function call. The function will hold execution (loop)
until the TWI_ISR has completed with the previous operation, before reading out the data and returning.
If there was an error in the previous transmission the function will return the TWI error code.
****************************************************************************/
unsigned char TWI::get_data_from_transceiver( unsigned char *msg, unsigned char msgSize )
{
	unsigned char i;

	while ( transceiver_busy() );             // Wait until TWI is ready for next transmission.

	if(TWI_statusReg.lastTransOK)               // Last transmission competed successfully.
	{
		for ( i=0; i<msgSize; i++ )                 // Copy data from Transceiver buffer.
		{
			msg[ i ] = TWI_buf[ i ];
		}
	}
	return( TWI_statusReg.lastTransOK );
}

//Custom functions. Might be more intuitive.
void TWI::read_data_from_address(unsigned char chip_address, unsigned char register_address, unsigned char* msg, unsigned char msgSize){
	//Put address in the first position and read register
	//in the second.
	unsigned char temp[msgSize+1];
	
	temp[0] = chip_address << 1 | WRITE_FLAG;
	temp[1] = register_address;
	start_transceiver_with_data(temp, 2);
	
	temp[0] = chip_address << 1 | READ_FLAG;

	start_transceiver_with_data(temp, msgSize + 1);
	get_data_from_transceiver(temp, msgSize + 1);
	
	for(int i = 0; i < msgSize; i++){
		msg[i] = temp[i+1];
	}
}


void TWI::write_data_to_register(unsigned char chip_address, unsigned char register_address, unsigned char value){
	unsigned char temp[3] = {chip_address << 1 | WRITE_FLAG, register_address, value};
	start_transceiver_with_data(temp, 3);
}


// ********** Interrupt Handlers ********** //
/****************************************************************************
This function is the Interrupt Service Routine (ISR), and called when the TWI interrupt is triggered;
that is whenever a TWI event has occurred. This function should not be called directly from the main
application.
****************************************************************************/
ISR(TWI_vect)
{
	static unsigned char TWI_bufPtr;
	switch (TWSR)
	{
		case TWI_START:             // START has been transmitted
		//printf("START\n");
		case TWI_REP_START:         // Repeated START has been transmitted
		//printf("REP_START\n");
		TWI_bufPtr = 0;                                     // Set buffer pointer to the TWI Address location
		case TWI_MTX_ADR_ACK:       // SLA+W has been tramsmitted and ACK received
		//printf("SLA+W\n");
		case TWI_MTX_DATA_ACK:      // Data byte has been tramsmitted and ACK received
		//printf("MTX_DATA_ACK\n");
		if (TWI_bufPtr < TWI_msgSize)
		{
		//	printf("TWI_buffer: %d\n", TWI_buf[TWI_bufPtr]);
			TWDR = TWI_buf[TWI_bufPtr++];
			TWCR = (1<<TWEN)|                                 // TWI Interface enabled
			(1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interupt and clear the flag to send byte
			(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           //
			(0<<TWWC);                                 //
		}else                    // Send STOP after last byte
		{
			TWI_statusReg.lastTransOK = TRUE;                 // Set status bits to completed successfully.
			TWCR = (1<<TWEN)|                                 // TWI Interface enabled
			(0<<TWIE)|(1<<TWINT)|                      // Disable TWI Interrupt and clear the flag
			(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|           // Initiate a STOP condition.
			(0<<TWWC);                                 //
		//	printf("Stop condition sent.\n");
		}
		break;
		case TWI_MRX_DATA_ACK:      // Data byte has been received and ACK tramsmitted
		//printf("MRX_DATA_ACK\n");
		TWI_buf[TWI_bufPtr++] = TWDR;
		case TWI_MRX_ADR_ACK:       // SLA+R has been tramsmitted and ACK received
		//printf("MRX_ADR_ACK\n");
		if (TWI_bufPtr < (TWI_msgSize-1) )                  // Detect the last byte to NACK it.
		{
			TWCR = (1<<TWEN)|                                 // TWI Interface enabled
			(1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interupt and clear the flag to read next byte
			(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // Send ACK after reception
			(0<<TWWC);                                 //
		}else                    // Send NACK after next reception
		{
			TWCR = (1<<TWEN)|                                 // TWI Interface enabled
			(1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interupt and clear the flag to read next byte
			(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // Send NACK after reception
			(0<<TWWC);                                 //
		}
		break;
		case TWI_MRX_DATA_NACK:     // Data byte has been received and NACK tramsmitted
		//printf("MRX_DATA_NACK\n");
		TWI_buf[TWI_bufPtr] = TWDR;
		TWI_statusReg.lastTransOK = TRUE;                 // Set status bits to completed successfully.
		TWCR = (1<<TWEN)|                                 // TWI Interface enabled
		(0<<TWIE)|(1<<TWINT)|                      // Disable TWI Interrupt and clear the flag
		(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|           // Initiate a STOP condition.
		(0<<TWWC);                                 //
		break;
		case TWI_ARB_LOST:          // Arbitration lost
		//printf("ARB_LOST\n");
		TWCR = (1<<TWEN)|                                 // TWI Interface enabled
		(1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interupt and clear the flag
		(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|           // Initiate a (RE)START condition.
		(0<<TWWC);                                 //
		break;
		case TWI_MTX_ADR_NACK:      // SLA+W has been tramsmitted and NACK received
		//printf("MTX_ADR_NACK\n");
		case TWI_MRX_ADR_NACK:      // SLA+R has been tramsmitted and NACK received
		//printf("MRX_ADR_NACK\n");
		case TWI_MTX_DATA_NACK:     // Data byte has been tramsmitted and NACK received
		//printf("MTX_DATA_NACK\n");
		//    case TWI_NO_STATE              // No relevant state information available; TWINT = ?0?
		case TWI_BUS_ERROR:         // Bus error due to an illegal START or STOP condition
		//printf("BUS_ERROR\n");
		default:
		TWI_state = TWSR;                                 // Store TWSR and automatically sets clears noErrors bit.
		// Reset TWI Interface
		TWCR = (1<<TWEN)|                                 // Enable TWI-interface and release TWI pins
		(0<<TWIE)|(0<<TWINT)|                      // Disable Interupt
		(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // No Signal requests
		(0<<TWWC);                                 //
	}
}