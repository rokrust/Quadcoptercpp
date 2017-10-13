#include "HMC5883L.h"
#include <math.h>

HMC5883L::HMC5883L(){
	uint8_t data[1];
	
	//Average over four samples and set output rate to 15 Hz
	data[0] = (1 << MA1) | (1 << DO2);
	_twi.write_data_to_register(HMC5883L_ADDRESS, HMC5883L_CONFIG_A, data);
	
	//Set I2C to high speed mode and chip to continuous measurement mode
	data[0] = (1 << HS);
	_twi.write_data_to_register(HMC5883L_ADDRESS, HMC5883L_MODE, data);
}

void HMC5883L::read_raw_data(int16_t *raw_data){
	uint8_t data[HMC5883L_N_RAW_DATA_BYTES];
	
	//read 8 bit data
	_twi.read_data_from_address(HMC5883L_ADDRESS, HMC5883L_X_OUT_BASE, data, HMC5883L_N_RAW_DATA_BYTES);
	
	//Concatenate x, y and z bytes in three 16 bit variables
	for(int i = 0; i < HMC5883L_N_RAW_DATA_BYTES; i += 2){
		raw_data[i/2] = ((int16_t)data[i] << 8) | data[i + 1];
	}
}

//Most likely not used
void HMC5883L::calculate_orientation(int16_t *raw_data){
	int16_t pitch, yaw, roll;
	
	pitch = atan2(raw_data[2], raw_data[1]);
	roll = atan2(-raw_data[0], raw_data[1]);
	yaw = atan2(raw_data[0], raw_data[2]);
}