#include "EdulcoWaterSmart.h"

EdulcoWaterSmart::EdulcoWaterSmart()
{
}

bool EdulcoWaterSmart::begin()
{
    return true;
}

float EdulcoWaterSmart::getPH(EdulcoChannel channel)
{
    return 7.00;
}

float EdulcoWaterSmart::getORP(EdulcoChannel channel)
{
    return 250.0;
}

float EdulcoWaterSmart::getTemperatureNTC(EdulcoChannel channel)
{
    return 25.0;
}

float EdulcoWaterSmart::getTemperatureDS18()
{
    return 24.8;
}

float EdulcoWaterSmart::getEC(EdulcoChannel channel)
{
    return 1.50;
}

float EdulcoWaterSmart::getTDS(EdulcoChannel channel)
{
    return 750.0;
}

float EdulcoWaterSmart::getSAL(EdulcoChannel channel)
{
    return 0.80;
}

float EdulcoWaterSmart::readAnalogChannel(EdulcoChannel channel)
{
    return 0.0;
}

float EdulcoWaterSmart::convertPH(float raw)
{
    return raw;
}

float EdulcoWaterSmart::convertORP(float raw)
{
    return raw;
}

float EdulcoWaterSmart::convertNTC(float raw)
{
    return raw;
}

float EdulcoWaterSmart::convertEC(float raw)
{
    return raw;
}

float EdulcoWaterSmart::ecToTDS(float ec)
{
    return ec;
}

float EdulcoWaterSmart::ecToSAL(float ec)
{
    return ec;
}
