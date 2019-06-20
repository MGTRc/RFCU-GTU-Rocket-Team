#include "BMP180.h"

BMP180_StatusTypeDef initBMP180(struct BMP180 *BMP180){
	BMP180->myI2C = hi2c1;
	BMP180->statusBMP180 = 0;
	setOverSamplingMode(BMP180, BMP180_HighResolution);
	
	return BMP180_OK;
};

BMP180_StatusTypeDef isBMP180Ready(struct BMP180 *BMP180){
	if(HAL_I2C_IsDeviceReady(&BMP180->myI2C,BMP180_Write_Adress,100,100) == HAL_OK){
		if(HAL_I2C_IsDeviceReady(&BMP180->myI2C,BMP180_Read_Adress,100,100) == HAL_OK){
			BMP180->statusBMP180 = 1;
			return BMP180_OK;
		}
	}else{
		return BMP180_ERROR;
	}
};

BMP180_StatusTypeDef setOverSamplingMode(struct BMP180 *BMP180, BMP180_OverSamplingMode BMP180_OverSamplingMode){
	BMP180->BMP180_OverSamplingMode = BMP180_OverSamplingMode;
	return BMP180_OK;
}

void readCalibrationValues(struct BMP180 *BMP180){
	/*int16_t parameters*/
	BMP180->ac1 = (int16_t)read2Bytes(BMP180,BMP180_AC1);
	BMP180->ac2 = (int16_t)read2Bytes(BMP180,BMP180_AC2);
	BMP180->ac3 = (int16_t)read2Bytes(BMP180,BMP180_AC3);
	BMP180->b1 = (int16_t)read2Bytes(BMP180,BMP180_B1);
	BMP180->b2 = (int16_t)read2Bytes(BMP180,BMP180_B2);
	BMP180->mb = (int16_t)read2Bytes(BMP180,BMP180_MB);
	BMP180->mc = (int16_t)read2Bytes(BMP180,BMP180_MC);
	BMP180->md = (int16_t)read2Bytes(BMP180,BMP180_MD);
	
	/*uint16_t parameters*/
	BMP180->ac4 = read2Bytes(BMP180,BMP180_AC4);
	BMP180->ac5 = read2Bytes(BMP180,BMP180_AC5);
	BMP180->ac6 = read2Bytes(BMP180,BMP180_AC6);
};

uint16_t read2Bytes(struct BMP180 *BMP180, uint8_t adress){
	uint8_t dataBuffer[2];
	HAL_I2C_Mem_Read(&BMP180->myI2C,BMP180_Read_Adress,adress,I2C_MEMADD_SIZE_8BIT,dataBuffer,2,100);
	return  (uint16_t)(dataBuffer[1]) | (dataBuffer[0] << 8); 
};

void writeByte(struct BMP180 *BMP180,uint8_t adress, uint8_t command){
	HAL_I2C_Mem_Write(&BMP180->myI2C,BMP180_Write_Adress,adress,I2C_MEMADD_SIZE_8BIT,&command,1,100);
};

void readByte (struct BMP180 *BMP180,uint8_t adress, uint8_t *toWrite){
	HAL_I2C_Mem_Read(&BMP180->myI2C,BMP180_Read_Adress,adress,I2C_MEMADD_SIZE_8BIT,toWrite,1,100);
};
