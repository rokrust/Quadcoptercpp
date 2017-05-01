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
	
	//Controller references
	int8_t xAngleRef, yAngleRef, zAngleRef;
	int16_t zTranslationRef;

	//Controller constants
	uint16_t Kp_xr, Kp_yr, Kp_zr, Kp_zt;
	
	//Motor inputs
	int16_t u_xr, u_yr, u_zr, u_zt;

	int16_t saturateMin(int16_t value, int16_t limit);
	int16_t saturateMax(int16_t value, int16_t limit);

public:
	MotorControl();

	void determineMotorInputs(uint8_t zTransVal);
	void setMotorInputs();

	void calibrateEscs();
	void startUpSequence();

	int8_t getXAngleRef(){ return xAngleRef; }
	int8_t getYAngleRef(){ return yAngleRef; }
	int8_t getZAngleRef(){ return zAngleRef; }

	void test(){ 
		mpu.updateSensorValues();
		printf("x: %d, y: %d, z: %d\n", mpu.getXRotationDeg(), mpu.getYRotationDeg(), mpu.getZRotationDeg());
	}
};
