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
	
	int8_t xAngleRef, yAngleRef, zAngleRef;
	int16_t zTranslationRef;
	uint16_t Kp_xr, Kp_yr, Kp_zr, Kp_zt;

	uint8_t max(uint8_t a, uint8_t b); //To avoid negative values
	uint8_t saturate(uint16_t value, uint8_t limit); //To avoid too large values

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
