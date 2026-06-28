#include "01_AppControls.h"

#include "10_WebServerMain.h"
#include "11_MonitorPage_private.h"

//--- Private Variables -------------------------------------------------------

unsigned long lastCheckTimeMonitor = 0;
#define REMOTE_MONITOR_INTERVAL 500
uint8_t ui8IndexMonitor = 0;

//--- Private Variables -------------------------------------------------------

/**

 */

/**

 */

//--- Public Variables --------------------------------------------------------

//--- Private Function Declarations ------------------------------------------

//--- Public Function Declarations -------------------------------------------
void WSER_InitMonitorSection();

void WSER_MonitorLoop();

void WSER_handleMonitorGetStatus();

//internal fucntions
void WSER_WiFiMonitorPage();
String formatSensorValue(uint8_t type, int16_t rawVal);
//-----------------------------------------------------------------------------
//--- PUBLIC Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void WSER_InitMonitorSection(){

}

/**

 */
void WSER_MonitorLoop(){

  if (millis() - lastCheckTimeMonitor > REMOTE_MONITOR_INTERVAL) {
    lastCheckTimeMonitor = millis();

  }

}

//-----------------------------------------------------------------------------
//--- PRIVATE Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void WSER_WiFiMonitorPage() {
  Serial.print("MONITOR: Heap available before send page: ");
  Serial.println(ESP.getFreeHeap());

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send_P(200, "text/html", WSER_MONITOR_HEAD);

   // 🔧 CSS con colori dinamici
  server.sendContent_P(WSER_COMMON_PAGES_STYLE);      
  String styleBlock = FPSTR(WSER_MONITOR_STYLE);

  String cardColor1 = "#007BFF"; 
  String cardColor2 = "#dc3545";
  String cardColor3 = "#FFA500"; 
  String cardColor4 = "#28a745";

  String typesens1 = "💧 PH";
  String typesens2 = "🌡️ Temp";
  String typesens3 = "⚡ EC";
  String typesens4 = "⚗️ ORP";

  if (s_Ctrl[0].type == _PH) {
    typesens1 = "💧 PH";
    cardColor1 = "#007BFF";
  } else if (s_Ctrl[0].type == _RX) {
    typesens1 = "⚗️ ORP";
    cardColor1 = "#28a745";
  } 

  typesens2 = "🌡️ Temp";
  cardColor2 = "#dc3545"; 

  if (s_Ctrl[2].type == _EC) {
    typesens3 = "⚡ EC";
    cardColor3 = "#FFA500";
  } else if (s_Ctrl[2].type == _TDS) {
    typesens3 = "💎 TDS";
    cardColor3 = "#A0522D";
  } else if (s_Ctrl[2].type == _SAL) {
    typesens3 = "🧂 SAL";
    cardColor3 = "#6C757D";
  } else if (s_Ctrl[2].type == _NTC) {
    typesens3 = "🌡️ Temp";
    cardColor3 = "#dc3545";
  } else if (s_Ctrl[2].type == _DIFF) {
    typesens3 = "🌡️ Diff Temp";
    cardColor3 = "#dc3545";
  } else if (s_Ctrl[2].type == _AVG) {
    typesens3 = "🌡️ Avg Temp";
    cardColor3 = "#dc3545";
  } 

  if (s_Ctrl[3].type == _PH) {
    typesens4 = "💧 PH";
    cardColor4 = "#007BFF";
  } else if (s_Ctrl[3].type == _RX) {
    typesens4 = "⚗️ ORP";
    cardColor4 = "#28a745";
  }

  styleBlock.replace("{CARD_TYPE_COLOR_1}", cardColor1);
  styleBlock.replace("{CARD_TYPE_COLOR_2}", cardColor2);
  styleBlock.replace("{CARD_TYPE_COLOR_3}", cardColor3);
  styleBlock.replace("{CARD_TYPE_COLOR_4}", cardColor4);
  server.sendContent(styleBlock);

  server.sendContent_P(WSER_MONITOR_HEAD_OPEN);
  server.sendContent(WSER_GenerateNavBar("monitor"));
  server.sendContent_P(WSER_MONITOR_BODY_OPEN);

  String temp = FPSTR(WSER_MONITOR_BODY_CONTENT);

  temp.replace("{SENSOR1_LABEL}",     typesens1);
  temp.replace("{CARD_TYPE_COLOR_1}", cardColor1);
  temp.replace("{SENSOR2_LABEL}",     typesens2);
  temp.replace("{CARD_TYPE_COLOR_2}", cardColor2);
  temp.replace("{SENSOR3_LABEL}",     typesens3);
  temp.replace("{CARD_TYPE_COLOR_3}", cardColor3);
  temp.replace("{SENSOR4_LABEL}",     typesens4);
  temp.replace("{CARD_TYPE_COLOR_4}", cardColor4);

  server.sendContent(temp);

  server.sendContent_P(WSER_MONITOR_SCRIPT);
  server.sendContent_P(WSER_MONITOR_CLOSE_PAGE);

  Serial.print("MONITOR: Heap available after send page: ");
  Serial.println(ESP.getFreeHeap());
}

/**

 */
