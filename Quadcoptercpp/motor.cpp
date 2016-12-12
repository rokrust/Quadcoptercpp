#include "motor.h"
#include "pwm.h"
#include "pwm16.h"

#define ESC_PWM_MAX 128
#define ESC_PWM_MIN 64
#define ESC_PWM_RESOLUTION ESC_PWM_MAX-ESC_PWM_MIN
#define ESC_CALIBRATION_MS_DELAY 3000;
#define MOTOR_MAX_SPEED 255.0

#define F_CLOCK 16000000 //Should probably declare this in a designated header file


Motor::Motor(){
	pwm = Pwm();


}


Motor::Motor(int motorNumber){
	pwm = Pwm(motorNumber, ESC_PWM_MIN);
}

//Max value is 255
void Motor::setSpeed(uint8_t speed){
	//Split speed into 64 values
	int temp = (speed+1); //Might not be needed. Safety against overflow
	uint8_t pwmValue = (temp/ESC_PWM_RESOLUTION) + ESC_PWM_MIN;

	pwm.setCompare(pwmValue);
}