#include "motor_control.h"

Motorcontrol::Motorcontrol(){
	for(int i = 0; i < N_MOTORS; i++){
		motors[i] = Motor(F_PWM); //Should find somewhere to put the esc frequency here
	}
	
	//Here comes a calibration part
}
