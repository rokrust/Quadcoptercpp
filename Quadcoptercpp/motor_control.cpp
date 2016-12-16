#include "motor_control.h"

#include <util/delay.h>

MotorControl::MotorControl(): xAngleRef(0), yAngleRef(0), zAngleRef(0), zTranslationRef(0){
	
	printf("Initializing motors..\n");
	for(int i = 0; i < N_MOTORS; i++){
		motors[i] = Motor(i);
	}
	printf("Motors initialized!\n");

	Kp_xr = 4; 
	Kp_yr = 4; 
	Kp_zr = 4; 
	Kp_zt = 320;
	printf("Calibrating escs..\n");
	calibrateEscs();
	printf("Escs calibrated!\n");
}


void MotorControl::determineMotorInputs(){
	//Divide by controller parameters to avoid 
	//floating point multiplication
	mpu.updateSensorValues();
	
	int8_t u_xr = (xAngleRef - mpu.getXRotationDegrees())*Kp_xr;
	int8_t u_yr = (yAngleRef - mpu.getYRotationDegrees())*Kp_yr;
	int8_t u_zr = (zAngleRef - mpu.getZRotationDegrees())*Kp_zr;
	int8_t u_zt = 70;//(zTranslationRef - mpu.getZTranslation())/Kp_zt;
	printf("u_xr: %d, u_yr: %d, u_zr: %d, u_zt: %d\n", u_xr, u_yr, u_zr, u_zt);
	setMotorInputs(u_xr, u_yr, u_zr, u_zt);
	//printf("Angle x: %d, y: %d, z: %d\n", mpu.getXRotationDegrees(), mpu.getYRotationDegrees(), mpu.getZRotationDegrees());	
}

void MotorControl::startUpSequence(){
	//_delay_ms is ugly but should work nicely for initilization
	//
	for (int i = 0; i < N_MOTORS; i++){
		motors[i] = Motor();
		motors[i].setSpeed(-127);
	}
	_delay_ms(3000);

	for (int i = 0; i < N_MOTORS; i++){
		motors[i].setSpeed(127);
	}
	_delay_ms(3000);

	for (int i = 0; i < N_MOTORS; i++){
		motors[i].setSpeed(-127);
	}
}


void MotorControl::calibrateEscs(){
	//_delay_ms is ugly but should work nicely for initilization
	//
	for (int i = 0; i < N_MOTORS; i++){
		motors[i] = Motor();
		motors[i].setSpeed(127);
	}
	_delay_ms(3000);

	for (int i = 0; i < N_MOTORS; i++){
		motors[i].setSpeed(-127);
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