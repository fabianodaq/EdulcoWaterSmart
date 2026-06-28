#ifndef APP_EEPROM_H
#define APP_EEPROM_H

#include <EEPROM.h>

//----EEPROM map definition------------------
#define EEPROM_SIZE 500

#define FLAG_SIZE 1             // Dimensione flags

#define INT_BYTES_SIZE      2 
#define CHAR_BYTES_SIZE     1 
#define SLOT_SIZE_BYTES     16

#define CREDENTIALS_SIZE        32  // Dimensione delle credenziali
#define EMAIL_ADDRESS_SIZE      32  // Dimensione delle credenziali

//------------EEPROM credentials------------
#define AP_SSID_EEPROM_ADDR         0                                           // (0)
#define AP_PWD_EEPROM_ADDR          (AP_SSID_EEPROM_ADDR    + CREDENTIALS_SIZE) // (32)

#define STA_SSID_EEPROM_ADDR        (AP_PWD_EEPROM_ADDR     + CREDENTIALS_SIZE) // (64)
#define STA_PWD_EEPROM_ADDR         (STA_SSID_EEPROM_ADDR   + CREDENTIALS_SIZE) // (96)

#define RECEIVER_EMAIL_ADDR           (STA_PWD_EEPROM_ADDR    + CREDENTIALS_SIZE) // (128)

#define SENDER_EMAIL_ADDR         (RECEIVER_EMAIL_ADDR      + CREDENTIALS_SIZE) // (160)
#define SENDER_PWD_ADDR           (SENDER_EMAIL_ADDR        + CREDENTIALS_SIZE) // (192)

#define EEPROM_INIT_FLAG_CREDENTIALS_ADDR      (SENDER_PWD_ADDR         + EMAIL_ADDRESS_SIZE)   // (224)
#define EEPROM_INIT_CREDENTIALS_FLAG           0xD1                           // (0)
#define EEPROM_INIT_FLAG_CREDENTIALS_NEG_ADDR  (EEPROM_INIT_FLAG_CREDENTIALS_ADDR   + FLAG_SIZE)    // (225)
#define EEPROM_INIT_FLAG_CREDENTIALS_NEG       0x1D                           // (0)

//--------------Sensors----------------------
#define SENS_1_TYPE     (EEPROM_INIT_FLAG_CREDENTIALS_NEG_ADDR +  FLAG_SIZE) // (226)
#define SENS_1_CAL_X1   (SENS_1_TYPE    +  CHAR_BYTES_SIZE)                  // (227..228)
#define SENS_1_CAL_Y1   (SENS_1_CAL_X1  +  INT_BYTES_SIZE)                   // (229..230)
#define SENS_1_CAL_X2   (SENS_1_CAL_Y1  +  INT_BYTES_SIZE)                   // (231..232)
#define SENS_1_CAL_Y2   (SENS_1_CAL_X2  +  INT_BYTES_SIZE)                   // (233..234)

#define SENS_2_TYPE     (SENS_1_CAL_Y2  +  INT_BYTES_SIZE)                   // (235)
#define SENS_2_CAL_X1   (SENS_2_TYPE    +  CHAR_BYTES_SIZE)                  // (236..237)
#define SENS_2_CAL_Y1   (SENS_2_CAL_X1  +  INT_BYTES_SIZE)                   // (238..239)
#define SENS_2_CAL_X2   (SENS_2_CAL_Y1  +  INT_BYTES_SIZE)                   // (240..241)
#define SENS_2_CAL_Y2   (SENS_2_CAL_X2  +  INT_BYTES_SIZE)                   // (242..243)

#define SENS_3_TYPE     (SENS_2_CAL_Y2  +  INT_BYTES_SIZE)                   // (244)
#define SENS_3_CAL_X1   (SENS_3_TYPE    +  CHAR_BYTES_SIZE)                  // (245..246)
#define SENS_3_CAL_Y1   (SENS_3_CAL_X1  +  INT_BYTES_SIZE)                   // (247..248)
#define SENS_3_CAL_X2   (SENS_3_CAL_Y1  +  INT_BYTES_SIZE)                   // (249..250)
#define SENS_3_CAL_Y2   (SENS_3_CAL_X2  +  INT_BYTES_SIZE)                   // (251..252)

