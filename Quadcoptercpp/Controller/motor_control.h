#pragma once

#include "Drivers/Sensors/MPU6050.h"
#include "Drivers/Sensors/gps.h"
#include "Drivers/Motor.h"

//Once again: Put in a designated header file
#define N_MOTORS 4
#define F_PWM 300

class MotorControl{
private:
	Motor motors[N_MOTORS];
	
	//Controller references
	int8_t xAngleRef, yAngleRef, zAngleRef;
	int16_t zTranslationRef;

	//Controller constants
	uint16_t Kp_xr, Kp_yr, Kp_zr, Kp_zt;
	uint16_t Kd_xr, Kd_yr, Kd_zr;
	
	//Motor inputs
	int16_t u_xr, u_yr, u_zr, u_zt;

	int16_t saturateMin(int16_t value, int16_t limit);
	int16_t saturateMax(int16_t value, int16_t limit);

public:
	MotorControl();

	void determineMotorInputs(int16_t xRot, int16_t yRot, int16_t zRot, uint8_t zTrans);
	void setMotorInputs();

	void calibrateEscs();
	void startUpSequence();

	int8_t getXAngleRef(){ return xAngleRef; }
	int8_t getYAngleRef(){ return yAngleRef; }
	int8_t getZAngleRef(){ return zAngleRef; }

};
