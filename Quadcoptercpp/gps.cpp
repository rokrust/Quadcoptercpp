#include "gps.h"
#include "usart.h"

#include <stdio.h>

void GPS::clearString(){
	for(int i = 0; i < STRLEN; i++){
		gpsString[i] = '0';
		
	}
}

void GPS::updateString(){
	//clearString();
	int i = 0;
	char gpsTemp = ' ';

	//Put every character from GPS into string
	while(gpsTemp != '\n'){
		gpsString[i++] = USART_receive();
		printf("%c", gpsTemp);
	}

	gpsString[i] = '\0';
}

GPS::GPS(){
	printf("Initializing GPS.. (If the program hangs, you've probably forgot to connect the UART)\n");
	for(int i = 0; i < STRLEN; i++){
		homeString[i] = 'q';
		gpsString[i] = 'q';
	}
	
	updateString();
	
	for(int i = 0; gpsString[i] != '\0'; i++){
		homeString[i] = gpsString[i];
	}
	printf("GPS initialized!\n");
}