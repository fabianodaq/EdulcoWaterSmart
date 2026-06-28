#include "src/EdulcoWaterSmart.h"

EdulcoWaterSmart myEdulco;

static void handleSerialBasicCommands();

void setup() {
  Serial.begin(115200);
  delay(1000);
  myEdulco.begin();

}

void loop() {

  delay(2000);
  handleSerialBasicCommands();
  Serial.println("\n\n---------CHANNEL 1 PH/ORP------------");


  if (_PH == myEdulco.getPhOrpType_Channel1()) {
    Serial.print("[BASIC] TYPE = PH; Value: ");
    Serial.println(myEdulco.getPh(1), 2);

  } else if (_RX == myEdulco.getPhOrpType_Channel1()) {
    Serial.print("[BASIC] TYPE = RX; Value: ");
    Serial.print(myEdulco.getOrp(1), 0);
    Serial.println(" mV");

  } 

  delay(1000);
  handleSerialBasicCommands();
  Serial.println("\n---------CHANNEL 2 PH/ORP------------");

  if (_PH == myEdulco.getPhOrpType_Channel2()) {
    Serial.print("[BASIC] TYPE = PH; Value: ");
    Serial.println(myEdulco.getPh(2), 2);

  } else if (_RX == myEdulco.getPhOrpType_Channel2()) {
    Serial.print("[BASIC] TYPE = RX; Value: ");
    Serial.print(myEdulco.getOrp(2), 0);
    Serial.println(" mV");

  } 

  delay(1000);
  handleSerialBasicCommands();
  Serial.println("\n---------CHANNEL EC/TDS/SAL)------------");

  if (_EC == myEdulco.getEcTdsSalType()) {
    Serial.print("[BASIC] TYPE = EC; Value: ");
    Serial.print(myEdulco.getEcTdsSal(), 0);
    Serial.println(" uS/cm");

  } else if (_TDS == myEdulco.getEcTdsSalType()) {
    Serial.print("[BASIC] TYPE = TDS; Value: ");
    Serial.print(myEdulco.getEcTdsSal(), 0);
    Serial.println(" ppm");

  } else if (_SAL == myEdulco.getEcTdsSalType()) {
    Serial.print("[BASIC] TYPE = SAL; Value: ");
    Serial.print(myEdulco.getEcTdsSal(), 0);
    Serial.println(" ppm");

  } 

  delay(1000);
  handleSerialBasicCommands();
  Serial.println("\n---------CHANNEL DIGITAL TEMP (DS18)------------");

    Serial.print("[BASIC] DIGITAL TEMP Value:"); 
    Serial.print(myEdulco.getTemperatureDS18(), 0);
    Serial.println(" °C");

  delay(1000);
  handleSerialBasicCommands();
  Serial.println("\n---------CHANNEL ANALOG TEMP (NTC)------------");

    Serial.print("[BASIC] ANALOG TEMP Value: "); 
    Serial.print(myEdulco.getTemperatureNTC(), 0);
    Serial.println(" °C");

  Serial.println("\n--------- END CYCLE --------------------------");


}




static void handleSerialBasicCommands() {
  if (!Serial.available()) {
    return;
  }

  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  cmd.toUpperCase();

  //-----------Calibration Commands


  //-----------Set Type Commands


  //-----------Set Relay Status Commands
    if (cmd == "REL1 ON") {
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