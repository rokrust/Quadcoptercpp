float fast_inverse_sqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
	//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

//Float type between 0 and 1 with guards against over- and underflow.
class Fixfloat_t{
private:
	unsigned int decimals;
	
	//Mathematically exponent will always be zero and way less than 255
	//Sign bit will therefore be saved here.
	int8_t exponent;
	
public:	
	Fixfloat_t(): decimals(0), exponent(0){}
	Fixfloat_t(unsigned int d, uint8_t e): decimals(d), exponent(e){}

	//Operators
	Fixfloat_t operator+(const Fixfloat_t &rhs);
	Fixfloat_t operator-(const Fixfloat_t &rhs);
	Fixfloat_t operator*(const Fixfloat_t &rhs);
	Fixfloat_t operator+=(Fixfloat_t rhs);
	Fixfloat_t operator-=(Fixfloat_t rhs);
	Fixfloat_t operator*=(Fixfloat_t rhs);
};

class Quaternion{
private:
	Fixfloat_t eta;
	Fixfloat_t epsilon;
	Fixfloat_t q_dot[4];

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