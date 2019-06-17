/**
 * @file     ADS1115.H																									
 * @author   Mete Can Gazi																										
 * @brief    ADS1115 Library							
 * @revision 07.02.2019
 **/

#ifndef _ADS1115_H
#define _ADS1115_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "i2c.h"

typedef enum{
	ADS1115_OK,
	ADS1115_ERROR
}ADS1115_StatusTypeDef;


typedef struct ADS1115{
	I2C_HandleTypeDef ADS1115_I2C;
	
	uint8_t writeAdress;
	uint8_t readAdress;
	uint8_t checkADS1115;
	uint8_t config[3];
	uint8_t buffer[2];
	uint8_t dataAdress;
}ADS1115_HandleTypeDef;

ADS1115_StatusTypeDef initADS1115(struct ADS1115 *ADS1115, uint8_t selectAdress);
uint16_t ADS1115_getRawData(struct ADS1115 *ADS1115, uint8_t selectPin);

#ifdef __cplusplus
}
#endif

#endif
