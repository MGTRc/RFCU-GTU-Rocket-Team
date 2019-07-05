#include "GPS.h"

/**
* @brief This function init GPS Library.
**/
GPS_StatusTypeDef initGPS(struct GPS *GPS, UART_HandleTypeDef myUART){
	GPS->gpsIndex = 0;
	GPS->packageFind = 0;
	GPS->myUART = myUART;
	
	return GPS_OK;
};

GPS_StatusTypeDef isGPSReady(struct GPS *GPS){
	HAL_Delay(500);
	settingGPS(GPS);

	/*Sync. Transmitter*/
	uint8_t buffer,status;
	status = 1;
	while(status){
		HAL_UART_Receive(&huart1,&buffer,1,1000);
		if(buffer  == 10){
			HAL_UART_Receive_DMA(&huart1,&GPS->checkBuffer,1);
			status = 0;
		}
	}

	return GPS_OK;

};

/**
* @brief This function parses GPS Data;
**/
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
	GPS->latitude = atof((char*)GPS->lat) / 100.0;
	
	GPS->lon = (uint8_t*)calloc(sizeof(uint8_t) , GPS->commoIndex[5]-GPS->commoIndex[4]);
	GPS->lon = &GPS->gpsBuffer[GPS->commoIndex[4]+1] ;
	GPS->longtitude = atof((char*)GPS->lon) / 100.0;
	
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

