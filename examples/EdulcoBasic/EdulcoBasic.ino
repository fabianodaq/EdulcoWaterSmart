/*============================================================================
  EdulcoWaterSmart - EdulcoBasic_temp example
  ----------------------------------------------------------------------------

  Author:
  - Created by Fabiano (project owner)

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

EdulcoWaterSmart myEdulco;

static void handleSerialBasicCommands();


/*

*/
void setup() {
  Serial.begin(115200);
  delay(1000);
  myEdulco.begin();

}


/*

*/
void loop() {

  delay(2000);
  handleSerialBasicCommands();
  Serial.println("\n\n---------CHANNEL 1 PH/ORP------------");

  if (_PH == myEdulco.getPhOrpType_Channel1()) {
    Serial.print("TYPE = PH💧; Value: ");
    Serial.println(myEdulco.getPh(1), 2);

    int s_ycal[2];
    myEdulco.getCalPointPhOrp_1_Y1(s_ycal);

    Serial.print("CAL Y1: ");
    Serial.print((float)s_ycal[0]/100.0f, 2);
    Serial.print(", CAL X1: ");
    Serial.print(s_ycal[1]);
    Serial.println(" mv");
    myEdulco.getCalPointPhOrp_1_Y2(s_ycal);
    Serial.print("CAL Y2: ");
    Serial.print((float)s_ycal[0]/100.0f, 2);
    Serial.print(", CAL X2: ");
    Serial.print(s_ycal[1]);
    Serial.println(" mv");

  } else if (_RX == myEdulco.getPhOrpType_Channel1()) {
    Serial.print("TYPE = RX⚗️; Value: ");
    Serial.print(myEdulco.getOrp(1), 0);
    Serial.println(" mV");

    int s_ycal[2];
    myEdulco.getCalPointPhOrp_1_Y1(s_ycal);

    Serial.print("CAL Y1: ");
    Serial.print(s_ycal[0]);
    Serial.print(" mv");
    Serial.print(", CAL X1: ");
    Serial.print(s_ycal[1]);
    Serial.println(" mv");
    myEdulco.getCalPointPhOrp_1_Y2(s_ycal);
    Serial.print("CAL Y2: ");
    Serial.print(s_ycal[0]);
    Serial.print(" mv");
    Serial.print(", CAL X2: ");
    Serial.print(s_ycal[1]);
    Serial.println(" mv");

  } 

  delay(2000);
  handleSerialBasicCommands();
  Serial.println("\n---------CHANNEL 2 PH/ORP------------");

  if (_PH == myEdulco.getPhOrpType_Channel2()) {
    Serial.print("TYPE = PH💧; Value: ");
    Serial.println(myEdulco.getPh(2), 2);

    int s_ycal[2];
    myEdulco.getCalPointPhOrp_2_Y1(s_ycal);

    Serial.print("CAL Y1: ");
    Serial.print((float)s_ycal[0]/100.0f, 2);
    Serial.print(", CAL X1: ");
    Serial.print(s_ycal[1]);
    Serial.println(" mv");
    myEdulco.getCalPointPhOrp_2_Y2(s_ycal);
    Serial.print("CAL Y2: ");
    Serial.print((float)s_ycal[0]/100.0f, 2);
    Serial.print(", CAL X2: ");
    Serial.print(s_ycal[1]);
    Serial.println(" mv");

  } else if (_RX == myEdulco.getPhOrpType_Channel2()) {
    Serial.print("TYPE = RX⚗️; Value: ");
    Serial.print(myEdulco.getOrp(2), 0);
    Serial.println(" mV");

    int s_ycal[2];
    myEdulco.getCalPointPhOrp_2_Y1(s_ycal);

    Serial.print("CAL Y1: ");
    Serial.print(s_ycal[0]);
    Serial.print(" mv");
    Serial.print(", CAL X1: ");
    Serial.print(s_ycal[1]);
    Serial.println(" mv");
    myEdulco.getCalPointPhOrp_2_Y2(s_ycal);
    Serial.print("CAL Y2: ");
    Serial.print(s_ycal[0]);
    Serial.print(" mv");
    Serial.print(", CAL X2: ");
    Serial.print(s_ycal[1]);
    Serial.println(" mv");

  } 

  delay(2000);
  handleSerialBasicCommands();
  Serial.println("\n---------CHANNEL EC/TDS/SAL)------------");

  if (_EC == myEdulco.getEcTdsSalType()) {
    Serial.print("TYPE = EC⚡; Value: ");
    Serial.print(myEdulco.getEcTdsSal(), 0);
    Serial.println(" uS/cm");

    int s_ycal[2];
    myEdulco.getCalPointEcTdsSal_Y1(s_ycal);

    Serial.print("CAL Y1: ");
    Serial.print(s_ycal[0]);
    Serial.print(", CAL X1: ");
    Serial.println(s_ycal[1]);

    myEdulco.getCalPointEcTdsSal_Y2(s_ycal);
    Serial.print("CAL Y2: ");
    Serial.print(s_ycal[0]);
    Serial.print(", CAL X2: ");
    Serial.println(s_ycal[1]);

  } else if (_TDS == myEdulco.getEcTdsSalType()) {
    Serial.print("TYPE = TDS💎; Value: ");
    Serial.print(myEdulco.getEcTdsSal(), 0);
    Serial.println(" ppm");

    int s_ycal[2];
    myEdulco.getCalPointEcTdsSal_Y1(s_ycal);

    Serial.print("CAL Y1: ");
    Serial.print(s_ycal[0]);
    Serial.print(", CAL X1: ");
    Serial.println(s_ycal[1]);

    myEdulco.getCalPointEcTdsSal_Y2(s_ycal);
    Serial.print("CAL Y2: ");
    Serial.print(s_ycal[0]);
    Serial.print(", CAL X2: ");
    Serial.println(s_ycal[1]);

  } else if (_SAL == myEdulco.getEcTdsSalType()) {
    Serial.print("TYPE = SAL🧂; Value: ");
    Serial.print(myEdulco.getEcTdsSal(), 0);
    Serial.println(" ppm");

    int s_ycal[2];
    myEdulco.getCalPointEcTdsSal_Y1(s_ycal);

    Serial.print("CAL Y1: ");
    Serial.print(s_ycal[0]);
    Serial.print(", CAL X1: ");
    Serial.println(s_ycal[1]);

    myEdulco.getCalPointEcTdsSal_Y2(s_ycal);
    Serial.print("CAL Y2: ");
    Serial.print(s_ycal[0]);
    Serial.print(", CAL X2: ");
    Serial.println(s_ycal[1]);

  } 

  delay(2000);
  handleSerialBasicCommands();
  Serial.println("\n---------CHANNEL DIGITAL TEMP (DS18)------------");

    Serial.print("TYPE = DIGITAL TEMP 🌡️; Value:"); 
    Serial.print(myEdulco.getTemperatureDS18(), 1);
    Serial.println(" °C");

  delay(2000);
  handleSerialBasicCommands();
  Serial.println("\n---------CHANNEL ANALOG TEMP (NTC)------------");

    Serial.print("TYPE = ANALOG TEMP 🌡️; Value: "); 
    Serial.print(myEdulco.getTemperatureNTC(), 1);
    Serial.println(" °C");

  Serial.println("\n--------- END CYCLE --------------------------");

}


