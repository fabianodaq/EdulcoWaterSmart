#ifndef APP_SENSORS_H
#define APP_SENSORS_H

//------- Global variables---------------
extern void Sens_InitSens();
extern float Sens_DS18_Get();
extern float Sens_Ntc_Get();
extern float Sens_Ph_Get(uint8_t channel);
extern float Sens_Orp_Get(uint8_t channel);
extern float Sens_Ec_Get();

#endif 