GPS_StatusTypeDef settingGPS(struct GPS *GPS){
	/* Set Update Rate*/
	uint8_t set1[] = {0xB5,0x62,0x06,0x08,0x06,0x00,0xE8,0x03,0x01,0x00,0x01,0x00,0x01,0x39};
	uint8_t set2[] = {0xB5,0x62,0x06,0x08,0x00,0x00,0x0E,0x30};
	HAL_UART_Transmit(&GPS->myUART,set1,sizeof(set1),1000);
	HAL_UART_Transmit(&GPS->myUART,set1,sizeof(set2),1000);
	HAL_Delay(100);
	/*Disable All Messages*/
	uint8_t msg1[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x0A,0x00,0x04,0x23};
	for(int i=0; i<7; i++){
		HAL_UART_Transmit(&GPS->myUART,msg1,sizeof(msg1),100);
		HAL_Delay(50);
	}

	uint8_t msg2[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x09,0x00,0x03,0x21};
	for(int i=0; i<7; i++){
				HAL_UART_Transmit(&GPS->myUART,msg2,sizeof(msg2),100);
				HAL_Delay(50);
	}

	uint8_t msg3[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x00,0x00,0xFA,0x0F};
	for(int i=0; i<7; i++){
				HAL_UART_Transmit(&GPS->myUART,msg3,sizeof(msg3),100);
				HAL_Delay(50);
	}

	uint8_t msg4[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x01,0x00,0xFB,0x11};
	for(int i=0; i<7; i++){
				HAL_UART_Transmit(&GPS->myUART,msg4,sizeof(msg4),100);
				HAL_Delay(50);
	}

	uint8_t msg5[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x0D,0x00,0x07,0x29};
	for(int i=0; i<7; i++){
				HAL_UART_Transmit(&GPS->myUART,msg5,sizeof(msg5),100);
				HAL_Delay(50);
	}

	uint8_t msg6[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x06,0x00,0x00,0x1B};
	for(int i=0; i<7; i++){
				HAL_UART_Transmit(&GPS->myUART,msg6,sizeof(msg6),100);
				HAL_Delay(50);
	}

	uint8_t msg7[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x02,0x00,0xFC,0x13};
	for(int i=0; i<7; i++){
				HAL_UART_Transmit(&GPS->myUART,msg7,sizeof(msg7),100);
				HAL_Delay(50);
	}

	uint8_t msg8[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x07,0x00,0x01,0x1D};
	for(int i=0; i<7; i++){
				HAL_UART_Transmit(&GPS->myUART,msg8,sizeof(msg8),100);
				HAL_Delay(50);
	}

	uint8_t msg9[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x03,0x00,0xFD,0x15};
	for(int i=0; i<7; i++){
				HAL_UART_Transmit(&GPS->myUART,msg9,sizeof(msg9),100);
				HAL_Delay(50);
	}

	uint8_t msg10[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x03,0x00,0xFD,0x15};
	for(int i=0; i<10; i++){
				HAL_UART_Transmit(&GPS->myUART,msg10,sizeof(msg10),100);
				HAL_Delay(50);
	}

	uint8_t msg11[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x04,0x00,0xFE,0x17};
	for(int i=0; i<10; i++){
				HAL_UART_Transmit(&GPS->myUART,msg11,sizeof(msg11),100);
				HAL_Delay(50);
	}

	uint8_t msg12[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x04,0x00,0xFE,0x17};
	for(int i=0; i<7; i++){
				HAL_UART_Transmit(&GPS->myUART,msg12,sizeof(msg12),100);
				HAL_Delay(50);
	}

	uint8_t msg13[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x0F,0x00,0x09,0x2D};
	for(int i=0; i<7; i++){
				HAL_UART_Transmit(&GPS->myUART,msg13,sizeof(msg13),100);
				HAL_Delay(50);
	}

	uint8_t msg14[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x05,0x00,0xFF,0x19};
	for(int i=0; i<7; i++){
				HAL_UART_Transmit(&GPS->myUART,msg14,sizeof(msg14),100);
				HAL_Delay(50);
	}

	uint8_t msg15[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x08,0x00,0x02,0x1F};
	for(int i=0; i<7; i++){
				HAL_UART_Transmit(&GPS->myUART,msg15,sizeof(msg15),100);
				HAL_Delay(50);
	}

	uint8_t msg16[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF1,0x00,0x00,0xFB,0x12};
	HAL_UART_Transmit(&GPS->myUART,msg16,sizeof(msg16),100);
	HAL_Delay(50);

	uint8_t msg17[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF1,0x01,0x00,0xFC,0x14};
	HAL_UART_Transmit(&GPS->myUART,msg17,sizeof(msg17),100);
	HAL_Delay(50);

	uint8_t msg18[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF1,0x03,0x00,0xFE,0x18};
	HAL_UART_Transmit(&GPS->myUART,msg18,sizeof(msg18),100);
	HAL_Delay(50);

	uint8_t msg19[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF1,0x04,0x00,0xFF,0x1A};
	HAL_UART_Transmit(&GPS->myUART,msg19,sizeof(msg19),100);
	HAL_Delay(50);

	uint8_t msg20[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF1,0x05,0x00,0x00,0x1C};
	HAL_UART_Transmit(&GPS->myUART,msg20,sizeof(msg20),100);
	HAL_Delay(50);

	uint8_t msg21[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF1,0x06,0x00,0x01,0x1E};
	HAL_UART_Transmit(&GPS->myUART,msg21,sizeof(msg21),100);
	HAL_Delay(50);

	uint8_t msg22[] = {0xB5,0x62,0x05,0x01,0x02,0x00,0x06,0x01,0x0F,0x38};
	HAL_UART_Transmit(&GPS->myUART,msg22,sizeof(msg22),100);
	HAL_Delay(50);

	uint8_t msg23[] = {0xB5,0x62,0x0A,0x04,0x00,0x00,0x0E,0x34};
	HAL_UART_Transmit(&GPS->myUART,msg23,sizeof(msg23),100);
	HAL_Delay(50);

	uint8_t msg24[] = {0xB5,0x62,0x05,0x01,0x02,0x00,0x06,0x01,0x0F,0x38};
	for(int i=0; i<94; i++){
				HAL_UART_Transmit(&GPS->myUART,msg24,sizeof(msg24),100);
				HAL_Delay(50);
	}

	uint8_t msg25[] = {0xB5,0x62,0x05,0x00,0x02,0x00,0x06,0x01,0x0E,0x33};
	HAL_UART_Transmit(&GPS->myUART,msg25,sizeof(msg25),100);
	HAL_Delay(50);

	uint8_t msg26[] = {0xB5,0x62,0x05,0x01,0x02,0x00,0x06,0x01,0x0F,0x38};
	HAL_UART_Transmit(&GPS->myUART,msg26,sizeof(msg26),100);
	HAL_Delay(50);

	uint8_t msg27[] = {0xB5,0x62,0x05,0x01,0x02,0x00,0x06,0x01,0x0F,0x38};
	HAL_UART_Transmit(&GPS->myUART,msg27,sizeof(msg27),100);
	HAL_Delay(50);

	uint8_t msg28[] = {0xB5,0x62,0x05,0x00,0x02,0x00,0x06,0x01,0x0E,0x33};
	HAL_UART_Transmit(&GPS->myUART,msg28,sizeof(msg28),100);
	HAL_Delay(50);

	uint8_t msg29[] = {0xB5,0x62,0x05,0x00,0x02,0x00,0x06,0x01,0x0E,0x33};
	HAL_UART_Transmit(&GPS->myUART,msg29,sizeof(msg29),100);
	HAL_Delay(50);

	/*Enable PUBX*/

	uint8_t msg30[] = {0xB5,0x62,0x06,0x01,0x03,0x00,0xF1,0x00,0x01,0xFC,0x13};
	HAL_UART_Transmit(&GPS->myUART,msg30,sizeof(msg30),100);
	HAL_Delay(50);

	uint8_t msg31[] = {0xB5,0x62,0x05,0x01,0x02,0x00,0x06,0x01,0x0F,0x38};
	HAL_UART_Transmit(&GPS->myUART,msg31,sizeof(msg31),100);
	HAL_Delay(50);

	uint8_t msg32[] = {	0xB5,0x62,0x0A,0x04,0x00,0x00,0x0E,0x34};
	HAL_UART_Transmit(&GPS->myUART,msg32,sizeof(msg32),100);
	HAL_Delay(50);


	HAL_Delay(5000);
	return GPS_OK;
};



