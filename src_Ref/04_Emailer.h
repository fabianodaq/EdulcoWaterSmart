#ifndef APP_EMAILER_H
#define APP_EMAILER_H

extern uint8_t email_default_remaining_email;

extern void EML_Emailer_Setup();  // inizializzazioni emailer
extern void EML_Emailer_Loop();   // chiamalo spesso nel loop

extern void EML_SentinelSendResolvedAlarm   (     uint8_t sentidx, int16_t actual_val,     int16_t min_val,  int16_t max_val);
extern void EML_SentinelSendNewAlarm        (     uint8_t sentidx, int16_t actual_val,     int16_t min_val, int16_t max_val, uint16_t persistence_min, uint16_t occurrance_min );

extern void EML_EmailReconnect();

extern bool EML_SendCsvReport(uint8_t number, String filepath);

bool EML_CopilotSendMessage(String messageToCopilot, String fileSystemInfoPath);


#endif // APP_EMAILER_H