#define SENS_4_TYPE     (SENS_3_CAL_Y2  +  INT_BYTES_SIZE)                   // (253)
#define SENS_4_CAL_X1   (SENS_4_TYPE    +  CHAR_BYTES_SIZE)                  // (254..255)
#define SENS_4_CAL_Y1   (SENS_4_CAL_X1  +  INT_BYTES_SIZE)                   // (256..257)
#define SENS_4_CAL_X2   (SENS_4_CAL_Y1  +  INT_BYTES_SIZE)                   // (258..259)
#define SENS_4_CAL_Y2   (SENS_4_CAL_X2  +  INT_BYTES_SIZE)                   // (260..261)

#define EEPROM_INIT_FLAG_SENS_ADDR      (SENS_4_CAL_Y2  +  INT_BYTES_SIZE)          // (262)
#define EEPROM_INIT_SENS_FLAG                0xA5                           // (0)
#define EEPROM_INIT_FLAG_SENS_NEG_ADDR  (EEPROM_INIT_FLAG_SENS_ADDR + FLAG_SIZE)    // (263)
#define EEPROM_INIT_FLAG_SENS_NEG            0x5A                           // (0)

//---------------------Controls----------------------------
#define CTRL_1_TYPE     (EEPROM_INIT_FLAG_SENS_NEG_ADDR + FLAG_SIZE)       // (264)
#define CTRL_1_ACTIVE   (CTRL_1_TYPE                    + CHAR_BYTES_SIZE)       // (265)
#define CTRL_1_NUM      (CTRL_1_ACTIVE                  + CHAR_BYTES_SIZE)       // (266)
#define CTRL_1_CLOCK    (CTRL_1_NUM                     + CHAR_BYTES_SIZE)       // (267)
#define CTRL_1_SET_PT   (CTRL_1_CLOCK                   + INT_BYTES_SIZE)       // (268..269)
#define CTRL_1_LOGIK    (CTRL_1_SET_PT                  + INT_BYTES_SIZE)       // (270..271)
#define CTRL_1_HYSTER   (CTRL_1_LOGIK                   + CHAR_BYTES_SIZE)       // (272)

#define CTRL_2_TYPE     (CTRL_1_HYSTER                  + INT_BYTES_SIZE)       // (273)
#define CTRL_2_ACTIVE   (CTRL_2_TYPE                    + CHAR_BYTES_SIZE)       // (274)
#define CTRL_2_NUM      (CTRL_2_ACTIVE                  + CHAR_BYTES_SIZE)       // (275)
#define CTRL_2_CLOCK    (CTRL_2_NUM                     + CHAR_BYTES_SIZE)       // (276)
#define CTRL_2_SET_PT   (CTRL_2_CLOCK                   + INT_BYTES_SIZE)       // (277..278)
#define CTRL_2_LOGIK    (CTRL_2_SET_PT                  + INT_BYTES_SIZE)       // (279..280)
#define CTRL_2_HYSTER   (CTRL_2_LOGIK                   + CHAR_BYTES_SIZE)       // (281)

#define CTRL_3_TYPE     (CTRL_2_HYSTER                  + INT_BYTES_SIZE)       // (282)
#define CTRL_3_ACTIVE   (CTRL_3_TYPE                    + CHAR_BYTES_SIZE)       // (283)
#define CTRL_3_NUM      (CTRL_3_ACTIVE                  + CHAR_BYTES_SIZE)       // (284)
#define CTRL_3_CLOCK    (CTRL_3_NUM                     + CHAR_BYTES_SIZE)       // (285)
#define CTRL_3_SET_PT   (CTRL_3_CLOCK                   + INT_BYTES_SIZE)       // (286..287)
#define CTRL_3_LOGIK    (CTRL_3_SET_PT                  + INT_BYTES_SIZE)       // (288..289)
#define CTRL_3_HYSTER   (CTRL_3_LOGIK                   + CHAR_BYTES_SIZE)       // (290)

