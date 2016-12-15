#include "motor_control.h"

#include <util/delay.h>

MotorControl::MotorControl(): xmAngleRef(0), ymAngleRef(0), zmAngleRef(0), zTranslationRef(0){
	
	printf("Initializing motors..\n");
	for(int i = 0; i < N_MOTORS; i++){
		motors[i] = Motor(i);
	}
	printf("Motors initialized!\n");

	Kp_xr = 260; 
	Kp_yr = 260; 
	Kp_zr = 260; 
	Kp_zt = 260;
	printf("Calibrating escs..\n");
	calibrateEscs();
	printf("Escs calibrated!\n");
}


void MotorControl::determineMotorInputs(){
	//Divide by controller parameters to avoid 
	//floating point multiplication
	mpu.updateSensorValues();
	int16_t* positionData = mpu.getPositionData();
	int8_t u_xr = (xmAngleRef - positionData[3])/Kp_xr;
	int8_t u_yr = (ymAngleRef - positionData[4])/Kp_yr;
	int8_t u_zr = (zmAngleRef - positionData[5])/Kp_zr;
	int8_t u_zt = (zTranslationRef - mpu.getZTranslation())/Kp_zt;
	printf("u_xr: %d, u_yr: %d, u_zr: %d, u_zt: %d\n", positionData[3], positionData[4], positionData[5], u_zt);
	setMotorInputs(u_xr, u_yr, u_zr, u_zt);	
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