#include "01_AppControls.h"

#include "10_WebServerMain.h"
#include "12_SettingsPage_private.h"

//--- Private Variables -------------------------------------------------------
unsigned long lastCheckTimeSettings;

String messStatus1Settings;
String messStatus2Settings;
String messStatus3Settings;
String messStatus4Settings;

//--- Private Variables -------------------------------------------------------

//--- Public Variables --------------------------------------------------------


//--- Private Function Declarations ------------------------------------------

//--- Public Function Declarations -------------------------------------------
void WSER_InitSettingsSection();
void WSER_SettingsLoop();

//web page fucntions

//get fucntions
void WSER_handleSettingControlLogik();
void WSER_handleSettingControlHyster();
void WSER_handleSettingControlSetPoint();

//internal fucntions
//get fucntions
void WSER_handleSettingsGetStatus();

//internal fucntions
void WSER_WiFiSettingsgPage ();
String getStatusMessage(uint8_t index);
bool WSER_ExtCmdSetHysteresis(uint8_t ctrlIndex, int hysteresis);
bool WSER_ExtCmdSetLogic(uint8_t controlIndex, bool positiveLogic);
bool WSER_ExtCmdSetSetpoint(uint8_t controlIndex, int setpoint);
//-----------------------------------------------------------------------------
//--- PUBLIC Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void WSER_InitSettingsSection(){

  lastCheckTimeSettings = 0;
  
  messStatus1Settings = getStatusMessage(0);
  messStatus2Settings = getStatusMessage(1);
  messStatus3Settings = getStatusMessage(2);
  messStatus4Settings = getStatusMessage(3);

}

//-----------------------------------------------------------------------------
//--- PUBLIC Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void WSER_SettingsLoop(){
  
  unsigned long now = millis();

  if (now - lastCheckTimeSettings >= REMOTE_Settings_INTERVAL) { // every second
    lastCheckTimeSettings = now;

  }

}

//-----------------------------------------------------------------------------
//--- PRIVATE Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
String getStatusMessage(uint8_t index) {

  String target;
  String hysteresis;

  if (s_Ctrl[index].type == _EC) {
    // Conductivity in µS/cm
    target     = String(s_Ctrl[index].Set_Point)   + " µS/cm";
    hysteresis = String(s_Ctrl[index].Set_Hyster)  + " µS/cm";

  } else if (s_Ctrl[index].type == _TDS) {
    // Total Dissolved Solids in ppm
    target     = String(s_Ctrl[index].Set_Point)   + " ppm";
    hysteresis = String(s_Ctrl[index].Set_Hyster)  + " ppm";

  } else if (s_Ctrl[index].type == _SAL) {
    target     = String(s_Ctrl[index].Set_Point )   + " ppm";
    hysteresis = String(s_Ctrl[index].Set_Hyster )  + " ppm";

  } else if ((s_Ctrl[index].type == _NTC)||(s_Ctrl[index].type == _DS18)||(s_Ctrl[index].type == _DIFF)||(s_Ctrl[index].type == _AVG)) {
    target     = String((float)s_Ctrl[index].Set_Point / 10.0, 1)   + " °C";
    hysteresis = String((float)s_Ctrl[index].Set_Hyster / 10.0, 1)  + " °C";

  } else if (s_Ctrl[index].type == _PH) {
    target     = String((float)s_Ctrl[index].Set_Point / 100.0, 2);
    hysteresis = String((float)s_Ctrl[index].Set_Hyster / 100.0, 2);

  } else if (s_Ctrl[index].type == _RX) {
    target     = String(s_Ctrl[index].Set_Point) + " mV";
    hysteresis = String(s_Ctrl[index].Set_Hyster) + " mV";

  } else {
    // fallback generico
    target     = String(s_Ctrl[index].Set_Point);
    hysteresis = String(s_Ctrl[index].Set_Hyster);
  }

  String logic = (s_Ctrl[index].Set_PosLogik == false) ? "➖" : "➕";

  return "   🧠 Logik: " + logic +
         " | 🔁 Hysteresys: " + hysteresis +
         " | 🎯 Set Point: " + target;

}

