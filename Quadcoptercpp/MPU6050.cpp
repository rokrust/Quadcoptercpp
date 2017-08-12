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

		for(int i = 0; i < N_MOTION_VAR; i++){
			cumulative_data[i] += prev_data[i];
		}
	}

	//Take the average of all the data
	//and save the result in offset array
	for(int i = 0; i < N_MOTION_VAR; i++){
		cumulative_data[i] /= MPU6050_OFFSET_AVG_ITERATIONS;
		offset[i] = cumulative_data[i];
	}
}


//Must be called after TWI_Master_intialize() and sei()
MPU6050::MPU6050(){
	uint8_t data[1];
	
	//exit standby mode
	data[0] = WAKE_UP;
	twi.write_data_to_register(MPU_ADDRESS, PWR_MGMT_1, data);
	
	//set gyro sensitivity
	data[0] = DEG_S_1000;
	twi.write_data_to_register(MPU_ADDRESS, GYRO_CONFIG, data);
	
	//Set acceleration sensitivity
	data[0] = M_S2_2G;
	twi.write_data_to_register(MPU_ADDRESS, ACCEL_CONFIG, data);
	
	//Set external interrupt to active low
	//data[0] = ~(1 << INT_LEVEL);
	//twi.write_data_to_register(MPU_ADDRESS, INT_PIN_CFG, data);

	//Enable data ready interrupt, disable all else
	//data[0] = (1 << DATA_RDY_EN);
	//twi.write_data_to_register(MPU_ADDRESS, INT_ENABLE, data);

	//Read and store offset values
	_calculate_offset();
}

void MPU6050::config_gyroscope_range(uint8_t range){
	twi.write_data_to_register(MPU_ADDRESS, GYRO_CONFIG, &range);
}

void MPU6050::config_accelerometer_range(uint8_t range){
	twi.write_data_to_register(MPU_ADDRESS, ACCEL_CONFIG, &range);
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

//Same as read_motion_data but with offset values subtracted
void MPU6050::read_calibrated_motion_data(int16_t *data){
	_calibrate_sensor_data(data);
	read_motion_data(data);
}

//Subtracts the offset values from data
void MPU6050::_calibrate_sensor_data(int16_t *data){
	for(uint8_t i = 0; i < N_TRANS_VAR; i++){
		uint8_t j = i + N_TRANS_VAR;
		
		data[i] -= offset[i];
		data[j] -= offset[i];
	}
}
