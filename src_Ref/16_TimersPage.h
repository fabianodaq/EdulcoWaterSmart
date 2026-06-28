#ifndef WSER_TIMERS_H
#define WSER_TIMERS_H

//----------- Extern Global variables-----------------

// ===== Public page function =====

extern void WSER_InitPumpTimersSection();

extern void WSER_WiFiPumpTimersPage();
extern void WSER_PumpTimersLoop();

extern void WSER_handlePumpTimersGetStatus();

extern void WSER_handleSetTonToff();

extern void WSER_handleSetTimerActivation();

extern bool WSER_ExtCmdSetTimerActivation(uint8_t timerIndex, bool active);
extern bool WSER_ExtCmdSetTonToff(uint8_t timerIndex, uint16_t tonSeconds, uint16_t toffSeconds);
extern bool WSER_ExtCmdSetTonToff(uint8_t timerIndex, uint16_t tonSeconds, uint16_t toffSeconds);

extern void WSER_UpdateSettingsTimerStrings();

#endif