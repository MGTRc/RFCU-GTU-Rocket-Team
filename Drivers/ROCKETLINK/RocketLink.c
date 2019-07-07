#include "RocketLink.h"

RocketLink_HandleTypeDef RocketLink;

RocketLink_StatusTypeDef initRocketLink(struct RocketLink *RocketLink , UART_HandleTypeDef myUART){
	RocketLink->myUART = myUART;

	RocketLink->RocketLink_Init.teamID = 212;   // Team ID;
	RocketLink->RocketLink_Init.RCFUID = 1;			// RFCU ID;
	RocketLink->RocketLink_Init.compID = 1;			// Component ID;

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
		RocketLink->sendPackage[i] = 0;
	}

	RocketLink->sendPackage[126] = finishByte;
	RocketLink->sendPackage[127] = finishByte;
	return RocketLink_OK;
};

void setBit(struct RocketLink *RocketLink, uint8_t bitOrder){
	RocketLink->parameterCheck |= 0x01 << bitOrder;
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

void uint16tToUint8(uint16_t dataIn, uint8_t *adress){
	*(adress + 0) = 0x00 | (dataIn >> 8);
	*(adress + 1) |= dataIn;
};

RocketLink_StatusTypeDef setGPSLat(struct RocketLink *RocketLink, float gpslat){
	floatToUint8(gpslat,(RocketLink->sendPackage+5));
	return RocketLink_OK;
}

RocketLink_StatusTypeDef setGPSLong(struct RocketLink *RocketLink, float gpslong){
	floatToUint8(gpslong,(RocketLink->sendPackage+9));
	return RocketLink_OK;
};

RocketLink_StatusTypeDef setGPSHdop(struct RocketLink *RocketLink, float hdop){
	floatToUint8(hdop,(RocketLink->sendPackage+13));
	return RocketLink_OK;
};

RocketLink_StatusTypeDef setGPSAlt(struct RocketLink *RocketLink, float gpsalt){
	floatToUint8(gpsalt,(RocketLink->sendPackage+17));
	return RocketLink_OK;
};

RocketLink_StatusTypeDef setGPSSatelliteNumber(struct RocketLink *RocketLink, uint8_t satelliteNumber){
	RocketLink->sendPackage[15] = satelliteNumber;
	return RocketLink_OK;
};

RocketLink_StatusTypeDef setSensorStatus(struct RocketLink *RocketLink, RocketLink_SensorStatus SensorStatus){
	RocketLink->sendPackage[27] |= (0x01) << SensorStatus;
	return RocketLink_OK;
};

RocketLink_StatusTypeDef setRocketStage(struct RocketLink *RocketLink, RocketLink_Stage Stage){
	RocketLink->sendPackage[28] |= (0x01) << Stage;
	return RocketLink_OK;
};

