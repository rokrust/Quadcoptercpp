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
	//MPU6050 mpu;
	//Lcd lcd;
	//Adc adc;
	GPS gps;
	
	//Pwm pwm(120);

	/*
	lcd.writeString(0, 0, "AccX: ");
	lcd.writeString(8, 0, "AccY: ");
	lcd.writeString(16, 0, "AccZ: ");
	*/

	//pwm.enable();
	
	while (1)
	{	
		
		//mpu.update_movement();
		//gps.updateString();
		//printf("%s", gps.getString());
		printf("%c", USART_receive());
		
		//sprintf(numberArray, "%d", mpu.getMovementData(0));
		//lcd.writeString(0, 48, numberArray);
		//sprintf(numberArray, "%d", mpu.getMovementData(1));
		//lcd.writeString(8, 48, numberArray);
		//sprintf(numberArray, "%d", mpu.getMovementData(2));
		//lcd.writeString(16, 48, numberArray);

	}
}