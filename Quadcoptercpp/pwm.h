#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#define ESC_MAX_PWM 128
#define ESC_MIN_PWM 64
#define ESC_CALIBRATION_MS_DELAY 3000;
class Pwm{
private:
	uint8_t outputCmp;
	volatile uint8_t* outputCmpRegister;
	int timerNumber;

	void initializeTimer0A();
	void initializeTimer0B();
	void initializeTimer2A();
	void initializeTimer2B();

public:
	Pwm(int timerNumber);
	Pwm(int timerNumber, uint8_t outputCmp);
	Pwm(): Pwm(0){}

	void enable();
	void setCompare(uint8_t	cmp);
	void disable();
};

#endif
