#include "02_AppSensors.h"
#include "01_AppControls.h"

#include "10_WebServerMain.h"
#include "15_CalibrationPage_private.h"

//----- public variables
String messSens1ActValues     = "";  
String messSens2ActValues     = ""; 
String messSens3ActValues     = "";  
String messSens4ActValues     = ""; 

String messSens1ActCalPoints = ""; 
String messSens2ActCalPoints = ""; 
String messSens3ActCalPoints = ""; 
String messSens4ActCalPoints = ""; 

unsigned long ulLastUpdateTime = 0;

//--- Public Function Declarations -------------------------------------------
void WSER_InitCalibrationSection();

//web page fucntions

//get fucntions
void WSER_handleCalibrGetStatus();

//post fucntions
void WSER_handleSettingControlType();

void WSER_handleSetCalPoint();
void WSER_handleReSetCalPoint();

//internal fucntions
void WSER_WiFiCalibrationgPage();

void WSER_CalibrationLoop();

//---- SMart hlem functions--------------------------------
bool WSER_CalibrationSmartHomeGetStatus(uint8_t sensorIndex);

bool WSER_ExtCmdSetType(uint8_t ctrlIndex, const String& typeStr);
bool WSER_ExtCmdSetCalPoint(uint8_t sens_numb, uint8_t cal_point, int16_t cal_value);
bool WSER_ExtCmdCalReset(uint8_t sensorIndex, uint8_t calPoint);
//-----------------------------------------------------------------------------
//--- PUBLIC Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void WSER_InitCalibrationSection(){

  messSens1ActValues      = "Loading info sensor 1...";  // es: "PH ; 7.00"
  messSens2ActValues      = "Loading info sensor 2...";  // es: "PH ; 7.00"
  messSens3ActValues      = "Loading info sensor 1...";  // es: "PH ; 7.00"
  messSens4ActValues      = "Loading info sensor 2...";  // es: "PH ; 7.00"

  messSens1ActCalPoints   = "Loading info Calibration Points...";  // es: "PH ; 7.00"
  messSens2ActCalPoints   = "Loading info Calibration Points...";  // es: "PH ; 7.00"
  messSens3ActCalPoints   = "Loading info Calibration Points...";  // es: "PH ; 7.00"
  messSens4ActCalPoints   = "Loading info Calibration Points...";  // es: "PH ; 7.00"

}

/**

 */
