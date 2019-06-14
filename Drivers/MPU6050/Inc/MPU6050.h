/**
 * @file     MPU6050.H																									
 * @author   Mete Can Gazi																										
 * @brief      MPU6050 Libray for Rocket Application							
 * @revision 14.06.2019
 **/

#ifndef _MPU6050_H
#define _MPU6050_H

#ifdef __cplusplus
 extern "C" {
#endif

#define MPU6050_Write_Adress 0xD0
#define MPU6050_Read_Adress 0xD1

#include "stm32f4xx_hal.h"
#include "stdlib.h"
#include "i2c.h"

typedef enum{
	MPU6050_OK,
	MPU6050_ERROR,
}MPU6050_StatusTypeDef;


typedef struct MPU6050{
	I2C_HandleTypeDef myI2C;
	uint8_t statusMPU6050;
	
}MPU6050_HandleTypeDef;


//Public Functions
MPU6050_StatusTypeDef isMPU6050Ready(struct MPU6050 *MPU6050);
MPU6050_StatusTypeDef initMPU6050(struct MPU6050 *MPU6050, I2C_HandleTypeDef myI2C);

//Private Function
void writeByte(struct MPU6050 *MPU6050,uint8_t adress, uint8_t command);
void readByte(struct MPU6050 *MPU6050, uint8_t adress, uint8_t *toWrite);
void readBytes(struct MPU6050 *MPU6050, uint8_t adress, uint8_t *toWrite, uint8_t size);


#ifdef __cplusplus
}
#endif

#endif