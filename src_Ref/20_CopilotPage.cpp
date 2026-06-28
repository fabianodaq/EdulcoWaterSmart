
#include "01_AppControls.h"

#include "03_EEprom.h"
#include "04_Emailer.h"

#include <FS.h>
#include <LittleFS.h>

#include "10_WebServerMain.h"
#include "12_SettingsPage.h"
#include "14_ConfigPage.h"
#include "15_CalibrationPage.h"
#include "16_TimersPage.h"
#include "18_SentinelPage.h"
#include "19_ReporterPage.h" 
#include "20_CopilotPage_private.h"

// ====== Variabili GLOBALI (template) ========================================

unsigned long ulCopilotLastUpdateTime = 0; // tick per loop

// Definizione delle variabili per i messaggi
String messagePanel1Row1 = "---"; // Prima riga del pannello 1
String messagePanel1Row2 = "---"; // Prima riga del pannello 1

String CopilotInfoFileName;

unsigned long messagePanel2Timestamp = 0;
const unsigned long MESSAGE2_TIMEOUT_MS = 15000; // 15 sec

// ====== Dichiarazioni funzioni ==============================================
void WSER_InitCopilotSection();
void WSER_WiFiCopilotPage();
void WSER_CopilotLoop();
void WSER_handleCopilotGetStatus();
void WSER_handleSendMessage();
void WSER_handleExecuteMessage();

bool WSER_CopilotInitDataFile();
String WSER_ValidateAndExecuteCommands(const String& message);
// Smart Home (stub)

/**

 */
void WSER_InitCopilotSection(){

  CopilotInfoFileName = "/CopilotInfoLog.txt";

  //----- open data file------------------
  // 1) Monta FS (con fallback format)
  if (!LittleFS.begin()) {
    Serial.println("LittleFS.begin() FAILED. Provo a formattare...");
    if (!LittleFS.begin(true)) {
      Serial.println("LittleFS format/mount FAILED.");
      return;
    }
  }
  Serial.println("🤖 Copilot LittleFS montato.");
  //----- open data file------------------

  ulCopilotLastUpdateTime = millis();

  WSER_CopilotInitDataFile();

}

/**

 */
void WSER_CopilotLoop() {
  static unsigned long lastCopilotSampleTime = 0;
  unsigned long now = millis();

  if ((messagePanel1Row2 != "---") &&
      ((now - messagePanel2Timestamp) > MESSAGE2_TIMEOUT_MS)) {
      
      messagePanel1Row2 = "---";
  }


  if (now - lastCopilotSampleTime >= WSER_COPILOT_TIMEOUT_INTERVAL) { // ogni secondo
    lastCopilotSampleTime = now;

    if (!(WSER_IsAccessPointMode())) {

      messagePanel1Row1 = "🤖 NetSky Active | 📧 " + String(email_receiver);

      if ( WSER_IsEmailDefined() ) {

      } else {
        messagePanel1Row1= " 🤖 NetSky Not Active, 📧 Email not defined";
      }

    } else {

       messagePanel1Row1= "🤖 NetSky Not Active, System in AP mode";

    }
  }
}

// ====== FETCH HANDLERS ======================================================
/**

 */
void WSER_handleCopilotGetStatus() {
  // Tipo sensore in minuscolo

// JSON (semplice concatenazione)
String json = "{";
json += "\"panel1row1\":\"" + messagePanel1Row1 + "\",";
json += "\"panel1row2\":\"" + messagePanel1Row2 + "\"";
json += "}";

  server.send(200, "application/json", json);
}

// ====== FETCH HANDLERS ======================================================
/**

 */
void WSER_handleExecuteMessage(){

  Serial.println(F("⚙️  Entrato in WSER_handleExecuteMessage"));

  // Parametro presente?
  if (!server.hasArg("message")) {
    Serial.println(F("❌ Parametro 'message' mancante"));
    server.send(400, "text/plain", "Missing 'message' parameter");
    return;
  }

  String message = server.arg("message");
  Serial.print(F("📄 Messaggio ricevuto: "));
  Serial.println(message);

  // Esegue validazione + mock esecuzione
  String result = WSER_ValidateAndExecuteCommands(message);

  // Aggiorna pannello e log
  messagePanel1Row2 = result;

  messagePanel2Timestamp = millis();

  Serial.println(result);


    server.send(200, "text/plain", "OK");

}

