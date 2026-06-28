
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <pgmspace.h>
#include "01_AppControls.h"
#include "05_AppHW.h"

#include "04_Emailer.h"

#include "11_MonitorPage.h"
#include "12_SettingsPage.h"
#include "13_RelaysPage.h"

#include "15_CalibrationPage.h"

#include "14_ConfigPage.h"

#include "16_TimersPage.h"
#include "18_SentinelPage.h"
#include "19_ReporterPage.h"
#include "20_CopilotPage.h"
#include "21_AboutPage.h"

/**

 */
const String html_navbar = R"rawliteral(
<nav>
  <ul>
    <li><a href="/" class="{MONITOR_ACTIVE}">📊 Monitor</a></li>

    <li><a href="/calibration" class="{CALIBRATION_ACTIVE}">🧪 Sensors</a></li>
    <li><a href="/settings" class="{SETTINGS_ACTIVE}">⚙️ Controls</a></li>
    <li><a href="/pumptimers" class="{PUMPTIMERS_ACTIVE}">🕒 Timer</a></li>
    <li><a href="/remoterelays" class="{REMOTERELAYS_ACTIVE}">🔌 Relays</a></li>

    <li><a href="/wificonfig" class="{WIFICONFIG_ACTIVE}">📡 WiFiConfig</a></li>

    <li><a href="/wifisentinel" class="{WIFISENTINEL_ACTIVE}">🛡️ Sentinel</a></li>
    <a href="/reporter" class="{REPORTER_ACTIVE}">📈 Reporter</a></li>
    <li><a href="/copilot" class="{COPILOT_ACTIVE}">🤖 NetSky</a></li>
    
    <li style="margin-left:auto;"><a href="/about" class="{ABOUT_ACTIVE}">ℹ️ About</a></li>
  </ul>
</nav>
)rawliteral";

// Funzione per sostituire il tab attivo nella navbar
String WSER_GenerateNavBar(const String& activeTab) {
  String nav = html_navbar;
  nav.replace("{MONITOR_ACTIVE}", activeTab == "monitor" ? "active" : "");

  nav.replace("{CALIBRATION_ACTIVE}", activeTab == "calibration" ? "active" : "");
  nav.replace("{SETTINGS_ACTIVE}", activeTab == "settings" ? "active" : "");
  nav.replace("{PUMPTIMERS_ACTIVE}", activeTab == "pumptimers" ? "active" : ""); 
  nav.replace("{REMOTERELAYS_ACTIVE}", activeTab == "remoterelays" ? "active" : "");

  nav.replace("{WIFICONFIG_ACTIVE}", activeTab == "wificonfig" ? "active" : "");
  
  nav.replace("{WIFISENTINEL_ACTIVE}", activeTab == "wifisentinel" ? "active" : "");
  nav.replace("{REPORTER_ACTIVE}", activeTab == "reporter" ? "active" : ""); 
  nav.replace("{COPILOT_ACTIVE}", activeTab == "copilot" ? "active" : ""); 

  nav.replace("{ABOUT_ACTIVE}", activeTab == "about" ? "active" : "");
  return nav;
}


//--- Private Variables -------------------------------------------------------

//--- Public Variables --------------------------------------------------------
WebServer server(80);

//--- Private Function Declarations ------------------------------------------
void WSER_InitWebServer();
void WSER_HandleWebServer();

 void WSER_handleSmartCommand();
//--- Public Function Declarations -------------------------------------------

//-----------------------------------------------------------------------------
//--- PUBLIC Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------

/**

 */
