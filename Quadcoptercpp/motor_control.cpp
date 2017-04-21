#include "motor_control.h"
#define F_CPU 16000000UL
#include <util/delay.h>

MotorControl::MotorControl(): xAngleRef(0), yAngleRef(0), zAngleRef(0), zTranslationRef(10000){
	
	printf("Initializing motors..\n");
	for(int i = 0; i < N_MOTORS; i++){
		motors[i] = Motor(i);
	}
	printf("Motors initialized!\n");

	Kp_xr = 4; 
	Kp_yr = 4; 
	Kp_zr = 4; 
	Kp_zt = 1;
}


void MotorControl::determineMotorInputs(uint8_t zTransVal){

	mpu.updateSensorValues();
	
	u_xr = (xAngleRef - mpu.getXRotationDeg())*Kp_xr;
	u_yr = (yAngleRef - mpu.getYRotationDeg())*Kp_yr;
	u_zr = (zAngleRef - mpu.getZRotationDeg())*Kp_zr;
	u_zt = zTransVal * Kp_xr;

}


void MotorControl::setMotorInputs(){
	int16_t motorInput[N_MOTORS];

	motorInput[0] = u_zt + u_xr - u_yr + u_zr;
	motorInput[1] = u_zt - u_xr - u_yr - u_zr;
	motorInput[2] = u_zt - u_xr + u_yr + u_zr;
	motorInput[3] = u_zt + u_xr + u_yr - u_zr;

	for(int i = 0; i < N_MOTORS; i++){
		motorInput[i] = saturateMin(motorInput[i], 0);
		motorInput[i] = saturateMax(motorInput[i], 255);
		motors[i].setSpeed((uint8_t)motorInput[i]);
	}
}


int16_t MotorControl::saturateMin(int16_t value, int16_t limit){
	if(value >= limit){
		return value;
	}
	else{
		return limit;
	}
}

int16_t MotorControl::saturateMax(int16_t value, int16_t limit){
	if(value <= limit){
		return value;
	}
	else{
		return limit;
	}
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
