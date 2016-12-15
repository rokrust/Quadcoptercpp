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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>

int main(void)
{
	//MPU variables
	//char numberArray[10];

	//Constructors
	fdevopen((int (*)(char,  struct __file *))USART_transmit, (int (*)(struct __file *))USART_receive); //Link printf to USB
	USART_init(MYUBRR);
	//Lcd lcd;
	//Adc adc;
	//MotorControl motorControl;
	//MPU6050 mpu;
	
	
	
	/*
	lcd.writeString(0, 0, "AccX: ");
	lcd.writeString(8, 0, "AccY: ");
	lcd.writeString(16, 0, "AccZ: ");
	*/
	printf("Gonna make an object\n");
	MotorControl motorcontrol;
	printf("Made an object\n");
	while (1)
	{
	_delay_ms(10);
	motorcontrol.determineMotorInputs();
	//int16_t* position = mpu.getPositionData();
	//int16_t* velocity = mpu.getVelocityData();
	//int16_t* acceleration = mpu.getAccelerationData();

	//printf("Positions:\n");
	//printf("Translation:\tx: %d, y: %d, z: %d\n", position[0], position[1], position[2]);
	//printf("Angle:\tx: %d, y: %d, z: %d\n\n", position[3], position[4], position[5]);
	//printf("Velocity:\n");
	//printf("Translation:\tx: %d, y: %d, z: %d\n", velocity[0], velocity[1], velocity[2]);
	//printf("Angle:\tx: %d, y: %d, z: %d\n\n", velocity[3], velocity[4], velocity[5]);
	//printf("acc_z: %d\n", acceleration[2]);
	//printf("Acceleration\n");
	//printf("Translation:\tx: %d, y: %d, z: %d\n\n", acceleration[0], acceleration[1], acceleration[2]);

	
		//mpu.update_movement();
		//gps.updateString();
		//printf("%s", gps.getString());
		//printf("%c", USART_receive());
		
		//sprintf(numberArray, "%d", mpu.getMovementData(0));
		//lcd.writeString(0, 48, numberArray);
		//sprintf(numberArray, "%d", mpu.getMovementData(1));
		//lcd.writeString(8, 48, numberArray);
		//sprintf(numberArray, "%d", mpu.getMovementData(2));
		//lcd.writeString(16, 48, numberArray);

	}
}