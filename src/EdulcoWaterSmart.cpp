#include "EdulcoWaterSmart.h"
#include "05_AppHW.h"
#include "02_EdulcoSensors.h"

EdulcoWaterSmart::EdulcoWaterSmart()
{
}

bool EdulcoWaterSmart::begin()
{
    HW_InitHw();
    Sens_InitSens();
    return true;
}

bool EdulcoWaterSmart::setRelay(uint8_t relayNum, bool state)
{
    return HW_SetRelay(relayNum, state);
}

float EdulcoWaterSmart::getTemperatureDS18()
{
    float tempC = Sens_DS18_Get();
    if (isnan(tempC)) {
        return NAN;
    }
    return tempC;
}

float EdulcoWaterSmart::getTemperatureNTC()
{
    float ntcC = Sens_Ntc_Get();
    return ntcC;
}

float EdulcoWaterSmart::getEc()
{
    return Sens_Ec_Get();
}

float EdulcoWaterSmart::getPh(uint8_t channel)
{
    return Sens_Ph_Get(channel);
}

float EdulcoWaterSmart::getOrp(uint8_t channel)
{
    return Sens_Orp_Get(channel);
}
