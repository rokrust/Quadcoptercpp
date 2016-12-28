#pragma once
#include "adc.h"

#define JOYSTICK_X_CHANNEL 0
#define JOYSTICK_Y_CHANNEL 1


class Joystick{
private:
	Adc adc;

public:
	Joystick(){};

	unsigned char readX(){return adc.read(JOYSTICK_X_CHANNEL);}
	unsigned char readY(){return adc.read(JOYSTICK_Y_CHANNEL);}

};