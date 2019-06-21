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
	uint8_t gpsBuffer[117];
	UART_HandleTypeDef myUart;
}GPS_HandleTypeDef;

GPS_StatusTypeDef initGPS(struct GPS *GPS);

#ifdef __cplusplus
}
#endif

#endif