#pragma once

#include <stdint.h>

class Timer16{
private:
	uint8_t prescalerIndex;

public:
	Timer16();
	Timer16(uint16_t timerFreq);
	
	void enable();
	void disable();
	
	void setFrequency(uint16_t timerFreq);
	void setToInterruptMode(uint16_t timerFreq);

	void setToTickMode();
	uint32_t currentTime();
};
