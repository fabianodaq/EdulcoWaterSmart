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

bool EdulcoWaterSmart::setControlType(uint8_t ctrlIndex, const String& typeStr)
{
    return CTRL_SetType(ctrlIndex, typeStr);
}

bool EdulcoWaterSmart::setControlCalPoint(uint8_t sensNumb, uint8_t calPoint, int16_t calValue)
{
    return CTRL_SetCalPoint(sensNumb, calPoint, calValue);
}

bool EdulcoWaterSmart::resetControlCalPoint(uint8_t sensNumb, uint8_t calPoint)
{
    return CTRL_ResetCalPoint(sensNumb, calPoint);
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

void EdulcoWaterSmart::getCalPointPhOrp_1_Y1(int* s_ycal)
{
    s_ycal[0] = SensPhRx[0].CalY1;
    s_ycal[1] = SensPhRx[0].CalX1;
}

void EdulcoWaterSmart::getCalPointPhOrp_1_Y2(int* s_ycal)
{
    s_ycal[0] = SensPhRx[0].CalY2;
    s_ycal[1] = SensPhRx[0].CalX2;
}

void EdulcoWaterSmart::getCalPointPhOrp_2_Y1(int* s_ycal)
{
    s_ycal[0] = SensPhRx[1].CalY1;
    s_ycal[1] = SensPhRx[1].CalX1;
}

void EdulcoWaterSmart::getCalPointPhOrp_2_Y2(int* s_ycal)
{
    s_ycal[0] = SensPhRx[1].CalY2;
    s_ycal[1] = SensPhRx[1].CalX2;
}

void EdulcoWaterSmart::getCalPointEcTdsSal_Y1(int* s_ycal)
{
    s_ycal[0] = SensEc.CalY1;
    s_ycal[1] = SensEc.CalX1;
}

void EdulcoWaterSmart::getCalPointEcTdsSal_Y2(int* s_ycal)
{
    s_ycal[0] = (float)SensEc.CalY2;
    s_ycal[1] = (float)SensEc.CalX2;
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




