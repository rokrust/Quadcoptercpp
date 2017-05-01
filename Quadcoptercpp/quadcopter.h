#include "gps.h"
#include "lcd.h"
#include "NRF24L01.h"
#include "motor_control.h"
#include "timer16.h"

#define F_CPU 16000000

class Quadcopter{
private:
	MotorControl controller;
	//GPS gps;
	//Lcd lcd;
	NRF24L01 transceiver;
	Timer16 samplingTimer;

	uint8_t radioMsg[PAYLOAD_WIDTH];

public:
	Quadcopter();

	void updateSensors();

	void updateController();
	void recieveRemotePayload();

	void test(){controller.test();}
};