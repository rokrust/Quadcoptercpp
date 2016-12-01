#include "TWI_Master.h"
#include "MPU6050.h"

#include <stdio.h>

//Must be called after TWI_Master_intialize() and sei()
MPU6050::MPU6050(){
	//0x6B: power_management or something
	//0: Wake up sensorboard.
	unsigned char init_data[3] = {MPU_ADDRESS << 1 | WRITE_FLAG, PWR_MGMT_1, WAKE_UP};
	printf("Initializing mpu..\n");
	twi.start_transceiver_with_data(init_data, 3);
	
	//Read and store offset values
	update_movement();
	for(int i = 0; i < N_MESSURE_VAR; i++){
		sensorOffset[i] = sensorData[i];
	}

}

void MPU6050::update_movement(){
	unsigned char movement_registers[2*N_MESSURE_VAR + 1];
	
	//Set read register to ACC_X
	movement_registers[0] = MPU_ADDRESS << 1 | WRITE_FLAG;
	movement_registers[1] = ACC_X;
	twi.start_transceiver_with_data(movement_registers, 2);

	//Read the next fourteen values
	movement_registers[0] |= READ_FLAG;
	twi.start_transceiver_with_data(movement_registers, 2*N_MESSURE_VAR + 1);
	twi.get_data_from_transceiver(movement_registers, 2*N_MESSURE_VAR + 1);

	//Put high and low bytes (excluding address) into sensorData
	for(int i = 1; i < 2*N_MESSURE_VAR; i += 2){
		sensorData[i/2] = movement_registers[i] << 8 | movement_registers[i + 1];
	}
}