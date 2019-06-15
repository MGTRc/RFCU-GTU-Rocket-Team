#include "DSP.h"


DSP_StatusTypeDef initDSP(struct DSP *DSP){
	for(int i; i<3; i++){
		DSP->filterArray[i] = 0;
	}
	
	return DSP_OK;
}

int16_t applyDSP(struct DSP *DSP, int16_t dataIn){
	DSP->buffer = dataIn;
	
	for(uint8_t i=0; i<8; i++){
		DSP->buffer = DSP->buffer + DSP->filterArray[i]; 
	}
	
	DSP->filterArray[0] = DSP->filterArray[1];
	DSP->filterArray[1] = DSP->filterArray[2];
	DSP->filterArray[2] = dataIn;

	dataIn = (DSP->buffer)/4;
	return dataIn;
}
