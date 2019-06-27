/**
 * @file     SDCARD.H																									
 * @author   Mete Can Gazi																										
 * @brief    SDCARD Library							
 * @revision 5.11.2018
 **/

#ifndef _SDCARD_H
#define _SDCARD_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "sdio.h"
#include "gpio.h"

typedef enum{
	SDCARD_OK,
	SDCARD_ERROR
}SDCARD_StatusTypeDef;

typedef struct SDCARD{	
	FATFS   fatfs; 		/* FAT File System */
	FRESULT fresult;  /* FAT File Result */
	FIL logFile; 			/* FAT File*/
	FSIZE_t ofs;			/* FAT File Size*/
	uint8_t SDCARDStatus;
	char temp_array[100];
}SDCARD_HandleTypeDef;

SDCARD_StatusTypeDef initSDCARD(struct SDCARD *SDCARD);
SDCARD_StatusTypeDef SDCARD_writeData(struct SDCARD *SDCARD);

extern SDCARD_HandleTypeDef SDCARD;

#ifdef __cplusplus
}
#endif

#endif
