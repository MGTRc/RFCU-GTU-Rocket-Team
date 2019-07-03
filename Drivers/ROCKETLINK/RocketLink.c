#include "RocketLink.h"

RocketLink_HandleTypeDef RocketLink;

RocketLink_StatusTypeDef initRocketLink(struct RocketLink *RocketLink){
	RocketLink->RocketLink_Init.teamID = 212;   // Team ID;
	RocketLink->RocketLink_Init.RCFUID = 1;			// RFCU ID;
	RocketLink->RocketLink_Init.compID = 1;			// Component ID;

	RocketLink->rocketStage = PREPARING;
	RocketLink->parameterCheck = 0;
	RocketLink->error = 0;

	return RocketLink_OK;
}

RocketLink_StatusTypeDef initRocketLinkPackage(struct RocketLink *RocketLink){
	RocketLink->sendPackage[0] = startByte;
	RocketLink->sendPackage[1] = startByte;
	RocketLink->sendPackage[2] = RocketLink->RocketLink_Init.teamID;
	RocketLink->sendPackage[3] = RocketLink->RocketLink_Init.RCFUID;
	RocketLink->sendPackage[4] = RocketLink->RocketLink_Init.compID;

	for(int i=13; i<126; i++){
		RocketLink->sendPackage[i] = 77;
	}

	RocketLink->sendPackage[126] = finishByte;
	RocketLink->sendPackage[127] = finishByte;
	return RocketLink_OK;
};

RocketLink_StatusTypeDef setRocketStage(struct RocketLink *RocketLink, RocketLink_Stage Stage){
	RocketLink->rocketStage |= 1 << Stage;
	return RocketLink_OK;
};

RocketLink_StatusTypeDef setBatteryVoltage(struct RocketLink *RocketLink, float batteryVoltage){
	RocketLink->rocketBatteryVoltage = batteryVoltage;
	setBit(RocketLink,0);
	return RocketLink_OK;
};

void setBit(struct RocketLink *RocketLink, uint8_t bitOrder){
	RocketLink->parameterCheck |= 0x01 << bitOrder;
};

RocketLink_StatusTypeDef setSensorStatus(struct RocketLink *RocketLink, RocketLink_SensorStatus SensorStatus){
	RocketLink->sensorStatus |= (0x01) << SensorStatus;
	return RocketLink_OK;
};

void floatToUint8(float dataIn, uint8_t *adress){
	union FloatingPointIEEE754 {
			struct {
				unsigned int mantissa : 23;
				unsigned int exponent : 8;
				unsigned int sign : 1;
			} raw;
			float f;
		}number;

		number.f = dataIn;
		*(adress+3) = (number.raw.mantissa) | 0x00;
		*(adress+2) = (number.raw.mantissa >> 8) | 0x00;
		*(adress+1) = (number.raw.mantissa >> 16) | (number.raw.exponent << 7);
		*(adress+0) = (number.raw.exponent >> 1) | (number.raw.sign << 7);
};

RocketLink_StatusTypeDef setPressure(struct RocketLink *RocketLink, float pressure){
	floatToUint8(pressure, (RocketLink->sendPackage+5));
	return RocketLink_OK;
};