void WSER_InitWebServer(){

  WSER_InitWiFiSection(); 
  WSER_InitAboutSection();
  WSER_InitRemoteRelays();
  WSER_InitPumpTimersSection();
  WSER_InitCalibrationSection();
  WSER_InitMonitorSection();
  WSER_InitSettingsSection();
  WSER_InitSentinelSection();
  WSER_InitReporterSection();
  WSER_InitCopilotSection();

  EML_Emailer_Setup();

  server.on("/smartcommand", HTTP_POST, WSER_handleSmartCommand);

  server.on("/"             , WSER_WiFiMonitorPage      ); 
    server.on("/WiFiMonitorGetStatus", HTTP_GET, WSER_handleMonitorGetStatus);
  
  server.on("/calibration"                ,             WSER_WiFiCalibrationgPage  );
    server.on("/setControlType"           , HTTP_POST,  WSER_handleSettingControlType);
    server.on("/setCalPoint"              , HTTP_POST,  WSER_handleSetCalPoint);
    server.on("/resetCalPoint"            , HTTP_POST,  WSER_handleReSetCalPoint);
    server.on("/WiFiCalibrationGetStatus" , HTTP_GET,   WSER_handleCalibrGetStatus);

  server.on("/settings"           ,  WSER_WiFiSettingsgPage     );
    server.on("/setControlLogik"  ,     HTTP_POST, WSER_handleSettingControlLogik);
    server.on("/setControlHyster" ,     HTTP_POST, WSER_handleSettingControlHyster);
    server.on("/setControlTarget" ,     HTTP_POST, WSER_handleSettingControlSetPoint);
      server.on("/settingsGetStatus",   HTTP_GET,   WSER_handleSettingsGetStatus);

  server.on("/pumptimers" , WSER_WiFiPumpTimersPage  );
    server.on("/setTimerActivation"       , HTTP_POST,  WSER_handleSetTimerActivation);
    server.on("/setTonToff"               , HTTP_POST,  WSER_handleSetTonToff);
    server.on("/PumpTimersGetStatus"      , HTTP_GET,   WSER_handlePumpTimersGetStatus);

  server.on("/remoterelays", WSER_WiFiRemoteRelaysPage  );
    server.on("/saveRelay"                , HTTP_POST, WSER_handleSaveRelay);
    server.on("/RemoteRelaysGetStatus"    , HTTP_GET, WSER_handleRemoteRelaysGetStatus);

  server.on("/wificonfig"                 ,             WSER_WiFiConfigPage);
    server.on("/saveap"                   , HTTP_POST,  WSER_handleSaveAP);
    server.on("/savestation"              , HTTP_POST,  WSER_handleSaveSTA);
    server.on("/forgetsta"                , HTTP_POST,  WSER_handleForgetSTA);
    server.on("/saveEmail"                , HTTP_POST,  WSER_handleSaveEmail);
    server.on("/saveEmailSender"          , HTTP_POST,  WSER_handleSaveEmailSender);
    server.on("/WiFiConfigGetStatus"      , HTTP_GET,   WSER_handleGetStatus);

  server.on("/wifisentinel"                 ,             WSER_WiFiSentinelPage);
    server.on("/setsentinel"                , HTTP_POST,  WSER_handleSetSentinel);
    server.on("/wifisentinelGetStatus"      , HTTP_GET,   WSER_handleSentinelGetStatus);

  server.on("/reporter"               , WSER_WiFiReporterPage);
    server.on("/setreporter"            , HTTP_POST, WSER_handleSetReporter);
    server.on("/wifireporterGetStatus"  , HTTP_GET,  WSER_handleReporterGetStatus);

    server.on("/copilot", WSER_WiFiCopilotPage);
    server.on("/sendmessage", HTTP_POST, WSER_handleSendMessage);
    server.on("/executemessage", HTTP_POST, WSER_handleExecuteMessage);
    server.on("/wificopilotGetStatus", HTTP_GET, WSER_handleCopilotGetStatus);

  server.on("/about"        , WSER_WiFiAboutPage         ); 

  server.begin();
  //Serial.println("Web server avviato");
}

/**
 */
void WSER_HandleWebServer()
{
    server.handleClient();

    WSER_CalibrationLoop();
    WSER_RemoteRelaysLoop();
    WSER_PumpTimersLoop();
    WSER_WiFiConfigLoop();
    WSER_MonitorLoop();
    WSER_SettingsLoop();
    WSER_SentinelLoop();
    WSER_ReporterLoop();
    WSER_CopilotLoop();
    EML_Emailer_Loop();

}

