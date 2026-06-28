#ifndef WSER_SENSOR_H
#define WSER_SENSOR_H

// ===== Public page function =====
//Init and loop functions
extern void WSER_InitCalibrationSection();

extern void WSER_CalibrationLoop();

//Web Page
extern void WSER_WiFiCalibrationgPage();
//get function
extern void WSER_handleCalibrGetStatus();
//post function

extern void WSER_handleSettingControlType();


extern void WSER_handleSetCalPoint();
extern void WSER_handleReSetCalPoint();

extern bool WSER_ExtCmdCalReset(uint8_t sensorIndex, uint8_t calPoint );
extern bool WSER_CalibrationSmartHomeGetStatus(uint8_t sensorIndex);
extern bool WSER_ExtCmdSetType(uint8_t ctrlIndex, const String& typeStr);
extern bool WSER_ExtCmdSetCalPoint(uint8_t sens_numb, uint8_t cal_point, int16_t cal_value);

#endif 
