#include "TWI_Master.h"
#include "MPU6050.h"
#include <avr/interrupt.h>

#include <stdio.h>

//Calculates the offset by averaging a certain number of samples
//while in the initial state
void MPU6050::_calculate_offset(){
	//All sensor variables are two bytes long
	int32_t cumulative_data[N_MOTION_VAR];
	int16_t prev_data[N_MOTION_VAR];
	
	//Sum # OFFSET_AVG_ITERATIONS samples
	for(int i = 0; i < MPU6050_OFFSET_AVG_ITERATIONS; i++){
		read_motion_data(prev_data);

		for(int i = 0; i < N_MEASURE_VAR; i++){
			cumulative_data[i] += prev_data[i];
		}
	}

	//Take the average of all the data
	for(int i = 0; i < N_MEASURE_VAR; i++){
		cumulative_data[i] /= MPU6050_OFFSET_AVG_ITERATIONS;
	}

	//Split the result into the offset arrays
	for(int i = 0; i < N_TRANS_VAR; i++){
		int j = i + N_TRANS_VAR + 1;

		translational_data.offset[i] = (int16_t)cumulative_data[i];
		rotational_data.offset[i] = (int16_t)cumulative_data[j];
	}
}

void MPU6050::_update_acceleration_data(int16_t *data){
	for(int i = 0; i < N_TRANS_VAR; i++){
		translational_data.acceleration[i] = data[i];
	}
}

void MPU6050::_update_velocity_data(int16_t *data){
	//Integrate data from the accelerometers
	//Save the angular velocity (index j)
	for(uint8_t i = 0; i < N_TRANS_VAR; i++){
		uint8_t j = i + N_TRANS_VAR;

		translational_data.velocity[i] += translational_data.acceleration[i]/SAMPLING_FREQ;
		rotational_data.velocity[i] = data[j];
	}
}

//This must be called by a timer with frequency SAMPLING_FREQ
void MPU6050::_update_position_data(){
	//Integrate angular velocities
	for(uint8_t i = 0; i < N_TRANS_VAR; i++){
		rotational_data.position[i] += rotational_data.velocity[i]/SAMPLING_FREQ;
	}
}

//Saves the array data in the motion structs
void MPU6050::update_motion_data(int16_t *data){
	_calibrate_sensor_data(data);
	_update_acceleration_data(data);
	_update_velocity_data(data);
	_update_position_data();
}



//Must be called after TWI_Master_intialize() and sei()
MPU6050::MPU6050(){
	
	//exit standby mode
	twi.write_data_to_register(MPU_ADDRESS, PWR_MGMT_1, WAKE_UP);
	
	//set gyro sensitivity
	twi.write_data_to_register(MPU_ADDRESS, GYRO_CONFIG, DEG_S_1000);
	
	//Set acceleration sensitivity
	twi.write_data_to_register(MPU_ADDRESS, ACCEL_CONFIG, M_S2_2G);
	
	//Read and store offset values
	_calculate_offset();
}

void MPU6050::config_gyroscope_range(uint8_t range){
	twi.write_data_to_register(MPU_ADDRESS, GYRO_CONFIG, range);
}

void MPU6050::config_accelerometer_range(uint8_t range){
	twi.write_data_to_register(MPU_ADDRESS, ACCEL_CONFIG, range);
}



//Reads the motion registers from the MPU into data[]
void MPU6050::read_motion_data(int16_t *data){
	//Every sensor variable two bytes long
	unsigned char motion_registers[2*N_MEASURE_VAR];
	
	//Read all motion registers
	twi.read_data_from_address(MPU_ADDRESS, ACC_X, motion_registers, N_MEASURE_VAR*2);
	
	//Put high and low bytes into data
	for(int i = 0; i < 2*N_TRANS_VAR; i += 2){
		int j = i + 2*(N_TRANS_VAR + 1); //Used to skip temperature data

		data[i/2] = motion_registers[i] << 8 | motion_registers[i + 1];
		data[j/2] = motion_registers[j] << 8 | motion_registers[j + 1];
	}
}

//Subtracts the offset values from data
void MPU6050::_calibrate_sensor_data(int16_t *data){
	for(uint8_t i = 0; i < N_TRANS_VAR; i++){
		uint8_t j = i + N_TRANS_VAR;
		
		data[i] -= translational_data.offset[i];
		data[j] -= rotational_data.offset[i];
	}
}