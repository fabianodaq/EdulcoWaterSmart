#include <EdulcoWaterSmart.h>

EdulcoWaterSmart myEdulco;


void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("Edulco Water Smart - Basic Example");

  if (!myEdulco.begin()) {
    Serial.println("Errore inizializzazione Edulco");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("Edulco inizializzato correttamente");
}

void loop() {
  Serial.println("-----------------------------");

  Serial.print("pH CH1: ");
  Serial.println(myEdulco.getPH(EDULCO_CH1), 2);

  Serial.print("ORP CH4: ");
  Serial.print(myEdulco.getORP(EDULCO_CH4), 0);
  Serial.println(" mV");

  Serial.print("Temperatura NTC CH2: ");
  Serial.print(myEdulco.getTemperatureNTC(EDULCO_CH2), 2);
  Serial.println(" C");

  Serial.print("Temperatura DS18: ");
  Serial.print(myEdulco.getTemperatureDS18(), 2);
  Serial.println(" C");

  Serial.print("EC CH3: ");
  Serial.print(myEdulco.getEC(EDULCO_CH3), 3);
  Serial.println(" mS/cm");

  Serial.print("TDS CH3: ");
  Serial.print(myEdulco.getTDS(EDULCO_CH3), 0);
  Serial.println(" ppm");

  Serial.print("SAL CH3: ");
  Serial.print(myEdulco.getSAL(EDULCO_CH3), 2);
  Serial.println(" ppt");

  delay(2000);
}