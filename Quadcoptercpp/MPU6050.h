#pragma once

#include "TWI_master.h"
#include <stdint.h>


#define MPU_ADDRESS 0x68

#define PWR_MGMT_1 0x6B
#define WAKE_UP 0x00
#define ACC_X 0x3B

#define WRITE_FLAG 0
#define READ_FLAG 1

#define N_TRANS_VAR 3
#define N_ROT_VAR 3
#define N_MESSURE_VAR 7 //Acceleration, angular velocity and temperature

#define MS_SAMPLING_TIME 100 //Avoid float arithmetic

class MPU6050{
private:
	TWI twi;
	int16_t accelerationData[N_TRANS_VAR];
	int16_t velocityData[N_TRANS_VAR + N_ROT_VAR];
	int16_t positionData[N_TRANS_VAR + N_ROT_VAR];

	int16_t sensorOffset[N_MESSURE_VAR];
	
	void calibrateData(int16_t* sensorData);
	void updateDataArrays(int16_t* sensorData);
	void determineOffsetArray();

public:
	MPU6050();
	void updateSensorValues();
	
	int16_t* getAccelerationData(){return accelerationData;}
	int16_t* getVelocityData(){return velocityData;}
	int16_t* getPositionData(){return positionData;}
	int16_t getXTranslation(){return positionData[0];}
	int16_t getYTranslation(){return positionData[1];}
	int16_t getZTranslation(){return positionData[2];}
	int16_t getXRotation(){return positionData[3];}
	int16_t getYRotation(){return positionData[4];}
	int16_t getZRotation(){return positionData[5];}
};