/**

*/
void WSER_WiFiSettingsgPage () {

  Serial.print("SETTINGS: Heap disponibile prima della pagina: ");
  Serial.println(ESP.getFreeHeap());

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send_P(200, "text/html", WSER_SETTINGS_HEAD);

  server.sendContent_P(WSER_COMMON_PAGES_STYLE);
  server.sendContent_P(WSER_SETTINGS_STYLE);
  server.sendContent_P(WSER_SETTINGS_HEAD_OPEN);

  String temp = WSER_GenerateNavBar("settings");
  server.sendContent(temp);

  server.sendContent_P(WSER_SETTINGS_BODY_OPEN);

  temp = FPSTR(WSER_SETTINGS_SENSOR1_BODY_CONTENT);

  if (s_Ctrl[0].type == _PH) {
    temp.replace("{CTRL_TYPE_1}", "💧 PH");
    temp.replace("{COLOR_CTRL_1}", "#00BFFF");
    temp.replace("{HYST_MIN_1}", "1");
    temp.replace("{HYST_MAX_1}", "100");
    temp.replace("{HYST_LABEL_1}", "0.5");
    temp.replace("{HYST_VALUE_1}", "50");
    temp.replace("{SET_MIN_1}", "1");
    temp.replace("{SET_MAX_1}", "1400");
    temp.replace("{SET_LABEL_1}", "7.00");
    temp.replace("{SET_VALUE_1}", "700");

  } else if (s_Ctrl[0].type == _RX) {
    temp.replace("{CTRL_TYPE_1}", "⚗️ ORP");
    temp.replace("{COLOR_CTRL_1}", "#28a745");
    temp.replace("{HYST_LABEL_1}", "50 mV");
    temp.replace("{HYST_MIN_1}", "1");
    temp.replace("{HYST_MAX_1}", "100");
    temp.replace("{HYST_VALUE_1}", "50");
    temp.replace("{SET_LABEL_1}", "0 mV");
    temp.replace("{SET_MIN_1}", "-2000");
    temp.replace("{SET_MAX_1}", "2000");
    temp.replace("{SET_VALUE_1}", "0");

  }

  temp.replace("{COLOR_CTRL_2}", "#dc3545");   // marrone
  temp.replace("{HYST_MIN_2}", "1");       // 0.1 °C
  temp.replace("{HYST_MAX_2", "100");      // 5.0 °C
  temp.replace("{HYST_VALUE_2}", "50");     // 0.5 °C
  temp.replace("{HYST_LABEL_2}", "5 °C");
  temp.replace("{SET_MIN_2}", "50");     // -40.0 °C
  temp.replace("{SET_MAX_2}", "700");     // 125.0 °C
  temp.replace("{SET_VALUE_2}", "250");    // 25.0 °C
  temp.replace("{SET_LABEL_2}", "25.0 °C");

  if (s_Ctrl[2].type == _EC) {
    temp.replace("{COLOR_CTRL_3}", "#FFA500");   
    temp.replace("{HYST_MIN_3}", "1");
    temp.replace("{HYST_MAX_3}", "100");
    temp.replace("{HYST_VALUE_3}", "50");
    temp.replace("{HYST_LABEL_3}", "50 µS/cm");
    temp.replace("{SET_MIN_3}", "10");
    temp.replace("{SET_MAX_3}", "20000");
    temp.replace("{SET_VALUE_3}", "1000");
    temp.replace("{SET_LABEL_3}", "1000 µS/cm");

  } else if (s_Ctrl[2].type == _SAL) {
    temp.replace("{COLOR_CTRL_3}", "#6C757D");   
    temp.replace("{HYST_MIN_3}", "1");
    temp.replace("{HYST_MAX_3}", "100");
    temp.replace("{HYST_VALUE_3}", "10");
    temp.replace("{HYST_LABEL_3}", "10 ppm");
    temp.replace("{SET_MIN_3}", "6");
    temp.replace("{SET_MAX_3}", "11000");
    temp.replace("{SET_VALUE_3}", "35");          
    temp.replace("{SET_LABEL_3}", "35 ppm");

  } else if (s_Ctrl[2].type == _TDS) {
    temp.replace("{COLOR_CTRL_3}", "#A0522D"); 
    temp.replace("{HYST_MIN_3}", "1");
    temp.replace("{HYST_MAX_3}", "100");
    temp.replace("{HYST_VALUE_3}", "50");
    temp.replace("{HYST_LABEL_3}", "50 ppm");
    temp.replace("{SET_MIN_3}", "6");
    temp.replace("{SET_MAX_3}", "12800");
    temp.replace("{SET_VALUE_3}", "500");
    temp.replace("{SET_LABEL_3}", "500 ppm");

  } else if ((s_Ctrl[2].type == _NTC) ||(s_Ctrl[2].type == _DIFF) ||(s_Ctrl[2].type == _AVG) ){
    temp.replace("{COLOR_CTRL_3}", "#dc3545");
    temp.replace("{HYST_MIN_3}", "1");
    temp.replace("{HYST_MAX_3}", "100");
    temp.replace("{HYST_VALUE_3}", "50");
    temp.replace("{HYST_LABEL_3}", "5 °C");
    temp.replace("{SET_MIN_3}", "50");
    temp.replace("{SET_MAX_3}", "700");
    temp.replace("{SET_VALUE_3}", "250");
    temp.replace("{SET_LABEL_3}", "25.0 °C");

  } 

  if (s_Ctrl[3].type == _PH) {
    temp.replace("{CTRL_TYPE_4}", "💧 PH");
    temp.replace("{COLOR_CTRL_4}", "#00BFFF");
    temp.replace("{HYST_MIN_4}", "1");
    temp.replace("{HYST_MAX_4}", "100");
    temp.replace("{HYST_LABEL_4}", "0.5");
    temp.replace("{HYST_VALUE_4}", "50");
    temp.replace("{SET_MIN_4}", "1");
    temp.replace("{SET_MAX_4}", "1400");
    temp.replace("{SET_LABEL_4}", "7.00");
    temp.replace("{SET_VALUE_4}", "700");

  } else if (s_Ctrl[3].type == _RX) {
    temp.replace("{CTRL_TYPE_4}", "⚗️ ORP");
    temp.replace("{COLOR_CTRL_4}", "#28a745");
    temp.replace("{HYST_LABEL_4}", "50 mV");
    temp.replace("{HYST_MIN_4}", "1");
    temp.replace("{HYST_MAX_4}", "100");
    temp.replace("{HYST_VALUE_4}", "50");
    temp.replace("{SET_LABEL_4}", "0 mV");
    temp.replace("{SET_MIN_4}", "-2000");
    temp.replace("{SET_MAX_4}", "2000");
    temp.replace("{SET_VALUE_4}", "0");

  }

  server.sendContent(temp);

  temp = FPSTR(WSER_SETTINGS_TYPE_DEF_CONTENT);

  temp.replace("{SENSOR1_SETTING_TYPE}", s_Ctrl[0].type == _PH ? "ph" : "orp");

  temp.replace("{SENSOR2_SETTING_TYPE}", "ds18"); //temp.replace("{SENSOR2_SETTING_TYPE}", "ntc");

  if      (s_Ctrl[2].type == _EC)  temp.replace("{SENSOR3_SETTING_TYPE}", "ec");
  else if (s_Ctrl[2].type == _SAL) temp.replace("{SENSOR3_SETTING_TYPE}", "sal");
  else if (s_Ctrl[2].type == _TDS) temp.replace("{SENSOR3_SETTING_TYPE}", "tds");
  else if (s_Ctrl[2].type == _NTC) temp.replace("{SENSOR3_SETTING_TYPE}", "ntc");
  else if (s_Ctrl[2].type == _DIFF) temp.replace("{SENSOR3_SETTING_TYPE}", "diff");
  else if (s_Ctrl[2].type == _AVG) temp.replace("{SENSOR3_SETTING_TYPE}", "avg");

  temp.replace("{SENSOR4_SETTING_TYPE}", s_Ctrl[3].type == _PH ? "ph" : "orp");

  server.sendContent(temp);

  server.sendContent_P(WSER_SETTINGS_JAVA_FUNTIONS_SCRIPT);

  server.sendContent_P(WSER_SETTINGS_CLOSE_PAGE);

  Serial.print("SETTINGS: Heap disponibile dopo l'invio pagina: ");
  Serial.println(ESP.getFreeHeap());
}

