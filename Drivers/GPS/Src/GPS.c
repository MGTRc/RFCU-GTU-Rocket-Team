#include "GPS.h"

GPS_StatusTypeDef initGPS(struct GPS *GPS){
	GPS->gpsIndex = 0;
	GPS->gpsStatus = 0;
	GPS->packageFind = 0;
	HAL_Delay(1000);
	uint8_t buffer,status;
	status = 1;
	while(status){
		HAL_UART_Receive(&huart1,&buffer,1,1000);
		if(buffer  == 10){
			HAL_UART_Receive_DMA(&huart1,&GPS->checkBuffer,1);
			status = 0;
		}
	}
};

GPS_StatusTypeDef parseGPS(struct GPS *GPS){
	uint8_t index = 0;
	uint8_t commo = 0;
	uint8_t loopControl = 1;
	
	while(loopControl){
		if(GPS->gpsBuffer[index] == 44){
			GPS->commoIndex[commo] = index;
			commo ++;
		}
		index ++;
		
		if(GPS->gpsBuffer[index] == 13){
			loopControl = 0;
		}
	};
	
	GPS->lat = (uint8_t*)calloc(sizeof(uint8_t) , GPS->commoIndex[3]-GPS->commoIndex[2]);
	GPS->lat = &GPS->gpsBuffer[GPS->commoIndex[2]+1] ;
	GPS->latitude = atof((char*)GPS->lat) / 100;
	
	GPS->lon = (uint8_t*)calloc(sizeof(uint8_t) , GPS->commoIndex[5]-GPS->commoIndex[4]);
	GPS->lon = &GPS->gpsBuffer[GPS->commoIndex[4]+1] ;
	GPS->longtitude = atof((char*)GPS->lon) / 100;
	
	GPS->alt = (uint8_t*)calloc(sizeof(uint8_t) , GPS->commoIndex[7]-GPS->commoIndex[6]);
	GPS->alt= &GPS->gpsBuffer[GPS->commoIndex[6]+1] ;
	GPS->altitude = atof((char*)GPS->alt);
	
	GPS->h_dop = (uint8_t*)calloc(sizeof(uint8_t) , GPS->commoIndex[15]-GPS->commoIndex[14]);
	GPS->h_dop= &GPS->gpsBuffer[GPS->commoIndex[14]+1] ;
	GPS->hdop = atof((char*)GPS->h_dop);
	
	GPS->v_dop = (uint8_t*)calloc(sizeof(uint8_t) , GPS->commoIndex[16]-GPS->commoIndex[15]);
	GPS->v_dop= &GPS->gpsBuffer[GPS->commoIndex[15]+1] ;
	GPS->vdop = atof((char*)GPS->v_dop);
	
	GPS->sat = (uint8_t*)calloc(sizeof(uint8_t) , GPS->commoIndex[18]-GPS->commoIndex[17]);
	GPS->sat= &GPS->gpsBuffer[GPS->commoIndex[17]+1] ;
	GPS->satellite = atoi((char*)GPS->sat);
	
	return GPS_OK;

};
