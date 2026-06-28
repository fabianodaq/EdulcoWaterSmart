#include "01_EdulcoControls_private.h"

bool CTRL_SetType(uint8_t ctrlIndex, const String& typeStr)
{
    String t = typeStr;
    t.toLowerCase();

    if (ctrlIndex == 0 || ctrlIndex == 1) {
        e_SensType newType;
        if (t == "ph") {
            newType = _PH;
        } else if (t == "orp" || t == "rx") {
            newType = _RX;
        } else {
            return false;
        }

        if (SensPhRx[ctrlIndex].type == newType) {
            return true;
        }

        EEPR_WriteByte(ctrlIndex == 0 ? SENS_1_TYPE : SENS_2_TYPE, (uint8_t)newType);
        SensPhRx[ctrlIndex].type  = (e_SensType)EEPR_ReadByte(ctrlIndex == 0 ? SENS_1_TYPE : SENS_2_TYPE);

        CTRL_ResetCalPoint(ctrlIndex, 1);
        CTRL_ResetCalPoint(ctrlIndex, 2);


        return true;
    }

    if (ctrlIndex == 2) {
        e_SensType newType;
        if (t == "ec") {
            newType = _EC;
        } else if (t == "tds") {
            newType = _TDS;
        } else if (t == "sal") {
            newType = _SAL;
        } else {
            return false;
        }

        if (SensEc.type == newType) {
            return true;
        }

        EEPR_WriteByte(SENS_EC_TYPE, (uint8_t)newType);
        SensEc.type  = (e_SensType)EEPR_ReadByte(SENS_EC_TYPE);

        CTRL_ResetCalPoint(ctrlIndex, 1);
        CTRL_ResetCalPoint(ctrlIndex, 2);
        
        return true;
    }

    return false;
}

bool CTRL_SetCalPoint(uint8_t sensNumb, uint8_t calPoint, int16_t calValue)
{
    const uint8_t sampleCount = 4;

    if (!((sensNumb == 0) || (sensNumb == 1) || (sensNumb == 2))) {
        return false;
    }

    if (!(calPoint == 1 || calPoint == 2)) {
        return false;
    }

    if (sensNumb == 0 || sensNumb == 1) {
        long rawSum = 0;

        if (SensPhRx[sensNumb].type == _PH) {
            for (uint8_t i = 0; i < sampleCount; i++) {
                (void)Sens_Ph_Get(sensNumb + 1);
                rawSum += SensPhRx[sensNumb].valueMcp3424;
                delay(20);
            }
        } else if (SensPhRx[sensNumb].type == _RX) {
            for (uint8_t i = 0; i < sampleCount; i++) {
                (void)Sens_Orp_Get(sensNumb + 1);
                rawSum += SensPhRx[sensNumb].valueMcp3424;
                delay(20);
            }
        } else {
            return false;
        }

        int16_t xRaw = (int16_t)(rawSum / sampleCount);

        if (sensNumb == 0) {
            if (calPoint == 1) {
                EEPR_WriteInt16(SENS_1_CAL_X1, xRaw);
                EEPR_WriteInt16(SENS_1_CAL_Y1, calValue);
            } else {
                EEPR_WriteInt16(SENS_1_CAL_X2, xRaw);
                EEPR_WriteInt16(SENS_1_CAL_Y2, calValue);
            }

            SensPhRx[0].CalX1 = EEPR_ReadInt16(SENS_1_CAL_X1);
            SensPhRx[0].CalY1 = EEPR_ReadInt16(SENS_1_CAL_Y1);
            SensPhRx[0].CalX2 = EEPR_ReadInt16(SENS_1_CAL_X2);
            SensPhRx[0].CalY2 = EEPR_ReadInt16(SENS_1_CAL_Y2);
        } else {
            if (calPoint == 1) {
                EEPR_WriteInt16(SENS_2_CAL_X1, xRaw);
                EEPR_WriteInt16(SENS_2_CAL_Y1, calValue);
            } else {
                EEPR_WriteInt16(SENS_2_CAL_X2, xRaw);
                EEPR_WriteInt16(SENS_2_CAL_Y2, calValue);
            }

            SensPhRx[1].CalX1 = EEPR_ReadInt16(SENS_2_CAL_X1);
            SensPhRx[1].CalY1 = EEPR_ReadInt16(SENS_2_CAL_Y1);
            SensPhRx[1].CalX2 = EEPR_ReadInt16(SENS_2_CAL_X2);
            SensPhRx[1].CalY2 = EEPR_ReadInt16(SENS_2_CAL_Y2);
        }

        return true;
    }

    if (!(SensEc.type == _EC || SensEc.type == _TDS || SensEc.type == _SAL)) {
        return false;
    }

    long ecRawResistanceSum = 0;
    for (uint8_t i = 0; i < sampleCount; i++) {
        (void)Sens_Ec_Get();
        ecRawResistanceSum += SensEc.value;
        delay(20);
    }

    float ecTarget25 = (float)calValue;
    if (SensEc.type == _TDS) {
        ecTarget25 /= 0.64f;
    } else if (SensEc.type == _SAL) {
        ecTarget25 /= 0.55f;
    }

    if (ecTarget25 < 1.0f) {
        ecTarget25 = 1.0f;
    }

    uint16_t targetResistance = (uint16_t)roundf(1000000.0f / ecTarget25);

    float tempC = Sens_Ntc_Get();
    float tempFactor = 1.0f + (0.02f * (tempC - 25.0f));
    float measuredResistanceAvg = (float)ecRawResistanceSum / (float)sampleCount;
    float measuredResistance25f = measuredResistanceAvg * tempFactor;

    if (measuredResistance25f < 1.0f) {
        measuredResistance25f = 1.0f;
    }

    uint16_t measuredResistance = (uint16_t)roundf(measuredResistance25f);

    if (calPoint == 1) {
        EEPR_WriteUint16(SENS_EC_CAL_X1, measuredResistance);
        EEPR_WriteUint16(SENS_EC_CAL_Y1, targetResistance);
    } else {
        EEPR_WriteUint16(SENS_EC_CAL_X2, measuredResistance);
        EEPR_WriteUint16(SENS_EC_CAL_Y2, targetResistance);
    }

    SensEc.CalX1 = EEPR_ReadInt16(SENS_EC_CAL_X1);
    SensEc.CalY1 = EEPR_ReadInt16(SENS_EC_CAL_Y1);
    SensEc.CalX2 = EEPR_ReadInt16(SENS_EC_CAL_X2);
    SensEc.CalY2 = EEPR_ReadInt16(SENS_EC_CAL_Y2);

    return true;
}