//-----SMART external and Route functions

/**
transfer data to the web page
 */
void WSER_handleSettingsGetStatus() {

  String jsonResponse = "{";
  jsonResponse += "\"MessSettingsStatusSens1\":\"" + messStatus1Settings        + "\",";
  jsonResponse += "\"MessSettingsStatusSens2\":\"" + messStatus2Settings        + "\",";
  jsonResponse += "\"MessSettingsStatusSens3\":\"" + messStatus3Settings        + "\",";
  jsonResponse += "\"MessSettingsStatusSens4\":\"" + messStatus4Settings        + "\"";

  jsonResponse += "}";

  server.send(200, "application/json", jsonResponse);
}

/**
 route function changing the set Hysteresis
 */
void WSER_handleSettingControlHyster() {

    Serial.print("Entrato in set hyster");

  if (!server.hasArg("control") || !server.hasArg("hyster")) {
    server.send(400, "application/json", "{\"error\":\"Missing parameters\"}");
    return;
  }

  uint8_t controlIndex = server.arg("control").toInt() - 1; // minus 1 because of the array indexes and the control web page
  int hysteresisValue = server.arg("hyster").toInt(); // Valore ricevuto

  Serial.print("Entrato in set isteresi;"); Serial.print(" Control: "); Serial.print(controlIndex); Serial.print(" isteresi: "); Serial.println(hysteresisValue);

  if ( WSER_ExtCmdSetHysteresis(controlIndex , hysteresisValue)){
    server.send(200, "text/plain", "Transferring new Hysteresis Value 🔁");
  } else {

    server.send(400, "text/plain", "Invalid Hysteresis value 🔁");
  }

}