void WSER_CalibrationLoop(){

  unsigned long now = millis();

  String sensor1_type_str = "";
  String sensor2_type_str = "";
  String sensor3_type_str = "";
  String sensor4_type_str = "";

  String sensor1_value_str = "";
  String sensor2_value_str = "";
  String sensor3_value_str = "";
  String sensor4_value_str = "";

  if (now - ulLastUpdateTime >= WSER_CAL_TIMEOUT_INTERVAL) {
    ulLastUpdateTime = now;

    //string composition type sensor
    if (s_Ctrl[0].type == _PH) {
      sensor1_type_str = "💧 PH";
    } else if (s_Ctrl[0].type == _RX) {
      sensor1_type_str = "⚗️ ORP";
    } else {
      sensor1_type_str = "N/A";
    }

    sensor2_type_str = "🌡️ TEMP";

    if (s_Ctrl[2].type == _EC) {
      sensor3_type_str = "⚡ EC";
    } else if (s_Ctrl[2].type == _TDS) {
      sensor3_type_str = "💎 TDS";
    } else if (s_Ctrl[2].type == _SAL) {
      sensor3_type_str = "🧂 SAL";
    } else if (s_Ctrl[2].type == _NTC) {
      sensor3_type_str = "🌡️ TEMP";
    } else if (s_Ctrl[2].type == _DIFF) {
      sensor3_type_str = "🌡️↕️ Diff Temp";
    } else if (s_Ctrl[2].type == _AVG) {
      sensor3_type_str = "🌡️∅ Avg Temp";
    } else {
      sensor3_type_str = "N/A";
    }

    if (s_Ctrl[3].type == _PH) {
      sensor4_type_str = "💧 PH";
    } else if (s_Ctrl[3].type == _RX) {
      sensor4_type_str = "⚗️ ORP";
    } else {
      sensor4_type_str = "N/A";
    }

    //value string composition
    if (s_Ctrl[0].type == _PH) {
      sensor1_value_str = String((float)s_Ctrl[0].ActReadVal / 100.0,2);
    } else if (s_Ctrl[0].type == _RX) {
      sensor1_value_str = (String(s_Ctrl[0].ActReadVal) + String(" mV"));
    } else {
      sensor1_value_str = "N/A";
    }

    sensor2_value_str = String((float)s_Ctrl[1].ActReadVal / 10.0, 1) + " °C";
  
    if (s_Ctrl[2].type == _EC) {
      sensor3_value_str = String(s_Ctrl[2].ActReadVal) + " µS/cm";
    } else if (s_Ctrl[2].type == _TDS) {
      sensor3_value_str = String(s_Ctrl[2].ActReadVal) + " ppm";
    } else if (s_Ctrl[2].type == _SAL) {
      sensor3_value_str = String(s_Ctrl[2].ActReadVal ) + " ppm";
    } else if (s_Ctrl[2].type == _NTC) {
      sensor3_value_str = "N/A";
    } else if (s_Ctrl[2].type == _DIFF) {
      sensor3_value_str = "N/A";
    } else if (s_Ctrl[2].type == _AVG) {
      sensor3_value_str = "N/A";
    }
    else {
      sensor3_value_str = "N/A";
    }
   
    if (s_Ctrl[3].type == _PH) {
      sensor4_value_str = String((float)s_Ctrl[3].ActReadVal / 100.0,2);
    } else if (s_Ctrl[3].type == _RX) {
      sensor4_value_str = (String(s_Ctrl[3].ActReadVal) + String(" mV"));
    } else {
      sensor4_value_str = "N/A";
    }

    // String composition
    messSens1ActValues = String("Sensor 1 type ") + sensor1_type_str + " ; Actual Value: " + sensor1_value_str;
    messSens2ActValues = String("Sensor 2 type ") + sensor2_type_str + " ; Actual Value: " + sensor2_value_str;
    messSens3ActValues = String("Sensor 3 type ") + sensor3_type_str + " ; Actual Value: " + sensor3_value_str;
    messSens4ActValues = String("Sensor 4 type ") + sensor4_type_str + " ; Actual Value: " + sensor4_value_str;
    //--------------------------
    // Calibration points string
    //--------------------------
    int16_t Y1;
    int16_t Y2;

    //------Sens 1--------------------
    Y1 = EEPR_ReadInt16(SENS_1_CAL_Y1);
    Y2 = EEPR_ReadInt16(SENS_1_CAL_Y2);

    if (s_Ctrl[0].type == _PH) {
      messSens1ActCalPoints = String("Y1 🔻: ") + String((float)Y1 / 100.0,2) +
                              " | Y2 🔺: " + String((float)Y2 / 100.0,2);
    } else if (s_Ctrl[0].type == _RX) {
      messSens1ActCalPoints = String("Y1 🔻: ") + (String(Y1) + String(" mV")) +
                              " | Y2 🔺: " + (String(Y2) + String(" mV")) ;
    } 

    //------Sens 2--------------------
    // temperature fix 

    //------Sens 3--------------------
    uint16_t Y1_res = EEPR_ReadUint16(SENS_3_CAL_Y1);
    uint16_t Y2_res = EEPR_ReadUint16(SENS_3_CAL_Y2);

    long Y1l = 0;
    long Y2l = 0;

    if (Y1_res < 1) Y1_res = 1;
    if (Y2_res < 1) Y2_res = 1;

    float EC_Y1 = 1000000.0f / (float)Y1_res;
    float EC_Y2 = 1000000.0f / (float)Y2_res;

    if (s_Ctrl[2].type == _EC) {

      Y1l = (long)roundf(EC_Y1);
      Y2l = (long)roundf(EC_Y2);

      messSens3ActCalPoints = String("Y1 (@25°C) 🔻: ") + String(Y1l) + " µS/cm | " +
                              "Y2 (@25°C) 🔺: " + String(Y2l) + " µS/cm";

     } else if (s_Ctrl[2].type == _TDS) {

      Y1l = (long)roundf(EC_Y1 * 0.64f);
      Y2l = (long)roundf(EC_Y2 * 0.64f);

      messSens3ActCalPoints = String("Y1 (@25°C) 🔻: ") + String(Y1l) + " ppm | " +
                              "Y2 (@25°C) 🔺: " + String(Y2l) + " ppm";

    } else if (s_Ctrl[2].type == _SAL) {

      Y1l = (long)roundf(EC_Y1 * 0.55f);
      Y2l = (long)roundf(EC_Y2 * 0.55f);

      messSens3ActCalPoints = String("Y1 (@25°C) 🔻: ") + String(Y1l) + " ppm | " +
                              "Y2 (@25°C) 🔺: " + String(Y2l) + " ppm";
    } else if ((s_Ctrl[2].type == _NTC) || (s_Ctrl[2].type == _DIFF) || (s_Ctrl[2].type == _AVG)){

      messSens3ActCalPoints = String("Y1 🔻: ") + " n/a | " +
                              "Y2 🔺: " + " n/a";
    }

    //------Sens 4--------------------
    Y1 = EEPR_ReadInt16(SENS_4_CAL_Y1);
    Y2 = EEPR_ReadInt16(SENS_4_CAL_Y2);

    if (s_Ctrl[3].type == _PH) {
      messSens4ActCalPoints = String("Y1 🔻: ") + String((float)Y1 / 100.0,2) +
                              " | Y2 🔺: " + String((float)Y2 / 100.0,2);
    } else if (s_Ctrl[3].type == _RX) {
      messSens4ActCalPoints = String("Y1 🔻: ") + (String(Y1) + String(" mV")) +
                              " | Y2 🔺: " + (String(Y2) + String(" mV")) ;
    } 

  }

}

