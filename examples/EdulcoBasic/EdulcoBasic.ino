#include "src/EdulcoWaterSmart.h"

EdulcoWaterSmart myEdulco;





void setup() {
  Serial.begin(115200);
  delay(1000);
  myEdulco.begin();

}

void loop() {
  handleSerialRelayCommand();

  Serial.println("-----------------------------");

  Serial.print("[BASIC] Temperatura DS18: ");
  Serial.print(myEdulco.getTemperatureDS18(), 2);
  Serial.println(" C");

  Serial.print("[BASIC] Temperatura NTC: ");
  Serial.print(myEdulco.getTemperatureNTC(), 2);
  Serial.println(" C");

  Serial.print("[BASIC] EC/TDS/SAL: ");
  Serial.println(myEdulco.getEc(), 0);

  Serial.print("[BASIC] CH1 pH: ");
  Serial.println(myEdulco.getPh(1), 2);

  Serial.print("[BASIC] CH1 Redox: ");
  Serial.print(myEdulco.getOrp(1), 0);
  Serial.println(" mV");

  Serial.print("[BASIC] CH2 pH: ");
  Serial.println(myEdulco.getPh(2), 2);

  Serial.print("[BASIC] CH2 Redox: ");
  Serial.print(myEdulco.getOrp(2), 0);
  Serial.println(" mV");

  delay(2000);
}




// Relay serial command examples: REL1 ON, REL1 OFF, REL2 ON, REL3 OFF
static void handleSerialRelayCommand() {
  if (!Serial.available()) {
    return;
  }

  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  cmd.toUpperCase();

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