#include "Drivers/Sensors/gps.h"
#include "Drivers/Sensors/NRF24L01.h"
#include "Controller/motor_control.h"
#include "Drivers/Timer/timer16.h"
#include "config.h"
#include "Controller/MotionControl.h"
#include "Drivers/Sensors/BMP180.h"

class Quadcopter{
private:
	//Motion_processor motion;
	BMP180 bmp;
	//GPS gps;
	//NRF24L01 transceiver;
	//MotorControl controller;
	//Timer16 samplingTimer;

	uint8_t radioMsg[PAYLOAD_WIDTH];

public:
	Quadcopter();
	
	//Read values from the IMU
	void readIMU();

	//Calculate acceleration/angular acceleration, velocity/angular velocity 
	//and position/angles from IMU data
	void update_motion_data();

	//Update motor speeds
	void updateController();

	//Read data from radio controller
	void recieveRemotePayload();

	void test1(){
		bmp.read_raw_data();
		int32_t temperature = bmp.calculate_temperature();
		uint32_t p = bmp.calculate_pressure();
		//int16_t altitude = bmp.calculate_altitude();
 		printf("Pressure: %ld\nTemperature: %d\n", p, temperature);
		//samplingTimer.setToTickMode(); 
		//samplingTimer.enable();
	}
	
	void test2(){
		//uint32_t timeStamp = samplingTimer.currentTime(); 
		//printf("time: %lu\n", timeStamp); 
		//samplingTimer.setToInterruptMode(SAMPLING_FREQ);
		//samplingTimer.enable();
	}

	void test3(){
		//printf("x: %d, y: %d\n", radioMsg[0], radioMsg[1]);
		//printf("x: %d, y: %d, z: %d\n", mpu.getXRotationDeg(), mpu.getYRotationDeg(), mpu.getZRotationDeg());
	}
};