#define CTRL_4_TYPE     (CTRL_3_HYSTER                  + INT_BYTES_SIZE)       // (291)
#define CTRL_4_ACTIVE   (CTRL_4_TYPE                    + CHAR_BYTES_SIZE)       // (292)
#define CTRL_4_NUM      (CTRL_4_ACTIVE                  + CHAR_BYTES_SIZE)       // (293)
#define CTRL_4_CLOCK    (CTRL_4_NUM                     + CHAR_BYTES_SIZE)       // (294)
#define CTRL_4_SET_PT   (CTRL_4_CLOCK                   + INT_BYTES_SIZE)       // (295..296)
#define CTRL_4_LOGIK    (CTRL_4_SET_PT                  + INT_BYTES_SIZE)       // (297..298)
#define CTRL_4_HYSTER   (CTRL_4_LOGIK                   + CHAR_BYTES_SIZE)       // (299)

#define EEPROM_INIT_FLAG_CTRL_ADDR       (CTRL_4_HYSTER                 + INT_BYTES_SIZE) // (300)
#define EEPROM_INIT_CTRL_FLAG            0xC3                           // (0)
#define EEPROM_INIT_FLAG_CTRL_NEG_ADDR   (EEPROM_INIT_FLAG_CTRL_ADDR    + FLAG_SIZE) // (301)
#define EEPROM_INIT_FLAG_CTRL_NEG        0x3C                           // (0)

//---------------------Timers----------------------------
#define PUMP_1_TIMER_ACTIVE        (EEPROM_INIT_FLAG_CTRL_NEG_ADDR      + FLAG_SIZE) // (302)
#define PUMP_1_TON_SECONDS_ADDR    (PUMP_1_TIMER_ACTIVE                 + 1) // (303..304)
#define PUMP_1_TOFF_SECONDS_ADDR   (PUMP_1_TON_SECONDS_ADDR             + INT_BYTES_SIZE) // (305..306)

#define PUMP_2_TIMER_ACTIVE        (PUMP_1_TOFF_SECONDS_ADDR            + 1) // (307)
#define PUMP_2_TON_SECONDS_ADDR    (PUMP_2_TIMER_ACTIVE                 + 1) // (308..309)
#define PUMP_2_TOFF_SECONDS_ADDR   (PUMP_2_TON_SECONDS_ADDR             + INT_BYTES_SIZE) // (310..311)

#define PUMP_3_TIMER_ACTIVE        (PUMP_2_TOFF_SECONDS_ADDR            + 1) // (312)
#define PUMP_3_TON_SECONDS_ADDR    (PUMP_3_TIMER_ACTIVE                 + 1) // (313..314)
#define PUMP_3_TOFF_SECONDS_ADDR   (PUMP_3_TON_SECONDS_ADDR             + INT_BYTES_SIZE) // (315..316)

#define PUMP_4_TIMER_ACTIVE        (PUMP_3_TOFF_SECONDS_ADDR            + 1) // (317)
#define PUMP_4_TON_SECONDS_ADDR    (PUMP_4_TIMER_ACTIVE                 + 1) // (318..319)
#define PUMP_4_TOFF_SECONDS_ADDR   (PUMP_4_TON_SECONDS_ADDR             + INT_BYTES_SIZE) // (320..321)

#define EEPROM_INIT_FLAG_PUMP_TIMER_ADDR      (PUMP_4_TOFF_SECONDS_ADDR + INT_BYTES_SIZE) // (322)
#define EEPROM_INIT_PUMP_TIMER_FLAG           0xB4                           // (0)
#define EEPROM_INIT_FLAG_PUMP_TIMER_NEG_ADDR  (EEPROM_INIT_FLAG_PUMP_TIMER_ADDR + FLAG_SIZE) // (323)
#define EEPROM_INIT_PUMP_TIMER_NEG            0x4B                           // (0)

