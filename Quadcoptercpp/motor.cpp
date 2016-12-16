#include "motor.h"
#include "pwm.h"
#include "pwm16.h"

#include <util/delay.h>

#define F_CLOCK 16000000 //Should probably declare this in a designated header file


Motor::Motor(){
	pwm = Pwm();
	pwm.enable();
}

void Motor::calibrateEsc(){
	setSpeed(MOTOR_MAX_SPEED);
	_delay_ms(3000);

	setSpeed(MOTOR_MIN_SPEED);
	_delay_ms(3000);
}

Motor::Motor(int motorNumber){
	pwm = Pwm(motorNumber, ESC_PWM_MIN);
	pwm.enable();
}

//Max value is 255
void Motor::setSpeed(uint8_t speed){
	//printf("Speed: %d\n", speed);

	//Split speed into $RESOLUTION amount of values between $ESC_MIN and $ESC_MAX 
	uint8_t divisionFactor = (MOTOR_MAX_SPEED + 1)/ESC_PWM_RESOLUTION;

	uint8_t pwmValue = (speed+1)/divisionFactor + ESC_PWM_MIN; //scale and move min value
	//printf(" %d, ", pwmValue);

	pwm.setCompare(pwmValue);
}