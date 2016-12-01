#include "lcd.h"
#include "spi.h"
#include "usart.h"
#include "fonts.h"
#include "superFonts.h"

#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

Lcd::Lcd(){
	//Pin config
	DDRB |= (1 << COMMAND_PIN) | (1 << RST);
	PORTB |= (1 << COMMAND_PIN) | (1 << RST) | (1 << SS);
	
	//Startup commands
	transmitCommand(SLPOUT);
	transmitCommand(DISPON);
	transmitCommand(NORON);

	//set pixel color to 16 bit
	transmitCommand(COLMOD);
		transmitData(0x05);
	
	//set current font color
	currentColor = 0xFF;
	clearScreen();	
}

/*Lcd::Lcd(uint16_t color): currentColor(color){
}*/

void Lcd::transmitCommand(char cData){
	PORTB &= ~(1 << COMMAND_PIN);
	transmitData(cData);
	PORTB |= (1 << COMMAND_PIN);
}

void Lcd::transmitData(char cData){
	PORTB &= ~(1 << SS);
	spi.transmit(cData);
	PORTB |= (1 << SS);
}


void Lcd::clearScreen(){
	set_address_pointer(0, SCRN_W, 0, SCRN_H);
	transmitCommand(RAMWR);

	for(int row = 0; row < SCRN_H; row++){
		for(int col = 0; col < SCRN_W; col++){
			transmitData(0x00);
			transmitData(0x00);
		}	
	}
}


void Lcd::set_address_pointer(int xStart, int xEnd, int yStart, int yEnd){
		transmitCommand(CASET);
		transmitData(xStart);
		transmitData(xStart);
		transmitData(xEnd);
		transmitData(xEnd);
		
		transmitCommand(RASET);
		transmitData(yStart);
		transmitData(yStart);
		transmitData(yEnd);
		transmitData(yEnd);
		
}

void Lcd::writeChar(int posX, int posY, char c){
	int pixelValue;

	set_address_pointer(posX, posX + 7, posY, posY + 7);

	transmitCommand(RAMWR);
	for(int row = 7; row >= 0; row--){
		pixelValue = pgm_read_byte(&superFont[c - ' '][row]);
		for(int col = 7; col >= 0; col--){ 
			if(pixelValue & (1 << col)){
				transmitData(0xff);
				transmitData(0xff);
			}

			else{
				transmitData(0x00);
				transmitData(0x00);
			}
		}
	}
}

void Lcd::writeString(int row, int col, char const * str){
	for(int i = 0; str[i+1] != '\0'; i++){
		writeChar(col + i*8, row, str[i]);
	}
}