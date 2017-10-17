#include "math.h"
#include <cmath>

#define INT_SIZE 16

Fixfloat_t Fixfloat_t::operator+(const Fixfloat_t &rhs){
		unsigned int d = this->decimals + rhs.decimals;
		uint8_t e = this->exponent;
		
		return Fixfloat_t(d, e);
}

//Needs an underflow check
Fixfloat_t Fixfloat_t::operator-(const Fixfloat_t &rhs){
	unsigned int d = this->decimals - rhs.decimals;
	uint8_t e = this->exponent;
	
	return Fixfloat_t(d, e);
}

//Inlcudes overflow check
Fixfloat_t Fixfloat_t::operator*(const Fixfloat_t &rhs){
	bool lhs_sign = signbit(this->exponent);
	bool rhs_sign = signbit(rhs.exponent);
	
	unsigned int decimals = this->decimals * rhs.decimals;
	uint8_t exponent = this->exponent + rhs.exponent;
	
	//Overflow check and fix. Should maintain highest possible precision
	if(decimals < this->decimals || decimals < rhs.decimals){
		//Stuff
	}
	
	return Fixfloat_t(decimals, exponent);
}


void Quaternion::calculate_q_dot(int16_t omega){
	q_dot[0] = (epsilon[3]*omega[0] - epsilon[2]*omega[1] + epsilon[1]*omega[2]) / 2;
	q_dot[1] = (epsilon[2]*omega[0] + epsilon[3]*omega[1] - eta*omega[2]) / 2;
	q_dot[2] = (-epsilon[1]*omega[0] + eta*omega[1] + epsilon[3]*omega[2]) / 2;
	q_dot[3] = (-eta*omega[0] - epsilon[1]*omega[1] - epsilon[2]*omega[2]) / 2;	
}

//https://www.ncbi.nlm.nih.gov/pmc/articles/PMC5017380/
void Quaternion::calculate_rotation_matrix(int16_t **R){
	int32_t q_00 = q[1]*q[1], q_11 = q[1]*q[1], q_22 = q[2]*q[2], q_33 = q[3]*q[3];
	int32_t q_01 = q[0]*q[1], q_02 = q[0]*q[2], q_03 = q[0]*q[3];
	int32_t q_12 = q[1]*q[2], q_13 = q[1]*q[3], q_23 = q[2]*q[3];
	int32_t Q = q_00 + q_11 + q_22 + q_33;
		
	/*
	R[0][0] = 1 - 2*(q_22 + q_33);	R[0][1] = 2*(q_12 - q_03);		R[0][2] = 2*(q_13 - q_02);
	R[1][0] = 2*(q_12 + q_03);		R[1][1] = 1 - 2*(q_11 + q_33);	R[1][2] = 2*(q_23 - q_01);
	R[2][0] = 2*(q_13 - q_02);		R[2][1] = 2*(q_23 + q_01);		R[2][2] = 1 - 2*(q_11 + q_22);
	*/
		
	R[0][0] = (q_00+q_11-q_22-q_33)/Q;	R[0][1] = (2*(q_12-q_03))/Q;		R[0][2] = (2*(q_02 + q_13))/Q;
	R[1][0] = (2*(q_03 + q_12))/Q;		R[1][1] = (q_00-q_11+q_22-q_33)/Q;	R[1][2] = (2*(q_23 - q_01))/Q;
	R[2][0] = (2*(q_13 - q_02))/Q;		R[2][1] = (2*(q_01+q_23))/Q;		R[2][2] = (q_00-q_11-q_22+q_33)/Q;
}

//Time sensitive
void Quaternion::integrate_q_dot(Quaternion q_dot){
	eta += q_dot.eta / SAMPLING_FREQ;
	epsilon[0] += q_dot.epsilon[0] / SAMPLING_FREQ;
	epsilon[1] += q_dot.epsilon[1] / SAMPLING_FREQ;
	epsilon[2] += q_dot.epsilon[2] / SAMPLING_FREQ;
}

Quaternion Quaternion::operator*(const Quaternion &q){
	Quaternion return_val;
	
	return_val.eta = eta*q.eta - epsilon[0]*q.epsilon[0] - epsilon[1]*q.epsilon[1] - epsilon[2]*q.epsilon[2];
	return_val.epsilon[0] = epsilon[0]*q.eta + eta - epsilon[2]*q.epsilon[1] + epsilon[1]*q.epsilon[2];
	return_val.epsilon[1] = epsilon[1]*q.eta + eta + epsilon[2]*q.epsilon[0] - epsilon[0]*q-epsilon[2];
	return_val.epsilon[2] = epsilon[2]*q.eta + eta - epsilon[1]*q.epsilon[0] + epsilon[0]*q-epsilon[1];
	
	return return_val;
}