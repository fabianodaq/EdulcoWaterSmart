#include "02_AppSensors.h"

#include <OneWire.h>
#include <DallasTemperature.h>

// -----------typedef defintion Sensors 0 and 3------------
typedef struct	
{	
	long			valueMcp3424;
	uint8_t		ui8Err;

  int		    valueFiltTemp;
  int		    valueFilt;
  int		    valueIst;

	long		  valueBuff[4];
	uint8_t		idxLast;

}	t_PhOrpSensor;

#define NUM_SENS_PH_RX 2

t_PhOrpSensor SensPhRx[NUM_SENS_PH_RX];

//---------------Channel Control 1----------------------
// DS18
struct	// DS18
{
  int				value;	
	uint8_t		ui8Err;
}	SensDs18;

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


//---------------Channel Control 3----------------------
typedef struct	 // EC/TDS/SAL
{
	unsigned char 	type;		

	long			      valueMcp3424;
  int		    valueFiltTemp;
  int       valueFilt;
	int       value;	
	uint8_t   ui8Err;

	long		  valueBuff[4];
	uint8_t		idxLast;

}	t_SensorEc;

t_SensorEc SensEc;



MCP342x adc(MCP3424_ADDR);

// DS18 sensor
OneWire oneWireDS18B20(DS18B20_PIN);
DallasTemperature DS18B20Sensors(&oneWireDS18B20);

//--- Global Variables Variables -------------------------------------------------------

//--- Private Function Declarations -------------------------------------------
void Sens_calc_PHx100_Orp(t_PhOrpSensor* sensor, MCP342x::Channel channel, uint8_t CtrlIdx);
void Sens_DS18_Get();
unsigned int Sens_Ntc_Get();
void Sens_calc_Ec();

//--- Public Function Declarations -------------------------------------------
void  Sens_InitSens();

//-----------------------------------------------------------------------------
//--- PUBLIC Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void Sens_InitSens()
{
  Wire.begin(); //init MCP3424
  delay(500);
  
  MCP342x::generalCallReset();
  delay(1); // MC342x needs 300us to settle, wait 1ms
  Wire.requestFrom(MCP3424_ADDR, (uint8_t)1);
  if (!Wire.available()) {
    Serial.print("[EDULCO SENS] AD Converter found");
    Serial.println(MCP3424_ADDR, HEX);
  }else {
  Serial.print("[EDULCO SENS] AD Converter found");
  Serial.println(MCP3424_ADDR, HEX);
  }

  DS18B20Sensors.begin();       //init DS18   Serial.print("[EDULCO SENS] DS18B20 detected: ");   //Serial.println(DS18B20Sensors.getDeviceCount());

}

 
//-----------------------------------------------------------------------------
//--- Private Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void  Sens_calc_PHx100_Orp(t_PhOrpSensor* sensor, MCP342x::Channel channel, unsigned char CtrlIdx) 
{
  MCP342x::Gain gain 				= MCP342x::gain2; //default PH type  
  MCP342x::Config status 		= MCP342x::Config(0);  

  if (s_Ctrl[CtrlIdx].type == _RX) 
  {
    gain 				= MCP342x::gain1;
  }

  sensor->ui8Err = adc.convertAndRead(  channel, MCP342x::continuous, MCP342x::resolution12, gain, TIMEOUT_US_MCP3424_CHANNEL, sensor->valueMcp3424, status);

  if  (sensor->ui8Err) {  //filter values

  } else {
    long mV = sensor->valueMcp3424; // Assuming mV is a member of the sensor structure
    long mV1 = s_Ctrl[CtrlIdx].CalX1;   int YH1 = s_Ctrl[CtrlIdx].CalY1;
    long mV2 = s_Ctrl[CtrlIdx].CalX2;   int YH2 = s_Ctrl[CtrlIdx].CalY2; 

      if (mV2 != mV1) {
        sensor->valueIst = YH1 + ((mV - mV1) * (YH2  - YH1)) / (mV2 - mV1);

      } else {
        sensor->valueIst = 0; // oppure lascia il valore precedente, o segnala errore

      }

    switch (s_Ctrl[CtrlIdx].type) {
      case _PH:
      {
        if (sensor->valueIst> MAX_PH_VAL) {
          sensor->valueIst = MAX_PH_VAL;
        }
        if (sensor->valueIst < MIN_PH_VAL) {
          sensor->valueIst = MIN_PH_VAL;
        }
        break;
      }

      case _RX:
      {
        if (sensor->valueIst > MAX_RX_VAL) {
            sensor->valueIst = MAX_RX_VAL;
        } else if (sensor->valueIst < MIN_RX_VAL) {
            sensor->valueIst = MIN_RX_VAL;
        }
        break;
      }
    }
   
    sensor->valueBuff[sensor->idxLast] = sensor->valueIst; //udate buffer
    sensor->idxLast = (sensor->idxLast + 1) % 4;
    sensor->valueFiltTemp = 0;

    for (uint8_t i = 0; i < 4; i++) {
      sensor->valueFiltTemp += sensor->valueBuff[i];
    }
    sensor->valueFilt = sensor->valueFiltTemp / 4;

    s_Ctrl[CtrlIdx].ActReadVal = sensor->valueFilt;
  }
}


