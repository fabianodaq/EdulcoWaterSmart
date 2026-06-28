#include "01_AppControls.h"

#include "10_WebServerMain.h"
#include "16_TimersPage_private.h"

#include <HTTPClient.h>

unsigned long lastCheckTimePumpTimers = 0;


//--- Private Function Declarations ------------------------------------------
String PumpTimer1_Settings;  // es: "✅ Active | Ton: 30s | Toff: 45s"
String PumpTimer1_Status;    

String PumpTimer2_Settings;  // es: "✅ Active | Ton: 30s | Toff: 45s"
String PumpTimer2_Status; 

String PumpTimer3_Settings;  // es: "✅ Active | Ton: 30s | Toff: 45s"
String PumpTimer3_Status; 

String PumpTimer4_Settings;  // es: "✅ Active | Ton: 30s | Toff: 45s"
String PumpTimer4_Status; 

//--- Public Function Declarations -------------------------------------------
void   WSER_InitPumpTimersSection();

void WSER_WiFiPumpTimersPage();
void WSER_PumpTimersLoop();

//web page fucntions

//get fucntions
void WSER_handleSetTonToff();
void WSER_handleSetTimerActivation();

//internal fucntions
void WSER_UpdateSettingsTimerStrings();
void WSER_UpdateStatusTimerStrings();
//get fucntions
void WSER_handlePumpTimersGetStatus();

//internal fucntions
bool WSER_ExtCmdSetTimerActivation(uint8_t timerIndex, bool active);
bool WSER_ExtCmdSetTonToff(uint8_t timerIndex, uint16_t tonSeconds, uint16_t toffSeconds);
//-----------------------------------------------------------------------------
//--- PUBLIC Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void   WSER_InitPumpTimersSection(){

  WSER_UpdateSettingsTimerStrings();
  WSER_UpdateStatusTimerStrings();

}

/**

 */
void WSER_PumpTimersLoop() {
  if (millis() - lastCheckTimePumpTimers > PUMP_TIMERS_CHECK_INTERVAL) {
    lastCheckTimePumpTimers = millis();

    WSER_UpdateStatusTimerStrings();

  }

}  

//-----------------------------------------------------------------------------
//--- PRIVATE Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void WSER_UpdateSettingsTimerStrings() {

  char bufferTon[9];   // hh:mm:ss → 8 + terminatore
  char bufferToff[9];

  uint16_t tonSec;
  uint16_t toffSec;

  uint8_t tonH;
  uint8_t tonM;
  uint8_t tonS;

  uint8_t toffH;
  uint8_t toffM;
  uint8_t toffS;

  // --- Timer 1 ---
  if (PumpTimers[0].Active) {
    PumpTimer1_Settings = "✅ Active";
  } else {
    PumpTimer1_Settings = "❌ Not Active";
  }

  tonSec  = PumpTimers[0].OnSeconds;
  toffSec = PumpTimers[0].OffSeconds;

  tonH = tonSec / 3600;
  tonM = (tonSec % 3600) / 60;
  tonS = tonSec % 60;

  toffH = toffSec / 3600;
  toffM = (toffSec % 3600) / 60;
  toffS = toffSec % 60;

  sprintf(bufferTon,  "%02d:%02d:%02d", tonH, tonM, tonS);
  sprintf(bufferToff, "%02d:%02d:%02d", toffH, toffM, toffS);

  PumpTimer1_Settings += " | Ton 🟢: ";
  PumpTimer1_Settings += bufferTon;

  PumpTimer1_Settings += " | Toff 🔴: ";
  PumpTimer1_Settings += bufferToff;

  // --- Timer 2 ---
  if (PumpTimers[1].Active) {
    PumpTimer2_Settings = "✅ Active";
  } else {
    PumpTimer2_Settings = "❌ Not Active";
  }

  tonSec  = PumpTimers[1].OnSeconds;
  toffSec = PumpTimers[1].OffSeconds;

  tonH = tonSec / 3600;
  tonM = (tonSec % 3600) / 60;
  tonS = tonSec % 60;

  toffH = toffSec / 3600;
  toffM = (toffSec % 3600) / 60;
  toffS = toffSec % 60;

  sprintf(bufferTon,  "%02d:%02d:%02d", tonH, tonM, tonS);
  sprintf(bufferToff, "%02d:%02d:%02d", toffH, toffM, toffS);

  PumpTimer2_Settings += " | Ton 🟢: ";
  PumpTimer2_Settings += bufferTon;

  PumpTimer2_Settings += " | Toff 🔴: ";
  PumpTimer2_Settings += bufferToff;

  // --- Timer 3 ---
  if (PumpTimers[2].Active) {
    PumpTimer3_Settings = "✅ Active";
  } else {
    PumpTimer3_Settings = "❌ Not Active";
  }

  tonSec  = PumpTimers[2].OnSeconds;
  toffSec = PumpTimers[2].OffSeconds;

  tonH = tonSec / 3600;
  tonM = (tonSec % 3600) / 60;
  tonS = tonSec % 60;

  toffH = toffSec / 3600;
  toffM = (toffSec % 3600) / 60;
  toffS = toffSec % 60;

  sprintf(bufferTon,  "%02d:%02d:%02d", tonH, tonM, tonS);
  sprintf(bufferToff, "%02d:%02d:%02d", toffH, toffM, toffS);

  PumpTimer3_Settings += " | Ton 🟢: ";
  PumpTimer3_Settings += bufferTon;

  PumpTimer3_Settings += " | Toff 🔴: ";
  PumpTimer3_Settings += bufferToff;

  // --- Timer 4 ---
  if (PumpTimers[3].Active) {
    PumpTimer4_Settings = "✅ Active";
  } else {
    PumpTimer4_Settings = "❌ Not Active";
  }

  tonSec  = PumpTimers[3].OnSeconds;
  toffSec = PumpTimers[3].OffSeconds;

  tonH = tonSec / 3600;
  tonM = (tonSec % 3600) / 60;
  tonS = tonSec % 60;

  toffH = toffSec / 3600;
  toffM = (toffSec % 3600) / 60;
  toffS = toffSec % 60;

  sprintf(bufferTon,  "%02d:%02d:%02d", tonH, tonM, tonS);
  sprintf(bufferToff, "%02d:%02d:%02d", toffH, toffM, toffS);

  PumpTimer4_Settings += " | Ton 🟢: ";
  PumpTimer4_Settings += bufferTon;

  PumpTimer4_Settings += " | Toff 🔴: ";
  PumpTimer4_Settings += bufferToff;

}

