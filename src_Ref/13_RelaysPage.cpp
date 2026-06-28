#include "01_AppControls.h"
#include <HTTPClient.h>

#include "10_WebServerMain.h"
#include "13_RelaysPage_private.h"
#include "13_RelaysPage.h"

//--- Private Variables -------------------------------------------------------

unsigned long lastCheckTimeRemoteRelays = 0;

RemoteRelaySlot_s RemoteRelay[MAX_REMOTE_RELAYS];

//--- Public Variables --------------------------------------------------------

//--- Private Function Declarations ------------------------------------------

//--- Public Function Declarations -------------------------------------------
void WSER_InitRemoteRelays();
void WSER_RemoteRelaysLoop();

//get fucntions
void WSER_WiFiRemoteRelaysPage();
void WSER_handleRemoteRelaysGetStatus();

//web page fucntions
void WSER_handleSaveRelay();

//internal fucntions
void WSER_SetHwRelayString (uint8_t ui8Index);

static inline bool _isOctetOk(const String& s);
static bool tcpProbeFast(const char* ipStr, uint16_t port, uint16_t timeoutMs);
void WSER_HandleStatusRemoteRelays(RemoteRelaySlot_s* SlotRelay, uint8_t slot_numb);
bool WSER_Conn_Exec_RemoteRelay(RemoteRelaySlot_s* SlotRelay);

