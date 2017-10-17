#pragma once
#include "Drivers/Communication/TWI_master.h"
#include <stdint.h>

#define BMP_ADDRESS 0x77

#define BMP_EEPROM_START_ADDR 0xAA
#define BMP_EEPROM_SIZE 22

#define BMP_CTRL_MEAS 0xF4
	#define BMP_OSS_1 0
	#define BMP_OSS_2 (1 << 6)
	#define BMP_OSS_4 (2 << 6)
	#define BMP_OSS_8 (3 << 6)

#define DATA_REG_BASE_ADDR 0xF6

#define BMP180_TEMP_READ_CMD                 0x2E // temperature measurement
#define BMP180_PRES_READ_CMD				 0x34 //Add _oss value for correct address
#define BMP180_P1_ADDR               0x34 // pressure measurement (OSS=0, 4.5ms)
#define BMP180_P2_ADDR               0x74 // pressure measurement (OSS=1, 7.5ms)
#define BMP180_P4_ADDR               0xB4 // pressure measurement (OSS=2, 13.5ms)
#define BMP180_P8_ADDR               0xF4 // pressure measurement (OSS=3, 25.5ms)

#define BMP180_P_RESOLUTION_BYTE 3
#define BMP180_T_RESOLUTION_BYTE 2


// Calibration parameters structure
typedef struct {
	int16_t AC1;
	int16_t AC2;
	int16_t AC3;
	uint16_t AC4;
	uint16_t AC5;
	uint16_t AC6;
	int16_t B1;
	int16_t B2;
	int16_t MB;
	int16_t MC;
	int16_t MD;
	int32_t B5;
} BMP180_calibration;

class BMP180{
private:
	TWI _twi;
	BMP180_calibration _calibration_parameters;
	uint8_t _n_oversamples;
	uint16_t _conversion_time;
	uint8_t _P_addr;
	uint32_t P_0;
	int16_t _raw_temperature_data;
	uint32_t _raw_pressure_data;
	uint8_t _oss;
	
	void _read_calibration_parameters();
	
public:
	BMP180(uint8_t oss = 0);
	void read_raw_data();
	uint32_t calculate_pressure();
	int32_t calculate_temperature();
	int16_t calculate_altitude();
	
};