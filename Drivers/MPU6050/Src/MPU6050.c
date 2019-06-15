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
	initKALMAN(&MPU6050->kalmanX);
	initKALMAN(&MPU6050->kalmanY);
	setMPU6050(MPU6050);
	
	HAL_Delay(1000);
	
	readRawData(MPU6050);
	MPU6050->angleX = atan2(MPU6050->accY, MPU6050->accZ) * RAD_TO_DEG;
	MPU6050->angleY = atan(-MPU6050->accX / sqrt(MPU6050->accY * MPU6050->accY + MPU6050->accZ * MPU6050->accZ)) * RAD_TO_DEG;
	setAngle(&MPU6050->kalmanX,MPU6050->angleX);
	setAngle(&MPU6050->kalmanY,MPU6050->angleY);
	MPU6050->timer = HAL_GetTick();
	
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

/**
* @Brief :  It will be use for next application
**/
/*
void calibrateMPU6050(struct MPU6050 *MPU6050){
	  uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
		uint16_t ii, packet_count, fifo_count;
		int32_t gyro_bias[3] = {0, 0, 0}, accel_bias[3] = {0, 0, 0};

  // reset device, reset all registers, clear gyro and accelerometer bias registers
		writeByte(MPU6050, PWR_MGMT_1, 0x80); // Write a one to bit 7 reset bit; toggle reset device
		HAL_Delay(100);
		
		writeByte(MPU6050, PWR_MGMT_1, 0x01);
		writeByte(MPU6050, PWR_MGMT_2, 0x00);
		HAL_Delay(200);
		
		writeByte(MPU6050, INT_ENABLE, 0x00);   // Disable all interrupts
		writeByte(MPU6050, FIFO_EN, 0x00);      // Disable FIFO
		writeByte(MPU6050, PWR_MGMT_1, 0x00);   // Turn on internal clock source
		writeByte(MPU6050, I2C_MST_CTRL, 0x00); // Disable I2C master
		writeByte(MPU6050, USER_CTRL, 0x00);    // Disable FIFO and I2C master modes
		writeByte(MPU6050, USER_CTRL, 0x0C);    // Reset FIFO and DMP
		HAL_Delay(15);
		
		// Configure MPU6050 gyro and accelerometer for bias calculation
  writeByte(MPU6050, CONFIG, 0x01);      // Set low-pass filter to 188 Hz
  writeByte(MPU6050, SMPLRT_DIV, 0x00);  // Set sample rate to 1 kHz
  writeByte(MPU6050, GYRO_CONFIG, 0x00);  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
  writeByte(MPU6050, ACCEL_CONFIG, 0x00); // Set accelerometer full-scale to 2 g, maximum sensitivity

  uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
  uint16_t  accelsensitivity = 16384;  // = 16384 LSB/g

  // Configure FIFO to capture accelerometer and gyro data for bias calculation
  writeByte(MPU6050, USER_CTRL, 0x40);   // Enable FIFO
  writeByte(MPU6050, FIFO_EN, 0x78);     // Enable gyro and accelerometer sensors for FIFO  (max size 1024 bytes in MPU-6050)
  HAL_Delay(80); // accumulate 80 samples in 80 milliseconds = 960 bytes

  // At end of sample accumulation, turn off FIFO sensor read
  writeByte(MPU6050, FIFO_EN, 0x00);        // Disable gyro and accelerometer sensors for FIFO
  readBytes(MPU6050, FIFO_COUNTH,&data[0],2); // read FIFO sample count
  fifo_count = ((uint16_t)data[0] << 8) | data[1];
  packet_count = fifo_count / 12; // How many sets of full gyro and accelerometer data for averaging

  for (ii = 0; ii < packet_count; ii++) {
    int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
    readBytes(MPU6050, FIFO_R_W, &data[0],12); // read data for averaging
    accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  ) ;  // Form signed 16-bit integer for each sample in FIFO
    accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  ) ;
    accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  ) ;
    gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  ) ;
    gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  ) ;
    gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]) ;

    accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
    accel_bias[1] += (int32_t) accel_temp[1];
    accel_bias[2] += (int32_t) accel_temp[2];
    gyro_bias[0]  += (int32_t) gyro_temp[0];
    gyro_bias[1]  += (int32_t) gyro_temp[1];
    gyro_bias[2]  += (int32_t) gyro_temp[2];

  }
		accel_bias[0] /= (int32_t) packet_count; // Normalize sums to get average count biases
		accel_bias[1] /= (int32_t) packet_count;
		accel_bias[2] /= (int32_t) packet_count;
		gyro_bias[0]  /= (int32_t) packet_count;
		gyro_bias[1]  /= (int32_t) packet_count;
		gyro_bias[2]  /= (int32_t) packet_count;

  if (accel_bias[2] > 0L) {
    accel_bias[2] -= (int32_t) accelsensitivity; // Remove gravity from the z-axis accelerometer bias calculation
  }
  else {
    accel_bias[2] += (int32_t) accelsensitivity;
  }

  // Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
  data[0] = (-gyro_bias[0] / 4  >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
  data[1] = (-gyro_bias[0] / 4)       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
  data[2] = (-gyro_bias[1] / 4  >> 8) & 0xFF;
  data[3] = (-gyro_bias[1] / 4)       & 0xFF;
  data[4] = (-gyro_bias[2] / 4  >> 8) & 0xFF;
  data[5] = (-gyro_bias[2] / 4)       & 0xFF;

  // Push gyro biases to hardware registers
  writeByte(MPU6050, XG_OFFS_USRH, data[0]);// might not be supported in MPU6050
  writeByte(MPU6050, XG_OFFS_USRL, data[1]);
  writeByte(MPU6050, YG_OFFS_USRH, data[2]);
  writeByte(MPU6050, YG_OFFS_USRL, data[3]);
  writeByte(MPU6050, ZG_OFFS_USRH, data[4]);
  writeByte(MPU6050, ZG_OFFS_USRL, data[5]);

  MPU6050->gyroOffset[0] = (float) gyro_bias[0] / (float) gyrosensitivity; // construct gyro bias in deg/s for later manual subtraction
  MPU6050->gyroOffset[1] = (float) gyro_bias[1] / (float) gyrosensitivity;
  MPU6050->gyroOffset[2]  = (float) gyro_bias[2] / (float) gyrosensitivity;

  int32_t accel_bias_reg[3] = {0, 0, 0}; // A place to hold the factory accelerometer trim biases
  readBytes(MPU6050, XA_OFFSET_H,&data[0],2); // Read factory accelerometer trim values
  accel_bias_reg[0] = (int16_t) ((int16_t)data[0] << 8) | data[1];
  readBytes(MPU6050, YA_OFFSET_H,&data[0],2);
  accel_bias_reg[1] = (int16_t) ((int16_t)data[0] << 8) | data[1];
  readBytes(MPU6050, ZA_OFFSET_H,&data[0],2);
  accel_bias_reg[2] = (int16_t) ((int16_t)data[0] << 8) | data[1];

  uint32_t mask = 1uL; // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
  uint8_t mask_bit[3] = {0, 0, 0}; // Define array to hold mask bit for each accelerometer bias axis

  for (ii = 0; ii < 3; ii++) {
    if (accel_bias_reg[ii] & mask) mask_bit[ii] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
  }

  // Construct total accelerometer bias, including calculated average accelerometer bias from above
  accel_bias_reg[0] -= (accel_bias[0] / 8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
  accel_bias_reg[1] -= (accel_bias[1] / 8);
  accel_bias_reg[2] -= (accel_bias[2] / 8);

  data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
  data[1] = (accel_bias_reg[0])      & 0xFF;
  data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
  data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
  data[3] = (accel_bias_reg[1])      & 0xFF;
  data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
  data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
  data[5] = (accel_bias_reg[2])      & 0xFF;
  data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers

  // Push accelerometer biases to hardware registers
  writeByte(MPU6050, XA_OFFSET_H, data[0]); // might not be supported in MPU6050
  writeByte(MPU6050, XA_OFFSET_L, data[1]);
  writeByte(MPU6050, YA_OFFSET_H, data[2]);
  writeByte(MPU6050, YA_OFFSET_L, data[3]);
  writeByte(MPU6050, ZA_OFFSET_H, data[4]);
  writeByte(MPU6050, ZA_OFFSET_L, data[5]);

  // Output scaled accelerometer biases for manual subtraction in the main program
  MPU6050->accOffset[0] = (float)accel_bias[0] / (float)accelsensitivity;
  MPU6050->accOffset[1] = (float)accel_bias[1] / (float)accelsensitivity;
  MPU6050->accOffset[2] = (float)accel_bias[2] / (float)accelsensitivity;
};
*/

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
	
	MPU6050->accX = MPU6050->accData[0] * 2.0 / 32768.0;
	MPU6050->accY = MPU6050->accData[1] * 2.0 / 32768.0;
	MPU6050->accZ = MPU6050->accData[2] * 2.0 / 32768.0;
	
  readBytes(MPU6050, GYRO_XOUT_H, &rawData[0],6);  // Read the six raw data registers sequentially into data array
  MPU6050->gyroData[0] = (int16_t)((rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
  MPU6050->gyroData[1] = (int16_t)((rawData[2] << 8) | rawData[3]) ;
  MPU6050->gyroData[2] = (int16_t)((rawData[4] << 8) | rawData[5]) ;
	
	MPU6050->gyroX = MPU6050->gyroData[0] * 250.0 / 32768.0;
	MPU6050->gyroY = MPU6050->gyroData[1] * 250.0 / 32768.0;
	MPU6050->gyroZ = MPU6050->gyroData[2] * 250.0 / 32768.0;

	HAL_Delay(200);
}

MPU6050_StatusTypeDef calculateAngles(struct MPU6050 *MPU6050){
	readRawData(MPU6050);
	
	float dt = (float)(HAL_GetTick() - MPU6050->timer) / 1000000; // Calculate delta time
	MPU6050->timer = HAL_GetTick();
	
	MPU6050->angleX = atan2(MPU6050->accY, MPU6050->accZ) * RAD_TO_DEG;
	MPU6050->angleY = atan(-MPU6050->accX / sqrt(MPU6050->accY * MPU6050->accY + MPU6050->accZ * MPU6050->accZ)) * RAD_TO_DEG;
}