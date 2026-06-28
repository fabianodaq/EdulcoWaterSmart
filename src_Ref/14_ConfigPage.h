#ifndef WSER_WIFICONFIGPAGE_H
#define WSER_WIFICONFIGPAGE_H



// ===== Public Constants =====

// ===== Public variables =====

extern char email_receiver[EMAIL_ADDRESS_SIZE + 1];
extern char author_email[EMAIL_ADDRESS_SIZE + 1];
extern char author_pwd[EMAIL_ADDRESS_SIZE + 1];
extern char email_Copilot_receiver[EMAIL_ADDRESS_SIZE + 1];

// ===== Public function =====
extern void WSER_InitWiFiSection();
extern void WSER_WiFiConfigLoop();

extern void WSER_WiFiConfigPage();

extern void WSER_handleGetStatus();
extern void WSER_handleSaveAP();
extern void WSER_handleSaveSTA();
extern void WSER_handleForgetSTA();
extern void WSER_handleSaveEmail();

extern void WSER_handleSaveEmailSender();

extern bool WSER_IsEmailDefined();
extern bool WSER_IsAccessPointMode();

#endif 
