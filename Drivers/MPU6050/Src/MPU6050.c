#include "MPU6050.h"

/**
* @Brief : This function is to check that is the MPU6050 working.
**/
MPU6050_StatusTypeDef isMPU6050Ready(struct MPU6050 *MPU6050){
	if(HAL_I2C_IsDeviceReady(&MPU6050->myI2C,MPU6050_Write_Adress,100,100) == HAL_OK){
		if(HAL_I2C_IsDeviceReady(&MPU6050->myI2C,MPU6050_Read_Adress,100,100) == HAL_OK){
			MPU6050->statusMPU6050 = 1;
			return MPU6050_OK;
		}
	}else{
		return MPU6050_ERROR;
	}
};

MPU6050_StatusTypeDef initMPU6050(struct MPU6050 *MPU6050, I2C_HandleTypeDef myI2C){
	MPU6050->statusMPU6050 = 0;
	MPU6050->myI2C = myI2C;
	
	return MPU6050_OK;
};

void writeByte(struct MPU6050 *MPU6050, uint8_t adress, uint8_t command){
	HAL_I2C_Mem_Write(&MPU6050->myI2C,MPU6050_Write_Adress,adress,I2C_MEMADD_SIZE_8BIT,&command,1,100);
};

void readByte(struct MPU6050 *MPU6050, uint8_t adress, uint8_t *toWrite){
	uint8_t buffer[1] = {adress};
	HAL_I2C_Master_Transmit(&MPU6050->myI2C,MPU6050_Write_Adress,buffer,1,100);
	HAL_I2C_Master_Receive(&MPU6050->myI2C,MPU6050_Read_Adress,toWrite,1,100);
};

void readBytes(struct MPU6050 *MPU6050, uint8_t adress, uint8_t *toWrite, uint8_t size){
	uint8_t buffer[1] = {adress};
	HAL_I2C_Master_Transmit(&MPU6050->myI2C,MPU6050_Write_Adress,buffer,1,100);
	HAL_I2C_Master_Receive(&MPU6050->myI2C,MPU6050_Read_Adress,toWrite,size,100);
}