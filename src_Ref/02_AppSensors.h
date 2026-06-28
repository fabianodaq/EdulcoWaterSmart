#ifndef APP_SENSORS_H
#define APP_SENSORS_H

#include <Arduino.h>
#include <Wire.h>
#include <MCP342x.h>
#include "03_EEprom.h"
#include "05_AppHW.h"

//----------- Constants definitions-----------------
#define MCP3424_ADDR 0x6E

#define TIMEOUT_US_MCP3424_CHANNEL 2000000  // 2 secondi

#define NUMER_OF_SENSORS 4 //usually one Ph one redox and 2 more... maybe NTC and EC
#define SENSORS_MAX_IDX (NUMER_OF_SENSORS-1)

#define SENS_TIME_REFRESH 1000 //in ms
#define SENS_TIME_INTERVAL (SENS_TIME_REFRESH/NUMER_OF_SENSORS) //in ms

//------- PH sensor limits 
#define MAX_PH_VAL 1400
#define MIN_PH_VAL 1

//------- ORP sensor limits 
#define MAX_RX_VAL 2000
#define MIN_RX_VAL -2000

//------- Temperature sensor limits (in tenths of degrees Celsius)
#define MAX_TEMP_VAL 1250   // 125.0 °C
#define MIN_TEMP_VAL -400   // -40.0 °C

//------- Humidity sensor limits (in tenths of percent)
#define MAX_HUM_VAL 1000    // 100.0 %
#define MIN_HUM_VAL 0       // 0.0 %

//------- NTC sensor limits (example values)
#define MAX_DS18_VAL   1250   // 125.0 °C
#define MIN_DS18_VAL   -550   // -55.0 °C

//------- DS18 sensor limits (example values)
#define MAX_DS18_VAL 10000   // 10,000 ohms (example)
#define MIN_DS18_VAL 0       // 0 ohms (example)

//------- EC sensor limits (example values)
#define MAX_EC_VAL 20000     // 2000 µS/cm (example)
#define MIN_EC_VAL 10        // 0 µS/cm (example)


#define AUTO_RANGE_HIGH_RES 	0 // autorange values
#define AUTO_RANGE_MID_RES 		1 
#define AUTO_RANGE_LOW_RES 		2 

#define RESOLUTION_MV_GAIN_1 0.25f		//resolution different gains 
#define RESOLUTION_MV_GAIN_2 0.125f
#define RESOLUTION_MV_GAIN_4 0.0625f

#define MAX_EC_VAL 100000UL

#define EC_TO_TDS_FACTOR        0.64f		//sal and tds factors
#define EC_TO_SALINITY_FACTOR   0.55f

#define AUTO_RANGE_HIGH_TO_MID_THRESHOLD   572		//autorange limits
#define AUTO_RANGE_MID_TO_LOW_THRESHOLD    588
#define AUTO_RANGE_MID_TO_HIGH_THRESHOLD   7600
#define AUTO_RANGE_LOW_TO_MID_THRESHOLD    7050

#define EC_TEMP_COMP_ALPHA            0.02f		//temperarure compensaztion
#define EC_TEMP_COMP_REFERENCE_C      25.0f
#define TEMP_SENSOR_SCALE_FACTOR      0.1f

//------- Global variables---------------
extern void Sens_HanldeSens();

extern void Sens_InitSens();

extern unsigned int Sens_Ntc_Get();


#endif 
