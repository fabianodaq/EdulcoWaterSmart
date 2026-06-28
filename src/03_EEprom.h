#ifndef APP_EEPROM_H
#define APP_EEPROM_H

#include <EEPROM.h>

//---- EEPROM map definition ------------------
#define EEPROM_SIZE 30

#define FLAG_SIZE 1             // dimensione flag

#define INT_BYTES_SIZE      2   // dimensione campi int16
#define CHAR_BYTES_SIZE     1   // dimensione campi byte

//-------------- Sensors ----------------------
#define SENS_1_TYPE     (0)                                                  // (0)
#define SENS_1_CAL_X1   (SENS_1_TYPE    +  CHAR_BYTES_SIZE)                  // (1..2)
#define SENS_1_CAL_Y1   (SENS_1_CAL_X1  +  INT_BYTES_SIZE)                   // (3..4)
#define SENS_1_CAL_X2   (SENS_1_CAL_Y1  +  INT_BYTES_SIZE)                   // (5..6)
#define SENS_1_CAL_Y2   (SENS_1_CAL_X2  +  INT_BYTES_SIZE)                   // (7..8)

#define SENS_2_TYPE     (SENS_1_CAL_Y2  +  INT_BYTES_SIZE)                   // (9)
#define SENS_2_CAL_X1   (SENS_2_TYPE    +  CHAR_BYTES_SIZE)                  // (10..11)
#define SENS_2_CAL_Y1   (SENS_2_CAL_X1  +  INT_BYTES_SIZE)                   // (12..13)
#define SENS_2_CAL_X2   (SENS_2_CAL_Y1  +  INT_BYTES_SIZE)                   // (14..15)
#define SENS_2_CAL_Y2   (SENS_2_CAL_X2  +  INT_BYTES_SIZE)                   // (16..17)

#define SENS_EC_TYPE     (SENS_2_CAL_Y2  +  INT_BYTES_SIZE)                   // (18)
#define SENS_EC_CAL_X1   (SENS_EC_TYPE    +  CHAR_BYTES_SIZE)                  // (19..20)
#define SENS_EC_CAL_Y1   (SENS_EC_CAL_X1  +  INT_BYTES_SIZE)                   // (21..22)
#define SENS_EC_CAL_X2   (SENS_EC_CAL_Y1  +  INT_BYTES_SIZE)                   // (23..24)
#define SENS_EC_CAL_Y2   (SENS_EC_CAL_X2  +  INT_BYTES_SIZE)                   // (25..26)

#define EEPROM_INIT_FLAG_SENS_ADDR      (SENS_EC_CAL_Y2  +  INT_BYTES_SIZE)          // (27)
#define EEPROM_INIT_SENS_FLAG                0xA5                           // init flag value
#define EEPROM_INIT_FLAG_SENS_NEG_ADDR  (EEPROM_INIT_FLAG_SENS_ADDR + FLAG_SIZE)    // (28)
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
