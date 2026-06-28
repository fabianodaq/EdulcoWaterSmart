#ifndef APP_SENSORS_PRIVATE_H
#define APP_SENSORS_PRIVATE_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include "02_EdulcoSensors.h"
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









// NTC channel 2 MCP3424
static const uint16_t NTC_ADC_MIN_MAX[66] PROGMEM = { // Vout[mV] per T = 5..70 °C (pull-up 7.5k, NTC 10k@25°C, β=3950, Vref=2.5V)
  1933,1911,1888,1865,1842,
  1818,1794,1769,1744,1719,
  1694,1668,1642,1616,1589,
  1562,1536,1509,1482,1455,
  1429,1401,1374,1347,1320,
  1293,1266,1240,1213,1187,
  1161,1135,1110,1085,1060,
  1035,1011, 986, 963, 939,
   916, 894, 872, 850, 828,
   809, 787, 766, 746, 727,
   708, 689, 671, 653, 636,
   619, 602, 586, 570, 555,
   540, 525, 511, 497, 483,
   470
};

#define NTC_TABLE_SIZE      (sizeof(NTC_ADC_MIN_MAX) / sizeof(uint16_t)) // Se puoi, evita l’indice hardcoded: calcolalo
#undef  NTC_TABLE_MAX_INDEX
#define NTC_TABLE_MAX_INDEX (NTC_TABLE_SIZE - 1)

// NTC 10k β3950, pull-up 7.5k, Vref 2.5V
// Vout[mV] per T = 70..120 °C
static const uint16_t NTC_ADC_HIGH_TEMP[51] PROGMEM = {
  475, 462, 450, 438, 426,
  414, 403, 392, 381, 371,
  361, 351, 341, 332, 323,
  314, 305, 297, 289, 281,
  273, 266, 259, 252, 245,
  238, 232, 226, 220, 214,
  208, 203, 197, 192, 187,
  182, 177, 173, 168, 164,
  160, 156, 152, 148, 145,
  142, 138, 135, 132, 129
};

#define NTC_HIGH_TEMP_MIN_C      70
#define NTC_HIGH_TEMP_MAX_C      120
#define NTC_HIGH_TABLE_SIZE      (sizeof(NTC_ADC_HIGH_TEMP) / sizeof(uint16_t))
#define NTC_HIGH_TABLE_MAX_INDEX (NTC_HIGH_TABLE_SIZE - 1)


// Sensors data structure
t_PhOrpSensor   SensPhRx[NUM_SENS_PH_RX];
t_SensorEc      SensEc;

// hw sensors structs
MCP342x adc(MCP3424_ADDR);
static bool s_mcp3424Available = false;

// DS18 sensor
OneWire oneWireDS18B20(DS18B20_PIN);
DallasTemperature DS18B20Sensors(&oneWireDS18B20);

//--- Global Variables Variables -------------------------------------------------------

//--- Function Declarations -------------------------------------------
void Sens_calc_PHx100_Orp(t_PhOrpSensor* sensor, MCP342x::Channel channel, uint8_t CtrlIdx);
float Sens_DS18_Get();
float Sens_Ntc_Get();
float Sens_Ph_Get(uint8_t channel);
float Sens_Orp_Get(uint8_t channel);
float Sens_Ec_Get();

void Sens_calc_Ec();

void  Sens_InitSens();

#endif // APP_SENSORS_PRIVATE_H