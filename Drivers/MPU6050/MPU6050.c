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

/**
* @Brief :  Setting of MPU6050.
**/
MPU6050_StatusTypeDef initMPU6050(struct MPU6050 *MPU6050, I2C_HandleTypeDef myI2C){
	MPU6050->statusMPU6050 = 0;
	MPU6050->myI2C = myI2C;
	setMovingAverageFloat(&MPU6050->filter_X,10);
	setMovingAverageFloat(&MPU6050->filter_Y,10);
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


void setMPU6050(struct MPU6050 *MPU6050){
	uint8_t writeData;
	/*Reset Device*/
	writeData = 0x80;
	writeByte(MPU6050, PWR_MGMT_1, writeData);
	HAL_Delay(100);
	writeData = 0x01;
	writeByte(MPU6050, PWR_MGMT_1,writeData);

	/*SampleRate Divider*/
	writeData = 0x00;
	writeByte(MPU6050,SMPLRT_DIV,writeData); // SampleRate = 1Khz

	/*Config*/
	writeData = 0x04;
	writeByte(MPU6050,CONFIG,writeData); //DLPF = 20Hz;

	/*Gyroscope Configration*/
	writeData = 0x00;
	writeByte(MPU6050,GYRO_CONFIG,writeData);

	/*Acc. Configration*/
	writeData = 0x00;
	writeByte(MPU6050,ACCEL_CONFIG,writeData);

	HAL_Delay(1000);

};

void readRawData(struct MPU6050 *MPU6050){
	uint8_t rawData[6];  // x/y/z accel register data stored here
	readBytes(MPU6050, ACCEL_XOUT_H, &rawData[0],6);  // Read the six raw data registers into data array
	MPU6050->accData[0]  = (int16_t)((rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
	MPU6050->accData[1]  = (int16_t)((rawData[2] << 8) | rawData[3]) ;
	MPU6050->accData[2]  = (int16_t)((rawData[4] << 8) | rawData[5]) ;
	
	MPU6050->accX = (float)MPU6050->accData[0]*2.0/32768.0;
	MPU6050->accY = (float)MPU6050->accData[2]*2.0/32768.0;
	MPU6050->accZ = (float)MPU6050->accData[1]*2.0/32768.0;
}

MPU6050_StatusTypeDef calculateAngles(struct MPU6050 *MPU6050){
	readRawData(MPU6050);
	
	MPU6050->angleX = atan2(MPU6050->accY, MPU6050->accZ) * RAD_TO_DEG;
	MPU6050->angleY = atan(-MPU6050->accX / sqrt(MPU6050->accY * MPU6050->accY + MPU6050->accZ * MPU6050->accZ)) * RAD_TO_DEG;

	MPU6050->angleX = 180.0 - fabs(MPU6050->angleX);
	MPU6050->angleY = fabs(MPU6050->angleY);

	updateMovingAverageFloat(&MPU6050->filter_X, MPU6050->angleX);
	updateMovingAverageFloat(&MPU6050->filter_Y, MPU6050->angleY);

	return MPU6050_OK;
}



