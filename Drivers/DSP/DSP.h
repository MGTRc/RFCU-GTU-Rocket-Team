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


#if defined(STMF1_Device)
	#include "stm32f1xx_hal.h"
#elif defined(STMF4_Device)
	#include "stm32f4xx_hal.h"
#endif

#include "stdlib.h"
#include "stdio.h"

typedef enum{
	DSP_OK,
	DSP_ERROR
}DSP_StatusTypeDef;

typedef struct movingAverageFloat{
	uint8_t size;
	float buffer[20];
	float average;
}movingAverageFloat;

/*Functions*/
DSP_StatusTypeDef setMovingAverageFloat(struct movingAverageFloat *movingAverageFloat, uint8_t size);
DSP_StatusTypeDef updateMovingAverageFloat(struct movingAverageFloat *movingAverageFloat, float data);

#ifdef __cplusplus
}
#endif

#endif