//smart home commands
bool WSER_ExtCmdSaveHwRel(uint8_t ctrl, uint8_t RelayIdx);
bool WSER_ExtCmdSaveSlot(uint8_t slotNum, const String& ip1, const String& ip2, const String& ip3, const String& ip4);
bool WSER_ExtCmdFreeSlot(uint8_t slotnum);
//-----------------------------------------------------------------------------
//--- PUBLIC Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void   WSER_InitRemoteRelays(){

  uint8_t flag     = EEPR_ReadByte(EEPROM_INIT_FLAG_REMOTE_SLOT_ADDR);
  uint8_t flag_neg = EEPR_ReadByte(EEPROM_INIT_FLAG_REMOTE_SLOT_NEG_ADDR);

  // 🔁 Se i flag sono corrotti o non inizializzati, inizializza l'area EEPROM
  if (flag != EEPROM_INIT_REMOTE_SLOT_FLAG || flag_neg != EEPROM_INIT_REMOTE_SLOT_NEG) {

    EEPR_WriteCh16(SLOT_1_IP_ADDR, "                ");
    EEPR_WriteCh16(SLOT_2_IP_ADDR, "                ");
    EEPR_WriteCh16(SLOT_3_IP_ADDR, "                ");
    EEPR_WriteCh16(SLOT_4_IP_ADDR, "                ");

    EEPR_WriteByte(RELAY_CONTROL_1_ADDR, 1);
    EEPR_WriteByte(RELAY_CONTROL_2_ADDR, 0);
    EEPR_WriteByte(RELAY_CONTROL_3_ADDR, 2);
    EEPR_WriteByte(RELAY_CONTROL_4_ADDR, 3);

    EEPR_WriteByte(EEPROM_INIT_FLAG_REMOTE_SLOT_ADDR, EEPROM_INIT_REMOTE_SLOT_FLAG);
    EEPR_WriteByte(EEPROM_INIT_FLAG_REMOTE_SLOT_NEG_ADDR, EEPROM_INIT_REMOTE_SLOT_NEG);
  }

  // ----------- Slot 0 -----------
  EEPR_ReadCh16(SLOT_1_IP_ADDR, RemoteRelay[0].ip);

  if (strlen(RemoteRelay[0].ip) == 0 || String(RemoteRelay[0].ip) == "                ") {
    RemoteRelay[0].state = RELAY_SLOT_NOT_ASSIGNED;
    strcpy(RemoteRelay[0].id, "NOT ASSIGNED");
  } else {
    RemoteRelay[0].state = RELAY_SLOT_ASSIGNED_NOT_PRESENT;
    strcpy(RemoteRelay[0].id, "NOT PRESENT");
  }
  RemoteRelay[0].command_req       = RELAY_CMD_NO_REQ;
  RemoteRelay[0].last_connection_ms= 0; //ms last connection 

  s_Ctrl[0].HwRelay = EEPR_ReadByte(RELAY_CONTROL_1_ADDR);

  // ----------- Slot 1 -----------
  EEPR_ReadCh16(SLOT_2_IP_ADDR, RemoteRelay[1].ip);

  if (strlen(RemoteRelay[1].ip) == 0 || String(RemoteRelay[1].ip) == "                ") {
    RemoteRelay[1].state = RELAY_SLOT_NOT_ASSIGNED;
    strcpy(RemoteRelay[1].id, "NOT ASSIGNED");
  } else {
    RemoteRelay[1].state = RELAY_SLOT_ASSIGNED_NOT_PRESENT;
    strcpy(RemoteRelay[1].id, "NOT PRESENT");
  }
  RemoteRelay[1].command_req       = RELAY_CMD_NO_REQ;
  RemoteRelay[1].last_connection_ms = 0;

  s_Ctrl[1].HwRelay = EEPR_ReadByte(RELAY_CONTROL_2_ADDR);

  // ----------- Slot 2 -----------
  EEPR_ReadCh16(SLOT_3_IP_ADDR, RemoteRelay[2].ip);

  if (strlen(RemoteRelay[2].ip) == 0 || String(RemoteRelay[2].ip) == "                ") {
    RemoteRelay[2].state = RELAY_SLOT_NOT_ASSIGNED;
    strcpy(RemoteRelay[2].id, "NOT ASSIGNED");
  } else {
    RemoteRelay[2].state = RELAY_SLOT_ASSIGNED_NOT_PRESENT;
    strcpy(RemoteRelay[2].id, "NOT PRESENT");
  }
  RemoteRelay[2].command_req       = RELAY_CMD_NO_REQ;
  RemoteRelay[2].last_connection_ms = 0;

  s_Ctrl[2].HwRelay = EEPR_ReadByte(RELAY_CONTROL_3_ADDR);

  // ----------- Slot 3 -----------
  EEPR_ReadCh16(SLOT_4_IP_ADDR, RemoteRelay[3].ip);

  if (strlen(RemoteRelay[3].ip) == 0 || String(RemoteRelay[3].ip) == "                ") {
    RemoteRelay[3].state = RELAY_SLOT_NOT_ASSIGNED;
    strcpy(RemoteRelay[3].id, "NOT ASSIGNED");
  } else {
    RemoteRelay[3].state = RELAY_SLOT_ASSIGNED_NOT_PRESENT;
    strcpy(RemoteRelay[3].id, "NOT PRESENT");
  }
  RemoteRelay[3].command_req       = RELAY_CMD_NO_REQ;
  RemoteRelay[3].last_connection_ms = 0;

  s_Ctrl[3].HwRelay = EEPR_ReadByte(RELAY_CONTROL_4_ADDR);


}

/**

 */
void WSER_RemoteRelaysLoop() {
  static uint8_t ui8IndexRelay = 0;

  if (millis() - lastCheckTimeRemoteRelays > REMOTE_RELAYS_CHECK_INTERVAL) {
    lastCheckTimeRemoteRelays = millis();

    WSER_SetHwRelayString (ui8IndexRelay);

    WSER_HandleStatusRemoteRelays(&RemoteRelay[ui8IndexRelay], ui8IndexRelay);

    ui8IndexRelay = (ui8IndexRelay + 1) % MAX_REMOTE_RELAYS;
  }
}

//-----------------------------------------------------------------------------
//--- Private Relay handing functions-------- ---------------------------------
//-----------------------------------------------------------------------------
/**

 */
