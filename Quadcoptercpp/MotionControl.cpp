#include "MotionControl.h"

void Motion_processor::_integrate_acceleration(){
	for(uint8_t i = 0; i < N_TRANS_VAR; i++){
		translational_data.velocity[i] += translational_data.acceleration[i]/SAMPLING_FREQ;
	}
}

void Motion_processor::_integrate_angular_velocity(){
	for(uint8_t i = 0; i < N_ROT_VAR; i++){
		rotational_data.position[i] += rotational_data.velocity[i]/SAMPLING_FREQ;
	}
}

//Reads the raw data from the IMU and saves it in the motion structs
void Motion_processor::read_raw_motion_data(){
	int16_t data[N_MOTION_VAR];
	imu.read_calibrated_motion_data(data);

	for(uint8_t i = 0; i < N_TRANS_VAR; i++){
		uint8_t j = N_TRANS_VAR + i;

		translational_data.acceleration[i] = data[i];
		rotational_data.velocity[i] = data[j];
	}
}


//Integrate and do other time sensitive operations
void Motion_processor::process_raw_motion_data(){
	_integrate_angular_velocity();
	_integrate_acceleration();
}

//Sets every value in the motion structs to zero
void Motion_processor::clear_motion_data(){
	for(uint8_t i = 0; i < N_TRANS_VAR; i++){
		translational_data.acceleration[i] = 0;
		translational_data.velocity[i] = 0;
		translational_data.position[i] = 0;

		rotational_data.acceleration[i] = 0;
		rotational_data.velocity[i] = 0;
		rotational_data.position[i] = 0;
	}
}

/********************* Sensor Fusion *************************/
/*
//exponential moving avverage
accel_mag_ema = a * accel_mag + (1 - a) * accel_mag_ema;

//Subtract gyro bias
gyro_ema = b * gyro_raw + (1 - b) * gyro_ema;
gyro_emd = c * abs(gyro_raw - gyro_ema) + (1 - c) * gyro_emd;
d = (gyro_emd > thresh) ? low_val : high_val;
gyro_bias = d * gyro_raw + (1 - d) * gyro_bias;

orient = accel_mag_ema + gyro_raw - gyro_bias;
*/