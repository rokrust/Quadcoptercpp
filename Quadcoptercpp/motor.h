#pragma once
#include "pwm16.h"
#include "pwm.h"

#define ESC_MAX_PWM 128
#define ESC_MIN_PWM 64
#define ESC_CALIBRATION_MS_DELAY 3000;

class Motor{
private:
	Pwm pwm;


public:
	Motor();
	Motor(int motorNumber);
	
	void setSpeed(int speed);
};
