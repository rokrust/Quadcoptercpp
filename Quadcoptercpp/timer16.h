#ifndef __TIMER16_H__
#define __TIMER16_H__
#include <stdint.h>

class Timer16{
private:
	uint16_t outputCmp;
public:
	Timer16();
	Timer16(uint16_t outputCmp);
	void enable();
	void disable();
};

#endif