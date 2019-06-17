#include "BMP180.h"

BMP180_StatusTypeDef initBMP180(struct BMP180 *BMP180){
	BMP180->myI2C = hi2c1;
	BMP180->statusBMP180 = 0;
	setOverSamplingMode(BMP180, BMP180_HighResolution);
	readCalibrationValues(BMP180);
	
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
	BMP180->ac1 = (int16_t)read2Bytes(BMP180,BMP180_AC1);
	
};

uint16_t read2Bytes(struct BMP180 *BMP180, uint8_t adress){

	return 0;
}
