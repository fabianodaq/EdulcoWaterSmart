#ifndef EDULCO_WATER_SMART_H
#define EDULCO_WATER_SMART_H

#include <Arduino.h>
#include "05_AppHW.h"
#include "02_EdulcoSensors.h"
#include "01_EdulcoControls.h"

class EdulcoWaterSmart
{
public:
    EdulcoWaterSmart();

    bool begin();

    // Hw functions
    bool setRelay(uint8_t relayNum, bool state);

    // Control functions
    bool setControlType(uint8_t ctrlIndex, const String& typeStr);
    bool setControlCalPoint(uint8_t sensNumb, uint8_t calPoint, int16_t calValue);
    bool resetControlCalPoint(uint8_t sensNumb, uint8_t calPoint);

    // ph orp functions
    e_SensType getPhOrpType_Channel1();
    e_SensType getPhOrpType_Channel2();
    float getPh(uint8_t channel);
    float getOrp(uint8_t channel);

    void getCalPointPhOrp_1_Y1(int* s_ycal);
    void getCalPointPhOrp_1_Y2(int* s_ycal);

    void getCalPointPhOrp_2_Y1(int* s_ycal);
    void getCalPointPhOrp_2_Y2(int* s_ycal);

    // ec tds sal functions
    e_SensType getEcTdsSalType();
    float getEcTdsSal();

    void getCalPointEcTdsSal_Y1(int* s_ycal);
    void getCalPointEcTdsSal_Y2(int* s_ycal);

    // Temp functions
    float getTemperatureDS18();
    float getTemperatureNTC();

};

#endif
