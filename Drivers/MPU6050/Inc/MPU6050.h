/**
 * @file     MPU6050.H																									
 * @author   Mete Can Gazi																										
 * @brief      MPU6050 Libray for Rocket Application							
 * @revision 15.06.2019
 **/

#ifndef _MPU6050_H
#define _MPU6050_H

#ifdef __cplusplus
 extern "C" {
#endif

#define RAD_TO_DEG 57.2957

//Device Register Adress
#define MPU6050_Write_Adress 0xD0
#define MPU6050_Read_Adress 0xD1
#define USER_CTRL 0x6A
#define PWR_MGMT_1			0x6B
#define PWR_MGMT_2			0x6C	
#define INT_ENABLE					0x38
#define FIFO_EN								0x23
#define I2C_MST_CTRL			0x24
#define CONFIG								0x1A
#define SMPLRT_DIV					0x19
#define GYRO_CONFIG			0x1B
#define ACCEL_CONFIG 		0x1C
#define FIFO_COUNTH			0x72
#define FIFO_R_W 						0x74
#define XG_OFFS_USRH		0x13
#define	XG_OFFS_USRL		0x14
#define YG_OFFS_USRH   0x15
#define YG_OFFS_USRL		0x16
#define ZG_OFFS_USRH   0x17
#define ZG_OFFS_USRL		0x18
#define XA_OFFSET_H			0x06
#define XA_OFFSET_L				0x07
#define YA_OFFSET_H			0x08
#define YA_OFFSET_L			0x09
#define ZA_OFFSET_H			0x0A
#define ZA_OFFSET_L				0x0B
#define INT_PIN_CFG				0x37
#define ACCEL_XOUT_H 		0x3B

#include "stm32f4xx_hal.h"
#include "stdlib.h"
#include "i2c.h"
#include "math.h"
#include "KalmanFilter.h"

typedef enum{
	MPU6050_OK,
	MPU6050_ERROR,
}MPU6050_StatusTypeDef;

typedef struct MPU6050{
	I2C_HandleTypeDef myI2C;
	uint8_t statusMPU6050;
	float accOffset[3];
	float accData[3];
	float accX,accY,accZ;
	
	float gyroData[3];
	float gyroOffset[3];
	float gyroX,gyroY,gyroZ;
	
	float angleX,angleY;
	
	KALMAN_HandleTypeDef kalmanX;
	KALMAN_HandleTypeDef kalmanY;
	
	long timer;
	
}MPU6050_HandleTypeDef;

//Public Functions
MPU6050_StatusTypeDef isMPU6050Ready(struct MPU6050 *MPU6050);
MPU6050_StatusTypeDef initMPU6050(struct MPU6050 *MPU6050, I2C_HandleTypeDef myI2C);

//Private Function
void writeByte(struct MPU6050 *MPU6050,uint8_t adress, uint8_t command);
void readByte(struct MPU6050 *MPU6050, uint8_t adress, uint8_t *toWrite);
void readBytes(struct MPU6050 *MPU6050, uint8_t adress, uint8_t *toWrite, uint8_t size);
void calibrateMPU6050(struct MPU6050 *MPU6050);
void setMPU6050(struct MPU6050 *MPU6050);
void readAccelData(struct MPU6050 *MPU6050);
void calculateAngles(struct MPU6050 *MPU6050);

#ifdef __cplusplus
}
#endif

#endif