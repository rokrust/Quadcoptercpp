#pragma once
#include "Drivers/Timer/pwm16.h"
#include "Drivers/Timer/pwm.h"

#include <stdint.h>

#define ESC_PWM_MAX 128
#define ESC_PWM_MIN 64
#define ESC_PWM_RESOLUTION 64
#define ESC_CALIBRATION_MS_DELAY 3000;
#define MOTOR_MAX_SPEED 255
#define MOTOR_MIN_SPEED 0


class Motor{
private:
	Pwm pwm;


public:
	Motor();
	Motor(int motorNumber);
	Motor(int motorNumber, int pwm_value);

	void setSpeed(uint8_t speed);
	void calibrateEsc();
};
