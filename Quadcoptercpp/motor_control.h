#pragma once

#include "MPU6050.h"
#include "Motor.h"
#include "gps.h"

#define ESC_MAX_VALUE 0x03ff;
#define ESC_MIN_VALUE 0x00ff;

//Once again: Put in a designated header file
#define N_MOTORS 4
#define F_PWM 300

class Motorcontrol{
private:
	Motor motors[N_MOTORS];	
	MPU6050 mpu;
	GPS gps;
	
public:
	Motorcontrol();
	void calibrate_escs(); 

};