//-----------------------------------------------------------------------------
//--- PRIVATE Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void WSER_WiFiCalibrationgPage() {
  Serial.print("CALIBRATION: Heap available before send page");
  Serial.println(ESP.getFreeHeap());

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send_P(200, "text/html", WSER_HTML_CALIBRATION_HEAD); // <!DOCTYPE html><head>

  // Stili in RAM e PROGMEM
   server.sendContent_P(WSER_COMMON_PAGES_STYLE);          // CSS
  server.sendContent_P(WSER_HTML_CALIBRATION_STYLE);          // CSS
  server.sendContent_P(WSER_HTML_CALIBRATION_HEAD_OPEN);      // </head><body>

  // Navbar dinamica
  String temp = WSER_GenerateNavBar("calibration");
  server.sendContent(temp);

  // Apertura body
  server.sendContent_P(WSER_HTML_CALIBRATION_BODY_OPEN);

  // Corpo HTML sensore 1 (con sostituzioni dinamiche)
  temp = FPSTR(WSER_HTML_SENSOR_BODY_CONTENT);

  // sensor 1
  if (s_Ctrl[0].type == _PH) {
    temp.replace("{S1_MIN}", "1");
    temp.replace("{S1_MAX}", "1400");
    temp.replace("{S1_VALUE}", "700");
    temp.replace("{S1_LOW_LABEL}",  "7.00");
    temp.replace("{S1_HIGH_LABEL}", "7.00");
  } else {
    temp.replace("{S1_MIN}", "-2000");
    temp.replace("{S1_MAX}", "2000");
    temp.replace("{S1_VALUE}", "0");
    temp.replace("{S1_LOW_LABEL}",  "0 mV");
    temp.replace("{S1_HIGH_LABEL}", "0 mV");
  }

  // sensor 2 missing 

  // sensor 3 
  if (s_Ctrl[2].type == _EC) {
    temp.replace("{S3_MIN}", "10");        // EC 0 µS/cm
    temp.replace("{S3_MAX}", "20000");     
    temp.replace("{S3_VALUE}", "1000");   // default 1000 µS/cm
    temp.replace("{S3_LOW_LABEL}",  "1000 µS/cm");
    temp.replace("{S3_HIGH_LABEL}", "1000 µS/cm");

  } else if (s_Ctrl[2].type == _TDS) {
    temp.replace("{S3_MIN}", "6");        // TDS 0 ppm
    temp.replace("{S3_MAX}", "12800");     
    temp.replace("{S3_VALUE}", "500");    // default 500 ppm
    temp.replace("{S3_LOW_LABEL}",  "500 ppm");
    temp.replace("{S3_HIGH_LABEL}", "500 ppm");

  } else if (s_Ctrl[2].type == _SAL) {
    temp.replace("{S3_MIN}", "6");        
    temp.replace("{S3_MAX}", "11000");      
    temp.replace("{S3_VALUE}", "35");     
    temp.replace("{S3_LOW_LABEL}",  "35 ppm");
    temp.replace("{S3_HIGH_LABEL}", "35 ppm");

  } else if ((s_Ctrl[2].type == _NTC) || (s_Ctrl[2].type == _DIFF) || (s_Ctrl[2].type == _AVG) ){
    temp.replace("{S3_MIN}", "250");        
    temp.replace("{S3_MAX}", "250");      
    temp.replace("{S3_VALUE}", "250");     
    temp.replace("{S3_LOW_LABEL}",  "25.0 °C");
    temp.replace("{S3_HIGH_LABEL}", "25.0 °C");
    
  }

  // sensor 4 
  if (s_Ctrl[3].type == _PH) {
    temp.replace("{S4_MIN}", "1");
    temp.replace("{S4_MAX}", "1400");
    temp.replace("{S4_VALUE}", "700");
    temp.replace("{S4_LOW_LABEL}",  "7.00");
    temp.replace("{S4_HIGH_LABEL}", "7.00");
  } else {
    temp.replace("{S4_MIN}", "-2000");
    temp.replace("{S4_MAX}", "2000");
    temp.replace("{S4_VALUE}", "0");
    temp.replace("{S4_LOW_LABEL}",  "0 mV");
    temp.replace("{S4_HIGH_LABEL}", "0 mV");
  }

  server.sendContent(temp);

  // Script dinamico per tipo sensore (ph / orp)
  temp = FPSTR(WSER_SCRIPT_SENSOR_CALIBRATION);

  String sensor1Type = "ph";  // default
  //String sensor2Type = "ntc";  // default
  String sensor2Type = "ds18";  // default
  String sensor3Type = "ec";  // default
  String sensor4Type = "opr";  // default

  if      (s_Ctrl[0].type == _PH)  sensor1Type = "ph";
  else if (s_Ctrl[0].type == _RX)  sensor1Type = "orp";

  //sensor2Type = "ntc";
  sensor2Type = "ds18";

  if      (s_Ctrl[2].type == _EC)  sensor3Type = "ec";
  else if (s_Ctrl[2].type == _TDS) sensor3Type = "tds";
  else if (s_Ctrl[2].type == _SAL) sensor3Type = "sal";
  else if (s_Ctrl[2].type == _NTC) sensor3Type = "ntc";
  else if (s_Ctrl[2].type == _DIFF) sensor3Type = "diff";
  else if (s_Ctrl[2].type == _AVG) sensor3Type = "avg";

  if      (s_Ctrl[3].type == _PH)  sensor4Type = "ph";
  else if (s_Ctrl[3].type == _RX)  sensor4Type = "orp";


  temp.replace("{SENSOR1_TYPE}", sensor1Type);
  temp.replace("{SENSOR2_TYPE}", sensor2Type);
  temp.replace("{SENSOR3_TYPE}", sensor3Type);
  temp.replace("{SENSOR4_TYPE}", sensor4Type);

  server.sendContent(temp);

  server.sendContent_P(WSER_CALIBRATION_CLOSE_PAGE);

  Serial.print("CALIBRATION: Heap available after send page: ");
  Serial.println(ESP.getFreeHeap());
}

/**

 */
void WSER_handleCalibrGetStatus() {

  String jsonResponse = "{";

  jsonResponse += "\"MessStatusSens1Row1\":\"" + messSens1ActValues     + "\",";
  jsonResponse += "\"MessStatusSens1Row2\":\"" + messSens1ActCalPoints  + "\",";

  jsonResponse += "\"MessStatusSens2Row1\":\"" + messSens2ActValues     + "\",";
  jsonResponse += "\"MessStatusSens2Row2\":\"" + messSens2ActCalPoints  + "\",";

  jsonResponse += "\"MessStatusSens3Row1\":\"" + messSens3ActValues     + "\",";
  jsonResponse += "\"MessStatusSens3Row2\":\"" + messSens3ActCalPoints  + "\",";

  jsonResponse += "\"MessStatusSens4Row1\":\"" + messSens4ActValues     + "\",";
  jsonResponse += "\"MessStatusSens4Row2\":\"" + messSens4ActCalPoints  + "\"";

  jsonResponse += "}";

  server.send(200, "application/json", jsonResponse);
}

