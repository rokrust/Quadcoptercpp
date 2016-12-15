#pragma once
#include "pwm16.h"
#include "pwm.h"

#include <stdint.h>

class Motor{
private:
	Pwm pwm;


public:
	Motor();
	Motor(int motorNumber);
	
	void setSpeed(int8_t speed);
};
