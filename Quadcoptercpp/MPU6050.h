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

struct Motion_data{
	int16_t acceleration[N_TRANS_VAR];
	int16_t velocity[N_TRANS_VAR];
	int16_t position[N_TRANS_VAR];
	int16_t offset[N_TRANS_VAR];

};

class MPU6050{
private:
	TWI twi;
	
	Motion_data translational_data;
	Motion_data rotational_data;

	void _calibrate_sensor_data(int16_t *data);
	void _calculate_offset();

	void _update_acceleration_data(int16_t *data);
	void _update_velocity_data(int16_t *data);
	void _update_position_data();


public:
	MPU6050();

	void config_gyroscope_range(uint8_t range);
	void config_accelerometer_range(uint8_t range);

	//Used for receiving and storing motion data.
	void read_motion_data(int16_t *data);
	void update_motion_data(int16_t *data);

	int16_t* getTranslation(){return translational_data.position;}
	int16_t* getRotation(){return rotational_data.position;}
	
	struct Motion_data get_rotational_data(){return rotational_data;}
	struct Motion_data get_translational_data(){return translational_data;}

	/*//Convert from sensor values to degrees per second
	int16_t getXRotationDeg(){return (int16_t)(((int32_t)positionData[3]*MPU6050_MAX_DEG_S_VALUE)/MPU6050_MAX_SENSOR_VALUE);}
	int16_t getYRotationDeg(){return (int16_t)(((int32_t)positionData[4]*MPU6050_MAX_DEG_S_VALUE)/MPU6050_MAX_SENSOR_VALUE);}
	int16_t getZRotationDeg(){return (int16_t)(((int32_t)positionData[5]*MPU6050_MAX_DEG_S_VALUE)/MPU6050_MAX_SENSOR_VALUE);}*/
};
