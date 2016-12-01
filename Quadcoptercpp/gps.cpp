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
		gpsTemp = USART_receive();
		gpsString[i] = gpsTemp;
		i++;
		printf("%c", gpsTemp);
	}

	gpsString[i] = '\0';
}

GPS::GPS(){
	for(int i = 0; i < STRLEN; i++){
		homeString[i] = 'q';
		gpsString[i] = 'q';
	}
	
	updateString();
	
	for(int i = 0; gpsString[i] != '\0'; i++){
		homeString[i] = gpsString[i];
	}
}