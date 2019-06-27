#include "DSP.h"

DSP_movingAverageInt16_HandleTypeDef setMovingAverageInt16(struct movingAverageInt16 *movingAverageInt16){
	movingAverageInt16->result = 0;
	movingAverageInt16->buffer = (int16_t*)calloc(sizeof(int16_t),movingAverageInt16->size);

	return DSP_OK;
};

DSP_movingAverageUint16_HandleTypeDef setMovingAverageUint16(struct movingAverageUint16 *movingAverageUint16){
	movingAverageUInt16->result = 0;
	movingAverageUInt16->buffer = (uint16_t*)calloc(sizeof(uint16_t),movingAverageUint16->size);

	return DPS_OK;
};


int16_t updateMovingAverageInt16(struct movingAverageInt16 *movingAverageInt16, int16_t newValue){
	movingAverageInt16->result = (movingAverageInt16->result*10) - (*(movingAverageInt16->buffer)*10);

	/*Data is shifted left*/
	for (int i = 0; i < movingAverageInt16->size-1 ; i++)
	{
		*(movingAverageInt16->buffer+i) = *(movingAverageInt16->buffer+i+1);
	}

	*((movingAverageInt16->buffer) + movingAverageInt16->size - 1) = newValue;

	movingAverageInt16-> result = (movingAverageInt16-> result + newValue) / movingAverageInt16->size; 

	return movingAverageInt16-> result;
}