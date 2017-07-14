#include "TWI_Master.h"
#include "MPU6050.h"
#include <avr/interrupt.h>

#include <stdio.h>

void MPU6050::determineOffsetArray(){
	//All sensor variables are two bytes long
	unsigned char readArray[2*N_MESSURE_VAR];

	twi.read_data_from_address(MPU_ADDRESS, ACC_X, readArray, 2*N_MESSURE_VAR);
	
	for(int i = 0; i < 2*N_MESSURE_VAR; i += 2){
		sensorOffset[i/2] = (readArray[i] << 8) | readArray[i + 1];
	}

}

void MPU6050::updateAccelerationData(){
	for(int i = 0; i < N_TRANS_VAR; i++){
		accelerationData[i] = sensorData[i];
	}
}

void MPU6050::updateVelocityData(){
	//Integrate data from the accelerometers
	for(int i = 0; i < N_TRANS_VAR; i++){
		velocityData[i] += accelerationData[i]/SAMPLING_FREQ;
	}

	//Read the angular velocities directly
	for(int i = N_TRANS_VAR + 1; i < N_MESSURE_VAR; i++){
		velocityData[i - 1] = sensorData[i];
	}
}

//This must be called by a timer with frequency SAMPLING_FREQ
void MPU6050::updatePositionData(){
	//Integrate angular velocities
	for(int i = 0; i < N_MOTION_VAR; i++){
		positionData[i] += velocityData[i]/SAMPLING_FREQ;
	}
}

void MPU6050::updateDataArrays(){
	updateAccelerationData();
	updateVelocityData();
	updatePositionData();
}



//Must be called after TWI_Master_intialize() and sei()
MPU6050::MPU6050(){
	for(int i = 0; i < N_MOTION_VAR; i++){
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
	determineOffsetArray();
}

//Reads the motion registers from the MPU and saves the data in sensorData
void MPU6050::readMotionData(){
	//Every sensor variable two bytes long
	unsigned char motion_registers[2*N_MESSURE_VAR];
	
	//Read all motion registers
	twi.read_data_from_address(MPU_ADDRESS, ACC_X, motion_registers, N_MESSURE_VAR*2);
	
	//Put high and low bytes into sensorData
	for(int i = 0; i < 2*N_MESSURE_VAR; i += 2){
		sensorData[i/2] = motion_registers[i] << 8 | motion_registers[i + 1];
	}

	//Add/subtract offset
	calibrateSensorData();
}

void MPU6050::calibrateSensorData(){
	for(int i = 0; i < N_MESSURE_VAR; i++){
		sensorData[i] -= sensorOffset[i];
	}
}