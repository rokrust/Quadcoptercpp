#pragma once

#include "MPU6050.h"
#include "Motor.h"
#include "gps.h"

//Once again: Put in a designated header file
#define N_MOTORS 4
#define F_PWM 300

class MotorControl{
private:
	Motor motors[N_MOTORS];	
	MPU6050 mpu;
	//GPS gps;
	
	int8_t xAngleRef, yAngleRef, zAngleRef, zTranslationRef;
	uint16_t Kp_xr, Kp_yr, Kp_zr, Kp_zt;

public:
	MotorControl();

	void determineMotorInputs();

	void calibrateEscs();
	void startUpSequence();

	int8_t setXAngleRef(){ return xAngleRef; }
	int8_t setYAngleRef(){ return yAngleRef; }
	int8_t setZAngleRef(){ return zAngleRef; }
	
	void setMotorInputs(uint8_t z_trans, uint8_t  x_rot, uint8_t y_rot, uint8_t z_rot);
};