//-----------------------------------------------------------------------------
//--- Private Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**
 * @brief Gestione dei comandi remoti via POST su endpoint /smartcommand
 * 
 * Questa funzione gestisce tutti i comandi di tipo GET e SET del sistema EdulcoWater Modular,
 * tramite una singola interfaccia HTTP POST. I comandi sono identificati dal parametro `cmd`,
 * con eventuali parametri aggiuntivi. Restituisce risposte in testo o formato JSON.
 * 
 * === COMANDI GET ===
 * 
 * 1. Get device info (About)..
 *    $ curl -X POST http://192.168.178.132/smartcommand -d "cmd=getdeviceabout"
 *
 * 2. Get stato controllo....
 *  $ curl -X POST http://192.168.178.132/smartcommand -d "cmd=getcontrolstatus&ctrl=0"
 * 
 * 3. Get stato calibrazione....
 *    - curl -X POST http://192.168.178.132/smartcommand -d "cmd=getcalstatus&sensor=1"
 * 
 * === COMANDI SET ===
 * 4. Set tipo controllo
 *    $ curl -X POST http://192.168.178.132/smartcommand -d "cmd=setcontroltype&ctrl=3&type=ph"
 * 
 * 5. Set isteresi controllo...
 *    curl -X POST http://192.168.178.132/smartcommand -d "cmd=setcontrolhyster&ctrl=3&hysteresis=57"
 * 
 * 6. Set logica controllo....
 *    curl -X POST http://192.168.178.132/smartcommand -d "cmd=setcontrollogik&ctrl=0&logic=false"
 * 
 * 7. Set setpoint controllo.....
 *    curl -X POST http://192.168.178.132/smartcommand -d "cmd=setcontrolsetpoint&ctrl=0&setpoint=704"
 *
 * 8. Attiva/disattiva timer
 *     - curl -X POST http://192.168.178.132/smartcommand -d "cmd=settimeractivation&timer=0&active=false"
 * 
 * 9. Set tempo TON (ON)
 *     curl -X POST http://192.168.178.132/smartcommand -d "cmd=settontoff&timer=0&ton=180&toff=240"
 * 
 * 10. Reset calibrazione.....
 *       curl -X POST http://192.168.178.132/smartcommand -d "cmd=resetcalibration&sensor=0&calpoint=1"
 *
 *11. Set and reset  Sentinel.....
 *       curl -X POST http://192.168.178.132/smartcommand -d "cmd=setsentinel&ctrl=1&status=true&min=300&max=900&time_persist=15&time_psnooze=60"
 *
 * 12. Set and reset  Reporter.....
 *  curl -X POST http://192.168.178.132/smartcommand -d "cmd=setreporter&ctrl=1&active=true&period=60"
 *
 * 13. Get Reporter Status.....
 *  curl -X POST http://192.168.178.132/smartcommand -d "cmd=getreporterstatus&ctrl=2"
 *
 * @note Sostituire <IP> con l'indirizzo IP del modulo (es. 192.168.4.1 o 192.168.1.132)
 * 
 * @see Tutti i comandi rispondono con codice 200 e messaggio o JSON. In caso di errore 400.
 */
