/**
 * @file     RocketLink.h																								
 * @author   GTU-Roket Takimi																										
 * @brief    RocketLink Library							
 * @revision 22.06.2019
 **/

#ifndef _ROCKETLINK_H
#define _ROCKETLINK_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stdlib.h"

#define startByte 					36
#define finishByte 					35

typedef enum{
		RocketLink_ERROR = 0,
		RocketLink_OK = 1,
		RocketLink_COMPLETED = 1
}RocketLink_StatusTypeDef;

typedef enum{
		PREPARING = 0,
		BAROMETER_CALIBRATING = 1,
		ACC_CALIBRATING = 2,
		READY_FOR_IGNITION = 3,
		EJECTED = 4,
		ALTITUDE_THRESHOLD = 5,
		APOGEE = 6,
		PARACHUTE_PAYLOAD_DEPLOYED = 7,
		DESCENTING = 8,
		LANDED	= 9,
		WAITING_FOR_RESCUE	= 10
}RocketLink_Stage;

typedef enum{
	BAROMETER_IS_READY = 0,
	BAROMETER_CALIBRATION = 1,
	IMU_IS_READY = 2,
	IMU_CALIBRATION = 3,
	GPS_IS_READY = 4,
}RocketLink_SensorStatus;

typedef struct{
	uint8_t teamID;
	uint8_t compID;  // 0 = Rocket; 1= Payload
	uint8_t RCFUID; // 1 = Main RFCU 2 = Backup RCFU
}RocketLink_InitTypeDef;

typedef  struct RocketLink{
	RocketLink_InitTypeDef RocketLink_Init;
	//Rocket Dynamic Parameters
	uint8_t sensorStatus;
	uint8_t sendPackage[128];
	uint16_t parameterCheck;
	uint8_t error;
	
}RocketLink_HandleTypeDef;

extern RocketLink_HandleTypeDef RocketLink;

RocketLink_StatusTypeDef initRocketLink(struct RocketLink *RocketLink);
RocketLink_StatusTypeDef initRocketLinkPackage(struct RocketLink *RocketLink);
RocketLink_StatusTypeDef setRocketStage(struct RocketLink *RocketLink, RocketLink_Stage Stage);
RocketLink_StatusTypeDef setBatteryVoltage(struct RocketLink *RocketLink, float batteryVoltage);
RocketLink_StatusTypeDef setBusVoltage(struct RocketLink *RocketLink, float busVoltage);
RocketLink_StatusTypeDef setCurrent(struct RocketLink *RocketLink, float current);
RocketLink_StatusTypeDef setRFCUTemp(struct RocketLink *RocketLink, float temp);
RocketLink_StatusTypeDef setPressure(struct RocketLink *RocketLink, float pressure);
RocketLink_StatusTypeDef setBarometricAlt(struct RocketLink *RocketLink, float baroalt);
RocketLink_StatusTypeDef setBarometricTemp(struct RocketLink *RocketLink, float barotemp);
/*GPS Parameters*/
RocketLink_StatusTypeDef setGPSLat(struct RocketLink *RocketLink, float gpslat);
RocketLink_StatusTypeDef setGPSLong(struct RocketLink *RocketLink, float gpslong);
RocketLink_StatusTypeDef setGPSAlt(struct RocketLink *RocketLink, float gpsalt);
RocketLink_StatusTypeDef setGPSHdop(struct RocketLink *RocketLink, float hdop);
RocketLink_StatusTypeDef setGPSSatelliteNumber(struct RocketLink *RocketLink, uint8_t satelliteNumber);
/*Acc. Parameters*/
RocketLink_StatusTypeDef setAngle(struct RocketLink *RocketLink, int8_t gpslong);
RocketLink_StatusTypeDef setSensorStatus(struct RocketLink *RocketLink, RocketLink_SensorStatus SensorStatus);

void floatToUint8(float dataIn, uint8_t *adress);
void uint16tToUint8(uint16_t dataIn, uint8_t *adress);
void int16tToUint8(uint16_t dataIn, uint8_t *adress);
void setBit(struct RocketLink *RocketLink, uint8_t bitOrder);

#ifdef __cplusplus
}
#endif

#endif
