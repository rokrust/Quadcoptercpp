#include "motor.h"
#include "Drivers/Timer/pwm.h"
#include "Drivers/Timer/pwm16.h"

#include <util/delay.h>
#include "config.h"


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
	
	//Split speed into $RESOLUTION amount of values between $ESC_MIN and $ESC_MAX 
	uint8_t divisionFactor = (MOTOR_MAX_SPEED + 1)/ESC_PWM_RESOLUTION;

	//scale and move min value
	uint8_t pwmValue = (speed+1)/divisionFactor + ESC_PWM_MIN; 

	pwm.setCompare(pwmValue);
}