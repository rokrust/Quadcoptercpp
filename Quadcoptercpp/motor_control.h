#define ESC_MAX_VALUE 0x03ff;
#define ESC_MIN_VALUE 0x00ff;

//Should probably use an ESC or Motor middle class here
//and have an array of Motor/ESC classes as members
class MotorControl{
private:
	//Might use an 8 bit pwm as well
	Pwm16 pwm;

public:
	MotorControl();


};