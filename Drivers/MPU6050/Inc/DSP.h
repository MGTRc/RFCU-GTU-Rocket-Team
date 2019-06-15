/**
 * @file     SDCARD.H																									
 * @author   Mete Can Gazi																										
 * @brief    SDCARD Library							
 * @revision 5.11.2018
 **/

#ifndef _DSP_H
#define _DSP_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stdlib.h"

typedef enum{
	DSP_OK,
	DSP_ERROR
}DSP_StatusTypeDef;

typedef struct DSP{
	int16_t filterArray[3];
	int16_t buffer;
	int16_t outputData;
	uint8_t size;
}DSP_HandleTypeDef;

DSP_StatusTypeDef initDSP(struct DSP *DSP);
int16_t applyDSP(struct DSP *DSP , int16_t dataIn);

#ifdef __cplusplus
}
#endif

#endif