/**

 */
bool WSER_CalibrationSmartHomeGetStatus(uint8_t sensorIndex) {

    Serial.printf("SMART HOME REQUEST: richiesta sensore calibrazione numero" + sensorIndex);

    String json = "{";
    json += "\"sensor\":" + String(sensorIndex) + ",";
    json += "\"type\":" + String(s_Ctrl[sensorIndex].type) + ",";
    json += "\"value\":" + String(s_Ctrl[sensorIndex].ActReadVal) + ",";          // ✅ Valore attuale del sensore
    json += "\"calX1\":" + String(s_Ctrl[sensorIndex].CalX1) + ",";
    json += "\"calY1\":" + String(s_Ctrl[sensorIndex].CalY1) + ",";
    json += "\"calX2\":" + String(s_Ctrl[sensorIndex].CalX2) + ",";
    json += "\"calY2\":" + String(s_Ctrl[sensorIndex].CalY2) + ",";
    json += "}";

    Serial.println("Spedito stato di calibrazione per sensore " + String(sensorIndex));
    server.send(200, "application/json", json);

    return true;

}

/**

 */
void WSER_handleSetCalPoint() {
  if (!(server.hasArg("sensorNumb") && server.hasArg("calPoint") && server.hasArg("calPointValue"))) {
    server.send(400, "application/json", "{\"error\":\"Missing parameters\"}");
    return;
  }

  uint8_t sens_numb = server.arg("sensorNumb").toInt() - 1;  // --- Parametri ---
  uint8_t cal_point = server.arg("calPoint").toInt();
  int16_t cal_value = server.arg("calPointValue").toInt();   

  if (!((sens_numb == 0) || (sens_numb == 2) || (sens_numb == 3))) {
    server.send(400, "application/json", "{\"error\":\"Bad sensorNumb\"}");
    return;
  }

  if (!(cal_point == 1 || cal_point == 2)) {
    server.send(400, "application/json", "{\"error\":\"Bad calPoint\"}");
    return;
  }

  if (WSER_ExtCmdSetCalPoint(sens_numb, cal_point, cal_value)){
    server.send(200, "application/json", "{\"ok\":true}");
  } else {
    server.send(400, "text/plain", "CalPoint NOT updated");
  }

}

/**

 */
bool WSER_ExtCmdSetCalPoint(uint8_t sens_numb, uint8_t cal_point, int16_t cal_value) {

  if (!((sens_numb == 0) || (sens_numb == 2) || (sens_numb == 3))) {
    return false;
  }
  if (!(cal_point == 1 || cal_point == 2)) {
    return false;
  }

  long x_raw_l = s_Ctrl[sens_numb].ActReadVal;   // usa qui la sorgente raw che preferisci

  if (sens_numb  == 0) {
    if (cal_point == 1) {
      EEPR_WriteInt16(SENS_1_CAL_X1, (int16_t) x_raw_l);
      EEPR_WriteInt16(SENS_1_CAL_Y1, cal_value);

    } else if (cal_point == 2) {
      EEPR_WriteInt16(SENS_1_CAL_X2, (int16_t) x_raw_l);
      EEPR_WriteInt16(SENS_1_CAL_Y2, cal_value);
    } 
  } else if (sens_numb  == 2) {

  Serial.println("---- EC/TDS/SAL CAL SET ----");

  Serial.print("Sensor: ");
  Serial.println(sens_numb);

  Serial.print("Cal point: ");
  Serial.println(cal_point);

  Serial.print("Sensor type: ");
  Serial.println(s_Ctrl[2].type);

  Serial.print("User value (@25C): ");
  Serial.println(cal_value);

  // =====================================================
  // TARGET (Y) -> valore standard cliente convertito in EC
  // =====================================================
  float ecTarget25 = (float)cal_value;

  if (s_Ctrl[2].type == _TDS) {
    ecTarget25 /= 0.64f;
  }
  else if (s_Ctrl[2].type == _SAL) {
    ecTarget25 /= 0.55f;
  }

  if (ecTarget25 < 1.0f) {
    ecTarget25 = 1.0f;
  }

  uint16_t targetResistance =
      (uint16_t)roundf(1000000.0f / ecTarget25);

  // =====================================================
  // MEASURED (X) -> resistenza reale compensata a 25°C
  // =====================================================
  unsigned int tempDeciC = Sens_Ntc_Get();
  float tempC = tempDeciC * 0.1f;

  float tempFactor = 1.0f + (0.02f * (tempC - 25.0f));

  float measuredResistance25f = (float)s_Ctrl[2].ActReadVal * tempFactor;

  if (measuredResistance25f < 1.0f) {
    measuredResistance25f = 1.0f;
  }

  uint16_t measuredResistance = (uint16_t)roundf(measuredResistance25f);

  // =====================================================
  // DEBUG
  // =====================================================
  Serial.print("Temperature: ");
  Serial.print(tempC, 1);
  Serial.println(" C");

  Serial.print("Temp factor: ");
  Serial.println(tempFactor, 4);

  Serial.print("Raw resistance: ");
 Serial.println(s_Ctrl[2].ActReadVal);

  Serial.print("Measured resistance @25C (X): ");
  Serial.println(measuredResistance);

  Serial.print("Equivalent EC target @25C: ");
  Serial.println(ecTarget25);

  Serial.print("Target resistance (Y): ");
  Serial.println(targetResistance);

  // =====================================================
  // SAVE
  // =====================================================
  if (cal_point == 1) {
    EEPR_WriteUint16(SENS_3_CAL_X1, measuredResistance);
    EEPR_WriteUint16(SENS_3_CAL_Y1, targetResistance);
  }
  else if (cal_point == 2) {
    EEPR_WriteUint16(SENS_3_CAL_X2, measuredResistance);
    EEPR_WriteUint16(SENS_3_CAL_Y2, targetResistance);
  }

  Serial.println("----------------------------");

  }   else if (sens_numb  == 3) {
    if (cal_point == 1) {
      EEPR_WriteInt16(SENS_4_CAL_X1, (int16_t) x_raw_l);
      EEPR_WriteInt16(SENS_4_CAL_Y1, cal_value);

    } else if (cal_point == 2) {
      EEPR_WriteInt16(SENS_4_CAL_X2, (int16_t) x_raw_l);
      EEPR_WriteInt16(SENS_4_CAL_Y2, cal_value);
    } 
  }

  if (sens_numb == 0) {  // Aggiorna calibrazione in RAM dopo scrittura EEPROM
    s_Ctrl[0].CalX1 = EEPR_ReadInt16(SENS_1_CAL_X1);
    s_Ctrl[0].CalY1 = EEPR_ReadInt16(SENS_1_CAL_Y1);
    s_Ctrl[0].CalX2 = EEPR_ReadInt16(SENS_1_CAL_X2);
    s_Ctrl[0].CalY2 = EEPR_ReadInt16(SENS_1_CAL_Y2);
  } 
  else if (sens_numb == 2) {
    s_Ctrl[2].CalX1 = EEPR_ReadInt16(SENS_3_CAL_X1);
    s_Ctrl[2].CalY1 = EEPR_ReadInt16(SENS_3_CAL_Y1);
    s_Ctrl[2].CalX2 = EEPR_ReadInt16(SENS_3_CAL_X2);
    s_Ctrl[2].CalY2 = EEPR_ReadInt16(SENS_3_CAL_Y2);
  } 
  else if (sens_numb == 3) {
    s_Ctrl[3].CalX1 = EEPR_ReadInt16(SENS_4_CAL_X1);
    s_Ctrl[3].CalY1 = EEPR_ReadInt16(SENS_4_CAL_Y1);
    s_Ctrl[3].CalX2 = EEPR_ReadInt16(SENS_4_CAL_X2);
    s_Ctrl[3].CalY2 = EEPR_ReadInt16(SENS_4_CAL_Y2);
  }

  return true;

}

