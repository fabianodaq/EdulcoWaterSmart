#include "EdulcoWaterSmart.h"




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


e_SensType EdulcoWaterSmart::getPhOrpType_Channel1()
{
    return SensPhRx[0].type;
}

e_SensType EdulcoWaterSmart::getPhOrpType_Channel2()
{
    return SensPhRx[1].type;
}

float EdulcoWaterSmart::getPh(uint8_t channel)
{
    return Sens_Ph_Get(channel);
}

float EdulcoWaterSmart::getOrp(uint8_t channel)
{
    return Sens_Orp_Get(channel);
}


e_SensType EdulcoWaterSmart::getEcTdsSalType()
{
    return SensEc.type;
}

float EdulcoWaterSmart::getEcTdsSal()
{
    return Sens_Ec_Get();
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




