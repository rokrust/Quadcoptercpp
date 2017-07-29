/*
 * Quadcopter.cpp
 *
 * Created: 10.02.2016 12:35:16
 * Author : rokrust
 */ 

#include "quadcopter.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>

Quadcopter quadcopter;

int main(void)
{

	while (1){
		quadcopter.readIMU();
	}

}

//Recieve message when the radio controller
//toggles the interrupt pin (msg recieved)
ISR(INT0_vect){
	quadcopter.recieveRemotePayload();

}


//Update sensor values
ISR(TIMER1_COMPA_vect){
	quadcopter.update_motion_data();
	quadcopter.updateController();
}

//OLD MAIN
/*
int main(void)
{
	//Constructors
	fdevopen((int (*)(char,  struct __file *))USART_transmit, (int (*)(struct __file *))USART_receive); //Link printf to USB
	USART_init(MYUBRR);

	
	//lcd.writeString(0, 0, "AccX: ");
	//lcd.writeString(8, 0, "AccY: ");
	//lcd.writeString(16, 0, "AccZ: ");
	
	
	uint8_t val[PAYLOAD_WIDTH];
	while (1)
	{
	//motorControl.determineMotorInputs();
		//sprintf(numberArray, "%d", mpu.getMovementData(0));
		//lcd.writeString(0, 48, numberArray);
		//sprintf(numberArray, "%d", mpu.getMovementData(1));
		//lcd.writeString(8, 48, numberArray);
		//sprintf(numberArray, "%d", mpu.getMovementData(2));
		//lcd.writeString(16, 48, numberArray);

	}
}*/