#include "02_EdulcoSensors_private.h"

//-----------------------------------------------------------------------------
//--- PUBLIC Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void Sens_InitSens()
{
    //initi hw Sensors 
  Wire.begin(); //init MCP3424
  delay(500);
  
  MCP342x::generalCallReset();
  delay(1); // MC342x needs 300us to settle, wait 1ms
    Wire.beginTransmission(MCP3424_ADDR);
    uint8_t i2cErr = Wire.endTransmission();
    s_mcp3424Available = (i2cErr == 0);

    if (s_mcp3424Available) {
        Serial.print("[EDULCO SENS] AD Converter found at 0x");
        Serial.println(MCP3424_ADDR, HEX);
    } else {
        Serial.print("[EDULCO SENS] AD Converter NOT found at 0x");
        Serial.print(MCP3424_ADDR, HEX);
        Serial.print(" (I2C err ");
        Serial.print(i2cErr);
        Serial.println(")");
    }

  DS18B20Sensors.begin();       //init DS18   Serial.print("[EDULCO SENS] DS18B20 detected: ");   //Serial.println(DS18B20Sensors.getDeviceCount());

    //initi Sw Sensors 

    uint8_t FlagEErpmCtrl     = EEPR_ReadByte(EEPROM_INIT_FLAG_SENS_ADDR);
    uint8_t FlagEErpmCtrlNeg  = EEPR_ReadByte(EEPROM_INIT_FLAG_SENS_NEG_ADDR); 

    if ((FlagEErpmCtrl != EEPROM_INIT_SENS_FLAG  ) || (FlagEErpmCtrlNeg != EEPROM_INIT_FLAG_SENS_NEG)){

        EEPR_WriteByte(SENS_1_TYPE, _PH);

        EEPR_WriteInt16 (SENS_1_CAL_X1, 709);
        EEPR_WriteInt16 (SENS_1_CAL_Y1, 100);
        EEPR_WriteInt16 (SENS_1_CAL_X2, -644);
        EEPR_WriteInt16 (SENS_1_CAL_Y2, 1245);

        EEPR_WriteByte(SENS_2_TYPE, _RX);

        EEPR_WriteInt16 (SENS_2_CAL_X1, -1798);
        EEPR_WriteInt16 (SENS_2_CAL_Y1, -1800);
        EEPR_WriteInt16 (SENS_2_CAL_X2, 1798);
        EEPR_WriteInt16 (SENS_2_CAL_Y2, 1800);

        EEPR_WriteByte(SENS_EC_TYPE, _EC);

        EEPR_WriteInt16 (SENS_EC_CAL_X1, 500);
        EEPR_WriteInt16 (SENS_EC_CAL_Y1, 500);
        EEPR_WriteInt16 (SENS_EC_CAL_X2, 2000);
        EEPR_WriteInt16 (SENS_EC_CAL_Y2, 2000);


        EEPR_WriteByte(EEPROM_INIT_FLAG_SENS_ADDR       ,EEPROM_INIT_SENS_FLAG      ); //---- Init EEProm as initialized----
        EEPR_WriteByte(EEPROM_INIT_FLAG_SENS_NEG_ADDR   ,EEPROM_INIT_FLAG_SENS_NEG  );

        Serial.println("[EDULCO SENS INIT] Initialized with default values");
    } 
  
	SensPhRx[0].type            = (e_SensType) (EEPR_ReadByte(SENS_1_TYPE));

    SensPhRx[0].CalX1           =  EEPR_ReadInt16(SENS_1_CAL_X1);
    SensPhRx[0].CalY1           =  EEPR_ReadInt16(SENS_1_CAL_Y1);
    SensPhRx[0].CalX2           =  EEPR_ReadInt16(SENS_1_CAL_X2);
    SensPhRx[0].CalY2           =  EEPR_ReadInt16(SENS_1_CAL_Y2);

	SensPhRx[1].type            = (e_SensType) (EEPR_ReadByte(SENS_2_TYPE));

    SensPhRx[1].CalX1           =  EEPR_ReadInt16(SENS_2_CAL_X1);
    SensPhRx[1].CalY1           =  EEPR_ReadInt16(SENS_2_CAL_Y1);
    SensPhRx[1].CalX2           =  EEPR_ReadInt16(SENS_2_CAL_X2);
    SensPhRx[1].CalY2           =  EEPR_ReadInt16(SENS_2_CAL_Y2);

    SensEc.type            = (e_SensType) (EEPR_ReadByte(SENS_EC_TYPE));

    SensEc.CalX1           =  EEPR_ReadInt16(SENS_EC_CAL_X1);
    SensEc.CalY1           =  EEPR_ReadInt16(SENS_EC_CAL_Y1);
    SensEc.CalX2           =  EEPR_ReadInt16(SENS_EC_CAL_X2);
    SensEc.CalY2           =  EEPR_ReadInt16(SENS_EC_CAL_Y2);


}



 
//-----------------------------------------------------------------------------
//--- Private Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------


 /**

 */
 float Sens_Ph_Get(uint8_t channel)
{
    if (channel == 1) {
        if (SensPhRx[0].type != _PH) {
            return 0.0f;
        }
        Sens_calc_PHx100_Orp(&SensPhRx[0], MCP342x::channel1, 0);
        if (SensPhRx[0].ui8Err) {
            return 0.0f;
        }
        return ((float)SensPhRx[0].valueIst) / 100.0f;
    }

    if (channel == 2) {
        if (SensPhRx[1].type != _PH) {
            return 0.0f;
        }
        Sens_calc_PHx100_Orp(&SensPhRx[1], MCP342x::channel4, 1);
        if (SensPhRx[1].ui8Err) {
            return 0.0f;
        }
        return ((float)SensPhRx[1].valueIst) / 100.0f;
    }

    return 0.0f;
}

