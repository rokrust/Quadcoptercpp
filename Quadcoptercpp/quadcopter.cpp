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
	
}

//Timing sensitive. Used in integration.
void Quadcopter::updateMotionData(){
	motion.update_motion_data();
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