/**
 External function to change the Hystereiss
 */
bool WSER_ExtCmdSetHysteresis(uint8_t ctrlIndex, int hysteresis) {
  if (ctrlIndex > 3) return false;

  if (hysteresis < 0) hysteresis = 0;
  if (hysteresis > 255) hysteresis = 255;

  if (ctrlIndex == 0) {
    EEPR_WriteByte(CTRL_1_HYSTER, (uint8_t)hysteresis);
    s_Ctrl[0].Set_Hyster = EEPR_ReadByte(CTRL_1_HYSTER);
    messStatus1Settings = getStatusMessage(0);
  } else if (ctrlIndex == 1) {
    EEPR_WriteByte(CTRL_2_HYSTER, (uint8_t)hysteresis);
    s_Ctrl[1].Set_Hyster = EEPR_ReadByte(CTRL_2_HYSTER);
    messStatus2Settings = getStatusMessage(1);
  } else if (ctrlIndex == 2) {
    EEPR_WriteByte(CTRL_3_HYSTER, (uint8_t)hysteresis);
    s_Ctrl[2].Set_Hyster = EEPR_ReadByte(CTRL_3_HYSTER);
    messStatus3Settings = getStatusMessage(2);
  } else { // ctrlIndex == 3
    EEPR_WriteByte(CTRL_4_HYSTER, (uint8_t)hysteresis);
    s_Ctrl[3].Set_Hyster = EEPR_ReadByte(CTRL_4_HYSTER);
    messStatus4Settings = getStatusMessage(3);
  }

  return true;
}

/**
 route function changing the set logik
 */