void WSER_SetHwRelayString(uint8_t ui8Index) {
  // 🔧 Placeholder provvisorio: visualizza solo relay e stato ON/OFF base
  // TODO: estendere con logica timer, controllo stato, relè associato.

  if ( 0 == s_Ctrl[ui8Index].HwRelay){
    s_Ctrl[ui8Index].messStatusRow1 = "🔌 Hw Relay: 🚫";
    s_Ctrl[ui8Index].messStatusRow2 = "🔌 Relay Status: 🚫 ";
  } else {
    s_Ctrl[ui8Index].messStatusRow1 = "🔌 Hw Relay: " +  String(s_Ctrl[ui8Index].HwRelay);
    s_Ctrl[ui8Index].messStatusRow2 = (s_Ctrl[ui8Index].CtrlSt) ? "🔌 Relay Status: ON 🟢" : "🔌 Relay Status: OFF 🔴";

  }

}

/**

 */
void WSER_HandleStatusRemoteRelays(RemoteRelaySlot_s* SlotRelay, uint8_t slot_numb) {

  String ip = String(SlotRelay->ip);
  ip.trim();

  // --- Se IP non valido o non assegnato ---
  if (ip.isEmpty() || ip == "0.0.0.0" || ip.indexOf("Not") >= 0) {
    SlotRelay->state = RELAY_SLOT_NOT_ASSIGNED;
    SlotRelay->messStatusRow1 = "🌐 Remote Relay 🔗 IP: 🚫 ";
    SlotRelay->messStatusRow2 = "⏳ ...";
    return;
  }

  switch (SlotRelay->state) {

    // --- Slot non assegnato ---
    case RELAY_SLOT_NOT_ASSIGNED:
      // già gestito sopra
      break;

    // --- Relay assegnato ma non presente ---
    case RELAY_SLOT_ASSIGNED_NOT_PRESENT:
    {
      if (millis() - SlotRelay->last_connection_ms >= REMOTE_RELAY_CONNETION_TIME_REFRESH) {
        SlotRelay->last_connection_ms = millis();
        SlotRelay->command_req = RELAY_CMD_CONNECT;

        if (WSER_Conn_Exec_RemoteRelay(SlotRelay)) {
          SlotRelay->messStatusRow1 = "🌐 Remote Relay 🔗 IP: " + String(SlotRelay->ip);
          SlotRelay->messStatusRow2 = "✅ Connection ok - 📶 SSID: " + String(SlotRelay->id);
          SlotRelay->state = RELAY_SLOT_ASSIGNED_PRESENT;
        } else {
          SlotRelay->messStatusRow1 = "🌐 Remote Relay 🔗 IP: " + String(SlotRelay->ip);
          SlotRelay->messStatusRow2 = "❌ Connection Failed - 📶 SSID: Not Present....";
          // ritenta tra 5 secondi per evitare stop ciclici
          SlotRelay->last_connection_ms = millis() + 5000;
        }
      }
    }
    break;

    // --- Relay assegnato e presente ---
    case RELAY_SLOT_ASSIGNED_PRESENT:
    {
      if (millis() - SlotRelay->last_connection_ms >= REMOTE_RELAY_CONNETION_TIME_REFRESH) {
        SlotRelay->last_connection_ms = millis();

        if (WSER_Conn_Exec_RemoteRelay(SlotRelay)) {
          SlotRelay->messStatusRow1 = "🌐 Remote Relay 🔗 IP: " + String(SlotRelay->ip);
          SlotRelay->messStatusRow2 = "✅ Command Executed - 📶 SSID: " + String(SlotRelay->id);
          SlotRelay->state = RELAY_SLOT_ASSIGNED_PRESENT;
        } else {
          SlotRelay->messStatusRow1 = "🌐 Remote Relay 🔗 IP: " + String(SlotRelay->ip);
          SlotRelay->messStatusRow2 = "❌ Connection Lost - 📶 SSID: " + String(SlotRelay->id);
          SlotRelay->state = RELAY_SLOT_ASSIGNED_NOT_PRESENT;
        }
      }
    }
    break;

    // --- Slot da liberare ---
    case RELAY_SLOT_FREE_SLOT:
    {
      if (millis() - SlotRelay->last_connection_ms >= REMOTE_RELAY_CONNETION_TIME_REFRESH) {
        SlotRelay->last_connection_ms = millis();
        SlotRelay->command_req = RELAY_CMD_TURN_OFF;

        if (WSER_Conn_Exec_RemoteRelay(SlotRelay)) {
          SlotRelay->messStatusRow1 = "🌐 Remote Relay 🔗 IP: 🚫";
          SlotRelay->messStatusRow2 = "✅ Remote Relay Slot Free";
        } else {
          SlotRelay->messStatusRow1 = "🌐 Remote Relay 🔗 IP: " + String(SlotRelay->ip);
          SlotRelay->messStatusRow2 = "❌ No Connection - 📶 SSID: " + String(SlotRelay->id);
        }

        // Cancella dati slot in EEPROM e RAM
        switch (slot_numb) {
          case 0: EEPR_WriteCh16(SLOT_1_IP_ADDR, "                "); break;
          case 1: EEPR_WriteCh16(SLOT_2_IP_ADDR, "                "); break;
          case 2: EEPR_WriteCh16(SLOT_3_IP_ADDR, "                "); break;
          case 3: EEPR_WriteCh16(SLOT_4_IP_ADDR, "                "); break;
        }

        strcpy(SlotRelay->ip, "0.0.0.0");
        strcpy(SlotRelay->id, "NOT ASSIGNED");
        SlotRelay->messStatusRow1 = "🌐 Remote Relay 🔗 IP: 🚫";
        SlotRelay->messStatusRow2 = "🗑️ Slot released";
        SlotRelay->state = RELAY_SLOT_NOT_ASSIGNED;
        SlotRelay->command_req = RELAY_CMD_NO_REQ;
        SlotRelay->last_connection_ms = 0;
      }
    }
    break;
  }
}


