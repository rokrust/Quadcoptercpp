#include "gps.h"
#include "lcd.h"
#include "NRF24L01.h"
#include "motor_control.h"
#include "timer16.h"
#include "config.h"
#include "MotionControl.h"

class Quadcopter{
private:
	Motion_processor motion;
	//GPS gps;
	NRF24L01 transceiver;
	MotorControl controller;
	Timer16 samplingTimer;

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
		samplingTimer.setToTickMode(); 
		samplingTimer.enable();
	}
	
	void test2(){
		uint32_t timeStamp = samplingTimer.currentTime(); 
		printf("time: %lu\n", timeStamp); 
		samplingTimer.setToInterruptMode(SAMPLING_FREQ);
		samplingTimer.enable();
	}

	void test3(){
		//printf("x: %d, y: %d\n", radioMsg[0], radioMsg[1]);
		//printf("x: %d, y: %d, z: %d\n", mpu.getXRotationDeg(), mpu.getYRotationDeg(), mpu.getZRotationDeg());
	}
};