void WSER_handleSettingControlLogik() {
  
  if (!server.hasArg("control") || !server.hasArg("logik")) {
    server.send(400, "application/json", "{\"error\":\"Missing parameters\"}");
    return;
  }

  uint8_t controlIndex = server.arg("control").toInt() - 1; // minus 1 because of the array indexes and the control web page
  bool blogic = (server.arg("logik") == "POS");

  Serial.print("Entrato in set Logik;"); Serial.print(" Control: "); Serial.print(controlIndex); Serial.print(" Logik: "); Serial.println(blogic);

  if ( WSER_ExtCmdSetLogic(controlIndex , blogic)){
    server.send(200, "text/plain", "SetLogik updated successfully 🧠");
  } else {

    server.send(400, "text/plain", "SetLogik NOT updated 🧠");
  }

}

/**
 External function to change the logic
 */
bool WSER_ExtCmdSetLogic(uint8_t controlIndex, bool positiveLogic) {
  if (controlIndex > 3) return false;

  if (controlIndex == 0) {
    EEPR_WriteByte(CTRL_1_LOGIK, positiveLogic ? 1 : 0);
    s_Ctrl[0].Set_PosLogik = EEPR_ReadByte(CTRL_1_LOGIK);
    messStatus1Settings = getStatusMessage(0);
  } else if (controlIndex == 1) {
    EEPR_WriteByte(CTRL_2_LOGIK, positiveLogic ? 1 : 0);
    s_Ctrl[1].Set_PosLogik = EEPR_ReadByte(CTRL_2_LOGIK);
    messStatus2Settings = getStatusMessage(1);
  } else if (controlIndex == 2) {
    EEPR_WriteByte(CTRL_3_LOGIK, positiveLogic ? 1 : 0);
    s_Ctrl[2].Set_PosLogik = EEPR_ReadByte(CTRL_3_LOGIK);
    messStatus3Settings = getStatusMessage(2);
  } else if (controlIndex == 3) {
    EEPR_WriteByte(CTRL_4_LOGIK, positiveLogic ? 1 : 0);
    s_Ctrl[3].Set_PosLogik = EEPR_ReadByte(CTRL_4_LOGIK);
    messStatus4Settings = getStatusMessage(3);
  }

  return true;
}

/**
 route function changing the set point
 */
void WSER_handleSettingControlSetPoint() {

  if (!server.hasArg("control") || !server.hasArg("target")) {
    server.send(400, "application/json", "{\"error\":\"Missing parameters\"}");
    return;
  }

  uint8_t controlIndex = server.arg("control").toInt() - 1; // minus 1 because of the array indexes and the control web page
  int targetValue  = server.arg("target").toInt(); 

  Serial.print("Entrato in set point;"); Serial.print(" Control: "); Serial.print(controlIndex); Serial.print(" targetValue: "); Serial.println(targetValue);

  if ( WSER_ExtCmdSetSetpoint(controlIndex , targetValue)){
    server.send(200, "text/plain", "SetPoint updated successfully 🎯");
  } else {

    server.send(400, "text/plain", "SetPoint NOT updated 🎯");
  }

} 

/**
 External function to change the set point
 */
bool WSER_ExtCmdSetSetpoint(uint8_t controlIndex, int setpoint) {
  if (controlIndex > 3) return false;

  if (controlIndex == 0) {
    EEPR_WriteInt16(CTRL_1_SET_PT, setpoint);
    s_Ctrl[0].Set_Point = EEPR_ReadInt16(CTRL_1_SET_PT);
    messStatus1Settings = getStatusMessage(0);
  } else if (controlIndex == 1){
    EEPR_WriteInt16(CTRL_2_SET_PT, setpoint);
    s_Ctrl[1].Set_Point = EEPR_ReadInt16(CTRL_2_SET_PT);
    messStatus2Settings = getStatusMessage(1);
  } else if (controlIndex == 2){
    EEPR_WriteInt16(CTRL_3_SET_PT, setpoint);
    s_Ctrl[2].Set_Point = EEPR_ReadInt16(CTRL_3_SET_PT);
    messStatus3Settings = getStatusMessage(2);
  } else if (controlIndex == 3){
    EEPR_WriteInt16(CTRL_4_SET_PT, setpoint);
    s_Ctrl[3].Set_Point = EEPR_ReadInt16(CTRL_4_SET_PT);
    messStatus4Settings = getStatusMessage(3);
  }

  return true;
}