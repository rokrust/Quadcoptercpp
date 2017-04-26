#include "TWI_Master.h"
#include "MPU6050.h"

#include <stdio.h>

void MPU6050::determineOffsetArray(){
	//All sensor variables are two bytes long
	unsigned char readArray[2*N_MESSURE_VAR];

	twi.read_data_from_address(MPU_ADDRESS, ACC_X, readArray, 2*N_MESSURE_VAR);
	
	for(int i = 0; i < 2*N_MESSURE_VAR; i += 2){
		sensorOffset[i/2] = (readArray[i] << 8) | readArray[i + 1];
	}

	//Set downwards to be the negative direction
	sensorOffset[2] += g_SENSOR_VALUE;
}

void MPU6050::updateAccelerationData(int16_t* sensorData){
	for(int i = 0; i < N_TRANS_VAR; i++){
		accelerationData[i] = sensorData[i];
	}
}

void MPU6050::updateVelocityData(int16_t* sensorData){
	for(int i = 0; i < N_TRANS_VAR; i++){
		velocityData[i] += accelerationData[i]/SAMPLING_FREQ;
	}

	for(int i = N_TRANS_VAR + 1; i < N_MESSURE_VAR; i++){
		velocityData[i - 1] = sensorData[i];
	}
}

void MPU6050::updatePositionData(){
	for(int i = 0; i < N_MOTION_VAR; i++){
		//This will probably lead to most values being zero
		positionData[i] += velocityData[i]/SAMPLING_FREQ;
	}
}

void MPU6050::updateDataArrays(int16_t* sensorData){
	updateAccelerationData(sensorData);
	updateVelocityData(sensorData);
	updatePositionData();
}



//Must be called after TWI_Master_intialize() and sei()
MPU6050::MPU6050(){
	printf("Initializing MPU..\n");
	
	determineOffsetArray();

	for(int i = 0; i < N_MESSURE_VAR - 1; i++){
		accelerationData[i % 3] = 0;
		velocityData[i] = 0;
		positionData[i] = 0;
	}
	
	//exit standby mode
	twi.write_data_to_register(MPU_ADDRESS, PWR_MGMT_1, WAKE_UP);
	
	//set gyro sensitivity
	twi.write_data_to_register(MPU_ADDRESS, GYRO_CONFIG, DEG_S_1000);
	
	//Set acceleration sensitivity
	twi.write_data_to_register(MPU_ADDRESS, ACCEL_CONFIG, M_S2_2G);
	
	//Read and store offset values
	updateSensorValues();
	printf("MPU initialized!\n");
}


void MPU6050::updateSensorValues(){
	//Every sensor variable two bytes long
	unsigned char movement_registers[2*N_MESSURE_VAR];
	
	twi.read_data_from_address(MPU_ADDRESS, ACC_X, movement_registers, N_MESSURE_VAR*2);
	
	//Put high and low bytes (excluding address) into sensorData
	int16_t sensorData[N_MESSURE_VAR];
	for(int i = 0; i < 2*N_MESSURE_VAR; i += 2){
		sensorData[i/2] = movement_registers[i] << 8 | movement_registers[i + 1];
	}

	//Add/subtract offset
	calibrateData(sensorData);
	updateDataArrays(sensorData);

}

void MPU6050::calibrateData(int16_t* sensorData){
	for(int i = 0; i < N_MESSURE_VAR; i++){
		sensorData[i] -= sensorOffset[i];
	}
}