
#include <stdint.h>

#include<cmath>
#include<stdio.h>


//Float type between 0 and 1 with guards against over- and underflow.
class Fixfloat_t{
private:
	unsigned int decimals;
	
	//Mathematically exponent will always be less than zero and way less than 255
	//Sign bit will therefore be saved here.
	uint8_t exponent;

public:	
	Fixfloat_t(): decimals(0), exponent(0){}
	Fixfloat_t(unsigned int d, uint8_t e): decimals(d), exponent(e){}
	Fixfloat_t(Fixfloat_t n): decimals(n.decimals), exponent(n.exponent){}


	//Operators
	Fixfloat_t operator+(const Fixfloat_t &rhs);
	Fixfloat_t operator-(const Fixfloat_t &rhs);
	Fixfloat_t operator*(Fixfloat_t rhs);
	Fixfloat_t operator+=(Fixfloat_t rhs);
	Fixfloat_t operator-=(Fixfloat_t rhs);
	Fixfloat_t operator*=(Fixfloat_t rhs);

        
	//Debug
	void print() { printf("%f\n",(float)this->decimals/pow(2,this->exponent));}
};

class Quaternion{
private:
	Fixfloat_t eta;
	Fixfloat_t epsilon;
	
public:
	//Uses angular velocity
	void calculate_q_dot(int16_t omega);
	void calculate_rotation_matrix(int16_t **R);
	void integrate_q_dot();
	
	Quaternion operator*(const Quaternion &q);
};

class INS{

private:
	Quaternion q;
	int16_t omega;
	int16_t v;
	int16_t v_dot;
	int16_t a;

public:
	void calculate_v_dot(){
		v_dot[0] = 2*(omega[2]*v[1] - omega[1]*v[2]) + a[0];
		v_dot[1] = 2*(omega[0]*v[2] - omega[2]*v[0]) + a[1];
		v_dot[2] = 2*(omega[1]*v[0] - omega[0]*v[1]) + a[2] + 10; //10 is actually 9.81
	}
	
};
