#include "motor.h"
#include "pwm.h"
#include "pwm16.h"

#define MOTOR_MAX_SPEED 255.0

#define F_CLOCK 16000000 //Should probably declare this in a designated header file


Motor::Motor(){
	


}


Motor::Motor(int motorNumber){
	pwm = Pwm(motorNumber, 0x7f);

}


void Motor::setSpeed(int speed){

}