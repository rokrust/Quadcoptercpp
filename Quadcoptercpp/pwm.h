#pragma once
#include <stdint.h>

class Pwm{
	private:
	uint8_t outputCmp;

	public:
	Pwm();
	Pwm(uint8_t outputCmp);
	void enable();
	void setCompare(uint8_t	cmp);
	void disable();
};
