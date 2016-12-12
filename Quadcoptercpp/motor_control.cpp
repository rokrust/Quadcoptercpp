#include "motor_control.h"

#include <util/delay.h>

MotorControl::MotorControl(){
	for(int i = 0; i < N_MOTORS; i++){
		motors[i] = Motor(i);
	}
	

	calibrateEscs();
}

void MotorControl::calibrateEscs(){
	//_delay_ms is ugly but should work nicely for initilization
	//
	for (int i = 0; i < 4; i++){
		motors[i] = Motor();
		motors[i].setSpeed(255);
	}
	_delay_ms(3000);

	for (int i = 0; i < 4; i++){
		motors[i].setSpeed(0);
	}
	_delay_ms(3000);

}