/***********************************************************************
 *                📌 LISTA COMANDI SUPPORTATI (COPILOT / SMARTCMD)
 *
 * Tutti i comandi devono essere inviati nel formato:
 *
 *        cmd=<comando>&param1=VAL1&param2=VAL2...
 *
 * --------------------------------------------------------------------
 * 1) SET CONTROL TYPE
 * --------------------------------------------------------------------
 * cmd=setcontroltype&ctrl=0|3&type=ph|orp
 * cmd=setcontroltype&ctrl=2&type=ec|tds|sal|ntc|diff|avg
 *
 * --------------------------------------------------------------------
 * 2) SET CONTROL HYSTERESIS
 * --------------------------------------------------------------------
 * cmd=setcontrolhyster&ctrl=0|3&hysteresis=VAL
 *
 * --------------------------------------------------------------------
 * 3) SET CONTROL LOGIC
 * --------------------------------------------------------------------
 * cmd=setcontrollogik&ctrl=0|3&logic=true|false
 *
 * --------------------------------------------------------------------
 * 4) SET CONTROL SETPOINT
 * --------------------------------------------------------------------
 * cmd=setcontrolsetpoint&ctrl=0|3&setpoint=VAL
 *
 * --------------------------------------------------------------------
 * 5) SET TIMER ACTIVATION
 * --------------------------------------------------------------------
 * cmd=settimeractivation&timer=0|1&active=true|false
 *
 * --------------------------------------------------------------------
 * 6) SET TON / TOFF
 * --------------------------------------------------------------------
 * cmd=settontoff&timer=0|1&ton=SEC&toff=SEC
 *
 * --------------------------------------------------------------------
 * 7) RESET CALIBRATION
 * --------------------------------------------------------------------
 * cmd=resetcalibration&sensor=0..3&calpoint=0|1
 *
 * --------------------------------------------------------------------
 * 8) SET SENTINEL
 * --------------------------------------------------------------------
 * cmd=setsentinel&ctrl=1..4&
 *     status=true|false&
 *     min=VAL&
 *     max=VAL&
 *     time_persist=MIN&
 *     time_psnooze=MIN
 *
 * --------------------------------------------------------------------
 * 9) SET REPORTER
 * --------------------------------------------------------------------
 * cmd=setreporter&ctrl=1..4&active=true|false&period=1|7|30
 *
 ***********************************************************************/
