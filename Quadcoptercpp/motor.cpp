#include "motor.h"
#include "pwm.h"
#include "pwm16.h"

#define ESC_PWM_MAX 128
#define ESC_PWM_MIN 64
#define ESC_PWM_RESOLUTION ESC_PWM_MAX-ESC_PWM_MIN
#define ESC_CALIBRATION_MS_DELAY 3000;
#define MOTOR_MAX_SPEED 127

#define F_CLOCK 16000000 //Should probably declare this in a designated header file


Motor::Motor(){
	pwm = Pwm();
}


Motor::Motor(int motorNumber){
	pwm = Pwm(motorNumber, ESC_PWM_MIN);
}

//Max value is 255
void Motor::setSpeed(int8_t speed){
	//Split speed into 64 values
	uint8_t divisionFactor = (MOTOR_MAX_SPEED+1)/ESC_PWM_RESOLUTION;
	uint8_t pwmValue = ((speed+1)/divisionFactor) + ESC_PWM_MIN; //scale and move min value

	pwm.setCompare(pwmValue);
}