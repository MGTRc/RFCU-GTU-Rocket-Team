/**
 * @file     GPS.H																									
 * @author   Mete Can Gazi																										
 * @brief      GPS Libray for Rocket Application							
 * @revision 21.06.2019
 **/

#ifndef _GPS_H
#define _GPS_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stdlib.h"
#include "usart.h"

typedef enum{
	GPS_OK,
	GPS_ERROR
}GPS_StatusTypeDef;

typedef struct GPS{
	UART_HandleTypeDef myUart;
	uint8_t gpsBuffer[120];
	uint8_t packageFind;
	uint8_t gpsIndex;
	uint8_t checkBuffer;
	
	uint8_t *lat,*lon,*alt,*h_dop,*v_dop,*sat;
	float latitude,longtitude,altitude,hdop,vdop;
	uint8_t commoIndex[20];
	uint8_t satellite;

	
}GPS_HandleTypeDef;

GPS_StatusTypeDef initGPS(struct GPS *GPS);
GPS_StatusTypeDef parseGPS(struct GPS *GPS);

#ifdef __cplusplus
}
#endif

#endif