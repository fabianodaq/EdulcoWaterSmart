#include "01_AppControls.h"
#include "05_AppHW.h"

//--- Private Variables -------------------------------------------------------
//--- Public Variables --------------------------------------------------------
t_ctrl	s_Ctrl[NUMER_OF_CONTROLS];	// controls for each possible control are defined

void Ctrl_InitControls();

static const char* Ctrl_TypeToName(uint8_t type)
{
    switch (type) {
        case _PH:   return "PH";
        case _RX:   return "ORP";
        case _DS18: return "DS18";
        case _NTC:  return "NTC";
        case _DIFF: return "DIFF";
        case _AVG:  return "AVG";
        case _EC:   return "EC";
        case _TDS:  return "TDS";
        case _SAL:  return "SAL";
        default:    return "UNKNOWN";
    }
}

//-----------------------------------------------------------------------------
//--- PUBLIC Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void Ctrl_InitControls(){

    unsigned char idx;

    uint8_t FlagEErpmCtrl     = EEPR_ReadByte(EEPROM_INIT_FLAG_CTRL_ADDR);
    uint8_t FlagEErpmCtrlNeg  = EEPR_ReadByte(EEPROM_INIT_FLAG_CTRL_NEG_ADDR); 

    if ((FlagEErpmCtrl != EEPROM_INIT_CTRL_FLAG ) || (FlagEErpmCtrlNeg != EEPROM_INIT_FLAG_CTRL_NEG)){

        EEPR_WriteByte(CTRL_1_TYPE, _PH);

        EEPR_WriteInt16 (SENS_1_CAL_X1, 709);
        EEPR_WriteInt16 (SENS_1_CAL_Y1, 100);
        EEPR_WriteInt16 (SENS_1_CAL_X2, -644);
        EEPR_WriteInt16 (SENS_1_CAL_Y2, 1245);

        EEPR_WriteByte(CTRL_2_TYPE, _DS18);

        EEPR_WriteUint16 (SENS_3_CAL_X1, 500);
        EEPR_WriteUint16 (SENS_3_CAL_Y1, 500);
        EEPR_WriteUint16 (SENS_3_CAL_X2, 2000);
        EEPR_WriteUint16 (SENS_3_CAL_Y2, 2000);

        EEPR_WriteByte(CTRL_3_TYPE, _EC);

        EEPR_WriteInt16 (SENS_4_CAL_X1, -1798);
        EEPR_WriteInt16 (SENS_4_CAL_Y1, -1800);
        EEPR_WriteInt16 (SENS_4_CAL_X2, 1798);
        EEPR_WriteInt16 (SENS_4_CAL_Y2, 1800);

        EEPR_WriteByte(CTRL_4_TYPE, _RX);

        EEPR_WriteByte(EEPROM_INIT_FLAG_CTRL_ADDR       ,EEPROM_INIT_CTRL_FLAG      ); //---- Init EEProm as initialized----
        EEPR_WriteByte(EEPROM_INIT_FLAG_CTRL_NEG_ADDR   ,EEPROM_INIT_FLAG_CTRL_NEG  );

        Serial.println("[EDULCO CTRL INIT] Initialized with default values");
    } 
  
	s_Ctrl[0].type            = EEPR_ReadByte(CTRL_1_TYPE);

    s_Ctrl[0].CalX1           =  EEPR_ReadInt16(SENS_1_CAL_X1);
    s_Ctrl[0].CalY1           =  EEPR_ReadInt16(SENS_1_CAL_Y1);
    s_Ctrl[0].CalX2           =  EEPR_ReadInt16(SENS_1_CAL_X2);
    s_Ctrl[0].CalY2           =  EEPR_ReadInt16(SENS_1_CAL_Y2);


	s_Ctrl[1].type            = EEPR_ReadByte(CTRL_2_TYPE);

    s_Ctrl[1].CalX1           =  EEPR_ReadInt16(SENS_2_CAL_X1);
    s_Ctrl[1].CalY1           =  EEPR_ReadInt16(SENS_2_CAL_Y1);
    s_Ctrl[1].CalX2           =  EEPR_ReadInt16(SENS_2_CAL_X2);
    s_Ctrl[1].CalY2           =  EEPR_ReadInt16(SENS_2_CAL_Y2);

	s_Ctrl[2].type            = EEPR_ReadByte(CTRL_3_TYPE);

    s_Ctrl[2].CalX1           =  EEPR_ReadInt16(SENS_3_CAL_X1);
    s_Ctrl[2].CalY1           =  EEPR_ReadInt16(SENS_3_CAL_Y1);
    s_Ctrl[2].CalX2           =  EEPR_ReadInt16(SENS_3_CAL_X2);
    s_Ctrl[2].CalY2           =  EEPR_ReadInt16(SENS_3_CAL_Y2);

	s_Ctrl[3].type            = EEPR_ReadByte(CTRL_4_TYPE);   

    s_Ctrl[3].CalX1           =  EEPR_ReadInt16(SENS_4_CAL_X1);
    s_Ctrl[3].CalY1           =  EEPR_ReadInt16(SENS_4_CAL_Y1);
    s_Ctrl[3].CalX2           =  EEPR_ReadInt16(SENS_4_CAL_X2);
    s_Ctrl[3].CalY2           =  EEPR_ReadInt16(SENS_4_CAL_Y2);

    Serial.print("Channel 1 | Type: ");
    Serial.print(Ctrl_TypeToName(s_Ctrl[0].type));
    Serial.print(" | Cal Point Y1: ");
    Serial.print(s_Ctrl[0].CalY1);
    Serial.print(" | Y2: ");
    Serial.println(s_Ctrl[0].CalY2);

    Serial.print("Channel 2 | Type: ");
    Serial.print(Ctrl_TypeToName(s_Ctrl[1].type));
    Serial.print(" | Cal Point Y1: ");
    Serial.print(s_Ctrl[1].CalY1);
    Serial.print(" | Y2: ");
    Serial.println(s_Ctrl[1].CalY2);

    Serial.print("Channel 3 | Type: ");
    Serial.print(Ctrl_TypeToName(s_Ctrl[2].type));
    Serial.print(" | Cal Point Y1: ");
    Serial.print(s_Ctrl[2].CalY1);
    Serial.print(" | Y2: ");
    Serial.println(s_Ctrl[2].CalY2);

    Serial.print("Channel 4 | Type: ");
    Serial.print(Ctrl_TypeToName(s_Ctrl[3].type));
    Serial.print(" | Cal Point Y1: ");
    Serial.print(s_Ctrl[3].CalY1);
    Serial.print(" | Y2: ");
    Serial.println(s_Ctrl[3].CalY2);



}


//-----------------------------------------------------------------------------
//--- Private Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------





