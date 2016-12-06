#pragma once

class Motor{
private:
	Pwm16 pwm;

public:
	Motor();
	Motor(double pwm_freq);
	
	
};
