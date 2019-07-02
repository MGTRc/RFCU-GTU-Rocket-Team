#include "BMP180.h"

BMP180_StatusTypeDef initBMP180(struct BMP180 *BMP180,I2C_HandleTypeDef myI2C){
	BMP180->myI2C = myI2C;
	BMP180->statusBMP180 = 0;
	return BMP180_OK;
};

BMP180_StatusTypeDef isBMP180Ready(struct BMP180 *BMP180){
	if(HAL_I2C_IsDeviceReady(&BMP180->myI2C,BMP180_Write_Adress,100,100) == HAL_OK){
		if(HAL_I2C_IsDeviceReady(&BMP180->myI2C,BMP180_Read_Adress,100,100) == HAL_OK){
			BMP180->statusBMP180 = 1;
			return BMP180_OK;
		}
	}else{
		return BMP180_ERROR;
	}
};

void readCalibrationValues(struct BMP180 *BMP180){
	/*int16_t parameters*/
	BMP180->ac1 = (int16_t)read2BytesBMP180(BMP180,BMP180_AC1);
	BMP180->ac2 = (int16_t)read2BytesBMP180(BMP180,BMP180_AC2);
	BMP180->ac3 = (int16_t)read2BytesBMP180(BMP180,BMP180_AC3);
	BMP180->b1 = (int16_t)read2BytesBMP180(BMP180,BMP180_B1);
	BMP180->b2 = (int16_t)read2BytesBMP180(BMP180,BMP180_B2);
	BMP180->mb = (int16_t)read2BytesBMP180(BMP180,BMP180_MB);
	BMP180->mc = (int16_t)read2BytesBMP180(BMP180,BMP180_MC);
	BMP180->md = (int16_t)read2BytesBMP180(BMP180,BMP180_MD);
	
	/*uint16_t parameters*/
	BMP180->ac4 = read2BytesBMP180(BMP180,BMP180_AC4);
	BMP180->ac5 = read2BytesBMP180(BMP180,BMP180_AC5);
	BMP180->ac6 = read2BytesBMP180(BMP180,BMP180_AC6);
};

uint16_t read2BytesBMP180(struct BMP180 *BMP180, uint8_t adress){
	uint8_t dataBuffer[2];
	HAL_I2C_Mem_Read(&BMP180->myI2C,BMP180_Read_Adress,adress,I2C_MEMADD_SIZE_8BIT,dataBuffer,2,100);
	return  (uint16_t)(dataBuffer[1]) | (dataBuffer[0] << 8); 
};

void writeByteBMP180(struct BMP180 *BMP180,uint8_t adress, uint8_t command){
	HAL_I2C_Mem_Write(&BMP180->myI2C,BMP180_Write_Adress,adress,I2C_MEMADD_SIZE_8BIT,&command,1,100);
};

void readByteBMP180 (struct BMP180 *BMP180,uint8_t adress, uint8_t *toWrite){
	HAL_I2C_Mem_Read(&BMP180->myI2C,BMP180_Read_Adress,adress,I2C_MEMADD_SIZE_8BIT,toWrite,1,100);
};

BMP180_StatusTypeDef calcPressure(struct BMP180 *BMP180){
	/*read uncompensated temp*/
	writeByteBMP180(BMP180,BMP180_CTRL_MEAS,0xAE);
	HAL_Delay(14);
	BMP180->ut = (int32_t)read2BytesBMP180(BMP180,BMP180_OUT_MSB);
	/*read uncompensated pressure*/
	writeByteBMP180(BMP180,BMP180_CTRL_MEAS,0xB4);
	HAL_Delay(14);
	uint8_t msb,lsb,xlsb;
	readByteBMP180(BMP180,BMP180_OUT_MSB,&msb);
	readByteBMP180(BMP180,BMP180_OUT_LSB,&lsb);
	readByteBMP180(BMP180,BMP180_OUT_XLSB,&xlsb);
	BMP180->up = (int32_t)((msb << 16) + (lsb<<8)) >> (6);
	/*calculate true temperature*/
	int32_t x1,x2,x3,b3,b4,b5,b6,b7;
	x1= (int32_t)((BMP180->ut-BMP180->ac6)*(BMP180->ac5 / pow(2,15)));
	x2 = (int32_t)((BMP180->mc*pow(2,11))/(x1+BMP180->md));
	b5 = (int32_t)(x1 + x2);
	BMP180->temperature =  (int32_t)((b5 + 8)/pow(2,4));
	BMP180->temperatureDeg = BMP180->temperature * 0.1;

	/*calculate true pressure*/	
	b6 = b5 - 4000;
	x1 = (int32_t)(BMP180->b2*(b6*b6/pow(2,12))/pow(2,11));
	x2 = BMP180->ac2 * b6 / pow(2,11);
	x3 = x1 + x2;
	b3 = (int32_t)((((int32_t)BMP180->ac1*4 + x3)<<2) / 4);
	x1 = (int32_t)((BMP180->ac3 *b6) / pow(2,13));
	x2 = (int32_t)((BMP180->b1*(b6*b6 / pow(2,12)))/(pow(2,16)));
	x3 = (int32_t)(((x1+x2)^2)/pow(2,2));
	b4 = (uint32_t)(BMP180->ac4 * (uint32_t)(x3+32768) / pow(2,15));
	b7 = (uint32_t)(((uint32_t)BMP180->up) - b3)*(50000>>2);
	
	if(b7 < 0x80000000){
		BMP180->pressure = (b7/b4*2);
	}
	
	x1 = (int32_t)((BMP180->pressure / pow(2,8))*(BMP180->pressure / pow(2,8)));
 	x1 = (int32_t)(x1 * 3038 / pow(2,16));
	x2 = (int32_t)(-7357*BMP180->pressure / pow(2,16));
	BMP180->pressure = (int32_t)(BMP180->pressure + ((x1 + x2 + 3791) / pow(2,4)));
	BMP180->pressurehPa = BMP180->pressure * 0.01;
	return BMP180_OK;
}

BMP180_StatusTypeDef calibrateBMP180(struct BMP180 *BMP180){
	readCalibrationValues(BMP180);
	
	int32_t *buffer = (int32_t*)calloc(sizeof(int32_t),100);
	
	for(uint8_t i=0; i<100; i++){
		calcPressure(BMP180);
		*(buffer+i) = BMP180->pressure;
	}
	
	bubblesort(buffer,100);
	BMP180->refPressure = *(buffer+50);
	
	return BMP180_OK;
};

BMP180_StatusTypeDef calcAbsAltitude(struct BMP180 *BMP180){
	BMP180->absAltitude = (float)((BMP180->pressure - BMP180->refPressure)*0.01*8.43)*-1.0;
	return BMP180_OK;
};

void bubblesort(int32_t *list, uint32_t n){
		int32_t c, d, t;
	 
		for (c = 0 ; c < n - 1; c++)
		{
			for (d = 0 ; d < n - c - 1; d++)
			{
				if (list[d] > list[d+1])
				{
					t = list[d];
					list[d] = list[d+1];
					list[d+1] = t;
				}
			}
		}
};