//-------SENTINELS
#define EEPROM_SENT0_ACTIVE_ADDR              (EEPROM_INIT_FLAG_PUMP_TIMER_NEG_ADDR + INT_BYTES_SIZE) // (324)
#define EEPROM_SENT0_MINVAL_ADDR              (EEPROM_SENT0_ACTIVE_ADDR             + CHAR_BYTES_SIZE) // (325..326)
#define EEPROM_SENT0_MAXVAL_ADDR              (EEPROM_SENT0_MINVAL_ADDR             + INT_BYTES_SIZE) // (327..328)
#define EEPROM_SENT0_PERSIST_MIN_ADDR         (EEPROM_SENT0_MAXVAL_ADDR             + INT_BYTES_SIZE) // (329..330)
#define EEPROM_SENT0_OCCUR_MIN_ADDR           (EEPROM_SENT0_PERSIST_MIN_ADDR        + INT_BYTES_SIZE) // (331..332)

#define EEPROM_SENT1_ACTIVE_ADDR              (EEPROM_SENT0_OCCUR_MIN_ADDR          + INT_BYTES_SIZE) // (333)
#define EEPROM_SENT1_MINVAL_ADDR              (EEPROM_SENT1_ACTIVE_ADDR             + CHAR_BYTES_SIZE) // (334..335)
#define EEPROM_SENT1_MAXVAL_ADDR              (EEPROM_SENT1_MINVAL_ADDR             + INT_BYTES_SIZE) // (336..337)
#define EEPROM_SENT1_PERSIST_MIN_ADDR         (EEPROM_SENT1_MAXVAL_ADDR             + INT_BYTES_SIZE) // (338..339)
#define EEPROM_SENT1_OCCUR_MIN_ADDR           (EEPROM_SENT1_PERSIST_MIN_ADDR        + INT_BYTES_SIZE) // (340..341)

#define EEPROM_SENT2_ACTIVE_ADDR              (EEPROM_SENT1_OCCUR_MIN_ADDR          + INT_BYTES_SIZE) // (342)
#define EEPROM_SENT2_MINVAL_ADDR              (EEPROM_SENT2_ACTIVE_ADDR             + CHAR_BYTES_SIZE) // (343..344)
#define EEPROM_SENT2_MAXVAL_ADDR              (EEPROM_SENT2_MINVAL_ADDR             + INT_BYTES_SIZE) // (345..346)
#define EEPROM_SENT2_PERSIST_MIN_ADDR         (EEPROM_SENT2_MAXVAL_ADDR             + INT_BYTES_SIZE) // (347..348)
#define EEPROM_SENT2_OCCUR_MIN_ADDR           (EEPROM_SENT2_PERSIST_MIN_ADDR        + INT_BYTES_SIZE) // (349..350)

#define EEPROM_SENT3_ACTIVE_ADDR              (EEPROM_SENT2_OCCUR_MIN_ADDR          + INT_BYTES_SIZE) // (351)
#define EEPROM_SENT3_MINVAL_ADDR              (EEPROM_SENT3_ACTIVE_ADDR             + CHAR_BYTES_SIZE) // (352..353)
#define EEPROM_SENT3_MAXVAL_ADDR              (EEPROM_SENT3_MINVAL_ADDR             + INT_BYTES_SIZE) // (354..355)
#define EEPROM_SENT3_PERSIST_MIN_ADDR         (EEPROM_SENT3_MAXVAL_ADDR             + INT_BYTES_SIZE) // (356..357)
#define EEPROM_SENT3_OCCUR_MIN_ADDR           (EEPROM_SENT3_PERSIST_MIN_ADDR        + INT_BYTES_SIZE) // (358..359)

#define EEPROM_INIT_FLAG_SENTINEL_ADDR        (EEPROM_SENT3_OCCUR_MIN_ADDR          + INT_BYTES_SIZE) // (360)
#define EEPROM_INIT_SENTINEL_FLAG             0xA7                           // (0)
#define EEPROM_INIT_FLAG_SENTINEL_NEG_ADDR    (EEPROM_INIT_FLAG_SENTINEL_ADDR       + FLAG_SIZE) // (361)
#define EEPROM_INIT_SENTINEL_NEG              0x58                           // (0)

