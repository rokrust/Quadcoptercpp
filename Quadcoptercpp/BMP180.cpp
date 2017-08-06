#include "BMP180.h"
#include <stdio.h>

void BMP180::_read_calibration_parameters(){
	uint8_t calibration_data[BMP_EEPROM_SIZE];
	printf("Calibration data allocated\n");
	_twi.read_data_from_address(BMP_ADDRESS, BMP_EEPROM_START_ADDR, calibration_data, BMP_EEPROM_SIZE);
	printf("Calibration data read\n");
	
	_calibration_parameters.AC1 = (calibration_data[0]  << 8) | calibration_data[1];
	_calibration_parameters.AC2 = (calibration_data[2]  << 8) | calibration_data[3];
	_calibration_parameters.AC3 = (calibration_data[4]  << 8) | calibration_data[5];
	_calibration_parameters.AC4 = (calibration_data[6]  << 8) | calibration_data[7];
	_calibration_parameters.AC5 = (calibration_data[8]  << 8) | calibration_data[9];
	_calibration_parameters.AC6 = (calibration_data[10] << 8) | calibration_data[11];
	_calibration_parameters.B1  = (calibration_data[12] << 8) | calibration_data[13];
	_calibration_parameters.B2  = (calibration_data[14] << 8) | calibration_data[15];
	_calibration_parameters.MB  = (calibration_data[16] << 8) | calibration_data[17];
	_calibration_parameters.MC  = (calibration_data[18] << 8) | calibration_data[19];
	_calibration_parameters.MD  = (calibration_data[20] << 8) | calibration_data[21];
}


BMP180::BMP180(uint8_t oss): _oss(oss >> 6){
	printf("BMP start\n");
	_twi.write_data_to_register(BMP_ADDRESS, BMP_CTRL_MEAS, oss);
	printf("oss set\n");
	_read_calibration_parameters();
	printf("Calibration read\n");
	read_raw_data();
	printf("Initial raw values read\n");
	
	uint16_t temp = calculate_temperature();
	printf("Initial temperature calculated\n");
	uint32_t P_0 = calculate_pressure();
	printf("Initial pressure calculated\n");
}

void BMP180::read_raw_data(){
	uint8_t data[BMP180_P_RESOLUTION_BYTE];
	
	_twi.read_data_from_address(BMP_ADDRESS, BMP180_TEMP_BASE_ADDR, data, BMP180_T_RESOLUTION_BYTE);
	_raw_temperature_data = data[1] << 8 | data[0];
	
	_twi.read_data_from_address(BMP_ADDRESS, BMP180_PRES_BASE_ADDR + (_oss << 6), data, BMP180_P_RESOLUTION_BYTE);
	_raw_pressure_data = (((uint32_t)data[2] << 16) + (data[1] << 8) + (data[0])) >> (8 - _oss);
}

//Mankind was never ment to understand how this works
uint16_t BMP180::calculate_temperature(){
	int32_t X1 = ((_raw_temperature_data - _calibration_parameters.AC6) * _calibration_parameters.AC5) >> 15;
	int32_t X2 = (_calibration_parameters.MC << 11)/(X1 + _calibration_parameters.MD);
	_calibration_parameters.B5 = X1 + X2;
	return (_calibration_parameters.B5 + 8) >> 4; //temp in 0.1 C
	
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
	return ((P-P_0)*52)/625;
	
}