#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H


#define ESC_MAX_VALUE 0x03ff;
#define ESC_MIN_VALUE 0x00ff;

//Once again: Put in a designated header file
#define N_MOTORS 4
#define F_PWM 300

#include "Motor.h"

class Motorcontrol{
private:
	Motor motors[N_MOTORS];	
	
public:
	Motorcontrol();
	calibrate_escs(); 

};

#endif