/**

 */
bool WSER_Conn_Exec_RemoteRelay(RemoteRelaySlot_s* SlotRelay) {
  String ipStr = String(SlotRelay->ip);
  ipStr.trim();
  if (ipStr.isEmpty() || ipStr == "0.0.0.0") return false;

  // probe TCP rapido
  bool reachable = tcpProbeFast(ipStr.c_str(), 80, REMOTE_RELAY_TCP_PROBE_TIMEOUT_MS);
  if (!reachable) return false;

  const String url = "http://" + ipStr + "/smartcommand";

  // TODO: URLEncode(...) se SSID/IP possono avere spazi/caratteri speciali
  String identData =
      "&ssid_ap="  + String(WiFi.softAPSSID()) +
      "&ip_ap="    + WiFi.softAPIP().toString() +
      "&ssid_sta=" + String(WiFi.SSID()) +
      "&ip_sta="   + WiFi.localIP().toString();

  String postData;
  switch (SlotRelay->command_req) {
    case RELAY_CMD_CONNECT:
      postData = "cmd=setworkingmode&mode=server" + identData;
      break;
    case RELAY_CMD_TURN_ON:
      if (SlotRelay->state != RELAY_SLOT_ASSIGNED_PRESENT) return false;
      postData = "cmd=setserverstatus&status=on" + identData;
      break;
    case RELAY_CMD_TURN_OFF:
      if (SlotRelay->state != RELAY_SLOT_ASSIGNED_PRESENT && SlotRelay->state != RELAY_SLOT_FREE_SLOT) return false;
      postData = "cmd=setserverstatus&status=off" + identData;
      break;
    case RELAY_CMD_NO_REQ:
      return true; // niente da fare
    default:
      return false;
  }
  if (postData.isEmpty()) return false;

  WiFiClient client;
  client.setTimeout(120);

  HTTPClient http;
  http.setTimeout(200);

  if (!http.begin(client, url)) return false;
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(postData);

  // ok solo 200/204
  if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_NO_CONTENT) {
    String payload = http.getString();

    // Parse ID (opzionale)
    int idPos = payload.indexOf("ID:");
    if (idPos >= 0) {
      size_t start = idPos + 3;
      while (start < payload.length() && (payload[start] == ' ' || payload[start] == '\t')) start++;
      size_t end = start;
      while (end < payload.length() && payload[end] != '\r' && payload[end] != '\n') end++;
      String devId = payload.substring(start, end);
      devId.trim();

      // keep only safe chars
      String clean; clean.reserve(devId.length());
      for (size_t i = 0; i < devId.length(); ++i) {
        char c = devId[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
            c == '_' || c == '-' || c == '.' || c == ':') {
          clean += c;
        }
      }
      strncpy(SlotRelay->id, clean.c_str(), sizeof(SlotRelay->id) - 1);
      SlotRelay->id[sizeof(SlotRelay->id) - 1] = '\0';
    }
    http.end();
    return true;
  }

  http.end();
  return false;
}


