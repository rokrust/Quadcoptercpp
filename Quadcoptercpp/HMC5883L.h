#include <stdint.h>

#define HMC5883L_ADDRESS 0x1E
#define HMC5883L_CONFIG_A 0x00
	#define MA1 6
	#define MA0 5
	#define DO2 4
	#define DO1 3
	#define DO0 2
	#define MS1 1
	#define MS0 0
	
#define HMC5883L_CONFIG_B 0x01
#define HMC5883L_MODE 0x02
#define HMC5883L_X_OUT_BASE 0x03
#define HMC5883L_Z_OUT_BASE 0x05
#define HMC5883L_Y_OUT_BASE 0x07
#define HMC5883L_STATUS 0x09
#define HMC5883L_ID_A 0x0A
#define HMC5883L_ID_B 0x0B
#define HMC5883L_ID_C 0x0C

class HMC5883L{
private:
	TWI _twi;

public:
	HMC5883L();
	void read_raw_data();
	void calculate_orientation(int16_t *orientation);
	
};