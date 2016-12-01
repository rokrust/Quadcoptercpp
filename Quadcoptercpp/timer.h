#pragma once
#include <stdint.h>

class Timer{
private:
	uint8_t outputCmp;

public:
	Timer();
	Timer(uint8_t outputCmp);
	void enable();
	void disable();
};
