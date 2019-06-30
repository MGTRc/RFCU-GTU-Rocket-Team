/**
 * @file     SDCARD.c																								
 * @author   Mete Can Gazi																										
 * @brief    SDCARD Library							
 * @revision 22.06.2018
 **/

#include "SDCARD.h"

SDCARD_HandleTypeDef SDCARD;

SDCARD_StatusTypeDef initSDCARD(struct SDCARD *SDCARD){
	
	HAL_Delay(100);

	
	 if(f_mount(&SDCARD->fatfs,"",1)== FR_OK){
		SDCARD->SDCARDStatus = 1;
	 }
	
	f_open(&SDCARD->logFile,"rocket.txt",FA_CREATE_ALWAYS);
	f_close(&SDCARD->logFile);
	 
	 
	f_open(&SDCARD->logFile,"rocket.txt",FA_WRITE);
	f_printf(&SDCARD->logFile,"GTU Roket Takimi - BMP180 Test Datasi\n");
	f_close(&SDCARD->logFile);
	 
	 
	return SDCARD_OK;
}

SDCARD_StatusTypeDef SDCARD_writeData(struct SDCARD *SDCARD){



}