/**

 */
static bool tcpProbeFast(const char* ipStr, uint16_t port, uint16_t timeoutMs) {
  String ip = String(ipStr);
  ip.trim();
  if (ip.isEmpty()) return false;

  WiFiClient probe;
  probe.setTimeout(timeoutMs);
  bool ok = probe.connect(ip.c_str(), port);
  probe.stop();
   return true;
}

//-----------------------------------------------------------------------------
//--- Public Web Server Functions Definitions ---------------------------------
//-----------------------------------------------------------------------------
/**

 */
void WSER_handleRemoteRelaysGetStatus() {

  String json = "{";

  // Relay locali (s_Ctrl)
  json += "\"MessStatusRelay1Row1\":\"" + s_Ctrl[0].messStatusRow1 + "\",";  
  json += "\"MessStatusRelay1Row2\":\"" + s_Ctrl[0].messStatusRow2 + "\",";  
  json += "\"MessStatusRelay2Row1\":\"" + s_Ctrl[1].messStatusRow1 + "\",";  
  json += "\"MessStatusRelay2Row2\":\"" + s_Ctrl[1].messStatusRow2 + "\",";  
  json += "\"MessStatusRelay3Row1\":\"" + s_Ctrl[2].messStatusRow1 + "\",";  
  json += "\"MessStatusRelay3Row2\":\"" + s_Ctrl[2].messStatusRow2 + "\",";  
  json += "\"MessStatusRelay4Row1\":\"" + s_Ctrl[3].messStatusRow1 + "\",";  
  json += "\"MessStatusRelay4Row2\":\"" + s_Ctrl[3].messStatusRow2 + "\"";  
 
  json += "}";

  server.send(200, "application/json", json);
}

/**

 */
void WSER_WiFiRemoteRelaysPage() {
  Serial.print("REMOTERELAYS: Heap disponibile prima invio: ");
  Serial.println(ESP.getFreeHeap());

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send_P(200, "text/html", WSER_REMOTE_REL_HEAD);

  server.sendContent_P(WSER_COMMON_PAGES_STYLE);
  server.sendContent_P(WSER_REMOTE_REL_STYLE);
  server.sendContent_P(WSER_REMOTE_REL_HEAD_OPEN);

  String temp = WSER_GenerateNavBar("remoterelays");
  server.sendContent(temp);

  server.sendContent_P(WSER_REMOTE_REL_BODY_CONTENT);

  server.sendContent_P(WSER_REMOTE_REL_SCRIPT);
  server.sendContent_P(WSER_REMOTE_REL_CLOSE_PAGE);

  Serial.print("REMOTERELAYS: Heap disponibile dopo invio: ");
  Serial.println(ESP.getFreeHeap());
}

/**

 */
void WSER_handleSaveRelay() {
  if (!server.hasArg("slot") || !server.hasArg("hwrel")) {
    server.send(400, "text/plain", "Missing parameters: slot, hwrel");
    return;
  }


  int slotOneBased = server.arg("slot").toInt();   // atteso 1..4
  int hwRelayInt   = server.arg("hwrel").toInt();  // atteso 0..3

  // Normalizza/valida
  if (slotOneBased < 1 || slotOneBased > 4 || hwRelayInt < 0 || hwRelayInt > 3) {
    server.send(400, "text/plain", "Invalid values: slot(1..4), hwrel(0..3)");
    return;
  }

  uint8_t ctrl    = (uint8_t)(slotOneBased - 1);   // 0..3
  uint8_t hwRelay = (uint8_t)hwRelayInt;           // 0..3

  // Salva con regole di unicità
  if (WSER_ExtCmdSaveHwRel(ctrl, hwRelay)) {
    server.send(200, "text/plain", "Hw Relay stored");
  } else {
    server.send(409, "text/plain", "Hw Relay already used"); // 409 Conflict
  }
}


