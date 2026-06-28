#ifndef APP_HW_H
#define APP_HW_H

//---------------PIN OUT DEFINITIONS-----------

// Relays
#define RELAY_3 26
#define RELAY_2 5
#define RELAY_1 23  //---- conflict to be solved in the premium

//for EC 
#define EC_PD_470_PIN   27   // io27 → polarità positiva / depolarizzazione
#define EC_PD_33K_PIN   32   // io32 → polarità negativa / misura
#define EC_PD_3K3_PIN   33   // io33 → polarità negativa / misura

#define R_PULL_UP_HIGH  33000.0f
#define R_PULL_UP_MID    3300.0f
#define R_PULL_UP_LOW     470.0f

#define V_PULL_UP_LOW    2500.0f

//for DS18 
#define DS18B20_PIN 16

extern void HW_InitHw();
extern bool HW_SetRelay(uint8_t relayNum, bool state);


extern void SensEc_SetMidRange();
extern void SensEc_SetLowRange();
extern void SensEc_SetOff();
extern void SensEc_Depolarize();
extern void SensEc_SetHighRange();


#endif 
