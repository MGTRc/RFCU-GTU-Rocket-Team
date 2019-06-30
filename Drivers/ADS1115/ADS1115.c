#include "ADS1115.h"


ADS1115_StatusTypeDef initADS1115(struct ADS1115 *ADS1115, uint8_t selectAdress){
	
	if(selectAdress >= 4){
		return ADS1115_ERROR;
	}
	
	ADS1115->ADS1115_I2C = hi2c1;
	
	ADS1115->config[0] = 0x01;
	ADS1115->config[2] = 0xE3;
	
	ADS1115->dataAdress = 0x00;
	
	if(selectAdress == 0){
		ADS1115->writeAdress 	= 0x90;
		ADS1115->readAdress 	= 0x91;
	}else if(selectAdress == 1){
		ADS1115->writeAdress 	= 0x92;
		ADS1115->readAdress 	= 0x93;
	}else if(selectAdress == 2){
		ADS1115->writeAdress 	= 0x94;
		ADS1115->readAdress 	= 0x95;
	}else if(selectAdress == 3){
		ADS1115->writeAdress 	= 0x96;
		ADS1115->readAdress 	= 0x97;
	}else{
		return ADS1115_ERROR;
	}
	
	if(HAL_I2C_IsDeviceReady(&ADS1115->ADS1115_I2C,ADS1115->writeAdress,10,100) == HAL_OK){
		if(HAL_I2C_IsDeviceReady(&ADS1115->ADS1115_I2C,ADS1115->readAdress,10,100) == HAL_OK){
		ADS1115->checkADS1115 = 1;
		}
	}else{
		ADS1115->checkADS1115 = 0;
		return ADS1115_ERROR;
	}
	
	return ADS1115_OK;
}

uint16_t ADS1115_getRawData(struct ADS1115 *ADS1115, uint8_t selectPin){
	if(selectPin == 0){
			ADS1115->config[1] = 0xC3;
	}else if(selectPin == 1){
			ADS1115->config[1] = 0xD3;
	}else if(selectPin == 2){
			ADS1115->config[1] = 0xE3;
	}else if(selectPin == 3){
			ADS1115->config[1] = 0xF3;
		;
	}else{
		return ADS1115_ERROR;
	}
	
	HAL_I2C_Master_Transmit(&ADS1115->ADS1115_I2C,ADS1115->writeAdress,ADS1115->config,3,100);
	HAL_I2C_Master_Transmit(&ADS1115->ADS1115_I2C,ADS1115->writeAdress,&ADS1115->dataAdress,1,100);
	HAL_Delay(2);
	HAL_I2C_Master_Receive(&ADS1115->ADS1115_I2C,ADS1115->readAdress,ADS1115->buffer,2,100);
	
	return (uint16_t)(ADS1115->buffer[1] | ((ADS1115->buffer[0]) << 8));
}