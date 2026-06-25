#ifndef EDULCO_WATER_SMART_H
#define EDULCO_WATER_SMART_H

#include <Arduino.h>

enum EdulcoChannel {
    EDULCO_CH1 = 1,
    EDULCO_CH2 = 2,
    EDULCO_CH3 = 3,
    EDULCO_CH4 = 4
};

class EdulcoWaterSmart
{
public:
    EdulcoWaterSmart();

    bool begin();

    float getPH(EdulcoChannel channel);
    float getORP(EdulcoChannel channel);

    float getTemperatureNTC(EdulcoChannel channel);
    float getTemperatureDS18();

    float getEC(EdulcoChannel channel);
    float getTDS(EdulcoChannel channel);
    float getSAL(EdulcoChannel channel);

private:
    float readAnalogChannel(EdulcoChannel channel);

    float convertPH(float raw);
    float convertORP(float raw);
    float convertNTC(float raw);
    float convertEC(float raw);

    float ecToTDS(float ec);
    float ecToSAL(float ec);
};

#endif
