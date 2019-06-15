/**
 * @file     KalmanFilter.h																								
 * @author   GTU-Roket Takimi																										
 * @brief    KalmanFilter Library							
 * @revision 05.06.2019
 **/

#ifndef _KALMANFILTER_H
#define _KALMANFILTER_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stdlib.h"
	 
typedef enum{
	KALMAN_OK,
	KALMAN_ERROR,
}KALMAN_StatusTypeDef;

typedef struct KALMAN{
		//Float Variables
		float Q_angle;
		float R_measure;
		float Q_bias;
		float angle; 
		float bias;
		float rate; 
		float P[2][2];
}KALMAN_HandleTypeDef;

KALMAN_StatusTypeDef initKALMAN(struct KALMAN *KALMAN);
float getAngle(struct KALMAN *KALMAN,float newAngle, float newRate, float dt);
	 
#ifdef __cplusplus
}
#endif

#endif