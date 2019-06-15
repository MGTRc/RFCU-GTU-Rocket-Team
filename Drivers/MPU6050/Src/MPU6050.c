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
	
	isMPU6050Ready(MPU6050);
	setMPU6050(MPU6050);

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
	// wake up device-don't need this here if using calibration function below
  writeByte(MPU6050, PWR_MGMT_1, 0x00); // Clear sleep mode bit (6), enable all sensors
  HAL_Delay(100); // Delay 100 ms for PLL to get established on x-axis gyro; should check for PLL ready interrupt

  // get stable time source
  writeByte(MPU6050, PWR_MGMT_1, 0x01);  // Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001

  // Configure Gyro and Accelerometer
  // Disable FSYNC and set accelerometer and gyro bandwidth to 44 and 42 Hz, respectively;
  // DLPF_CFG = bits 2:0 = 010; this sets the sample rate at 1 kHz for both
  // Maximum delay time is 4.9 ms corresponding to just over 200 Hz sample rate
  writeByte(MPU6050, CONFIG, 0x03);

  // Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
  writeByte(MPU6050, SMPLRT_DIV, 0x04);  // Use a 200 Hz rate; the same rate set in CONFIG above

  // Set gyroscope full scale range
  // Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3
  uint8_t c;
	uint8_t Gscale,Ascale = 0;
	readByte(MPU6050, GYRO_CONFIG,&c);
  writeByte(MPU6050, GYRO_CONFIG, c & ~0xE0); // Clear self-test bits [7:5]
  writeByte(MPU6050, GYRO_CONFIG, c & ~0x18); // Clear AFS bits [4:3]
  writeByte(MPU6050, GYRO_CONFIG, c | Gscale << 3); // Set full scale range for the gyro

  // Set accelerometer configuration
  readByte(MPU6050, ACCEL_CONFIG,&c);
  writeByte(MPU6050, ACCEL_CONFIG, c & ~0xE0); // Clear self-test bits [7:5]
  writeByte(MPU6050, ACCEL_CONFIG, c & ~0x18); // Clear AFS bits [4:3]
  writeByte(MPU6050, ACCEL_CONFIG, c | Ascale << 3); // Set full scale range for the accelerometer

  // Configure Interrupts and Bypass Enable
  // Set interrupt pin active high, push-pull, and clear on read of INT_STATUS, enable I2C_BYPASS_EN so additional chips
  // can join the I2C bus and all can be controlled by the Arduino as master
  writeByte(MPU6050, INT_PIN_CFG, 0x22);
	writeByte(MPU6050, INT_ENABLE, 0x01); // Enable data ready (bit 0) interrupt
}

void readRawData(struct MPU6050 *MPU6050){
	uint8_t rawData[6];  // x/y/z accel register data stored here
  readBytes(MPU6050, ACCEL_XOUT_H, &rawData[0],6);  // Read the six raw data registers into data array
	MPU6050->accData[0] 	= (int16_t)((rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
  MPU6050->accData[1]  = (int16_t)((rawData[2] << 8) | rawData[3]) ;
	MPU6050->accData[2]  = (int16_t)((rawData[4] << 8) | rawData[5]) ;
	
	MPU6050->accX = (float)MPU6050->accData[0]*2.0/32768.0;
	MPU6050->accY = (float)MPU6050->accData[2]*2.0/32768.0;
	MPU6050->accZ = (float)MPU6050->accData[1]*2.0/32768.0;
}

MPU6050_StatusTypeDef calculateAngles(struct MPU6050 *MPU6050){
	readRawData(MPU6050);
	
	for(uint8_t i = 0; i<9; i++){
		MPU6050->bufferX[i] = MPU6050->bufferX[i+1];
		MPU6050->bufferY[i] = MPU6050->bufferY[i+1];
		MPU6050->bufferZ[i] = MPU6050->bufferZ[i+1];
	}
	
	MPU6050->bufferX[9] = MPU6050->accX;
	MPU6050->bufferY[9] = MPU6050->accY;
	MPU6050->bufferZ[9] = MPU6050->accZ;
	
	for(uint8_t i = 0; i<10; i++){
		MPU6050->filtreredAccX = MPU6050->filtreredAccX + MPU6050->bufferX[i];
		MPU6050->filtreredAccY = MPU6050->filtreredAccY + MPU6050->bufferY[i];
		MPU6050->filtreredAccZ = MPU6050->filtreredAccZ + MPU6050->bufferZ[i];
	}
	
	MPU6050->filtreredAccX = MPU6050->filtreredAccX / 11;
	MPU6050->filtreredAccY = MPU6050->filtreredAccY / 11;
	MPU6050->filtreredAccZ = MPU6050->filtreredAccZ / 11;
	
	MPU6050->angleX = atan2(MPU6050->accY, MPU6050->accZ) * RAD_TO_DEG;
	MPU6050->angleY = atan(-MPU6050->accX / sqrt(MPU6050->accY * MPU6050->accY + MPU6050->accZ * MPU6050->accZ)) * RAD_TO_DEG;

	
	return MPU6050_OK;
}

void filterMPU6050(struct MPU6050 *MPU6050){





}


