#include "gps.h"
#include "lcd.h"
#include "radiocontroller.h"
#include "motor_control.h"

class Quadcopter{
private:
	MotorControl controller;
	GPS gps;
	Lcd lcd;
	RadioController radio;

public:
	Quadcopter();
};