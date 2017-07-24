#include "MPU6050.h"

struct Motion_data{
	int16_t acceleration[N_TRANS_VAR];
	int16_t velocity[N_TRANS_VAR];
	int16_t position[N_TRANS_VAR];
};

class Motion_processor{
private:
	MPU6050 imu;
	Motion_data translational_data;
	Motion_data rotational_data;


	void _integrate_acceleration();
	void _integrate_angular_velocity();
	void _update_position_data();

public:
	void read_raw_motion_data();
	void process_raw_motion_data();
	void clear_motion_data();

	struct Motion_data get_rotational_data() {return rotational_data;}
	struct Motion_data get_translational_data() {return translational_data;}
};

/*//Convert from sensor values to degrees per second
	int16_t getXRotationDeg(){return (int16_t)(((int32_t)positionData[3]*MPU6050_MAX_DEG_S_VALUE)/MPU6050_MAX_SENSOR_VALUE);}
	int16_t getYRotationDeg(){return (int16_t)(((int32_t)positionData[4]*MPU6050_MAX_DEG_S_VALUE)/MPU6050_MAX_SENSOR_VALUE);}
	int16_t getZRotationDeg(){return (int16_t)(((int32_t)positionData[5]*MPU6050_MAX_DEG_S_VALUE)/MPU6050_MAX_SENSOR_VALUE);}*/