/**

 */
bool WSER_ExtCmdSaveHwRel(uint8_t ctrl, uint8_t RelayIdx) {
  // Validazioni base
  if (ctrl > 3 || RelayIdx > 3) return false;

  // Salvo il vecchio relay associato a questo controllo
  uint8_t oldRelayIdx = s_Ctrl[ctrl].HwRelay;

  if (RelayIdx != 0) {
    for (uint8_t i = 0; i < 4; ++i) {
      if (i == ctrl) continue;
      if (s_Ctrl[i].HwRelay == RelayIdx) return false;
    }
  }

  if (oldRelayIdx != 0 && oldRelayIdx != RelayIdx) {
    switch (oldRelayIdx) {
      case 1: digitalWrite(RELAY_1, LOW); break;
      case 2: digitalWrite(RELAY_2, LOW); break;
      case 3: digitalWrite(RELAY_3, LOW); break;
    }
  }

  EEPR_WriteByte(RELAY_CONTROL_1_ADDR + ctrl, RelayIdx);
  s_Ctrl[ctrl].HwRelay = EEPR_ReadByte(RELAY_CONTROL_1_ADDR + ctrl);

  return true;
}

/**

 */
static inline bool _isOctetOk(const String& s) {
  if (s.length() == 0 || s.length() > 3) return false;
  for (uint16_t i = 0; i < s.length(); ++i) if (!isDigit(s[i])) return false;
  int v = s.toInt();
  return v >= 0 && v <= 255;
}

/**

 */
bool WSER_ExtCmdSaveSlot(uint8_t slotNum, const String& ip1, const String& ip2, const String& ip3, const String& ip4) {

  if (slotNum < 1 || slotNum > 4) return false;

  if (!_isOctetOk(ip1) || !_isOctetOk(ip2) || !_isOctetOk(ip3) || !_isOctetOk(ip4)) return false;

  char ipBuf[17];
  snprintf(ipBuf, sizeof(ipBuf), "%d.%d.%d.%d", ip1.toInt(), ip2.toInt(), ip3.toInt(), ip4.toInt());

  uint8_t index = (uint8_t)(slotNum - 1);

  switch (slotNum) {
    case 1:
      EEPR_WriteCh16(SLOT_1_IP_ADDR, ipBuf);
      EEPR_ReadCh16(SLOT_1_IP_ADDR, RemoteRelay[index].ip);
      break;
    case 2:
      EEPR_WriteCh16(SLOT_2_IP_ADDR, ipBuf);
      EEPR_ReadCh16(SLOT_2_IP_ADDR, RemoteRelay[index].ip);
      break;
    case 3:
      EEPR_WriteCh16(SLOT_3_IP_ADDR, ipBuf);
      EEPR_ReadCh16(SLOT_3_IP_ADDR, RemoteRelay[index].ip);
      break;
    case 4:
      EEPR_WriteCh16(SLOT_4_IP_ADDR, ipBuf);
      EEPR_ReadCh16(SLOT_4_IP_ADDR, RemoteRelay[index].ip);
      break;
    default:
      return false;
  }

  RemoteRelay[index].ip[16] = '\0';

  RemoteRelay[index].state = RELAY_SLOT_ASSIGNED_NOT_PRESENT;

  return true;
}



/**

 */
bool WSER_ExtCmdFreeSlot(uint8_t slotnum) {
  if ((slotnum > 4) || (slotnum < 1)) {
    return false;
  }

  RemoteRelay[slotnum - 1].state      = RELAY_SLOT_FREE_SLOT;

  return true;
}
