#include "motor.h"
#include "pwm.h"
#include "pwm16.h"

#include <util/delay.h>

#define ESC_PWM_MAX 128
#define ESC_PWM_MIN 64
#define ESC_PWM_RESOLUTION ESC_PWM_MAX-ESC_PWM_MIN
#define ESC_CALIBRATION_MS_DELAY 3000;
#define MOTOR_MAX_SPEED 127

#define F_CLOCK 16000000 //Should probably declare this in a designated header file


Motor::Motor(){
	pwm = Pwm();
	pwm.enable();
}


Motor::Motor(int motorNumber){
	pwm = Pwm(motorNumber, ESC_PWM_MIN);
	pwm.enable();
}

//Max value is 255
void Motor::setSpeed(int8_t speed){
	//Pretty ugly, but needed to get the correct end values
	int16_t temp= speed + 2;
	if(speed < 0){
		temp = speed;
	}

	//Split speed into $RESOLUTION amount of values between $ESC_MIN and $ESC_MAX 
	uint8_t divisionFactor = (MOTOR_MAX_SPEED+1)/ESC_PWM_RESOLUTION;
	uint8_t pwmValue = (temp+MOTOR_MAX_SPEED)/divisionFactor + ESC_PWM_MIN; //scale and move min value

	printf("Pwm value: %d\n", pwmValue);

	pwm.setCompare(pwmValue);
}