/**

 */
void Sens_DS18_Get(void) {

    DS18B20Sensors.requestTemperatures();
    float tempC = DS18B20Sensors.getTempCByIndex(0);
    if (tempC == DEVICE_DISCONNECTED_C) {
        SensDs18.ui8Err = 1;
        Serial.println("DS18B20 disconnected!");
        return;
    }
    SensDs18.value = (int)(tempC * 10.0f);
    SensDs18.ui8Err = 0;
}


/**

 */
unsigned int Sens_Ntc_Get()
{
    long tempRaw = 0;
    MCP342x::Config status;
    uint8_t err = 0;
    err = adc.convertAndRead( MCP342x::channel2, MCP342x::continuous, MCP342x::resolution12,
        MCP342x::gain1, TIMEOUT_US_MCP3424_CHANNEL, tempRaw, status);
    if (err) {
        Serial.print("EC TEMP READ ERROR: ");
        Serial.println(err);
        return 250;   // fallback 25.0°C
    }

    long mV = tempRaw;
    long ntcCalc = 0;

    const uint16_t v5C   = pgm_read_word(&NTC_ADC_MIN_MAX[0]);
    const uint16_t v70C  = pgm_read_word(&NTC_ADC_MIN_MAX[NTC_TABLE_MAX_INDEX]);
    const uint16_t v120C = pgm_read_word(&NTC_ADC_HIGH_TEMP[NTC_HIGH_TABLE_MAX_INDEX]);

    // -------- LOW RANGE 5..70 --------
    if (mV >= v5C) {
        ntcCalc = 5 * 10;
    }
    else if (mV >= v70C) {

        int lo = 0;
        int hi = 0;

        for (int i = 0; i < NTC_TABLE_MAX_INDEX; i++) {
            uint16_t v1 = pgm_read_word(&NTC_ADC_MIN_MAX[i]);
            uint16_t v2 = pgm_read_word(&NTC_ADC_MIN_MAX[i + 1]);

            if (mV <= v1 && mV >= v2) {
                lo = i;
                hi = i + 1;
                break;
            }
        }

        uint16_t v1 = pgm_read_word(&NTC_ADC_MIN_MAX[lo]);
        uint16_t v2 = pgm_read_word(&NTC_ADC_MIN_MAX[hi]);

        int T1 = 5 + lo;

        long denom = (long)v1 - (long)v2;

        if (denom <= 0) {
            ntcCalc = (long)T1 * 10;
        } else {
            long num = (long)v1 - mV;
            ntcCalc = (long)T1 * 10 + (num * 10) / denom;
        }
    }

    else if (mV >= v120C) {

        int lo = 0;
        int hi = 0;

        for (int i = 0; i < NTC_HIGH_TABLE_MAX_INDEX; i++) {
            uint16_t v1 = pgm_read_word(&NTC_ADC_HIGH_TEMP[i]);
            uint16_t v2 = pgm_read_word(&NTC_ADC_HIGH_TEMP[i + 1]);

            if (mV <= v1 && mV >= v2) {
                lo = i;
                hi = i + 1;
                break;
            }
        }

        uint16_t v1 = pgm_read_word(&NTC_ADC_HIGH_TEMP[lo]);
        uint16_t v2 = pgm_read_word(&NTC_ADC_HIGH_TEMP[hi]);

        int T1 = 70 + lo;

        long denom = (long)v1 - (long)v2;

        if (denom <= 0) {
            ntcCalc = (long)T1 * 10;
        } else {
            long num = (long)v1 - mV;
            ntcCalc = (long)T1 * 10 + (num * 10) / denom;
        }
    }

    // oltre 120°C
    else {
        ntcCalc = 120 * 10;
    }

    return ntcCalc;

}


/**

 */
