#pragma once 

#include <stdint.h>

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
