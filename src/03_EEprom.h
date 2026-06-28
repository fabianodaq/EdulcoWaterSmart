#ifndef APP_EEPROM_H
#define APP_EEPROM_H

#include <EEPROM.h>

//---- EEPROM map definition ------------------
#define EEPROM_SIZE 100

#define FLAG_SIZE 1             // dimensione flag

#define INT_BYTES_SIZE      2   // dimensione campi int16
#define CHAR_BYTES_SIZE     1   // dimensione campi byte

#define WIFI_STR_BYTES_SIZE 32

//-------------- WiFi credentials ----------------------
// Richiesto: prima password, poi SSID
#define WIFI_PASSWORD_ADDR  (0)                                  // (0..31)
#define WIFI_SSID_ADDR      (WIFI_PASSWORD_ADDR + WIFI_STR_BYTES_SIZE) // (32..63)

//-------------- Sensors ----------------------
#define SENS_1_TYPE     (WIFI_SSID_ADDR + WIFI_STR_BYTES_SIZE)               // (64)
#define SENS_1_CAL_X1   (SENS_1_TYPE    +  CHAR_BYTES_SIZE)                  // (65..66)
#define SENS_1_CAL_Y1   (SENS_1_CAL_X1  +  INT_BYTES_SIZE)                   // (67..68)
#define SENS_1_CAL_X2   (SENS_1_CAL_Y1  +  INT_BYTES_SIZE)                   // (69..70)
#define SENS_1_CAL_Y2   (SENS_1_CAL_X2  +  INT_BYTES_SIZE)                   // (71..72)

#define SENS_2_TYPE     (SENS_1_CAL_Y2  +  INT_BYTES_SIZE)                   // (73)
#define SENS_2_CAL_X1   (SENS_2_TYPE    +  CHAR_BYTES_SIZE)                  // (74..75)
#define SENS_2_CAL_Y1   (SENS_2_CAL_X1  +  INT_BYTES_SIZE)                   // (76..77)
#define SENS_2_CAL_X2   (SENS_2_CAL_Y1  +  INT_BYTES_SIZE)                   // (78..79)
#define SENS_2_CAL_Y2   (SENS_2_CAL_X2  +  INT_BYTES_SIZE)                   // (80..81)

#define SENS_EC_TYPE     (SENS_2_CAL_Y2  +  INT_BYTES_SIZE)                   // (82)
#define SENS_EC_CAL_X1   (SENS_EC_TYPE    +  CHAR_BYTES_SIZE)                  // (83..84)
#define SENS_EC_CAL_Y1   (SENS_EC_CAL_X1  +  INT_BYTES_SIZE)                   // (85..86)
#define SENS_EC_CAL_X2   (SENS_EC_CAL_Y1  +  INT_BYTES_SIZE)                   // (87..88)
#define SENS_EC_CAL_Y2   (SENS_EC_CAL_X2  +  INT_BYTES_SIZE)                   // (89..90)

#define EEPROM_INIT_FLAG_SENS_ADDR      (SENS_EC_CAL_Y2  +  INT_BYTES_SIZE)          // (91)
#define EEPROM_INIT_SENS_FLAG                0xA5                           // init flag value
#define EEPROM_INIT_FLAG_SENS_NEG_ADDR  (EEPROM_INIT_FLAG_SENS_ADDR + FLAG_SIZE)    // (92)
#define EEPROM_INIT_FLAG_SENS_NEG            0x5A                           // complement init flag value



//----- Object definitions -----

extern void        EEPR_WriteCh32  (int start, const char* data);
extern void        EEPR_ReadCh32   (int start, char* buffer);

extern void        EEPR_WriteCh16  (int start, const char* data);
extern void        EEPR_ReadCh16   (int start, char* buffer);

extern void        EEPR_WriteByte  (int addr, uint8_t value);
extern uint8_t     EEPR_ReadByte   (int addr);

extern void        EEPR_WriteUint16(int addr, uint16_t value);
extern uint16_t    EEPR_ReadUint16 (int addr);

extern void        EEPR_WriteInt16 (int addr, int16_t value);
extern int16_t     EEPR_ReadInt16  (int addr);

#endif // APP_EEPROM_H
