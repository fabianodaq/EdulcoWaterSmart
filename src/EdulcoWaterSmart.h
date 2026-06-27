#ifndef EDULCO_WATER_SMART_H
#define EDULCO_WATER_SMART_H

#include <Arduino.h>


class EdulcoWaterSmart
{
public:
    EdulcoWaterSmart();

    bool begin();
    bool setRelay(uint8_t relayNum, bool state);
    
    float getTemperatureDS18();
    float getTemperatureNTC();
    float getEc();
    float getPh(uint8_t channel);
    float getOrp(uint8_t channel);

};

#endif