/**

 */
float Sens_Orp_Get(uint8_t channel)
{   
    if (channel == 1) {
        if (SensPhRx[0].type != _RX) {
            return 0.0f;
        }
        Sens_calc_PHx100_Orp(&SensPhRx[0], MCP342x::channel1, 0);
        if (SensPhRx[0].ui8Err) {
            return 0.0f;
        }
        return (float)SensPhRx[0].valueIst;
    }

    if (channel == 2) {
        if (SensPhRx[1].type != _RX) {
            return 0.0f;
        }
        Sens_calc_PHx100_Orp(&SensPhRx[1], MCP342x::channel4, 1);
        if (SensPhRx[1].ui8Err) {
            return 0.0f;
        }
        return (float)SensPhRx[1].valueIst;
    }

    return 0.0f;
}


/**

 */
void  Sens_calc_PHx100_Orp(t_PhOrpSensor* sensor, MCP342x::Channel channel, unsigned char CtrlIdx) 
{
  MCP342x::Gain gain 			= MCP342x::gain2; //default PH type  
  MCP342x::Config status 		= MCP342x::Config(0);  

  if (SensPhRx[CtrlIdx].type == _RX) 
  {
    gain 				= MCP342x::gain1;
  }

  sensor->ui8Err = adc.convertAndRead(  channel, MCP342x::continuous, MCP342x::resolution12, gain, TIMEOUT_US_MCP3424_CHANNEL, sensor->valueMcp3424, status);

  if  (sensor->ui8Err) {  //filter values

  } else {
    long mV = sensor->valueMcp3424; // Assuming mV is a member of the sensor structure
    long mV1 = SensPhRx[CtrlIdx].CalX1;   int YH1 = SensPhRx[CtrlIdx].CalY1;
    long mV2 = SensPhRx[CtrlIdx].CalX2;   int YH2 = SensPhRx[CtrlIdx].CalY2; 

      if (mV2 != mV1) {
        sensor->valueIst = YH1 + ((mV - mV1) * (YH2  - YH1)) / (mV2 - mV1);

      } else {
        sensor->valueIst = 0; // oppure lascia il valore precedente, o segnala errore

      }

    switch (SensPhRx[CtrlIdx].type) {
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
   
  }
}


/**

 */
float Sens_DS18_Get(void) {

    DS18B20Sensors.requestTemperatures();
    return DS18B20Sensors.getTempCByIndex(0);
}


/**

 */
float  Sens_Ntc_Get()
{

    long tempRaw = 0;
    MCP342x::Config status(0);
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

    return ((float)ntcCalc) / 10.0f;

}

float Sens_Ec_Get()
{
    if (SensEc.type != _EC && SensEc.type != _TDS && SensEc.type != _SAL) {
        return 0.0f;
    }

    Sens_calc_Ec();

    if (SensEc.ui8Err) {
        return 0.0f;
    }

    return (float)SensEc.value;
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
            //Serial.println("AUTORANGE: HIGH -> MID");
        }
    }
    else if (autorangeStatus == AUTO_RANGE_MID_RES)
    {
        if (adcAbs < AUTO_RANGE_MID_TO_LOW_THRESHOLD)
        {
            autorangeStatus = AUTO_RANGE_LOW_RES;
            //Serial.println("AUTORANGE: MID -> LOW");
        }

        if (adcAbs > AUTO_RANGE_MID_TO_HIGH_THRESHOLD)
        {
            autorangeStatus = AUTO_RANGE_HIGH_RES;
            //Serial.println("AUTORANGE: MID -> HIGH");
        }
    }
    else if (autorangeStatus == AUTO_RANGE_LOW_RES)
    {
        if (adcAbs > AUTO_RANGE_LOW_TO_MID_THRESHOLD)
        {
            autorangeStatus = AUTO_RANGE_MID_RES;
            //Serial.println("AUTORANGE: LOW -> MID");
        }
    }

    unsigned long rawResistance = SensEc.valueFilt;
    unsigned long calResistance;

    long x1 = SensEc.CalX1;
    long y1 = SensEc.CalY1;
    long x2 = SensEc.CalX2;
    long y2 = SensEc.CalY2;

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

    if (SensEc.type == _EC) {
        SensEc.value = Ec_us;
    } 
    else if (SensEc.type == _TDS) { 
        SensEc.value = TDS_ppm;
    } 
    else if (SensEc.type == _SAL) { 
        SensEc.value = salinity_ppm;
    }

    float temperatureC = Sens_Ntc_Get();
    float denom = 1.0f + EC_TEMP_COMP_ALPHA * (temperatureC - EC_TEMP_COMP_REFERENCE_C);

    if (denom > 0.0f) {
        SensEc.value = (unsigned long)roundf(SensEc.value / denom);
    }

}