/*

*/
static void handleSerialBasicCommands() {
  if (!Serial.available()) {
    return;
  }

  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  cmd.toUpperCase();

  //-----------Calibration Commands
  if (cmd == "CAL1 P1 700") {
    Serial.println(myEdulco.setControlCalPoint(0, 1, 700) ? "OK CAL1 P1" : "ERR CAL1 P1");

  } else if (cmd == "CAL1 P2 400") {
    Serial.println(myEdulco.setControlCalPoint(0, 2, 400) ? "OK CAL1 P2" : "ERR CAL1 P2");

  } else if (cmd == "CAL2 P1 -1800") {
    Serial.println(myEdulco.setControlCalPoint(1, 1, -1800) ? "OK CAL2 P1" : "ERR CAL2 P1");

  } else if (cmd == "CAL2 P2 1800") {
    Serial.println(myEdulco.setControlCalPoint(1, 2, 1800) ? "OK CAL2 P2" : "ERR CAL2 P2");

  } else if (cmd == "CAL3 P1 1413") {
    Serial.println(myEdulco.setControlCalPoint(2, 1, 1413) ? "OK CAL3 P1" : "ERR CAL3 P1");

  } else if (cmd == "CAL3 P2 2764") {
    Serial.println(myEdulco.setControlCalPoint(2, 2, 2764) ? "OK CAL3 P2" : "ERR CAL3 P2");

  } else if (cmd == "RCAL1 P1") {
    Serial.println(myEdulco.resetControlCalPoint(0, 1) ? "OK RCAL1 P1" : "ERR RCAL1 P1");

  } else if (cmd == "RCAL1 P2") {
    Serial.println(myEdulco.resetControlCalPoint(0, 2) ? "OK RCAL1 P2" : "ERR RCAL1 P2");

  } else if (cmd == "RCAL2 P1") {
    Serial.println(myEdulco.resetControlCalPoint(1, 1) ? "OK RCAL2 P1" : "ERR RCAL2 P1");

  } else if (cmd == "RCAL2 P2") {
    Serial.println(myEdulco.resetControlCalPoint(1, 2) ? "OK RCAL2 P2" : "ERR RCAL2 P2");

  } else if (cmd == "RCAL3 P1") {
    Serial.println(myEdulco.resetControlCalPoint(2, 1) ? "OK RCAL3 P1" : "ERR RCAL3 P1");

  } else if (cmd == "RCAL3 P2") {
    Serial.println(myEdulco.resetControlCalPoint(2, 2) ? "OK RCAL3 P2" : "ERR RCAL3 P2");


  //-----------Set Type Commands
  } else if (cmd == "TYPE1 PH") {
    Serial.println(myEdulco.setControlType(0, "ph") ? "OK TYPE1 PH" : "ERR TYPE1 PH");

  } else if (cmd == "TYPE1 RX") {
    Serial.println(myEdulco.setControlType(0, "rx") ? "OK TYPE1 RX" : "ERR TYPE1 RX");

  } else if (cmd == "TYPE2 PH") {
    Serial.println(myEdulco.setControlType(1, "ph") ? "OK TYPE2 PH" : "ERR TYPE2 PH");

  } else if (cmd == "TYPE2 RX") {
    Serial.println(myEdulco.setControlType(1, "rx") ? "OK TYPE2 RX" : "ERR TYPE2 RX");

  } else if (cmd == "TYPE3 EC") {
    Serial.println(myEdulco.setControlType(2, "ec") ? "OK TYPE3 EC" : "ERR TYPE3 EC");

  } else if (cmd == "TYPE3 TDS") {
    Serial.println(myEdulco.setControlType(2, "tds") ? "OK TYPE3 TDS" : "ERR TYPE3 TDS");

  } else if (cmd == "TYPE3 SAL") {
    Serial.println(myEdulco.setControlType(2, "sal") ? "OK TYPE3 SAL" : "ERR TYPE3 SAL");

  //-----------Set Relay Status Commands
  } else if (cmd == "REL1 ON") {
      myEdulco.setRelay(1, true);

    } else if (cmd == "REL1 OFF") {
      myEdulco.setRelay(1, false);

    } else if (cmd == "REL2 ON") {
      myEdulco.setRelay(2, true);

    } else if (cmd == "REL2 OFF") {
      myEdulco.setRelay(2, false);
      
    } else if (cmd == "REL3 ON") {
      myEdulco.setRelay(3, true);
      
    } else if (cmd == "REL3 OFF") {
      myEdulco.setRelay(3, false);
  }

}