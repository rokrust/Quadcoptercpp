#define F_CLOCK 16000000 //Should probably declare this in a designated header file

Motor::Motor(){



}

Motor::Motor(double pwm_freq){
	int topVal = (int)(F_CLOCK/pwm_freq)-1;

	pwm = Pwm16(topVal, 0); //Start esc with lowest value
	

}
