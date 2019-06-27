/**
 * @file		DSP.h																								
 * @author   Mete Can Gazi																										
 * @brief      DSP Libray for Rocket Application							
 * @revision 27.06.2019
 **/

#ifndef  _DSP_H
#define _DSP_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdlib.h"

typedef enum{
	DSP_OK,
	DSP_ERROR
}DSP_StatusTypeDef;

typedef struct movingAverageInt16{
	uint8_t size;
	int16_t *buffer;

}DSP_movingAverageInt16_HandleTypeDef;

typedef struct movingAverageUint16{
	uint8_t size;
	uint16_t *buffer;
}DSP_movingAverageUint16_HandleTypeDef;

/*Functions*/
DSP_movingAverageInt16_HandleTypeDef setMovingAverageInt16(struct movingAverageInt16 *movingAverageInt16);
DSP_movingAverageUint16_HandleTypeDef setMovingAverageUint16(struct movingAverageUint16 *movingAverageUint16);
int16_t updateMovingAverageInt16(struct movingAverageInt16 *movingAverageInt16, int16_t newValue);
uint16_t updateMovingAverageUint16(struct movingAverageUint16 *movingAverageUint16);

#ifdef __cplusplus
}
#endif

#endif