void WSER_handleSmartCommand() {
  if (!server.hasArg("cmd")) {
    server.send(400, "text/plain", "❌ Missing 'cmd' argument");
    return;
  }

  String cmd = server.arg("cmd");

  //---------------- get device info -------------------

  if (cmd == "getdeviceabout") {
    Serial.println("[SMART] Spedite informazioni about del dispositivo");

    String json = WSER_ExtCmdGetDeviceAbout();
    server.send(200, "application/json", json);
  }
  //---------------- get control number status -------------------

  else if (cmd == "getcontrolstatus") {
    if (!server.hasArg("ctrl")) {
      server.send(400, "text/plain", "❌ Missing 'ctrl' argument");
      return;
    }

    int ctrlIndex = server.arg("ctrl").toInt();

    if (ctrlIndex < 0 || ctrlIndex > 3) {
      server.send(400, "text/plain", "❌ Invalid control index (0 - 3 supported)");
      return;
    }

    String json = WSER_ExtCmdGetStatus(ctrlIndex);
    server.send(200, "application/json", json);
  }

  //---------------- get calibration status -------------------

  else if (cmd == "getcalstatus") {

    if (!server.hasArg("sensor")) {
      server.send(400, "text/plain", "❌ Missing 'sensor' argument");
      return;
    }

    uint8_t sensor = server.arg("sensor").toInt();
    if (sensor >= NUMER_OF_SENSORS) {
      server.send(400, "text/plain", "❌ Invalid sensor number");
      return;
    }

    WSER_CalibrationSmartHomeGetStatus(sensor);

  }
  //---------------- set commands-------------------

  else if (cmd == "setcontroltype") {
    String ctrlStr = server.arg("ctrl");     // "0" o "3"
    String typeStr = server.arg("type");     // "ph" o "orp"

    int ctrl = ctrlStr.toInt();

    Serial.printf("Set Control[%d] Type: %s\n", ctrl, typeStr.c_str());

    if (WSER_ExtCmdSetType(ctrl, typeStr)) {
      server.send(200, "text/plain", "✅ Control type set");
    } else {
      server.send(400, "text/plain", "❌ Failed to set control type");
    }
  }

  else if (cmd == "setcontrolhyster") {
    int ctrl = server.arg("ctrl").toInt();
    int hyster = server.arg("hysteresis").toInt();
    Serial.printf("Set Control[%d] Hysteresis: %d\n", ctrl, hyster);

    if (WSER_ExtCmdSetHysteresis(ctrl, hyster)) {
      server.send(200, "text/plain", "✅ Hysteresis set");
    } else {
      server.send(400, "text/plain", "❌ Failed to set hysteresis");
    }
  }

  else if (cmd == "setcontrollogik") {
    int ctrl = server.arg("ctrl").toInt();                         // 0 o 3
    bool logic = server.arg("logic") == "true";                    // true o false
    Serial.printf("Set Control[%d] Logic: %s\n", ctrl, logic ? "true" : "false");

    if (WSER_ExtCmdSetLogic(ctrl, logic)) {
      server.send(200, "text/plain", "✅ Logic set");
    } else {
      server.send(400, "text/plain", "❌ Failed to set logic");
    }
  }

  else if (cmd == "setcontrolsetpoint") {
    int ctrl = server.arg("ctrl").toInt();                // 0 o 3
    int sp = server.arg("setpoint").toInt();              // valore setpoint

    Serial.printf("Set Control[%d] SetPoint: %d\n", ctrl, sp);

    if (WSER_ExtCmdSetSetpoint(ctrl, sp)) {
      server.send(200, "text/plain", "✅ SetPoint set");
    } else {
      server.send(400, "text/plain", "❌ Failed to set SetPoint");
    }
  }

else if (cmd == "settimeractivation") {
  int timer = server.arg("timer").toInt();
  bool active = server.arg("active") == "true";
  Serial.printf("Set Timer[%d] Active: %s\n", timer, active ? "true" : "false");

  if (WSER_ExtCmdSetTimerActivation(timer, active)) {
    server.send(200, "text/plain", "✅ Timer activation set");
  } else {
    server.send(400, "text/plain", "❌ Failed to set timer activation");
  }
}

  else if (cmd == "settontoff") {
    int timer = server.arg("timer").toInt();
    int ton = server.arg("ton").toInt();
    int toff = server.arg("toff").toInt();

    Serial.printf("Set Timer[%d] TON: %d sec, TOFF: %d sec\n", timer, ton, toff);

    if (WSER_ExtCmdSetTonToff(timer, ton, toff)) {
      server.send(200, "text/plain", "✅ TON and TOFF set");
    } else {
      server.send(400, "text/plain", "❌ Failed to set TON or TOFF");
    }
  }

  else if (cmd == "resetcalibration") {
    if ((!server.hasArg("sensor")) || (!server.hasArg("calpoint")) ) {
      server.send(400, "application/json", "{\"error\":\"Missing sensor parameter\"}");
      return;
    }

    uint8_t sensor = server.arg("sensor").toInt();
    uint8_t calpoint= server.arg("calpoint").toInt();

    if (WSER_ExtCmdCalReset(sensor, calpoint)) {
      server.send(200, "text/plain", "✅ Calibration reset to default");
    } else {
      server.send(500, "application/json", "{\"error\":\"Reset failed\"}");
    }
  } 
  
  else if (cmd == "setsentinel") {

    if (!server.hasArg("ctrl")      ||
        !server.hasArg("status")       ||
        !server.hasArg("min")          ||
        !server.hasArg("max")          ||
        !server.hasArg("time_persist") ||
        !server.hasArg("time_psnooze") )
    {
      server.send(400, "application/json", "{\"error\":\"Missing parameters\"}");
      return;
    }

    uint8_t ctrl   = server.arg("ctrl").toInt();
    String  statusStr = server.arg("status");

    int16_t  minV = server.arg("min").toInt();
    int16_t  maxV = server.arg("max").toInt();
    uint16_t pers = server.arg("time_persist").toInt();   // minuti
    uint16_t occ  = server.arg("time_psnooze").toInt();   // minuti

     bool active = statusStr.equalsIgnoreCase("true") ||  statusStr == "1";

    if (WSER_ExtCmdSetSentinel(ctrl, active, minV, maxV, pers, occ)) {
      server.send(200, "text/plain", "✅ Sentinel updated");
    } else {
      server.send(500, "application/json", "{\"error\":\"Set sentinel failed\"}");
    }
  }
  //---------------- Reporter -------------------
  else if (cmd == "setreporter") {

      if (!server.hasArg("ctrl") ||
          !server.hasArg("active") ||
          !server.hasArg("period")) {
          server.send(400, "application/json", "{\"error\":\"Missing parameters\"}");
          return;
      }

      int ctrlParsed = server.arg("ctrl").toInt();
      String activeStr = server.arg("active");
      int periodParsed = server.arg("period").toInt();

      if ((ctrlParsed == 0) || (ctrlParsed > 4)) {
          server.send(400, "application/json", "{\"error\":\"Invalid control index (must be 1 or 2 3 4)\"}");
          return;
      }

      if (activeStr != "true" && activeStr != "false") {
          server.send(400, "application/json", "{\"error\":\"Invalid active value (must be 'true' or 'false')\"}");
          return;
      }

      if (periodParsed != 1 && periodParsed != 7 && periodParsed != 30) {
          server.send(400, "application/json", "{\"error\":\"Invalid period (must be 1, 7, or 30 days)\"}");
          return;
      }

      uint8_t ctrl = static_cast<uint8_t>(ctrlParsed);
      bool active = (activeStr == "true");
      uint8_t period = static_cast<uint8_t>(periodParsed);


      if (WSER_ExtCmdSetReporter(ctrl, active, period)) {
          server.send(200, "application/json", "{\"status\":\"ok\",\"message\":\"Reporter updated\"}");
      } else {
          server.send(500, "application/json", "{\"error\":\"Set reporter failed\"}");
      }
  }

  else if (cmd == "getreporterstatus") {

      if (!server.hasArg("ctrl")) {
          server.send(400, "application/json", "{\"error\":\"Missing control parameter\"}");
          return;
      }
      int ctrlIndex = server.arg("ctrl").toInt();
      if ((ctrlIndex == 0) || (ctrlIndex > 4)) {
        server.send(400, "text/plain", "❌ Invalid Reporter index (only 1 - 4 supported)");
        return;
      }

      String json = WSER_ExtCmdReporterGetStatus(ctrlIndex);
      server.send(200, "application/json", json);
  }

  //---------------- unknown command -------------------

  else {
    server.send(400, "text/plain", "❌ Unknown command");
  }
}
