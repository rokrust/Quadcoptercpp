#include "MotionControl.h"

void Motion_processor::_update_acceleration_data(int16_t *data){
	for(int i = 0; i < N_TRANS_VAR; i++){
		translational_data.acceleration[i] = data[i];
	}
}

void Motion_processor::_update_velocity_data(int16_t *data){
	//Integrate data from the accelerometers
	//Save the angular velocity (index j)
	for(uint8_t i = 0; i < N_TRANS_VAR; i++){
		uint8_t j = i + N_TRANS_VAR;

		translational_data.velocity[i] += translational_data.acceleration[i]/SAMPLING_FREQ;
		rotational_data.velocity[i] = data[j];
	}
}

//This must be called by a timer with frequency SAMPLING_FREQ
void Motion_processor::_update_position_data(){
	//Integrate angular velocities
	for(uint8_t i = 0; i < N_TRANS_VAR; i++){
		rotational_data.position[i] += rotational_data.velocity[i]/SAMPLING_FREQ;
	}
}

void read_raw_motion_data(){
	
}


//Read IMU values and save in the motion structs
void Motion_processor::update_motion_data(){
	int16_t data[N_MOTION_VAR];
	imu.read_calibrated_motion_data(data);

	_update_acceleration_data(data);
	_update_velocity_data(data);
	_update_position_data();
}