/**

 */
void WSER_handleReSetCalPoint(){

  if (!(server.hasArg("sensorNumb") && server.hasArg("calPoint"))) {
    server.send(400, "application/json", "{\"error\":\"Missing parameters\"}");
    return;
  }

  uint8_t sens_numb = server.arg("sensorNumb").toInt()-1;
  uint8_t cal_point = server.arg("calPoint").toInt();

  if (WSER_ExtCmdCalReset(sens_numb, cal_point)){
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "restcal NOT updated");
  }
}


/**

 */
 bool WSER_ExtCmdCalReset(uint8_t sensorIndex, uint8_t calPoint ) {

  if ((sensorIndex > 3)  || ((calPoint != 1) && (calPoint != 2))) {
    return false;
  }

  if (sensorIndex == 0) {

    if (s_Ctrl[0].type == _PH) {
      if (calPoint == 1) {
        EEPR_WriteInt16(SENS_1_CAL_X1, 709);
        EEPR_WriteInt16(SENS_1_CAL_Y1, 100);
      } else {
        EEPR_WriteInt16(SENS_1_CAL_X2, -644); // calPoint == 2
        EEPR_WriteInt16(SENS_1_CAL_Y2, 1245);
      }
    } else if (s_Ctrl[0].type == _RX) {
      if (calPoint == 1) {
        EEPR_WriteInt16(SENS_1_CAL_X1, -1798);
        EEPR_WriteInt16(SENS_1_CAL_Y1, -1800);
      } else {
        EEPR_WriteInt16(SENS_1_CAL_X2, 1798); // calPoint == 2
        EEPR_WriteInt16(SENS_1_CAL_Y2, 1800);
      }
    }

    s_Ctrl[0].CalX1 = EEPR_ReadInt16(SENS_1_CAL_X1);    // Aggiorna in RAM
    s_Ctrl[0].CalY1 = EEPR_ReadInt16(SENS_1_CAL_Y1);
    s_Ctrl[0].CalX2 = EEPR_ReadInt16(SENS_1_CAL_X2);
    s_Ctrl[0].CalY2 = EEPR_ReadInt16(SENS_1_CAL_Y2);


  } else if (sensorIndex == 2) {

    if (
        s_Ctrl[2].type == _EC ||
        s_Ctrl[2].type == _TDS ||
        s_Ctrl[2].type == _SAL
    ) {
      if (calPoint == 1) {
        EEPR_WriteUint16(SENS_3_CAL_X1, 500);
        EEPR_WriteUint16(SENS_3_CAL_Y1, 500);
      } else {
        EEPR_WriteUint16(SENS_3_CAL_X2, 2000); // calPoint == 2
        EEPR_WriteUint16(SENS_3_CAL_Y2, 2000);
      }
    }

    s_Ctrl[2].CalX1 = EEPR_ReadInt16(SENS_3_CAL_X1);    // Aggiorna in RAM
    s_Ctrl[2].CalY1 = EEPR_ReadInt16(SENS_3_CAL_Y1);
    s_Ctrl[2].CalX2 = EEPR_ReadInt16(SENS_3_CAL_X2);
    s_Ctrl[2].CalY2 = EEPR_ReadInt16(SENS_3_CAL_Y2);

  } else if (sensorIndex == 3) {

    if (s_Ctrl[3].type == _PH) {
      if (calPoint == 1) {
        EEPR_WriteInt16(SENS_4_CAL_X1, 709);
        EEPR_WriteInt16(SENS_4_CAL_Y1, 100);
      } else {
        EEPR_WriteInt16(SENS_4_CAL_X2, -644); // calPoint == 2
        EEPR_WriteInt16(SENS_4_CAL_Y2, 1245);
      }
    } else if (s_Ctrl[3].type == _RX) {
      if (calPoint == 1) {
        EEPR_WriteInt16(SENS_4_CAL_X1, -1798);
        EEPR_WriteInt16(SENS_4_CAL_Y1, -1800);
      } else {
        EEPR_WriteInt16(SENS_4_CAL_X2, 1798); // calPoint == 2
        EEPR_WriteInt16(SENS_4_CAL_Y2, 1800);
      }
    }

    s_Ctrl[3].CalX1 = EEPR_ReadInt16(SENS_4_CAL_X1);    // Aggiorna in RAM
    s_Ctrl[3].CalY1 = EEPR_ReadInt16(SENS_4_CAL_Y1);
    s_Ctrl[3].CalX2 = EEPR_ReadInt16(SENS_4_CAL_X2);
    s_Ctrl[3].CalY2 = EEPR_ReadInt16(SENS_4_CAL_Y2);

  }

  Serial.printf("✅ Calibration for Sensor[%d] reset to default (type=%d)\n",
                sensorIndex, s_Ctrl[sensorIndex].type);
  return true;
}

