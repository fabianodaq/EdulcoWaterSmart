#ifndef WSER_RELAYS_H
#define WSER_RELAYS_H

#define MAX_REMOTE_RELAYS 4

typedef enum {
  RELAY_SLOT_NOT_ASSIGNED = 0,           // Nessuna configurazione nello slot
  RELAY_SLOT_ASSIGNED_NOT_PRESENT,       // Configurato, ma il modulo non risponde
  RELAY_SLOT_ASSIGNED_PRESENT,    // Modulo connesso e associato correttamente
  RELAY_SLOT_FREE_SLOT
} RemoteRelayState_e;

typedef enum{
  RELAY_CMD_NO_REQ,
  RELAY_CMD_CONNECT,
  RELAY_CMD_TURN_ON,
  RELAY_CMD_TURN_OFF,
} requestType_e;

typedef enum{
  RELAY_RESULT_OK,
  RELAY_RESULT_NOT_OK
} request_result_e;

struct RemoteRelaySlot_s {
  //identifiers
  char id[33];    // 32 + null terminator
  char ip[17];    // 16 + null terminator
  //relay handling
  RemoteRelayState_e      state;      //status of the relay
  requestType_e           command_req;
  uint16_t                last_connection_ms; //ms last connection 
  //relay web page
  String messStatusRow1;
  String messStatusRow2;

};

extern RemoteRelaySlot_s RemoteRelay[MAX_REMOTE_RELAYS];
//----------- Extern Global variables-----------------

// ===== Public page function =====

extern void WSER_InitRemoteRelays();
extern void WSER_RemoteRelaysLoop();

extern void WSER_handleSaveRelay();

extern void WSER_handleRemoteRelaysGetStatus();

extern void WSER_WiFiRemoteRelaysPage();


extern bool WSER_ExtCmdSaveHwRel(uint8_t ctrl, uint8_t RelayIdx);
extern bool WSER_ExtCmdSaveSlot(uint8_t slotNum, const String& ip1, const String& ip2, const String& ip3, const String& ip4);
extern bool WSER_ExtCmdFreeSlot(uint8_t slotnum);

#endif 