/**

 */
void WSER_UpdateStatusTimerStrings() {

  char bufferTime[9]; // formato hh:mm:ss

  uint16_t elapsed;

  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;

  // --- Timer 1 ---
  if (!PumpTimers[0].Active) {
    PumpTimer1_Status = "⏸️ Not Active";
  } else {
    elapsed = PumpTimers[0].ElapsedTime;

    hours   = elapsed / 3600;
    minutes = (elapsed % 3600) / 60;
    seconds = elapsed % 60;

    sprintf(bufferTime, "%02d:%02d:%02d", hours, minutes, seconds);

    if (PumpTimers[0].OnPhase) {
      PumpTimer1_Status = "🟢 ON | ";
    } else {
      PumpTimer1_Status = "🔴 OFF | ";
    }

    PumpTimer1_Status += bufferTime;
  }

  // --- Timer 2 ---
  if (!PumpTimers[1].Active) {
    PumpTimer2_Status = "⏸️ Not Active";
  } else {
    elapsed = PumpTimers[1].ElapsedTime;

    hours   = elapsed / 3600;
    minutes = (elapsed % 3600) / 60;
    seconds = elapsed % 60;

    sprintf(bufferTime, "%02d:%02d:%02d", hours, minutes, seconds);

    if (PumpTimers[1].OnPhase) {
      PumpTimer2_Status = "🟢 ON | ";
    } else {
      PumpTimer2_Status = "🔴 OFF | ";
    }

    PumpTimer2_Status += bufferTime;
  }

  // --- Timer 3 ---
  if (!PumpTimers[2].Active) {
    PumpTimer3_Status = "⏸️ Not Active";
  } else {
    elapsed = PumpTimers[2].ElapsedTime;

    hours   = elapsed / 3600;
    minutes = (elapsed % 3600) / 60;
    seconds = elapsed % 60;

    sprintf(bufferTime, "%02d:%02d:%02d", hours, minutes, seconds);

    if (PumpTimers[2].OnPhase) {
      PumpTimer3_Status = "🟢 ON | ";
    } else {
      PumpTimer3_Status = "🔴 OFF | ";
    }

    PumpTimer3_Status += bufferTime;
  }

  // --- Timer 4 ---
  if (!PumpTimers[3].Active) {
    PumpTimer4_Status = "⏸️ Not Active";
  } else {
    elapsed = PumpTimers[3].ElapsedTime;

    hours   = elapsed / 3600;
    minutes = (elapsed % 3600) / 60;
    seconds = elapsed % 60;

    sprintf(bufferTime, "%02d:%02d:%02d", hours, minutes, seconds);

    if (PumpTimers[3].OnPhase) {
      PumpTimer4_Status = "🟢 ON | ";
    } else {
      PumpTimer4_Status = "🔴 OFF | ";
    }

    PumpTimer4_Status += bufferTime;
  }

}

