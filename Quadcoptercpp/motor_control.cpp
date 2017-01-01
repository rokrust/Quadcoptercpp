#include "motor_control.h"
#define F_CPU 16000000UL
#include <util/delay.h>

MotorControl::MotorControl(): xAngleRef(0), yAngleRef(0), zAngleRef(0), zTranslationRef(10000){
	
	printf("Initializing motors..\n");
	for(int i = 0; i < N_MOTORS; i++){
		motors[i] = Motor(i);
	}
	printf("Motors initialized!\n");

	Kp_xr = 2; 
	Kp_yr = 2; 
	Kp_zr = 2; 
	Kp_zt = 320;
	printf("Calibrating escs..\n");
	//startUpSequence();
	//_delay_ms(10000);
	calibrateEscs();
	
	printf("Escs calibrated!\n");
}


void MotorControl::determineMotorInputs(){
	//Divide by controller parameters to avoid 
	//floating point multiplication
	mpu.updateSensorValues();
	
	int16_t u_xr = (xAngleRef - mpu.getXRotationDegrees())*Kp_xr;
	int16_t u_yr = (yAngleRef - mpu.getYRotationDegrees())*Kp_yr;
	int16_t u_zr = (zAngleRef - mpu.getZRotationDegrees())*Kp_zr;
	int16_t u_zt = (zTranslationRef - mpu.getZTranslation())/Kp_zt;
	//u_xr = 0;
	//u_yr = 0;
	//u_zr = 0;

	u_zt = 50; //translation drifts like crazy

	//printf("u_xr: %d, u_yr: %d, u_zr: %d, u_zt: %d\n", u_xr, u_yr, u_zr, u_zt);
	setMotorInputs(u_zt, u_xr, u_yr, u_zr);
}

void MotorControl::startUpSequence(){
	//_delay_ms is ugly but should work nicely for initilization
	for (int i = 0; i < N_MOTORS; i++){
		motors[i] = Motor();
		motors[i].setSpeed(MOTOR_MIN_SPEED);
	}
	_delay_ms(3000);
	/*
	for (int i = 0; i < N_MOTORS; i++){
		motors[i].setSpeed(MOTOR_MAX_SPEED);
	}

	_delay_ms(3000);

	for (int i = 0; i < N_MOTORS; i++){
		motors[i].setSpeed(MOTOR_MIN_SPEED);
	}
	*/
}


void MotorControl::calibrateEscs(){
	//_delay_ms is ugly but should work nicely for initilization
	for (int i = 0; i < N_MOTORS; i++){
		motors[i].setSpeed(MOTOR_MAX_SPEED);
	}
	
	_delay_ms(3000);
	for (int i = 0; i < N_MOTORS; i++){
		motors[i].setSpeed(MOTOR_MIN_SPEED);
	}
	_delay_ms(3000);

}

void MotorControl::setMotorInputs(uint8_t z_trans, uint8_t  x_rot, uint8_t y_rot, uint8_t z_rot){
	uint16_t motorInput[N_MOTORS];

	motorInput[0] = z_trans + x_rot - y_rot + z_rot;
	motorInput[1] = z_trans - x_rot - y_rot - z_rot;
	motorInput[2] = z_trans - x_rot + y_rot + z_rot;
	motorInput[3] = z_trans + x_rot + y_rot - z_rot;

	//printf("\nspeed: ");
	for(int i = 0; i < N_MOTORS; i++){
		motorInput[i] = max(motorInput[i], 0);
		motorInput[i] = saturate(motorInput[i], 255);
		//printf("Motor %d: %d |", i, motorInput[i]);
		motors[i].setSpeed(motorInput[i]);
	}
	//printf("\n");
}


uint8_t MotorControl::max(uint8_t a, uint8_t b){
	if(a > b){
		return a;
	}
	else{
		return b;
	}
}

uint8_t MotorControl::saturate(uint16_t value, uint8_t limit){
	if(value <= limit){
		return (uint8_t)value;
	}
	else{
		return limit;
	}
}