String WSER_ValidateAndExecuteCommands(const String& message) {
  Serial.println(F("🔍 validateAndExecuteCommands() avviata"));
  Serial.print(F("📦 Input message: "));
  Serial.println(message);

  // Pulizia base
  String trimmed = message;
  trimmed.trim();

  if (!trimmed.startsWith("cmd=")) {
    Serial.println(F("⚠️ Messaggio non inizia con 'cmd=' → nessuna azione"));
    return F("ℹ️ Invalid commands format, 'cmd=' missing");
  }
  
  int cmdStart = 4; 
  int ampPos = trimmed.indexOf('&', cmdStart);

  String cmdValue;
  if (ampPos == -1) {
    cmdValue = trimmed.substring(cmdStart);
  } else {
    cmdValue = trimmed.substring(cmdStart, ampPos);
  }

  Serial.print(F("✅ Comando isolato: "));
  Serial.println(cmdValue);

  String ResutlString = "No Command executed";

  if  (cmdValue == "setcontroltype") {
    Serial.println(F("➡️ SET CONTROL TYPE"));

    // 🔹 Estrai ctrl=...
    int ctrlPos = trimmed.indexOf("ctrl=");
    if (ctrlPos < 0) {
      Serial.println(F("❌ Parametro 'ctrl' mancante"));
      return F("❌ Missing 'ctrl' parameter");
    }
    int ctrlStart = ctrlPos + 5; // dopo "ctrl="
    int ctrlEnd = trimmed.indexOf('&', ctrlStart);
    String ctrlStr;
    if (ctrlEnd == -1) {
      // fino a fine stringa
      ctrlStr = trimmed.substring(ctrlStart);
    } else {
      // fino al prossimo &
      ctrlStr = trimmed.substring(ctrlStart, ctrlEnd);
    }

    // 🔹 Estrai type=...
    int typePos = trimmed.indexOf("type=");
    if (typePos < 0) {
      Serial.println(F("❌ Parametro 'type' mancante"));
      return F("❌ Missing 'type' parameter");
    }
    int typeStart = typePos + 5; // dopo "type="
    int typeEnd = trimmed.indexOf('&', typeStart);
    String typeStr;
    if (typeEnd == -1) {
      typeStr = trimmed.substring(typeStart);
    } else {
      typeStr = trimmed.substring(typeStart, typeEnd);
    }

    // 🔹 Conversione e log
    int ctrl = ctrlStr.toInt();
    Serial.print(F("   ▶ ctrl = "));
    Serial.println(ctrl);
    Serial.print(F("   ▶ type = "));
    Serial.println(typeStr);

    // 🔹 Esecuzione reale (riuso dell’API esistente)
    if (WSER_ExtCmdSetType(ctrl, typeStr)) {
      return F("✅ Control type set");
    } else {
      return F("❌ Failed to set control type");
    }
  } 
    
  else if (cmdValue == "setcontrolhyster") {
  Serial.println(F("➡️ SET CONTROL HYSTERESIS"));

  // 🔹 Estrai ctrl=...
  int ctrlPos = trimmed.indexOf("ctrl=");
  if (ctrlPos < 0) {
    Serial.println(F("❌ Parametro 'ctrl' mancante"));
    return F("❌ Missing 'ctrl' parameter");
  }
  int ctrlStart = ctrlPos + 5;
  int ctrlEnd = trimmed.indexOf('&', ctrlStart);
  String ctrlStr = (ctrlEnd == -1) ? trimmed.substring(ctrlStart)
                                  : trimmed.substring(ctrlStart, ctrlEnd);

  // 🔹 Estrai hysteresis=...
  int hystPos = trimmed.indexOf("hysteresis=");
  if (hystPos < 0) {
    Serial.println(F("❌ Parametro 'hysteresis' mancante"));
    return F("❌ Missing 'hysteresis' parameter");
  }
  int hystStart = hystPos + 11;
  int hystEnd = trimmed.indexOf('&', hystStart);
  String hystStr = (hystEnd == -1) ? trimmed.substring(hystStart)
                                  : trimmed.substring(hystStart, hystEnd);

  // 🔹 Conversione
  int ctrl  = ctrlStr.toInt();
  int hyster = hystStr.toInt();

  Serial.print(F("   ▶ ctrl = "));
  Serial.println(ctrl);
  Serial.print(F("   ▶ hysteresis = "));
  Serial.println(hyster);

  // 🔹 Esecuzione reale
  if (WSER_ExtCmdSetHysteresis(ctrl, hyster)) {
    return F("✅ Hysteresis set");
  } else {
    return F("❌ Failed to set hysteresis");
  }
}

else if (cmdValue == "setcontrollogik") {
  Serial.println(F("➡️ SET CONTROL LOGIC"));

  // ctrl=...
  int ctrlPos = trimmed.indexOf("ctrl=");
  if (ctrlPos < 0) {
    Serial.println(F("❌ Parametro 'ctrl' mancante"));
    return F("❌ Missing 'ctrl' parameter");
  }
  int ctrlStart = ctrlPos + 5;
  int ctrlEnd = trimmed.indexOf('&', ctrlStart);
  String ctrlStr = (ctrlEnd == -1) ? trimmed.substring(ctrlStart)
                                   : trimmed.substring(ctrlStart, ctrlEnd);

  // logic=...
  int logicPos = trimmed.indexOf("logic=");
  if (logicPos < 0) {
    Serial.println(F("❌ Parametro 'logic' mancante"));
    return F("❌ Missing 'logic' parameter");
  }
  int logicStart = logicPos + 6;
  int logicEnd = trimmed.indexOf('&', logicStart);
  String logicStr = (logicEnd == -1) ? trimmed.substring(logicStart)
                                     : trimmed.substring(logicStart, logicEnd);

  int  ctrl  = ctrlStr.toInt();
  bool logic = (logicStr == "true" || logicStr == "1");

  Serial.print(F("   ▶ ctrl = "));
  Serial.println(ctrl);
  Serial.print(F("   ▶ logic = "));
  Serial.println(logic ? F("true") : F("false"));

  if (WSER_ExtCmdSetLogic(ctrl, logic)) {
    return F("✅ Logic set");
  } else {
    return F("❌ Failed to set logic");
  }

} else if (cmdValue == "setcontrolsetpoint") {
  Serial.println(F("➡️ SET CONTROL SETPOINT"));

  // ctrl=...
  int ctrlPos = trimmed.indexOf("ctrl=");
  if (ctrlPos < 0) {
    Serial.println(F("❌ Parametro 'ctrl' mancante"));
    return F("❌ Missing 'ctrl' parameter");
  }
  int ctrlStart = ctrlPos + 5;
  int ctrlEnd = trimmed.indexOf('&', ctrlStart);
  String ctrlStr = (ctrlEnd == -1) ? trimmed.substring(ctrlStart)
                                   : trimmed.substring(ctrlStart, ctrlEnd);

  // setpoint=...
  int spPos = trimmed.indexOf("setpoint=");
  if (spPos < 0) {
    Serial.println(F("❌ Parametro 'setpoint' mancante"));
    return F("❌ Missing 'setpoint' parameter");
  }
  int spStart = spPos + 9;
  int spEnd = trimmed.indexOf('&', spStart);
  String spStr = (spEnd == -1) ? trimmed.substring(spStart)
                               : trimmed.substring(spStart, spEnd);

  int ctrl = ctrlStr.toInt();
  int sp   = spStr.toInt();

  Serial.print(F("   ▶ ctrl = "));
  Serial.println(ctrl);
  Serial.print(F("   ▶ setpoint = "));
  Serial.println(sp);

  if (WSER_ExtCmdSetSetpoint(ctrl, sp)) {
    return F("✅ SetPoint set");
  } else {
    return F("❌ Failed to set SetPoint");
  }

} else if (cmdValue == "settimeractivation") {
  Serial.println(F("➡️ SET TIMER ACTIVATION"));

  // timer=...
  int timerPos = trimmed.indexOf("timer=");
  if (timerPos < 0) {
    Serial.println(F("❌ Parametro 'timer' mancante"));
    return F("❌ Missing 'timer' parameter");
  }
  int timerStart = timerPos + 6;
  int timerEnd = trimmed.indexOf('&', timerStart);
  String timerStr = (timerEnd == -1) ? trimmed.substring(timerStart)
                                     : trimmed.substring(timerStart, timerEnd);

  // active=...
  int activePos = trimmed.indexOf("active=");
  if (activePos < 0) {
    Serial.println(F("❌ Parametro 'active' mancante"));
    return F("❌ Missing 'active' parameter");
  }
  int activeStart = activePos + 7;
  int activeEnd = trimmed.indexOf('&', activeStart);
  String activeStr = (activeEnd == -1) ? trimmed.substring(activeStart)
                                       : trimmed.substring(activeStart, activeEnd);

  int  timer  = timerStr.toInt();
  bool active = (activeStr == "true" || activeStr == "1");

  Serial.print(F("   ▶ timer = "));
  Serial.println(timer);
  Serial.print(F("   ▶ active = "));
  Serial.println(active ? F("true") : F("false"));

  if (WSER_ExtCmdSetTimerActivation(timer, active)) {
    return F("✅ Timer activation set");
  } else {
    return F("❌ Failed to set timer activation");
  }

} else if (cmdValue == "settontoff") {
  Serial.println(F("➡️ SET TON / TOFF"));

  // timer=...
  int timerPos = trimmed.indexOf("timer=");
  if (timerPos < 0) {
    Serial.println(F("❌ Parametro 'timer' mancante"));
    return F("❌ Missing 'timer' parameter");
  }
  int timerStart = timerPos + 6;
  int timerEnd = trimmed.indexOf('&', timerStart);
  String timerStr = (timerEnd == -1) ? trimmed.substring(timerStart)
                                     : trimmed.substring(timerStart, timerEnd);

  // ton=...
  int tonPos = trimmed.indexOf("ton=");
  if (tonPos < 0) {
    Serial.println(F("❌ Parametro 'ton' mancante"));
    return F("❌ Missing 'ton' parameter");
  }
  int tonStart = tonPos + 4;
  int tonEnd = trimmed.indexOf('&', tonStart);
  String tonStr = (tonEnd == -1) ? trimmed.substring(tonStart)
                                 : trimmed.substring(tonStart, tonEnd);

  // toff=...
  int toffPos = trimmed.indexOf("toff=");
  if (toffPos < 0) {
    Serial.println(F("❌ Parametro 'toff' mancante"));
    return F("❌ Missing 'toff' parameter");
  }
  int toffStart = toffPos + 5;
  int toffEnd = trimmed.indexOf('&', toffStart);
  String toffStr = (toffEnd == -1) ? trimmed.substring(toffStart)
                                   : trimmed.substring(toffStart, toffEnd);

  int timer = timerStr.toInt();
  int ton   = tonStr.toInt();
  int toff  = toffStr.toInt();

  Serial.print(F("   ▶ timer = "));
  Serial.println(timer);
  Serial.print(F("   ▶ ton   = "));
  Serial.println(ton);
  Serial.print(F("   ▶ toff  = "));
  Serial.println(toff);

  if (WSER_ExtCmdSetTonToff(timer, ton, toff)) {
    return F("✅ TON and TOFF set");
  } else {
    return F("❌ Failed to set TON or TOFF");
  }

} else if (cmdValue == "resetcalibration") {
  Serial.println(F("➡️ RESET CALIBRATION"));

  // sensor=...
  int sensPos = trimmed.indexOf("sensor=");
  if (sensPos < 0) {
    Serial.println(F("❌ Parametro 'sensor' mancante"));
    return F("❌ Missing 'sensor' parameter");
  }
  int sensStart = sensPos + 7;
  int sensEnd = trimmed.indexOf('&', sensStart);
  String sensStr = (sensEnd == -1) ? trimmed.substring(sensStart)
                                   : trimmed.substring(sensStart, sensEnd);

  // calpoint=...
  int cpPos = trimmed.indexOf("calpoint=");
  if (cpPos < 0) {
    Serial.println(F("❌ Parametro 'calpoint' mancante"));
    return F("❌ Missing 'calpoint' parameter");
  }
  int cpStart = cpPos + 9;
  int cpEnd = trimmed.indexOf('&', cpStart);
  String cpStr = (cpEnd == -1) ? trimmed.substring(cpStart)
                               : trimmed.substring(cpStart, cpEnd);

  uint8_t sensor   = (uint8_t)sensStr.toInt();
  uint8_t calpoint = (uint8_t)cpStr.toInt();

  Serial.print(F("   ▶ sensor   = "));
  Serial.println(sensor);
  Serial.print(F("   ▶ calpoint = "));
  Serial.println(calpoint);

  if (WSER_ExtCmdCalReset(sensor, calpoint)) {
    return F("✅ Calibration reset to default");
  } else {
    return F("❌ Reset calibration failed");
  }

} else if (cmdValue == "setsentinel") {
  Serial.println(F("➡️ SET SENTINEL"));

  // ctrl=...
  int ctrlPos = trimmed.indexOf("ctrl=");
  int statusPos = trimmed.indexOf("status=");
  int minPos = trimmed.indexOf("min=");
  int maxPos = trimmed.indexOf("max=");
  int persPos = trimmed.indexOf("time_persist=");
  int snoozePos = trimmed.indexOf("time_psnooze=");

  if (ctrlPos < 0 || statusPos < 0 || minPos < 0 || maxPos < 0 ||
      persPos < 0 || snoozePos < 0) {
    Serial.println(F("❌ Parametri mancanti per setsentinel"));
    return F("❌ Missing parameters for setsentinel");
  }

  // ctrl
  int ctrlStart = ctrlPos + 5;
  int ctrlEnd = trimmed.indexOf('&', ctrlStart);
  String ctrlStr = (ctrlEnd == -1) ? trimmed.substring(ctrlStart)
                                   : trimmed.substring(ctrlStart, ctrlEnd);

  // status
  int statusStart = statusPos + 7;
  int statusEnd = trimmed.indexOf('&', statusStart);
  String statusStr = (statusEnd == -1) ? trimmed.substring(statusStart)
                                       : trimmed.substring(statusStart, statusEnd);

  // min
  int minStart = minPos + 4;
  int minEnd = trimmed.indexOf('&', minStart);
  String minStr = (minEnd == -1) ? trimmed.substring(minStart)
                                 : trimmed.substring(minStart, minEnd);

  // max
  int maxStart = maxPos + 4;
  int maxEnd = trimmed.indexOf('&', maxStart);
  String maxStr = (maxEnd == -1) ? trimmed.substring(maxStart)
                                 : trimmed.substring(maxStart, maxEnd);

  // time_persist
  int persStart = persPos + 13;
  int persEnd = trimmed.indexOf('&', persStart);
  String persStr = (persEnd == -1) ? trimmed.substring(persStart)
                                   : trimmed.substring(persStart, persEnd);

  // time_psnooze
  int snoozeStart = snoozePos + 13;
  int snoozeEnd = trimmed.indexOf('&', snoozeStart);
  String snoozeStr = (snoozeEnd == -1) ? trimmed.substring(snoozeStart)
                                       : trimmed.substring(snoozeStart, snoozeEnd);

  uint8_t ctrl = (uint8_t)ctrlStr.toInt();
  bool active  = statusStr.equalsIgnoreCase("true") || statusStr == "1";
  int16_t minV = (int16_t)minStr.toInt();
  int16_t maxV = (int16_t)maxStr.toInt();
  uint16_t pers = (uint16_t)persStr.toInt();
  uint16_t occ  = (uint16_t)snoozeStr.toInt();

  Serial.print(F("   ▶ ctrl    = "));
  Serial.println(ctrl);
  Serial.print(F("   ▶ status  = "));
  Serial.println(active ? F("true") : F("false"));
  Serial.print(F("   ▶ min     = "));
  Serial.println(minV);
  Serial.print(F("   ▶ max     = "));
  Serial.println(maxV);
  Serial.print(F("   ▶ persist = "));
  Serial.println(pers);
  Serial.print(F("   ▶ snooze  = "));
  Serial.println(occ);

  if (WSER_ExtCmdSetSentinel(ctrl, active, minV, maxV, pers, occ)) {
    return F("✅ Sentinel updated");
  } else {
    return F("❌ Set sentinel failed");
  }

} else if (cmdValue == "setreporter") {
  Serial.println(F("➡️ SET REPORTER"));

  int ctrlPos   = trimmed.indexOf("ctrl=");
  int activePos = trimmed.indexOf("active=");
  int periodPos = trimmed.indexOf("period=");

  if (ctrlPos < 0 || activePos < 0 || periodPos < 0) {
    Serial.println(F("❌ Parametri mancanti per setreporter"));
    return F("❌ Missing parameters for setreporter");
  }

  // ctrl
  int ctrlStart = ctrlPos + 5;
  int ctrlEnd = trimmed.indexOf('&', ctrlStart);
  String ctrlStr = (ctrlEnd == -1) ? trimmed.substring(ctrlStart)
                                   : trimmed.substring(ctrlStart, ctrlEnd);

  // active
  int activeStart = activePos + 7;
  int activeEnd = trimmed.indexOf('&', activeStart);
  String activeStr = (activeEnd == -1) ? trimmed.substring(activeStart)
                                       : trimmed.substring(activeStart, activeEnd);

  // period
  int periodStart = periodPos + 7;
  int periodEnd = trimmed.indexOf('&', periodStart);
  String periodStr = (periodEnd == -1) ? trimmed.substring(periodStart)
                                       : trimmed.substring(periodStart, periodEnd);

  uint8_t ctrl   = (uint8_t)ctrlStr.toInt();
  bool active    = (activeStr == "true" || activeStr == "1");
  uint8_t period = (uint8_t)periodStr.toInt();

  Serial.print(F("   ▶ ctrl   = "));
  Serial.println(ctrl);
  Serial.print(F("   ▶ active = "));
  Serial.println(active ? F("true") : F("false"));
  Serial.print(F("   ▶ period = "));
  Serial.println(period);

  if (WSER_ExtCmdSetReporter(ctrl, active, period)) {
    return F("✅ Reporter updated");
  } else {
    return F("❌ Set reporter failed");
  }
}
 else {
    Serial.println(F("❌ COMANDO NON RICONOSCIUTO"));
    return F("❌ Unknown command");
  }


}

