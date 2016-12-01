#pragma once

#define DATALEN 10
#define STRLEN 255

class GPS{
	public:
		GPS();
		void updateString();

		char* getString(){return gpsString;}
		int getLongitudeDec();
		int getLatitudeDec();


	private:
		//store number data
		int gpsData[DATALEN];

		//string from gps chip
		char gpsString[STRLEN];

		//initial data
		char homeString[STRLEN];

		void clearString();
};