/**

 */
void WSER_WiFiPumpTimersPage() {
  Serial.print("PUMP TIMERS: Heap available before send page: ");
  Serial.println(ESP.getFreeHeap());

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);  // invio chunked
  server.send_P(200, "text/html", WSER_HTML_PUMP_TIMERS_HEAD); // <!DOCTYPE html> + <head>...

  server.sendContent_P(WSER_COMMON_PAGES_STYLE);          // CSS
  server.sendContent_P(WSER_HTML_PUMP_TIMERS_STYLE);     // CSS
  server.sendContent_P(WSER_HTML_PUMP_TIMERS_HEAD_OPEN); // chiusura </head><body>

  // Navbar dinamica
  String temp = WSER_GenerateNavBar("pumptimers");
  server.sendContent(temp);

  // Corpo pagina
  server.sendContent_P(WSER_HTML_PUMP_TIMERS_BODY_OPEN);
  server.sendContent_P(WSER_HTML_PUMP_TIMERS_BODY_CONTENT);

  // Script JS
  server.sendContent_P(WSER_PUMP_TIMERS_CALIBRATION2);

  // Chiusura pagina
  server.sendContent_P(WSER_PUMP_TIMERS_CLOSE_PAGE);

  Serial.print("PUMP TIMERS: Heap available after send page: ");
  Serial.println(ESP.getFreeHeap());
}

//-----SMART external and Route functions

/**
transfer data to the web page
 */
/**

 */
void WSER_handlePumpTimersGetStatus() {
  String json = "{";

  json += "\"MessStatusPumpTimer1Row1\":\"" + PumpTimer1_Settings + "\",";
  json += "\"MessStatusPumpTimer1Row2\":\"" + PumpTimer1_Status + "\",";

  json += "\"MessStatusPumpTimer2Row1\":\"" + PumpTimer2_Settings + "\",";
  json += "\"MessStatusPumpTimer2Row2\":\"" + PumpTimer2_Status + "\",";

  json += "\"MessStatusPumpTimer3Row1\":\"" + PumpTimer3_Settings + "\",";
  json += "\"MessStatusPumpTimer3Row2\":\"" + PumpTimer3_Status + "\",";

  json += "\"MessStatusPumpTimer4Row1\":\"" + PumpTimer4_Settings + "\",";
  json += "\"MessStatusPumpTimer4Row2\":\"" + PumpTimer4_Status + "\"";

  json += "}";

  server.send(200, "application/json", json);
}

/**

 */
void WSER_handleSetTimerActivation() {
  Serial.println("📥 Entrato in SetTimerActivation");

  if (!server.hasArg("timer") || !server.hasArg("active")) {
    Serial.println("❌ Parametri mancanti in SetTimerActivation:");
    Serial.print("  timer presente? ");  Serial.println(server.hasArg("timer") ? "✅ Sì" : "❌ No");
    Serial.print("  active presente? "); Serial.println(server.hasArg("active") ? "✅ Sì" : "❌ No");

    server.send(400, "text/plain", "Missing parameters");
    return;
  }


  uint8_t timerindex = server.arg("timer").toInt() - 1;
  bool active = (server.arg("active") == "ACTIVE"); 

  if (WSER_ExtCmdSetTimerActivation(timerindex, active )){

    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Invalid timer value");
  }

}

/**

 */