void Sens_calc_Ec() 
{
  long tempvalue = 0;
  unsigned static autorangeStatus = AUTO_RANGE_HIGH_RES;

  MCP342x::Config status(0);
  MCP342x::Gain gain 				= MCP342x::gain1; 

  float Rpull;    //convert resistance...
  float lsb_mV;

  if (autorangeStatus == AUTO_RANGE_HIGH_RES)
  {
      SensEc_SetHighRange();
      gain   = MCP342x::gain1;
      Rpull  = R_PULL_UP_HIGH;
      lsb_mV = RESOLUTION_MV_GAIN_1;
  }
  else if (autorangeStatus == AUTO_RANGE_MID_RES)
  {
      SensEc_SetMidRange();
      gain   = MCP342x::gain2;
      Rpull  = R_PULL_UP_MID;
      lsb_mV = RESOLUTION_MV_GAIN_2;
  }
  else if (autorangeStatus == AUTO_RANGE_LOW_RES)
  {
      SensEc_SetLowRange();
      gain   = MCP342x::gain4;
      Rpull  = R_PULL_UP_LOW;
      lsb_mV = RESOLUTION_MV_GAIN_4;
  }

  SensEc.ui8Err = adc.convertAndRead(MCP342x::channel3, MCP342x::continuous, MCP342x::resolution14, gain,
              TIMEOUT_US_MCP3424_CHANNEL, tempvalue, status);

  long adcAbs = labs(tempvalue);

  delay(1);   // reding time

  SensEc_SetOff();

  float Vdiff = abs(tempvalue) * lsb_mV;
  float Rec = (Vdiff * Rpull) / (V_PULL_UP_LOW - Vdiff);    //calculate resistance

  if  (SensEc.ui8Err) {  //filter values

  } else {      //udate buffer
      SensEc.valueBuff[SensEc.idxLast] = Rec;
      SensEc.idxLast = (SensEc.idxLast + 1) % 4;
      SensEc.valueFiltTemp = 0;
      for (uint8_t i = 0; i < 4; i++) {
        SensEc.valueFiltTemp += SensEc.valueBuff[i];
      }
      SensEc.valueFilt = SensEc.valueFiltTemp / 4;

    }

    SensEc_Depolarize(); 

    if (autorangeStatus == AUTO_RANGE_HIGH_RES)
    {
        if (adcAbs < AUTO_RANGE_HIGH_TO_MID_THRESHOLD)
        {
            autorangeStatus = AUTO_RANGE_MID_RES;
            Serial.println("AUTORANGE: HIGH -> MID");
        }
    }
    else if (autorangeStatus == AUTO_RANGE_MID_RES)
    {
        if (adcAbs < AUTO_RANGE_MID_TO_LOW_THRESHOLD)
        {
            autorangeStatus = AUTO_RANGE_LOW_RES;
            Serial.println("AUTORANGE: MID -> LOW");
        }

        if (adcAbs > AUTO_RANGE_MID_TO_HIGH_THRESHOLD)
        {
            autorangeStatus = AUTO_RANGE_HIGH_RES;
            Serial.println("AUTORANGE: MID -> HIGH");
        }
    }
    else if (autorangeStatus == AUTO_RANGE_LOW_RES)
    {
        if (adcAbs > AUTO_RANGE_LOW_TO_MID_THRESHOLD)
        {
            autorangeStatus = AUTO_RANGE_MID_RES;
            Serial.println("AUTORANGE: LOW -> MID");
        }
    }

    unsigned long rawResistance = SensEc.valueFilt;
    unsigned long calResistance;

    long x1 = s_Ctrl[2].CalX1;
    long y1 = s_Ctrl[2].CalY1;
    long x2 = s_Ctrl[2].CalX2;
    long y2 = s_Ctrl[2].CalY2;

    if (x2 == x1) { // Evita divisione per zero
        calResistance = rawResistance;

    } else {
        long result = y1 + ((long)rawResistance - x1) * (y2 - y1) / (x2 - x1);

        if (result < 1) {        // Sicurezza
            result = 1;
        }

        calResistance = result;
    }

    SensEc.valueFilt = calResistance;   //calcualte ressitane

    unsigned long resistance_ohm = SensEc.valueFilt;

    if (resistance_ohm < 1) {
        resistance_ohm = 1; // evita divisione per zero
    }

    long Ec_us = 1000000UL / resistance_ohm;     // EC base

    if (Ec_us > MAX_EC_VAL) {
        Ec_us = MAX_EC_VAL;
    }
    
    long TDS_ppm      = (long)roundf((float)Ec_us * EC_TO_TDS_FACTOR);
    long salinity_ppm = (long)roundf((float)Ec_us * EC_TO_SALINITY_FACTOR);

    if (s_Ctrl[2].type == _EC) {
        SensEc.value = Ec_us;
    } 
    else if (s_Ctrl[2].type == _TDS) { 
        SensEc.value = TDS_ppm;
    } 
    else if (s_Ctrl[2].type == _SAL) { 
        SensEc.value = salinity_ppm;
    }

    unsigned int TemperatureEC = 250;   //read te,perature
    TemperatureEC = Sens_Ntc_Get();

    float temperatureC = TemperatureEC * TEMP_SENSOR_SCALE_FACTOR;
    float denom = 1.0f + EC_TEMP_COMP_ALPHA * (temperatureC - EC_TEMP_COMP_REFERENCE_C);

    if (denom > 0.0f) {
        SensEc.value = (unsigned long)roundf(SensEc.value / denom);
    }
    
    s_Ctrl[2].ActReadVal = SensEc.value;

}