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
	motion.read_raw_motion_data();
}

//Timing sensitive. Used in integration.
void Quadcopter::updateMotionData(){
	motion.process_raw_motion_data();
}

void Quadcopter::updateController(){
	uint8_t joystickZ = radioMsg[0];
	Motion_data rotation = motion.get_rotational_data();
	int16_t x = rotation.position[0], y = rotation.position[1], z = rotation.position[2];

	controller.determineMotorInputs(x, y, z, joystickZ);
	//controller.setMotorInputs();
}

void Quadcopter::recieveRemotePayload(){
	transceiver.receive(radioMsg);
	transceiver.listen();
}