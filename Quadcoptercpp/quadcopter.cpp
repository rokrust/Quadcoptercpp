#include "quadcopter.h"
#include "usart.h"

Quadcopter::Quadcopter(){
	fdevopen((int (*)(char,  struct __file *))USART_transmit, (int (*)(struct __file *))USART_receive); //Link printf to USB
	USART_init(MYUBRR);


	controller = MotorControl();
	
	cli();
	samplingTimer = Timer16(SAMPLING_FREQ);
	samplingTimer.enable();

	//Enable external interrupts
	printf("Quadcopter initialized!\n");
	sei();
}

void Quadcopter::updateControllerInputs(){
	uint8_t joystickZ = radioMsg[0];

	controller.determineMotorInputs(joystickZ);
	controller.setMotorInputs();
}

void Quadcopter::recieveRemotePayload(){
	//transciever.recieve(radioMsg);
	//transciever.listen();
}