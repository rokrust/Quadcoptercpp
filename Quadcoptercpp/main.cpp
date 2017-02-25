/*
 * Quadcopter.cpp
 *
 * Created: 10.02.2016 12:35:16
 * Author : rokrust
 */ 

#include "adc.h"
#include "timer.h"
#include "timer16.h"
#include "usart.h"
#include "TWI_master.h"
#include "MPU6050.h"
#include "spi.h"
#include "lcd.h"
#include "gps.h"
#include "pwm.h"
#include "pwm16.h"
#include "motor.h"
#include "motor_control.h"
#include "joystick.h"
#include "NRF24L01.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>

int main(void)
{
	//Constructors
	fdevopen((int (*)(char,  struct __file *))USART_transmit, (int (*)(struct __file *))USART_receive); //Link printf to USB
	USART_init(MYUBRR);
	//Lcd lcd;
	//MotorControl motorControl;
	//Joystick joystick;
	//NRF24L01 radioController;
	uint8_t joystick_data[2];
	Adc adc;

	/*
	lcd.writeString(0, 0, "AccX: ");
	lcd.writeString(8, 0, "AccY: ");
	lcd.writeString(16, 0, "AccZ: ");
	*/
	

	while (1)
	{
	//_delay_ms(200);
	joystick_data[0] = adc.read(0);
	joystick_data[1] = adc.read(1);
	printf("x: %d, y: %d\n", joystick_data[0], joystick_data[1]);
	//radioController.transmit(joystick_data);
	
	
	//printf("Test: %d\n", USART_receive());
	//unsigned char a = joystick.readX();
	//USART_transmit(a);
	//printf("joystick: %d\n", a);
	
	//motorControl.determineMotorInputs();
		//sprintf(numberArray, "%d", mpu.getMovementData(0));
		//lcd.writeString(0, 48, numberArray);
		//sprintf(numberArray, "%d", mpu.getMovementData(1));
		//lcd.writeString(8, 48, numberArray);
		//sprintf(numberArray, "%d", mpu.getMovementData(2));
		//lcd.writeString(16, 48, numberArray);

	}
}