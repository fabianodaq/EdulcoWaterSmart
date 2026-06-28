#ifndef EDULCO_WATER_SMART_H
#define EDULCO_WATER_SMART_H

#include <Arduino.h>
#include "05_AppHW.h"
#include "02_EdulcoSensors.h"

class EdulcoWaterSmart
{
public:
    EdulcoWaterSmart();

    bool begin();

    bool setRelay(uint8_t relayNum, bool state);

    e_SensType getPhOrpType_Channel1();
    e_SensType getPhOrpType_Channel2();
    float getPh(uint8_t channel);
    float getOrp(uint8_t channel);

    e_SensType getEcTdsSalType();
    float getEcTdsSal();

    float getTemperatureDS18();
    float getTemperatureNTC();

};

#endif
