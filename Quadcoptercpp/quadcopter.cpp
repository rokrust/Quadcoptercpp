#include "quadcopter.h"

Quadcopter::Quadcopter(){
	samplingTimer = Timer16(F_CPU/SAMPLING_FREQ);
}

void Quadcopter::updateControllerInputs(){
	uint8_t joystickZ = radioMsg[0];

	controller.determineMotorInputs(joystickZ);
	controller.setMotorInputs();
}

void Quadcopter::recieveRemotePayload(){
	transciever.recieve(radioMsg);
}