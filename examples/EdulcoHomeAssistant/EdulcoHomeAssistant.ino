/*============================================================================
  EdulcoWaterSmart - EdulcoHomeAssistant example
  ----------------------------------------------------------------------------

  Author:
  - Created by Fabiano DAquilio

  Library type:
  - Arduino library for EdulcoWaterSmart devices
  - Water sensor handling (PH/ORP, EC/TDS/SAL, Temperature)
  - Basic control and calibration command handling

  Purpose of this file:
  - This sketch is a basic demo/example
  - It shows how to use the main library features
  - It is not the final production firmware

  Function categories shown:
  1) Sensor readings
    - getPhOrpType_Channel1/2
    - getPh, getOrp
    - getEcTdsSalType, getEcTdsSal
    - getTemperatureDS18, getTemperatureNTC

  2) Calibration readings
    - getCalPointPhOrp_1_Y1 / getCalPointPhOrp_1_Y2
    - getCalPointPhOrp_2_Y1 / getCalPointPhOrp_2_Y2
    - getCalPointEcTdsSal_Y1 / getCalPointEcTdsSal_Y2

  3) Serial configuration commands
    - Change sensor type (TYPE...)
    - Set calibration point (CAL...)
    - Reset calibration point (RCAL...)
    - Relay commands (REL...)

  Note:
  - This example is intended for quick lab testing.
  - Final device comparison will be done on final hardware.
  ===========================================================================*/
#include <EdulcoWaterSmart.h>
#include <WiFi.h>
#include <WebServer.h>

/*
  Example overview:
  - Initializes EdulcoWaterSmart sensors.
  - Connects ESP32 to Wi-Fi in STA mode.
  - Starts an HTTP server with POST endpoint /smartcommand.
  - Exposes simple remote commands for:
      1) Device info (getdeviceabout)
      2) Single-channel reading (getcontrolstatus&ctrl=0..4)
      3) Relay control (setrelay&relay=1..3&status=true|false)
  - Prints live sensor values to Serial for quick local monitoring.
*/


// Home Assistant server
#define SSID_AP_DEFAULT 	"EdulcoAP_" //default AP name
static const char* WIFI_SSID = "FRITZ!Box 7530 YL";
static const char* WIFI_PASSWORD = "91887805532921207262";



// object for the EdulcoWaterSmart library
EdulcoWaterSmart myEdulco;


static WebServer server(80);
static EdulcoWaterSmart* g_device = nullptr;
static bool g_serverStarted = false;
static String g_wifiPassword = "";

bool WSER_Init(EdulcoWaterSmart* device, const char* ssid, const char* password);
void WSER_Handle();
extern void WSER_handleSmartCommand();
String WSER_ExecuteCommand(const String& cmd);
String WSER_ExtCmdGetChanStatus(uint8_t ctrlIndex);


/*

*/
void setup() {
  Serial.begin(115200);
  delay(1000);
  myEdulco.begin();

  const bool webReady = WSER_Init(&myEdulco, WIFI_SSID, WIFI_PASSWORD);
  Serial.println(webReady ? "HASS web server started" : "HASS web server failed");

}


