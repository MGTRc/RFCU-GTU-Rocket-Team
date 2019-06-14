#include "MPU6050.h"

/**
* @Brief : This function is to check that is MPU6050 working.
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

};