void WSER_handleMonitorGetStatus() {

  String stringPumpTimer1;
  String stringPumpTimer2;
  String stringPumpTimer3;
  String stringPumpTimer4;


  if (!PumpTimers[0].Active) {
    stringPumpTimer1 = "not active";
  } else {
    stringPumpTimer1 = PumpTimers[0].OnPhase ? "✅ on" : "⚪ off";
  }
  if (!PumpTimers[1].Active) {
    stringPumpTimer2 = "not active";
  } else {
    stringPumpTimer2 = PumpTimers[1].OnPhase ? "✅ on" : "⚪ off";
  }
  if (!PumpTimers[2].Active) {
    stringPumpTimer3 = "not active";
  } else {
    stringPumpTimer3 = PumpTimers[2].OnPhase ? "✅ on" : "⚪ off";
  }
  if (!PumpTimers[3].Active) {
    stringPumpTimer4 = "not active";
  } else {
    stringPumpTimer4 = PumpTimers[3].OnPhase ? "✅ on" : "⚪ off";
  }

  String jsonResponse = "{";

  jsonResponse += "\"MainValue1\":\""   + formatSensorValue(s_Ctrl[0].type, s_Ctrl[0].ActReadVal) + "\",";
  jsonResponse += "\"Actuator1\":\""    + String(s_Ctrl[0].CtrlSt  ? "⚡ ON" : "🔴 OFF") + "\",";
  jsonResponse += "\"Logic1\":\""       + String(s_Ctrl[0].Set_PosLogik ? "POS" : "NEG") + "\",";
  jsonResponse += "\"Hysteresis1\":\""  + formatSensorValue(s_Ctrl[0].type, s_Ctrl[0].Set_Hyster) + "\",";
  jsonResponse += "\"Target1\":\""      + formatSensorValue(s_Ctrl[0].type, s_Ctrl[0].Set_Point) + "\",";
  jsonResponse += "\"PlugSSID1\":\""    + stringPumpTimer1 + "\",";

  jsonResponse += "\"MainValue2\":\""   + formatSensorValue(s_Ctrl[1].type, s_Ctrl[1].ActReadVal) + "\",";
  jsonResponse += "\"Actuator2\":\""    + String(s_Ctrl[1].CtrlSt  ? "⚡ ON" : "🔴 OFF") + "\",";
  jsonResponse += "\"Logic2\":\""       + String(s_Ctrl[1].Set_PosLogik ? "POS" : "NEG") + "\",";
  jsonResponse += "\"Hysteresis2\":\""  + formatSensorValue(s_Ctrl[1].type, s_Ctrl[1].Set_Hyster) + "\",";
  jsonResponse += "\"Target2\":\""      + formatSensorValue(s_Ctrl[1].type, s_Ctrl[1].Set_Point) + "\",";
  jsonResponse += "\"PlugSSID2\":\""    + stringPumpTimer2 + "\",";

  jsonResponse += "\"MainValue3\":\""   + formatSensorValue(s_Ctrl[2].type, s_Ctrl[2].ActReadVal) + "\",";
  jsonResponse += "\"Actuator3\":\""    + String(s_Ctrl[2].CtrlSt  ? "⚡ ON" : "🔴 OFF") + "\",";
  jsonResponse += "\"Logic3\":\""       + String(s_Ctrl[2].Set_PosLogik ? "POS" : "NEG") + "\",";
  jsonResponse += "\"Hysteresis3\":\""  + formatSensorValue(s_Ctrl[2].type, s_Ctrl[2].Set_Hyster) + "\",";
  jsonResponse += "\"Target3\":\""      + formatSensorValue(s_Ctrl[2].type, s_Ctrl[2].Set_Point) + "\",";
  jsonResponse += "\"PlugSSID3\":\""    + stringPumpTimer3 + "\",";

  jsonResponse += "\"MainValue4\":\""   + formatSensorValue(s_Ctrl[3].type, s_Ctrl[3].ActReadVal) + "\",";
  jsonResponse += "\"Actuator4\":\""    + String(s_Ctrl[3].CtrlSt  ? "⚡ ON" : "🔴 OFF") + "\",";
  jsonResponse += "\"Logic4\":\""       + String(s_Ctrl[3].Set_PosLogik ? "POS" : "NEG") + "\",";
  jsonResponse += "\"Hysteresis4\":\""  + formatSensorValue(s_Ctrl[3].type, s_Ctrl[3].Set_Hyster) + "\",";
  jsonResponse += "\"Target4\":\""      + formatSensorValue(s_Ctrl[3].type, s_Ctrl[3].Set_Point) + "\",";
  jsonResponse += "\"PlugSSID4\":\""    + stringPumpTimer4 + "\"";

  jsonResponse += "}";

  server.send(200, "application/json", jsonResponse);
}

/**

 */
String formatSensorValue(uint8_t type, int16_t rawVal) {
  if (type == _PH) {
    return String(rawVal / 100.0, 2); 
  } else if (type == _RX) {
    return String(rawVal) + " mV";           
  } else if ((type == _NTC) || (type == _DS18) || (type == _DIFF)|| (type == _AVG)) {
    // Temperatura in decimi °C
    return String(rawVal / 10.0, 1) + " °C"; 
  }  else if (type == _EC) {
    // Conducibilità in µS/cm
    return String(rawVal) + " µS/cm"; 
  } else if (type == _TDS) {
    // TDS in ppm
    return String(rawVal) + " ppm";  
  } else if (type == _SAL) {
    
    return String(rawVal ) + " ppm"; 
  } else {
    return String(rawVal); 
  }
}