bool WSER_ExtCmdSetTimerActivation(uint8_t timerIndex, bool active) {

  if (timerIndex == 0) {
    EEPR_WriteByte(PUMP_1_TIMER_ACTIVE, active);
    PumpTimers[0].Active = EEPR_ReadByte(PUMP_1_TIMER_ACTIVE);
    PumpTimers[0].ElapsedTime = 0;
    PumpTimers[0].OnPhase = true;
  } else if (timerIndex == 1) {
    EEPR_WriteByte(PUMP_2_TIMER_ACTIVE, active);
    PumpTimers[1].Active = EEPR_ReadByte(PUMP_2_TIMER_ACTIVE);
    PumpTimers[1].ElapsedTime = 0;
    PumpTimers[1].OnPhase = true;
  } else if (timerIndex == 2) {
    EEPR_WriteByte(PUMP_3_TIMER_ACTIVE, active);
    PumpTimers[2].Active = EEPR_ReadByte(PUMP_3_TIMER_ACTIVE);
    PumpTimers[2].ElapsedTime = 0;
    PumpTimers[2].OnPhase = true;
  } else if (timerIndex == 3) {
    EEPR_WriteByte(PUMP_4_TIMER_ACTIVE, active);
    PumpTimers[3].Active = EEPR_ReadByte(PUMP_4_TIMER_ACTIVE);
    PumpTimers[3].ElapsedTime = 0;
    PumpTimers[3].OnPhase = true;
  } else {
    return false; // Timer non gestito
  }

  WSER_UpdateSettingsTimerStrings();

  return true;
}

/**

 */
void WSER_handleSetTonToff() {

  if (!server.hasArg("timer") ||
      !server.hasArg("tonSeconds") ||
      !server.hasArg("toffSeconds")) {
    server.send(400, "text/plain", "Missing parameters");
    return;
  }

  uint8_t timerindex = server.arg("timer").toInt() - 1;
  uint16_t ton = server.arg("tonSeconds").toInt(); 
  uint16_t toff = server.arg("toffSeconds").toInt(); 

  if (WSER_ExtCmdSetTonToff(timerindex, ton, toff)){

    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Invalid timer value");
  }

}

/**

 */
bool WSER_ExtCmdSetTonToff(uint8_t timerIndex, uint16_t tonSeconds, uint16_t toffSeconds) {
                
  // Scrittura in EEPROM
  if (timerIndex == 0) {
    EEPR_WriteUint16(PUMP_1_TON_SECONDS_ADDR,  tonSeconds);
    EEPR_WriteUint16(PUMP_1_TOFF_SECONDS_ADDR, toffSeconds);

    PumpTimers[0].OnSeconds  = EEPR_ReadUint16(PUMP_1_TON_SECONDS_ADDR);
    PumpTimers[0].OffSeconds = EEPR_ReadUint16(PUMP_1_TOFF_SECONDS_ADDR);
  } else if (timerIndex == 1) {
    EEPR_WriteUint16(PUMP_2_TON_SECONDS_ADDR,  tonSeconds);
    EEPR_WriteUint16(PUMP_2_TOFF_SECONDS_ADDR, toffSeconds);

    PumpTimers[1].OnSeconds  = EEPR_ReadUint16(PUMP_2_TON_SECONDS_ADDR);
    PumpTimers[1].OffSeconds = EEPR_ReadUint16(PUMP_2_TOFF_SECONDS_ADDR);
  } else if (timerIndex == 2) {
    EEPR_WriteUint16(PUMP_3_TON_SECONDS_ADDR,  tonSeconds);
    EEPR_WriteUint16(PUMP_3_TOFF_SECONDS_ADDR, toffSeconds);

    PumpTimers[2].OnSeconds  = EEPR_ReadUint16(PUMP_3_TON_SECONDS_ADDR);
    PumpTimers[2].OffSeconds = EEPR_ReadUint16(PUMP_3_TOFF_SECONDS_ADDR);
  } else if (timerIndex == 3) {
    EEPR_WriteUint16(PUMP_4_TON_SECONDS_ADDR,  tonSeconds);
    EEPR_WriteUint16(PUMP_4_TOFF_SECONDS_ADDR, toffSeconds);

    PumpTimers[3].OnSeconds  = EEPR_ReadUint16(PUMP_4_TON_SECONDS_ADDR);
    PumpTimers[3].OffSeconds = EEPR_ReadUint16(PUMP_4_TOFF_SECONDS_ADDR);
  } else {
    return false; // Timer non gestito
  }

  PumpTimers[timerIndex].ElapsedTime = 0;
  PumpTimers[timerIndex].OnPhase = true;

  WSER_UpdateSettingsTimerStrings();

  return true;
}
