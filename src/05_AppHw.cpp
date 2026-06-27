#include <Arduino.h>
#include "05_AppHW.h"
#include "03_EEprom.h"


void HW_InitHw();


void SensEc_SetMidRange();
void SensEc_SetLowRange();
void SensEc_SetOff();
void SensEc_Depolarize();
void SensEc_SetHighRange();

bool HW_SetRelay(uint8_t relayNum, bool state);



 //---- conflict to be solved in the premium
/**

 */
void HW_InitHw(){
	
  EEPROM.begin(EEPROM_SIZE); // o 1024, in base alla dimensione necessaria

  pinMode(RELAY_1, OUTPUT);    // imposta GPIO per RELAY_1 come output
  digitalWrite(RELAY_1, LOW);  // stato iniziale OFF

  pinMode(RELAY_2, OUTPUT);    // imposta GPIO per RELAY_2 come output
  digitalWrite(RELAY_2, LOW);  // stato iniziale OFF

  pinMode(RELAY_3, OUTPUT);    // imposta GPIO per RELAY_3 come output
  digitalWrite(RELAY_3, LOW);  // stato iniziale OFF

  //for ec
  pinMode(EC_PD_470_PIN, INPUT );
  pinMode(EC_PD_33K_PIN, INPUT);    // to be deleted fi the new hw doenst need those circuitry
  pinMode(EC_PD_3K3_PIN, INPUT); 

  // for DS18B20
  pinMode(DS18B20_PIN, INPUT); //in other module it is initialized for one wire functions



}

bool HW_SetRelay(uint8_t relayNum, bool state)
{
    uint8_t relayPin = 0;

    if (relayNum == 1) {
        relayPin = RELAY_1;
    } else if (relayNum == 2) {
        relayPin = RELAY_2;
    } else if (relayNum == 3) {
        relayPin = RELAY_3;
    } else {
        return false;
    }

    digitalWrite(relayPin, state ? HIGH : LOW);
    return true;
}


//--------------------------------------------------------------------
/**

 */
void SensEc_SetHighRange()
{
    pinMode(EC_PD_470_PIN, INPUT);
    pinMode(EC_PD_3K3_PIN, INPUT);
    pinMode(EC_PD_33K_PIN, OUTPUT);
    digitalWrite(EC_PD_33K_PIN, LOW);
}


/**

 */
void SensEc_SetMidRange()
{
    pinMode(EC_PD_470_PIN, INPUT);
    pinMode(EC_PD_3K3_PIN, OUTPUT);
    digitalWrite(EC_PD_3K3_PIN, LOW);
    pinMode(EC_PD_33K_PIN, INPUT);
}


/**

 */
void SensEc_SetLowRange()
{
    pinMode(EC_PD_470_PIN, OUTPUT);
    digitalWrite(EC_PD_470_PIN, LOW);
    pinMode(EC_PD_3K3_PIN, INPUT);
    pinMode(EC_PD_33K_PIN, INPUT);
}

/**

 */
void SensEc_SetOff()
{
    pinMode(EC_PD_470_PIN, INPUT);
    pinMode(EC_PD_3K3_PIN, INPUT);
    pinMode(EC_PD_33K_PIN, INPUT);
}


/**

 */
void SensEc_Depolarize()
{
    pinMode(EC_PD_3K3_PIN, OUTPUT);
    digitalWrite(EC_PD_3K3_PIN, HIGH);
    delay(5);
    pinMode(EC_PD_3K3_PIN, INPUT);
}

