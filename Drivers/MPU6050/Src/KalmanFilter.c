#include 	"KalmanFilter.h"

KALMAN_StatusTypeDef initKALMAN(struct KALMAN *KALMAN){
	KALMAN->Q_angle = 0.001f;
	KALMAN->Q_bias = 0.003f;
	KALMAN->R_measure = 0.03f;
	
	KALMAN->angle = 0.0f;
	KALMAN->bias = 0.0f;
	
	KALMAN->P[0][0] = 0;
	KALMAN->P[0][1] = 0;
	KALMAN->P[1][0] = 0;
	KALMAN->P[1][1] = 0;
}

float getAngle(struct KALMAN *KALMAN,float newAngle, float newRate, float dt){
	KALMAN->rate = newRate - KALMAN->bias;
	KALMAN->angle += dt * KALMAN->rate;
	
	KALMAN->P[0][0] += dt * (dt*KALMAN->P[1][1] - KALMAN->P[0][1] - KALMAN->P[1][0] + KALMAN->Q_angle);
	KALMAN->P[0][1] -=  dt * KALMAN->P[1][1];
	KALMAN->P[1][0] -= dt * KALMAN->P[1][1];
	KALMAN->P[1][1] += KALMAN->Q_bias * dt;
	
	float S = KALMAN->P[0][0] + KALMAN->R_measure;
	float K[2];
  K[0] = KALMAN->P[0][0] / S;
  K[1] = KALMAN->P[1][0] / S;
	
	 float y = newAngle - KALMAN->angle; 
   KALMAN->angle += K[0] * y;
	 KALMAN->bias += K[1] * y;
	
	float P00_temp = KALMAN->P[0][0];
  float P01_temp = KALMAN->P[0][1];
	
	 KALMAN->P[0][0] -= K[0] * P00_temp;
   KALMAN->P[0][1] -= K[0] * P01_temp;
   KALMAN->P[1][0] -= K[1] * P00_temp;
   KALMAN->P[1][1] -= K[1] * P01_temp;
	 
	 return KALMAN->angle;
}

void setAngle(struct KALMAN *KALMAN,float angle){
	KALMAN->angle = angle;
}