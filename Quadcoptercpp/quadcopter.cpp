#include "quadcopter.h"
#include "usart.h"

Quadcopter::Quadcopter(){
	fdevopen((int (*)(char,  struct __file *))USART_transmit, (int (*)(struct __file *))USART_receive); //Link printf to USB
	USART_init(MYUBRR);

	controller = MotorControl();

	samplingTimer = Timer16(SAMPLING_FREQ);
	samplingTimer.enable();

	transceiver.listen();
	
	//Enable external interrupts
	sei();
}

void Quadcopter::readIMU(){
	//mpu.read_motion_data();
}

//Timing sensitive. Used in integration.
void Quadcopter::updateMotionData(){
	//mpu.update_motion_arrays();
}

void Quadcopter::updateController(){
	uint8_t joystickZ = radioMsg[0];
	
	//controller.determineMotorInputs(mpu.getXRotationDeg(), mpu.getYRotationDeg(), mpu.getZRotationDeg(), joystickZ);
	//controller.setMotorInputs();
}

void Quadcopter::recieveRemotePayload(){
	transceiver.receive(radioMsg);
	transceiver.listen();
}