#include "motor_control.h"

#include <util/delay.h>

MotorControl::MotorControl(): xAngleRef(0), yAngleRef(0), zAngleRef(0){
	for(int i = 0; i < N_MOTORS; i++){
		motors[i] = Motor(i);
	}

	calibrateEscs();
}


void MotorControl::determineMotorInputs(){
	//Divide by controller parameters to avoid 
	//floating point multiplication

	int8_t u_xr = (xAngleRef - mpu.getXRotation())/Kp_xr;
	int8_t u_yr = (yAngleRef - mpu.getYRotation())/Kp_yr;
	int8_t u_zr = (zAngleRef - mpu.getZRotation())/Kp_zr;
	int8_t u_zt = (zTranslationRef - mpu.getZTranslation())/Kp_zt;
	
	setMotorInputs(u_xr, u_yr, u_zr, u_zt);	
}


void MotorControl::calibrateEscs(){
	//_delay_ms is ugly but should work nicely for initilization
	//
	for (int i = 0; i < N_MOTORS; i++){
		motors[i] = Motor();
		motors[i].setSpeed(255);
	}
	_delay_ms(3000);

	for (int i = 0; i < N_MOTORS; i++){
		motors[i].setSpeed(0);
	}
	_delay_ms(3000);

}

void MotorControl::setMotorInputs(uint8_t z_trans, uint8_t  x_rot, uint8_t y_rot, uint8_t z_rot){
	uint8_t motorInput[N_MOTORS];

	motorInput[0] = z_trans + x_rot - y_rot + z_rot;
	motorInput[1] = z_trans - x_rot - y_rot - z_rot;
	motorInput[2] = z_trans - x_rot + y_rot + z_rot;
	motorInput[3] = z_trans + x_rot + y_rot - z_rot;

	for(int i = 0; i < N_MOTORS; i++){
		motors[i].setSpeed(motorInput[i]);
	}
}