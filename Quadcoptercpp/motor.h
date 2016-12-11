#pragma once
#include "pwm16.h"

class Motor{
private:
	Pwm16 pwm;


public:
	Motor();
	Motor(double pwm_freq); //Frequency in Hz
	
	void setSpeed(int speed);
};
