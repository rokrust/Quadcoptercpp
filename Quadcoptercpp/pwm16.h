#include "timer16.h"
#include "adc.h"
#include "MPU6050.h"

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//The top value of this 16bit can be changed to yield different frequencies
//(Good for speakers hopefully)

class Pwm16{
private:
	uint32_t topVal;
	uint16_t outputCmp;

public:
	Pwm16::Pwm16();

	Pwm16(uint32_t topVal, uint16_t outputCmp);

	void enable();

	void disable();

};