//-------REPORTERS
#define EEPROM_REP0_ACTIVE_ADDR               (EEPROM_INIT_FLAG_SENTINEL_NEG_ADDR   + FLAG_SIZE) // (362)
#define EEPROM_REP0_PERIOD_ADDR               (EEPROM_REP0_ACTIVE_ADDR              + CHAR_BYTES_SIZE) // (363)

#define EEPROM_REP1_ACTIVE_ADDR               (EEPROM_REP0_PERIOD_ADDR              + CHAR_BYTES_SIZE) // (364)
#define EEPROM_REP1_PERIOD_ADDR               (EEPROM_REP1_ACTIVE_ADDR              + CHAR_BYTES_SIZE) // (365)

#define EEPROM_REP2_ACTIVE_ADDR               (EEPROM_REP1_PERIOD_ADDR              + CHAR_BYTES_SIZE) // (366)
#define EEPROM_REP2_PERIOD_ADDR               (EEPROM_REP2_ACTIVE_ADDR              + CHAR_BYTES_SIZE) // (367)

#define EEPROM_REP3_ACTIVE_ADDR               (EEPROM_REP2_PERIOD_ADDR              + CHAR_BYTES_SIZE) // (368)
#define EEPROM_REP3_PERIOD_ADDR               (EEPROM_REP3_ACTIVE_ADDR              + CHAR_BYTES_SIZE) // (369)

// Flag di inizializzazione area Reporter
#define EEPROM_INIT_FLAG_REPORTER_ADDR        (EEPROM_REP3_PERIOD_ADDR              + CHAR_BYTES_SIZE) // (370)
#define EEPROM_INIT_REPORTER_FLAG             0xB9                           // 1
#define EEPROM_INIT_FLAG_REPORTER_NEG_ADDR    (EEPROM_INIT_FLAG_REPORTER_ADDR       + FLAG_SIZE) // (371)
#define EEPROM_INIT_REPORTER_NEG              0x46                           // 1

// ------- Relays (Remote Slots)
#define SLOT_1_IP_ADDR                        (EEPROM_INIT_FLAG_REPORTER_NEG_ADDR + FLAG_SIZE)   // 372
#define SLOT_2_IP_ADDR                        (SLOT_1_IP_ADDR + SLOT_SIZE_BYTES)                 // 388
#define SLOT_3_IP_ADDR                        (SLOT_2_IP_ADDR + SLOT_SIZE_BYTES)                 // 404
#define SLOT_4_IP_ADDR                        (SLOT_3_IP_ADDR + SLOT_SIZE_BYTES)                 // 420

#define RELAY_CONTROL_1_ADDR                  (SLOT_4_IP_ADDR           + SLOT_SIZE_BYTES)      // 436
#define RELAY_CONTROL_2_ADDR                  (RELAY_CONTROL_1_ADDR     + CHAR_BYTES_SIZE)      // 437
#define RELAY_CONTROL_3_ADDR                  (RELAY_CONTROL_2_ADDR     + CHAR_BYTES_SIZE)      // 438
#define RELAY_CONTROL_4_ADDR                  (RELAY_CONTROL_3_ADDR     + CHAR_BYTES_SIZE)      // 439

// ------- Init flags Relays
#define EEPROM_INIT_FLAG_REMOTE_SLOT_ADDR     (RELAY_CONTROL_4_ADDR + CHAR_BYTES_SIZE)          // 440
#define EEPROM_INIT_REMOTE_SLOT_FLAG          0xE7
#define EEPROM_INIT_FLAG_REMOTE_SLOT_NEG_ADDR (EEPROM_INIT_FLAG_REMOTE_SLOT_ADDR + FLAG_SIZE)   // 441
#define EEPROM_INIT_REMOTE_SLOT_NEG           0x7E

//-----Object definitions-----

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
