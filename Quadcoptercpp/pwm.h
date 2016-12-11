#ifndef PWM_H
#define PWM_H

#include <stdint.h>

class Pwm{
private:
	uint8_t outputCmp;
	volatile uint8_t* outputCmpRegister;

	void setOutputCompareRegister(int timerNumber);

public:
	Pwm();
	Pwm(uint8_t outputCmp, int timerNumber);
	void enable();
	void setCompare(uint8_t	cmp);
	void disable();
};

#endif
