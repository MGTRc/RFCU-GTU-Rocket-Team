#include "DSP.h"

DSP_StatusTypeDef setMovingAverageFloat(struct movingAverageFloat *movingAverageFloat,uint8_t size){
	movingAverageFloat->size = size;
	movingAverageFloat->average = 0;

	for(int i=0; i<size; i++){
		*(movingAverageFloat->buffer+i) = 0;
	}
	return DSP_OK;
}

DSP_StatusTypeDef updateMovingAverageFloat(struct movingAverageFloat *movingAverageFloat, float data){
	float firstValue = movingAverageFloat->buffer[0];

	for(uint8_t i=0; i<movingAverageFloat->size-1; i++){
		movingAverageFloat->buffer[i] = movingAverageFloat->buffer[i+1];
	}

	movingAverageFloat->buffer[movingAverageFloat->size-1] = data;

	movingAverageFloat->average = ((movingAverageFloat->average) - (firstValue/(float)movingAverageFloat->size)) + (data/(float)movingAverageFloat->size);

	return DSP_OK;
}