bool CTRL_ResetCalPoint(uint8_t sensNumb, uint8_t calPoint)
{
    if (!((sensNumb == 0) || (sensNumb == 1) || (sensNumb == 2))) {
        return false;
    }

    if (!(calPoint == 1 || calPoint == 2)) {
        return false;
    }

    if (sensNumb == 0 || sensNumb == 1) {
        if (SensPhRx[sensNumb].type == _PH) {
            if (sensNumb == 0) {
                if (calPoint == 1) {
                    EEPR_WriteInt16(SENS_1_CAL_X1, 709);
                    EEPR_WriteInt16(SENS_1_CAL_Y1, 100);
                } else {
                    EEPR_WriteInt16(SENS_1_CAL_X2, -644);
                    EEPR_WriteInt16(SENS_1_CAL_Y2, 1245);
                }
            } else {
                if (calPoint == 1) {
                    EEPR_WriteInt16(SENS_2_CAL_X1, 709);
                    EEPR_WriteInt16(SENS_2_CAL_Y1, 100);
                } else {
                    EEPR_WriteInt16(SENS_2_CAL_X2, -644);
                    EEPR_WriteInt16(SENS_2_CAL_Y2, 1245);
                }
            }
        } else if (SensPhRx[sensNumb].type == _RX) {
            if (sensNumb == 0) {
                if (calPoint == 1) {
                    EEPR_WriteInt16(SENS_1_CAL_X1, -1798);
                    EEPR_WriteInt16(SENS_1_CAL_Y1, -1800);
                } else {
                    EEPR_WriteInt16(SENS_1_CAL_X2, 1798);
                    EEPR_WriteInt16(SENS_1_CAL_Y2, 1800);
                }
            } else {
                if (calPoint == 1) {
                    EEPR_WriteInt16(SENS_2_CAL_X1, -1798);
                    EEPR_WriteInt16(SENS_2_CAL_Y1, -1800);
                } else {
                    EEPR_WriteInt16(SENS_2_CAL_X2, 1798);
                    EEPR_WriteInt16(SENS_2_CAL_Y2, 1800);
                }
            }
        } else {
            return false;
        }

        if (sensNumb == 0) {
            SensPhRx[0].CalX1 = EEPR_ReadInt16(SENS_1_CAL_X1);
            SensPhRx[0].CalY1 = EEPR_ReadInt16(SENS_1_CAL_Y1);
            SensPhRx[0].CalX2 = EEPR_ReadInt16(SENS_1_CAL_X2);
            SensPhRx[0].CalY2 = EEPR_ReadInt16(SENS_1_CAL_Y2);
        } else {
            SensPhRx[1].CalX1 = EEPR_ReadInt16(SENS_2_CAL_X1);
            SensPhRx[1].CalY1 = EEPR_ReadInt16(SENS_2_CAL_Y1);
            SensPhRx[1].CalX2 = EEPR_ReadInt16(SENS_2_CAL_X2);
            SensPhRx[1].CalY2 = EEPR_ReadInt16(SENS_2_CAL_Y2);
        }

        return true;
    }

    if (!(SensEc.type == _EC || SensEc.type == _TDS || SensEc.type == _SAL)) {
        return false;
    }

    if (calPoint == 1) {
        EEPR_WriteUint16(SENS_EC_CAL_X1, 500);
        EEPR_WriteUint16(SENS_EC_CAL_Y1, 500);
    } else {
        EEPR_WriteUint16(SENS_EC_CAL_X2, 2000);
        EEPR_WriteUint16(SENS_EC_CAL_Y2, 2000);
    }

    SensEc.CalX1 = EEPR_ReadInt16(SENS_EC_CAL_X1);
    SensEc.CalY1 = EEPR_ReadInt16(SENS_EC_CAL_Y1);
    SensEc.CalX2 = EEPR_ReadInt16(SENS_EC_CAL_X2);
    SensEc.CalY2 = EEPR_ReadInt16(SENS_EC_CAL_Y2);

    return true;
}
