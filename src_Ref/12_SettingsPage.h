#ifndef WSER_SETTINGS_H
#define WSER_SETTINGS_H


//----------- Extern Global variables-----------------

// ===== Public page function =====

extern void WSER_InitSettingsSection();

extern void WSER_WiFiSettingsgPage ();

extern void WSER_handleSettingsGetStatus();

extern void WSER_SettingsLoop();

extern void WSER_handleSettingControlLogik();
extern void WSER_handleSettingControlHyster();
extern void WSER_handleSettingControlSetPoint();

extern bool WSER_ExtCmdSetHysteresis(uint8_t ctrlIndex, int hysteresis);
extern bool WSER_ExtCmdSetLogic(uint8_t controlIndex, bool positiveLogic);
extern bool WSER_ExtCmdSetSetpoint(uint8_t controlIndex, int setpoint);

#endif 
