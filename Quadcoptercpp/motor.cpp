#include "motor.h"
#include "pwm.h"
#include "pwm16.h"

#define MOTOR_MAX_SPEED 255.0

#define F_CLOCK 16000000 //Should probably declare this in a designated header file

//Gotta fix this overflow
Motor::Motor(double pwm_freq){
	int topVal = (int)(F_CLOCK/pwm_freq)-1;
	printf("Topval: %d\n", topVal);
	pwm = Pwm16(topVal, 0); //Start esc with lowest value
	

}

Motor::Motor(): Motor(0xffff){



}

//Should do some magic with the speed value.
//Probably set 255 as maximum
void Motor::setSpeed(int speed){
	//Scale speed according to the topValue of the pwm
	speed = (speed/MOTOR_MAX_SPEED)*pwm.getTopValue();

	pwm.setCompare(speed);
}