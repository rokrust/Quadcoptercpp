#pragma once

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>


//The top value of this 16bit can be changed to yield different frequencies
//(Good for speakers hopefully)

class Pwm16{
private:
	uint16_t topVal;
	uint16_t outputCmp;

public:
	Pwm16(uint16_t topVal, uint16_t outputCmp);
	Pwm16();

	//Should probably scale this to the top value
	void setCompare(int outputCmp){this->outputCmp = outputCmp;}
	uint16_t getTopValue(){return topVal;}

	void enable();
	void disable();

};
