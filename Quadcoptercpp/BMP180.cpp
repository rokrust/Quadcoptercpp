#include "BMP180.h"
#include <stdio.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <inttypes.h>

void BMP180::_read_calibration_parameters(){
	uint8_t calibration_data[BMP_EEPROM_SIZE];
	printf("Calibration data allocated\n");
	_twi.read_data_from_address(BMP_ADDRESS, BMP_EEPROM_START_ADDR, calibration_data, BMP_EEPROM_SIZE);
	printf("Calibration data read\n");
	
	_calibration_parameters.AC1 = (int16_t)(((int32_t)calibration_data[0]  << 8) | calibration_data[1]);
	_calibration_parameters.AC2 = (int16_t)(((int32_t)calibration_data[2]  << 8) | calibration_data[3]);
	_calibration_parameters.AC3 = (int16_t)(((int32_t)calibration_data[4]  << 8) | calibration_data[5]);
	_calibration_parameters.AC4 = (uint16_t)(((int32_t)calibration_data[6]  << 8) | calibration_data[7]);
	_calibration_parameters.AC5 = (uint16_t)(((int32_t)calibration_data[8]  << 8) | calibration_data[9]);
	_calibration_parameters.AC6 = (uint16_t)(((int32_t)calibration_data[10] << 8) | calibration_data[11]);
	_calibration_parameters.B1  = (int16_t)(((int32_t)calibration_data[12] << 8) | calibration_data[13]);
	_calibration_parameters.B2  = (int16_t)(((int32_t)calibration_data[14] << 8) | calibration_data[15]);
	_calibration_parameters.MB  = (int16_t)(((int32_t)calibration_data[16] << 8) | calibration_data[17]);
	_calibration_parameters.MC  = (int16_t)(((int32_t)calibration_data[18] << 8) | calibration_data[19]);
	_calibration_parameters.MD  = (int16_t)(((int32_t)calibration_data[20] << 8) | calibration_data[21]);
}


BMP180::BMP180(uint8_t oss): _oss(oss >> 6){
	_twi.write_data_to_register(BMP_ADDRESS, BMP_CTRL_MEAS, &oss);

	_read_calibration_parameters();
	read_raw_data();
	
	P_0 = calculate_pressure();
}

void BMP180::read_raw_data(){
	uint8_t data[BMP180_P_RESOLUTION_BYTE];
	
	//Set pointer to temperature registers
	data[0] = BMP180_TEMP_READ_CMD;
	_twi.write_data_to_register(BMP_ADDRESS, BMP_CTRL_MEAS, data);
	_delay_ms(100);
	
	//Read temperature from data registers
	_twi.read_data_from_address(BMP_ADDRESS, DATA_REG_BASE_ADDR, data, BMP180_T_RESOLUTION_BYTE);
	_raw_temperature_data = (data[1] << 8) + data[0];
		
	//Set pointer to pressure registers
	data[0] = BMP180_PRES_READ_CMD + (_oss << 6);
	_twi.write_data_to_register(BMP_ADDRESS, BMP_CTRL_MEAS, data);
	_delay_ms(100);
	
	//Read pressure from data registers
	_twi.read_data_from_address(BMP_ADDRESS, DATA_REG_BASE_ADDR, data, BMP180_P_RESOLUTION_BYTE);
	_raw_pressure_data = (((uint32_t)data[2] << 16) + (data[1] << 8) + (data[0])) >> (8 - _oss);
}

//Mankind was never ment to understand how this works
int32_t BMP180::calculate_temperature(){
	_raw_temperature_data = 20000;
	int32_t X1 = (((int32_t)_raw_temperature_data - (int32_t)_calibration_parameters.AC6) * (int32_t)_calibration_parameters.AC5) >> 15;
	int32_t X2 = ((int32_t)_calibration_parameters.MC << 11)/(X1 + _calibration_parameters.MD);
	_calibration_parameters.B5 = X1 + X2;
	
	int32_t temperature = (_calibration_parameters.B5 + 8) >> 4;
	return temperature; //temp in 0.1 C
	
}

//Turn away all ye who tries to understand this algorithm
uint32_t BMP180::calculate_pressure(){
	int32_t B6 = _calibration_parameters.B5 - 4000;
	int32_t X1 = (_calibration_parameters.B2 * (B6 * B6) >> 12) >> 11;
	int32_t X2 = (_calibration_parameters.AC2 * B6) << 11;
	int32_t X3 = X1 + X2;
	int32_t B3 = ((((_calibration_parameters.AC1 << 2) + X3) << _oss) + 2) >> 2;
	X1 = (_calibration_parameters.AC3 * B6) >> 13;
	X2 = (_calibration_parameters.B1 * (B6 * B6) >> 12) >> 16;
	X3 = ((X1 + X2) + 2) >> 2;
	uint32_t B4 = _calibration_parameters.AC4 * (uint32_t)(X3 + 32768) >> 15;
	uint32_t B7 = ((uint32_t)_raw_pressure_data - B3) * (50000 >> _oss);
	
	uint32_t p;
	if(B7 < 0x80000000){
		p = (B7 << 1) / B4;	
	}
	else{
		p = ((B7 / B4) << 1);
	}
	
	X1 = (p >> 8) * (p >> 8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * p) >> 16;
	p = p + ((X1 + X2 + 3791) >> 4);
	
	return p;
}

int16_t BMP180::calculate_altitude(){
	uint32_t P = calculate_pressure();
	printf("Pressure: %lu, %lu\n", P, P_0);
	//Decent approximation to height under 1000 meters
	int16_t altitude = (P-P_0)/12;
	printf("Altitude: %d\n", altitude);
	
	return altitude;
	
}