/**

 */
void WSER_handleSettingControlType(){

  if (!server.hasArg("control") || !server.hasArg("type")) {
    server.send(400, "application/json", "{\"error\":\"Missing parameters\"}");
    return;
  }

  uint8_t ctrl_numb = server.arg("control").toInt() -1;
  String ctrl_new_type = server.arg("type");

  if (WSER_ExtCmdSetType(ctrl_numb , ctrl_new_type)){
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "SetTye NOT updated");
  }

}

/**

 */
bool WSER_ExtCmdSetType(uint8_t ctrlIndex, const String& typeStr) {
  if (ctrlIndex > 3) {
      return false;
  }

  e_CtrlType newType = _EMPTY;

  if (ctrlIndex == 3 || ctrlIndex == 0) {  /// if it is number 1 then PH or RX can be
      if      (typeStr == "ph")   newType = _PH;
      else if (typeStr == "orp")  newType = _RX;
      else return false;
    }

  if (ctrlIndex == 2 ) {  /// if it is number 1 then PH or RX can be
      if      (typeStr == "tds")   newType = _SAL;
      else if (typeStr == "sal")  newType = _TDS;
      else if (typeStr == "ec")  newType = _EC;
      else if (typeStr == "ntc")  newType = _NTC;
      else if (typeStr == "diff")  newType = _DIFF;
      else if (typeStr == "avg")  newType = _AVG;
      else return false;
    }

  if (ctrlIndex == 1 ) {  /// if it is number 1 then PH or RX can be
      if (typeStr == "ds18")   newType = _DS18;
      else return false;
  }


  if (newType == _PH) { // === Aggiorna parametri sensore ===
    if (ctrlIndex == 0) {
      EEPR_WriteByte(CTRL_1_TYPE, _PH);
      EEPR_WriteInt16(SENS_1_CAL_X1, 709);  EEPR_WriteInt16(SENS_1_CAL_Y1, 100);
      EEPR_WriteInt16(SENS_1_CAL_X2, -644); EEPR_WriteInt16(SENS_1_CAL_Y2, 1245);

      s_Ctrl[0].type = EEPR_ReadByte(CTRL_1_TYPE);
      s_Ctrl[0].CalX1 = EEPR_ReadInt16(SENS_1_CAL_X1);
      s_Ctrl[0].CalY1 = EEPR_ReadInt16(SENS_1_CAL_Y1);
      s_Ctrl[0].CalX2 = EEPR_ReadInt16(SENS_1_CAL_X2);
      s_Ctrl[0].CalY2 = EEPR_ReadInt16(SENS_1_CAL_Y2);
    } else if (ctrlIndex == 3) {
      EEPR_WriteByte(CTRL_4_TYPE, _PH);
      EEPR_WriteInt16(SENS_4_CAL_X1, 709);  EEPR_WriteInt16(SENS_4_CAL_Y1, 100);
      EEPR_WriteInt16(SENS_4_CAL_X2, -644); EEPR_WriteInt16(SENS_4_CAL_Y2, 1245);

      s_Ctrl[3].type = EEPR_ReadByte(CTRL_4_TYPE);
      s_Ctrl[3].CalX1 = EEPR_ReadInt16(SENS_4_CAL_X1);
      s_Ctrl[3].CalY1 = EEPR_ReadInt16(SENS_4_CAL_Y1);
      s_Ctrl[3].CalX2 = EEPR_ReadInt16(SENS_4_CAL_X2);
      s_Ctrl[3].CalY2 = EEPR_ReadInt16(SENS_4_CAL_Y2);
    }

  }

  if (newType == _RX) {
    if (ctrlIndex == 0) {
      EEPR_WriteByte(CTRL_1_TYPE, _RX);
      EEPR_WriteInt16(SENS_1_CAL_X1, -1798);  EEPR_WriteInt16(SENS_1_CAL_Y1, -1800);
      EEPR_WriteInt16(SENS_1_CAL_X2, 1798); EEPR_WriteInt16(SENS_1_CAL_Y2, 1800);

      s_Ctrl[0].type = EEPR_ReadByte(CTRL_1_TYPE);
      s_Ctrl[0].CalX1 = EEPR_ReadInt16(SENS_1_CAL_X1);
      s_Ctrl[0].CalY1 = EEPR_ReadInt16(SENS_1_CAL_Y1);
      s_Ctrl[0].CalX2 = EEPR_ReadInt16(SENS_1_CAL_X2);
      s_Ctrl[0].CalY2 = EEPR_ReadInt16(SENS_1_CAL_Y2);
    } else if (ctrlIndex == 3) {
      EEPR_WriteByte(CTRL_4_TYPE, _RX);
      EEPR_WriteInt16(SENS_4_CAL_X1, -1798);  EEPR_WriteInt16(SENS_4_CAL_Y1, -1800);
      EEPR_WriteInt16(SENS_4_CAL_X2, 1798); EEPR_WriteInt16(SENS_4_CAL_Y2, 1800);

      s_Ctrl[3].type = EEPR_ReadByte(CTRL_4_TYPE);
      s_Ctrl[3].CalX1 = EEPR_ReadInt16(SENS_4_CAL_X1);
      s_Ctrl[3].CalY1 = EEPR_ReadInt16(SENS_4_CAL_Y1);
      s_Ctrl[3].CalX2 = EEPR_ReadInt16(SENS_4_CAL_X2);
      s_Ctrl[3].CalY2 = EEPR_ReadInt16(SENS_4_CAL_Y2);
    }

  }

  if (ctrlIndex == 0) {  // === Inizializzazione del controllo ===
    EEPR_WriteByte(CTRL_1_TYPE, newType);
    EEPR_WriteByte(CTRL_1_ACTIVE, true);
    EEPR_WriteByte(CTRL_1_NUM, 0);
    EEPR_WriteByte(CTRL_1_LOGIK, true);
    EEPR_WriteInt16(CTRL_1_CLOCK, 0);
    EEPR_WriteInt16(CTRL_1_SET_PT, newType == _PH ? 700 : 390);
    EEPR_WriteInt16(CTRL_1_HYSTER, newType == _PH ? 100 : 10);
    s_Ctrl[0].type         = EEPR_ReadByte(CTRL_1_TYPE);
    s_Ctrl[0].Active       = EEPR_ReadByte(CTRL_1_ACTIVE);
    s_Ctrl[0].SensNum      = EEPR_ReadByte(CTRL_1_NUM);
    s_Ctrl[0].CtrlSt       = false;
    s_Ctrl[0].ActReadVal   = 0;
    s_Ctrl[0].Clock        = EEPR_ReadInt16(CTRL_1_CLOCK);
    s_Ctrl[0].Set_Point    = EEPR_ReadInt16(CTRL_1_SET_PT);
    s_Ctrl[0].Set_PosLogik = EEPR_ReadByte(CTRL_1_LOGIK);
    s_Ctrl[0].Set_Hyster   = EEPR_ReadInt16(CTRL_1_HYSTER);
    s_Ctrl[0].ssid         = "N/A";
    s_Ctrl[0].CtrlStPlug   = false;
  }

  if (ctrlIndex == 3) {
    EEPR_WriteByte(CTRL_4_TYPE, newType);
    EEPR_WriteByte(CTRL_4_ACTIVE, true);
    EEPR_WriteByte(CTRL_4_NUM, 3);
    EEPR_WriteByte(CTRL_4_LOGIK, true);
    EEPR_WriteInt16(CTRL_4_CLOCK, 0);
    EEPR_WriteInt16(CTRL_4_SET_PT, newType == _PH ? 700 : 390);
    EEPR_WriteInt16(CTRL_4_HYSTER, newType == _PH ? 100 : 10);
    s_Ctrl[3].type         = EEPR_ReadByte(CTRL_4_TYPE);
    s_Ctrl[3].Active       = EEPR_ReadByte(CTRL_4_ACTIVE);
    s_Ctrl[3].SensNum      = EEPR_ReadByte(CTRL_4_NUM);
    s_Ctrl[3].CtrlSt       = false;
    s_Ctrl[3].ActReadVal   = 0;
    s_Ctrl[3].Clock        = EEPR_ReadInt16(CTRL_4_CLOCK);
    s_Ctrl[3].Set_Point    = EEPR_ReadInt16(CTRL_4_SET_PT);
    s_Ctrl[3].Set_PosLogik = EEPR_ReadByte(CTRL_4_LOGIK);
    s_Ctrl[3].Set_Hyster   = EEPR_ReadInt16(CTRL_4_HYSTER);
    s_Ctrl[3].ssid         = "N/A";
    s_Ctrl[3].CtrlStPlug   = false;
  }

  if (ctrlIndex == 2) { /// if it is number 2 then EC can be
    if (typeStr == "ec") newType = _EC;
    else if (typeStr == "tds") newType = _TDS;
    else if (typeStr == "sal") newType = _SAL;
    else if (typeStr == "ntc") newType = _NTC;
    else if (typeStr == "diff") newType = _DIFF;
    else if (typeStr == "avg") newType = _AVG;
    else return false;

  if (newType == _EC) { // === Aggiorna parametri sensore ===
    EEPR_WriteByte(CTRL_3_TYPE, _EC);

    EEPR_WriteUint16 (SENS_3_CAL_X1, 500);
    EEPR_WriteUint16 (SENS_3_CAL_Y1, 500);
    EEPR_WriteUint16 (SENS_3_CAL_X2, 2000);
    EEPR_WriteUint16 (SENS_3_CAL_Y2, 2000);

    s_Ctrl[2].type    = EEPR_ReadByte(SENS_3_TYPE);
    s_Ctrl[2].CalX1   = EEPR_ReadInt16(SENS_3_CAL_X1);
    s_Ctrl[2].CalY1   = EEPR_ReadInt16(SENS_3_CAL_Y1);
    s_Ctrl[2].CalX2   = EEPR_ReadInt16(SENS_3_CAL_X2);
    s_Ctrl[2].CalY2   = EEPR_ReadInt16(SENS_3_CAL_Y2);

  } else if (newType == _TDS) {
    EEPR_WriteByte(CTRL_3_TYPE, _TDS);
    EEPR_WriteUint16 (SENS_3_CAL_X1, 500);
    EEPR_WriteUint16 (SENS_3_CAL_Y1, 500);
    EEPR_WriteUint16 (SENS_3_CAL_X2, 2000);
    EEPR_WriteUint16 (SENS_3_CAL_Y2, 2000);

    s_Ctrl[2].type    = EEPR_ReadByte(CTRL_3_TYPE);
    s_Ctrl[2].CalX1   = EEPR_ReadInt16(SENS_3_CAL_X1);
    s_Ctrl[2].CalY1   = EEPR_ReadInt16(SENS_3_CAL_Y1);
    s_Ctrl[2].CalX2   = EEPR_ReadInt16(SENS_3_CAL_X2);
    s_Ctrl[2].CalY2   = EEPR_ReadInt16(SENS_3_CAL_Y2);

  } else if (newType == _SAL) {
    EEPR_WriteByte(CTRL_3_TYPE, _SAL);
    EEPR_WriteUint16 (SENS_3_CAL_X1, 500);
    EEPR_WriteUint16 (SENS_3_CAL_Y1, 500);
    EEPR_WriteUint16 (SENS_3_CAL_X2, 2000);
    EEPR_WriteUint16 (SENS_3_CAL_Y2, 2000);

    s_Ctrl[2].type    = EEPR_ReadByte(CTRL_3_TYPE);
    s_Ctrl[2].CalX1   = EEPR_ReadInt16(SENS_3_CAL_X1);
    s_Ctrl[2].CalY1   = EEPR_ReadInt16(SENS_3_CAL_Y1);
    s_Ctrl[2].CalX2   = EEPR_ReadInt16(SENS_3_CAL_X2);
    s_Ctrl[2].CalY2   = EEPR_ReadInt16(SENS_3_CAL_Y2);

  } else if (newType == _NTC) {
    EEPR_WriteByte(CTRL_3_TYPE, _NTC);
    s_Ctrl[2].type    = EEPR_ReadByte(CTRL_3_TYPE);

  }else if (newType == _DIFF) {
    EEPR_WriteByte(CTRL_3_TYPE, _DIFF);
    s_Ctrl[2].type    = EEPR_ReadByte(CTRL_3_TYPE);

  }
  else if (newType == _AVG) {
    EEPR_WriteByte(CTRL_3_TYPE, _AVG);
    s_Ctrl[2].type    = EEPR_ReadByte(CTRL_3_TYPE);

  }

  EEPR_WriteByte(CTRL_3_TYPE, newType);  // === Inizializzazione del controllo ===
  EEPR_WriteByte(CTRL_3_ACTIVE, true);
  EEPR_WriteByte(CTRL_3_NUM, 2);
  EEPR_WriteByte(CTRL_3_LOGIK, true);
  EEPR_WriteInt16(CTRL_3_CLOCK, 0);

  if (newType == _EC) {      // EC: µS/cm
    EEPR_WriteInt16(CTRL_3_SET_PT, 700);  // 700 µS/cm
    EEPR_WriteInt16(CTRL_3_HYSTER, 30);   // 10 µS/cm

  } else if (newType == _TDS) {      // TDS: ppm
    EEPR_WriteInt16(CTRL_3_SET_PT, 390);  // 390 ppm
    EEPR_WriteInt16(CTRL_3_HYSTER, 20);  // 100 ppm

  } else if (newType == _SAL) {
    EEPR_WriteInt16(CTRL_3_SET_PT, 300);  
    EEPR_WriteInt16(CTRL_3_HYSTER, 10);    

  } else if (newType == _NTC) {
    EEPR_WriteInt16(CTRL_3_SET_PT, 200);  
    EEPR_WriteInt16(CTRL_3_HYSTER, 25);  
  }
    else if (newType == _DIFF) {
    EEPR_WriteInt16(CTRL_3_SET_PT, 240);  
    EEPR_WriteInt16(CTRL_3_HYSTER, 35);

  }  else if (newType == _AVG) {
    EEPR_WriteInt16(CTRL_3_SET_PT, 260);  
    EEPR_WriteInt16(CTRL_3_HYSTER, 45);
    
  }
  s_Ctrl[2].type         = EEPR_ReadByte(CTRL_3_TYPE);
  s_Ctrl[2].Active       = EEPR_ReadByte(CTRL_3_ACTIVE);
  s_Ctrl[2].SensNum      = EEPR_ReadByte(CTRL_3_NUM);
  s_Ctrl[2].CtrlSt       = false;
  s_Ctrl[2].ActReadVal   = 0;
  s_Ctrl[2].Clock        = EEPR_ReadInt16(CTRL_3_CLOCK);
  s_Ctrl[2].Set_Point    = EEPR_ReadInt16(CTRL_3_SET_PT);
  s_Ctrl[2].Set_PosLogik = EEPR_ReadByte(CTRL_3_LOGIK);
  s_Ctrl[2].Set_Hyster   = EEPR_ReadInt16(CTRL_3_HYSTER);
  s_Ctrl[2].ssid         = "N/A";
  s_Ctrl[2].CtrlStPlug   = false;

  }

  return true;
}
