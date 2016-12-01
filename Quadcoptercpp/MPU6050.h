#ifndef __MPU6050_H__
#define __MPU6050_H__ 

#include "TWI_master.h"

#define MPU_ADDRESS 0x68

#define PWR_MGMT_1 0x6B
#define WAKE_UP 0x00
#define ACC_X 0x3B

#define WRITE_FLAG 0
#define READ_FLAG 1

#define N_MESSURE_VAR 7

class MPU6050{
private:
	TWI twi;
	int sensorData[7];
	int sensorOffset[7];

public:
	MPU6050();
	void update_movement();
	int getMovementData(int index){return sensorData[index];}
};
#endif