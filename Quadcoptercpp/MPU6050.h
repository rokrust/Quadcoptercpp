#pragma once

#include "TWI_master.h"
#include "config.h"

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
#define N_MEASURE_VAR 7 //Acceleration, angular velocity and temperature
#define g_SENSOR_VALUE 16384
#define MPU6050_MAX_SENSOR_VALUE 32767
#define MPU6050_MAX_DEG_S_VALUE 250
#define MPU6050_OFFSET_AVG_ITERATIONS 50

class MPU6050{
private:
	TWI twi;
	
	int16_t offset[N_MOTION_VAR];

	void _calculate_offset();

	void _update_acceleration_data(int16_t *data);
	void _update_velocity_data(int16_t *data);
	void _update_position_data();
	void _calibrate_sensor_data(int16_t *data);

public:
	MPU6050();

	void config_gyroscope_range(uint8_t range);
	void config_accelerometer_range(uint8_t range);

	//Used for receiving and storing motion data.
	void read_motion_data(int16_t *data);
	void read_calibrated_motion_data(int16_t *data);
};