/**

 */
void WSER_handleSendMessage() {

    Serial.println("📥 Entrato in WSER_handleSendMessage");

    // Controlla se il messaggio è presente
    if (!server.hasArg("message")) {
        Serial.println("❌ Parametro 'message' mancante");
        server.send(400, "text/plain", "Missing message parameter");
        return;
    }

    String message = server.arg("message");
    Serial.print("📄 Messaggio ricevuto: ");
    Serial.println(message);

    messagePanel1Row1= "Message NOT sent";

    if ( (!WSER_IsAccessPointMode()) && WSER_IsEmailDefined() ) {
        
      if ( EML_CopilotSendMessage(message, CopilotInfoFileName)){

        messagePanel1Row1 = "💌 Message sent";
      }
    
    }

    server.send(200, "text/plain", "OK");
}

/**

 */
void WSER_WiFiCopilotPage() {
    Serial.print("COPILOT: Heap available before send page: ");
    Serial.println(ESP.getFreeHeap());

    server.setContentLength(CONTENT_LENGTH_UNKNOWN);

    server.send_P(200, "text/html", WSER_HTML_COPILOT_HEAD);
    server.sendContent_P(WSER_COMMON_PAGES_STYLE);
    server.sendContent_P(WSER_HTML_COPILOT_STYLE); 
    server.sendContent_P(WSER_HTML_COPILOT_HEAD_OPEN);

    String nav = WSER_GenerateNavBar("copilot");
    server.sendContent(nav);

    server.sendContent_P(WSER_HTML_COPILOT_BODY_OPEN);
    server.sendContent_P(WSER_HTML_COPILOT_BODY_CONTENT);

    // Esegui il replace per i secondi, minuti e ore
    String scriptCopilot = FPSTR(WSER_SCRIPT_COPILOT);


    server.sendContent(scriptCopilot);
    server.sendContent_P(WSER_COPILOT_CLOSE_PAGE);

    Serial.print("COPILOT: Heap available after send page: ");
    Serial.println(ESP.getFreeHeap());
}

//files section 
/**

 */
bool WSER_CopilotInitDataFile() {
    bool isNew = false;

    // Verifica se il file esiste
    if (!LittleFS.exists(CopilotInfoFileName)) {
        isNew = true;
    }

    // Apre il file in modalità append (crea se non esiste)
    File dataFile = LittleFS.open(CopilotInfoFileName, "a");
    if (!dataFile) {
        Serial.println(F("❌ Errore: impossibile aprire il report file"));
        return false;
    }

    // Se è nuovo, scrivi l'intestazione o la prima riga
    if (isNew) {
        dataFile.println(email_receiver);
        dataFile.flush();
    }

    dataFile.close();
    Serial.println(F("📁 File Copilot inizializzato."));
    return true;
}