/*

*/
void loop() {

  WSER_Handle();

  delay(200);
  Serial.println("\n\n---------CHANNEL 1 PH/ORP------------");

  if (_PH == myEdulco.getPhOrpType_Channel1()) {
    Serial.print("TYPE = PH💧; Value: ");
    Serial.println(myEdulco.getPh(1), 2);

  } else if (_RX == myEdulco.getPhOrpType_Channel1()) {
    Serial.print("TYPE = RX⚗️; Value: ");
    Serial.print(myEdulco.getOrp(1), 0);
    Serial.println(" mV");

  } 

  delay(200);
  Serial.println("\n---------CHANNEL 2 PH/ORP------------");

  if (_PH == myEdulco.getPhOrpType_Channel2()) {
    Serial.print("TYPE = PH💧; Value: ");
    Serial.println(myEdulco.getPh(2), 2);

  } else if (_RX == myEdulco.getPhOrpType_Channel2()) {
    Serial.print("TYPE = RX⚗️; Value: ");
    Serial.print(myEdulco.getOrp(2), 0);
    Serial.println(" mV");

  } 

  delay(200);
  Serial.println("\n---------CHANNEL EC/TDS/SAL)------------");

  if (_EC == myEdulco.getEcTdsSalType()) {
    Serial.print("TYPE = EC⚡; Value: ");
    Serial.print(myEdulco.getEcTdsSal(), 0);
    Serial.println(" uS/cm");

  } else if (_TDS == myEdulco.getEcTdsSalType()) {
    Serial.print("TYPE = TDS💎; Value: ");
    Serial.print(myEdulco.getEcTdsSal(), 0);
    Serial.println(" ppm");

  } else if (_SAL == myEdulco.getEcTdsSalType()) {
    Serial.print("TYPE = SAL🧂; Value: ");
    Serial.print(myEdulco.getEcTdsSal(), 0);
    Serial.println(" ppm");

  } 

  delay(200);
  Serial.println("\n---------CHANNEL DIGITAL TEMP (DS18)------------");

    Serial.print("TYPE = DIGITAL TEMP 🌡️; Value:"); 
    Serial.print(myEdulco.getTemperatureDS18(), 1);
    Serial.println(" °C");

  delay(200);
  Serial.println("\n---------CHANNEL ANALOG TEMP (NTC)------------");

    Serial.print("TYPE = ANALOG TEMP 🌡️; Value: "); 
    Serial.print(myEdulco.getTemperatureNTC(), 1);
    Serial.println(" °C");

  Serial.println("\n--------- END CYCLE --------------------------");

}


/**

 */
bool WSER_Init(EdulcoWaterSmart* device, const char* ssid, const char* password)
{
    if (device == nullptr || ssid == nullptr || password == nullptr) {
        return false;
    }

    g_device = device;
    g_wifiPassword = String(password);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    const unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - start) < 20000UL) {
        delay(250);
    }

    if (WiFi.status() != WL_CONNECTED) {
        return false;
    }
    server.on("/smartcommand", HTTP_POST, WSER_handleSmartCommand);
    server.begin();
    g_serverStarted = true;

    Serial.print("HASS WebServer ready on http://");
    Serial.println(WiFi.localIP());
    return true;
}



/**

 */
void WSER_Handle()
{
    if (!g_serverStarted) {
        return;
    }
    server.handleClient();
}


//-----------------------------------------------------------------------------
//--- Private Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/*
  Mini guide: how to add a new smart command

  1) Choose the command name
    - Example: cmd=mynewcommand

  2) Add a new branch in WSER_handleSmartCommand()
    - Use else if (cmd == "mynewcommand")

  3) Validate required arguments
    - Check with server.hasArg("argName")
    - Return HTTP 400 if missing/invalid

  4) Execute the action
    - Call existing library APIs via g_device
    - Keep logic simple in this first-release example

  5) Send response
    - Success: server.send(200, ...)
    - Error:   server.send(400, ...)

  Quick template:

   else if (cmd == "mynewcommand") {
    if (!server.hasArg("arg1")) {
      server.send(400, "text/plain", "Missing arg1");
      return;
    }

    const String arg1 = server.arg("arg1");
    // Do action here...

    server.send(200, "text/plain", "OK");
   }
