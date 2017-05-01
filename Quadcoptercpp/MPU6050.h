#pragma once

#include "TWI_master.h"
#include <stdint.h>


#define MPU_ADDRESS 0x68

#define PWR_MGMT_1 0x6B
	#define WAKE_UP 0x00

#define ACC_X 0x3B
#define GYRO_CONFIG 0x1A
	#define DEG_S_250 0
	#define DEG_S_500 8
	#define DEG_S_1000 16
	#define DEG_S_2000 24

#define ACCEL_CONFIG 0x1B
	#define M_S2_2G 0
	#define M_S2_4G 8
	#define M_S2_8G 16
	#define M_S2_16G 24

#define N_TRANS_VAR 3
#define N_ROT_VAR 3
#define N_MOTION_VAR N_TRANS_VAR + N_ROT_VAR
#define N_MESSURE_VAR 7 //Acceleration, angular velocity and temperature
#define g_SENSOR_VALUE 16384
#define MPU6050_MAX_SENSOR_VALUE 32767
#define MPU6050_MAX_DEG_S_VALUE 250

#define SAMPLING_FREQ 48

class MPU6050{
private:
	TWI twi;
	int16_t accelerationData[N_TRANS_VAR];
	int16_t velocityData[N_MOTION_VAR];
	int16_t positionData[N_MOTION_VAR];

	int16_t sensorOffset[N_MESSURE_VAR];
	
	void calibrateData(int16_t* sensorData);
	void updateDataArrays(int16_t* sensorData);
	void determineOffsetArray();

	void updateAccelerationData(int16_t* sensorData);
	void updateVelocityData(int16_t* sensorData);
	void updatePositionData();


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

	//Convert from sensor values to degrees per second
	int16_t getXRotationDeg(){return (int16_t)(((int32_t)positionData[3]*MPU6050_MAX_DEG_S_VALUE)/MPU6050_MAX_SENSOR_VALUE);}
	int16_t getYRotationDeg(){return (int16_t)(((int32_t)positionData[4]*MPU6050_MAX_DEG_S_VALUE)/MPU6050_MAX_SENSOR_VALUE);}
	int16_t getZRotationDeg(){return (int16_t)(((int32_t)positionData[5]*MPU6050_MAX_DEG_S_VALUE)/MPU6050_MAX_SENSOR_VALUE);}
};
