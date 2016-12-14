#include "TWI_Master.h"
#include "MPU6050.h"

#include <stdio.h>
#include <util/delay.h>

void MPU6050::determineOffsetArray(){
	unsigned char readArray[2*N_MESSURE_VAR];

	twi.read_data_from_address(MPU_ADDRESS, ACC_X, readArray, 2*N_MESSURE_VAR);
	
	for(int i = 0; i < 2*N_MESSURE_VAR; i += 2){
		sensorOffset[i/2] = (readArray[i] << 8) | readArray[i + 1];
	}
}

void MPU6050::updateDataArrays(int16_t* sensorData){
	for(int i = 0; i < N_TRANS_VAR; i++){
		accelerationData[i] = sensorData[i];
		velocityData[i] += (sensorData[i] * MS_SAMPLING_TIME)/1000;
		positionData[i] += (velocityData[i]* MS_SAMPLING_TIME)/1000;

		//Skip temperature data
		velocityData[i + N_TRANS_VAR] = sensorData[i + N_TRANS_VAR + 1];
		positionData[i + N_TRANS_VAR] = (velocityData[i + N_TRANS_VAR] * MS_SAMPLING_TIME)/1000;
	}	
}


//Must be called after TWI_Master_intialize() and sei()
MPU6050::MPU6050(){
	determineOffsetArray();

	for(int i = 0; i < N_MESSURE_VAR - 1; i++){
		accelerationData[i % 3] = 0;
		velocityData[i] = 0;
		positionData[i] = 0;
	}

	unsigned char init_data[3] = {MPU_ADDRESS << 1 | WRITE_FLAG, PWR_MGMT_1, WAKE_UP};
	printf("Initializing mpu..\n");
	twi.start_transceiver_with_data(init_data, 3);
	
	//Read and store offset values
	updateSensorValues();

}


void MPU6050::updateSensorValues(){
	//Every variable represented by two values (2 bytes), +1 for the MPU address
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