*/
/**
 * @brief Handles remote commands via HTTP POST on /smartcommand.
 * 
 * This function handles all GET/SET commands for the EdulcoWater modular system
 * through a single HTTP POST interface. Commands are identified by the `cmd`
 * argument, with optional extra parameters. Responses are returned as plain text
 * or JSON.
 * 
 * === GET COMMANDS ===
 * 
 * 1. Get device info (About)
 *    $ curl -X POST http://192.168.178.132/smartcommand -d "cmd=getdeviceabout"
 *
 * 2. Get one control channel status
 *  $ curl -X POST http://192.168.178.132/smartcommand -d "cmd=getcontrolstatus&ctrl=0"
 
 * 
 * === SET COMMANDS ===
 * 3. Set relay output
 *    $ curl -X POST http://192.168.178.132/smartcommand -d "cmd=setrelay&relay=1&status=true"
 * 
 * @note Replace <IP> with the module IP address (for example 192.168.4.1 or 192.168.1.132).
 * 
 * @see All commands return HTTP 200 with a message or JSON on success, and HTTP 400 on invalid requests.
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

    if (ctrlIndex < 0 || ctrlIndex > 4) {
      server.send(400, "text/plain", "❌ Invalid control index (0 - 4 supported)");
      return;
    }

    String json = WSER_ExtCmdGetChanStatus(ctrlIndex);
    server.send(200, "application/json", json);
  }

  else if (cmd == "setrelay") {
    if (!server.hasArg("relay") || !server.hasArg("status")) {
      server.send(400, "text/plain", "❌ Missing 'relay' or 'status' argument");
      return;
    }

    const uint8_t relay = (uint8_t)server.arg("relay").toInt();
    String statusStr = server.arg("status");
    statusStr.toLowerCase();

    if (relay < 1 || relay > 3) {
      server.send(400, "text/plain", "❌ Invalid relay index (1 - 3 supported)");
      return;
    }

    if (!(statusStr == "true" || statusStr == "false" || statusStr == "1" || statusStr == "0")) {
      server.send(400, "text/plain", "❌ Invalid status value (true/false)");
      return;
    }

    const bool state = (statusStr == "true" || statusStr == "1");
    if (g_device->setRelay(relay, state)) {
      server.send(200, "text/plain", "✅ Relay updated");
    } else {
      server.send(400, "text/plain", "❌ Failed to set relay");
    }
  }

  //---------------- unknown command -------------------

  else {
    server.send(400, "text/plain", "❌ Unknown command");
  }
}

/**

 */
String WSER_ExtCmdGetDeviceAbout() {
  String connectedSsid = WiFi.SSID();
  if (connectedSsid.length() == 0) {
    connectedSsid = "N/A";
  }

  String json = "{";
  json += "\"deviceType\":\"" DEVICE_TYPE_NAME "\",";     // es. "EdulcoWater-Premium"
  json += "\"hwVersion\":\"" HW_VERSION_STRING "\",";     // es. "01.00"
  json += "\"swVersion\":\"" SW_VERSION_STRING "\",";     // es. "01.00"
  json += "\"ssid\":\"" + connectedSsid + "\",";
  json += "\"password\":\"" + g_wifiPassword + "\"";
  json += "}";
  return json;
}


/**

 */
String WSER_ExtCmdGetChanStatus(uint8_t ctrlIndex) {

  String type = "unknown";
  float value = 0.0f;

  if (ctrlIndex == 0) {
    const e_SensType t = g_device->getPhOrpType_Channel1();
    type = WSER_SensTypeToString(t);
    value = (t == _PH) ? g_device->getPh(1) : g_device->getOrp(1);

  } else if (ctrlIndex == 1) {
    const e_SensType t = g_device->getPhOrpType_Channel2();
    type = WSER_SensTypeToString(t);
    value = (t == _PH) ? g_device->getPh(2) : g_device->getOrp(2);

  } else if (ctrlIndex == 2) {
    const e_SensType t = g_device->getEcTdsSalType();
    type = WSER_SensTypeToString(t);
    value = g_device->getEcTdsSal();

  } else if (ctrlIndex == 3) {
    type = "ds18";
    value = g_device->getTemperatureDS18();

  } else if (ctrlIndex == 4) {
    type = "ntc";
    value = g_device->getTemperatureNTC();
    
  }

  String json = "{";
  json += "\"type\":\"" + type + "\",";
  json += "\"value\":" + String(value, 2);
  json += "}";
  return json;
}

static String WSER_SensTypeToString(e_SensType t) {
  if (t == _PH) return "ph";
  if (t == _RX) return "rx";
  if (t == _EC) return "ec";
  if (t == _TDS) return "tds";
  if (t == _SAL) return "sal";
  return "unknown";
}
