#ifndef EDULCO_CONTROLS_H
#define EDULCO_CONTROLS_H

#include <Arduino.h>

bool CTRL_SetType(uint8_t ctrlIndex, const String& typeStr);
bool CTRL_SetCalPoint(uint8_t sensNumb, uint8_t calPoint, int16_t calValue);
bool CTRL_ResetCalPoint(uint8_t sensNumb, uint8_t calPoint);

#endif
