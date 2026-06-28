#include "10_WebServerMain.h"
#include "14_ConfigPage_private.h"

//--- Public Variables -------------------------------------------------------
// AP global variables 
char ap_ssid[32] = SSID_AP_DEFAULT;
char ap_pass[32] = SSID_PWD_DEFAULT;

// STA global variables
char sta_ssid[32] = SSID_STA_DEFAULT;
char sta_pass[32] = SSID_STA_PWD_DEFAULT;

// EMAIL contacts global variables
char email_receiver[32] = EMAIL_RECEIVER_DEFAULT;
char author_email[32]   = AUTHOR_EMAIL_DEFAULT;
char author_pwd[32]     = AUTHOR_PASSWORD_DEFAULT;

char email_Copilot_receiver[32] = "edulcowaternetsky@gmail.com";
//-------------------------------------------------------------------------


String sta_ssid_message   = "ToDo";
String sta_ip_message     = "ToDo";
String email_message      = "ToDo";
String email_sender_message      = "ToDo";

IPAddress apIP(192, 168, 4, 1);  
IPAddress gateway(192, 168, 100, 1);
IPAddress subnet(255, 255, 255, 0);

//--- Private Function Declarations ------------------------------------------
//internal fucntions
void WSER_startAP();
String getShortID();

bool WSER_checkWiFiConditions();
bool WSER_connectToWiFi();

//--- Public Function Declarations -------------------------------------------
void WSER_InitWiFiSection();
void WSER_WiFiConfigLoop();

//web page fucntions
void WSER_WiFiConfigPage();

//post and get fucntions
void WSER_handleGetStatus();
void WSER_handleSaveAP();
void WSER_handleSaveSTA();
void WSER_handleForgetSTA();

void WSER_handleSaveEmailSender();

void WSER_handleSaveEmail();

bool WSER_IsEmailDefined();
bool WSER_IsAccessPointMode();

//-----------------------------------------------------------------------------
//--- PUBLIC Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */

void WSER_InitWiFiSection(){ // << Chiamata al nuovo modulo separato

  // get the eeprom credentials
  //-------------
  //check if eeprom was initialized
    uint8_t FlagEErpmWebCred     = EEPR_ReadByte(EEPROM_INIT_FLAG_CREDENTIALS_ADDR);
    uint8_t FlagEErpmWebCredNeg  = EEPR_ReadByte(EEPROM_INIT_FLAG_CREDENTIALS_NEG_ADDR);

    if ((FlagEErpmWebCred != EEPROM_INIT_CREDENTIALS_FLAG) || (FlagEErpmWebCredNeg != EEPROM_INIT_FLAG_CREDENTIALS_NEG)){
      Serial.println("Default values written since no values found in WEB credentials eeprom section");

        String ssid_AccessPoint = String(SSID_AP_DEFAULT) + getShortID();

      EEPR_WriteCh32(AP_SSID_EEPROM_ADDR, ssid_AccessPoint.c_str());

      String pwd_AccessPoint = SSID_PWD_DEFAULT;
      EEPR_WriteCh32(AP_PWD_EEPROM_ADDR, pwd_AccessPoint.c_str());

      //---STATION credentials default----

      String ssid_Station = SSID_STA_DEFAULT;
      EEPR_WriteCh32  (STA_SSID_EEPROM_ADDR,      ssid_Station.c_str());

      String pwd_Station = SSID_STA_PWD_DEFAULT;
      EEPR_WriteCh32  (STA_PWD_EEPROM_ADDR,       pwd_Station.c_str());

      String email_Contact = EMAIL_RECEIVER_DEFAULT;
      EEPR_WriteCh32  (RECEIVER_EMAIL_ADDR, email_Contact.c_str());

      String email_Author = AUTHOR_EMAIL_DEFAULT;
      EEPR_WriteCh32  (SENDER_EMAIL_ADDR, email_Author.c_str());

      String pwd_Author = AUTHOR_PASSWORD_DEFAULT;
      EEPR_WriteCh32  (SENDER_PWD_ADDR, pwd_Author.c_str());

      //---- Init EEProm as initialized----
      EEPR_WriteByte(EEPROM_INIT_FLAG_CREDENTIALS_ADDR        ,   EEPROM_INIT_CREDENTIALS_FLAG    );
      EEPR_WriteByte(EEPROM_INIT_FLAG_CREDENTIALS_NEG_ADDR    ,   EEPROM_INIT_FLAG_CREDENTIALS_NEG);
    }else{
        //Serial.println("WEB credentials eeprom section already initialized");
    }

  //get credentials from eeprom
    EEPR_ReadCh32(AP_SSID_EEPROM_ADDR, ap_ssid);
    EEPR_ReadCh32(AP_PWD_EEPROM_ADDR, ap_pass);
    EEPR_ReadCh32(STA_SSID_EEPROM_ADDR, sta_ssid);
    EEPR_ReadCh32(STA_PWD_EEPROM_ADDR, sta_pass);

    EEPR_ReadCh32(RECEIVER_EMAIL_ADDR, email_receiver);
    EEPR_ReadCh32(SENDER_EMAIL_ADDR, author_email);
    EEPR_ReadCh32(SENDER_PWD_ADDR, author_pwd);

  // Print sender credentials
  Serial.println("Sender Email: ");
  Serial.println(author_email);
  Serial.println("Sender Password: ");
  Serial.println(author_pwd);

  // --- Imposta il messaggio in base al contenuto ---
  if (strlen(email_receiver) == 0 || strcmp(email_receiver, EMAIL_RECEIVER_DEFAULT) == 0 ) {
    email_message = "Contact email missing";
    Serial.println("[Init] Nessuna email impostata (default/missing)");
  } else {
    email_message = String(email_receiver);
    Serial.printf("[Init] Email attiva: %s\n", email_receiver);
  }

  if (strcmp(author_email, AUTHOR_EMAIL_DEFAULT) == 0) {
    email_sender_message = String(author_email) + " (Limited)";
  } else {
    email_sender_message = String(author_email);
  }

  //------Access point start-
    WiFi.mode(WIFI_AP_STA);

    WSER_startAP();
  //------Station Start-

  if (WSER_checkWiFiConditions()) {
    Serial.println("✅ WiFi present and strong enough. Starting Station connection...");
    WSER_connectToWiFi();
  } else {
    Serial.println("⚠️ WiFi not present or too weak. Station connection not started.");
  }

}


/**

 */
#define WIFI_CHECK_INTERVAL 10000  // ogni 10 secondi

void WSER_WiFiConfigLoop() {
  static unsigned long lastCheck = 0;

  if (millis() - lastCheck > WIFI_CHECK_INTERVAL) {
    lastCheck = millis();

    if (WiFi.status() == WL_CONNECTED) {
      String currentIP = WiFi.localIP().toString();

      if (currentIP != sta_ip_message) {
        sta_ip_message = currentIP;
        Serial.print("📶 Station IP updated: ");
        Serial.println(sta_ip_message);
      }
      // Altrimenti non fare nulla
    } else {
      Serial.println("⚠️ Station mode not connected");
    }
  }
}

//-----------------------------------------------------------------------------
//--- PRIVATE Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
bool WSER_checkWiFiConditions() {
  if ((sta_ssid == SSID_STA_DEFAULT) || (sta_pass == SSID_STA_PWD_DEFAULT)) {
    sta_ssid_message = "No SSID stored";
    sta_ip_message = "No PWD stored";
    Serial.println("No credentials stored, therefore only in AP mode");
    return false;
  }

  Serial.println("Credentials found... starting to scan WiFi networks. Available networks:");
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    String currentSSID = WiFi.SSID(i);
    int currentRSSI = WiFi.RSSI(i);
    //bool isOpen = (WiFi.encryptionType(i) == AUTH_OPEN);
    bool isOpen = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN);

    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(currentSSID);
    Serial.print(" (");
    Serial.print(currentRSSI);
    Serial.print(" dBm)");
    Serial.println(isOpen ? " " : "*");

    if (currentSSID == sta_ssid) {
      Serial.print("Found stored SSID: ");
      Serial.println(currentSSID);

      if (currentRSSI >= WIFI_MIN_RSSI_THRESHOLD) {
        Serial.print("Signal good enough. Attempting connection to ");
        Serial.println(currentSSID);
        return true;
      } else {
        Serial.print("Signal strength too weak for connection: ");
        Serial.println(currentSSID);
        sta_ssid_message = String(sta_ssid) + " present";
        sta_ip_message = "Not connected, weak signal";
        return false;
      }
    }
  }

  // If no matching SSID was found at all
  Serial.println("Stored SSID not found in scanned networks.");
  sta_ssid_message = String(sta_ssid) + " not found";
  sta_ip_message = "Cannot connect";
  return false;  // fallback di sicurezza
}

/**

 */
bool WSER_connectToWiFi() {
  Serial.print("📶 Connecting to SSID: ");
  Serial.println(sta_ssid);
  Serial.print("🔐 Using Password: ");
  Serial.println(sta_pass);  // ⚠️ visibile solo per debug, rimuovere in produzione

  WiFi.begin(sta_ssid, sta_pass);

  for (int attempt = 1; attempt <= WIFI_MAX_ATTEMPTS; attempt++) {
    Serial.print("🔄 Attempt ");
    Serial.print(attempt);
    Serial.print(" of ");
    Serial.println(WIFI_MAX_ATTEMPTS);

    unsigned long startAttemptTime = millis();
    while (millis() - startAttemptTime < WIFI_ATTEMPT_INTERVAL) {
      delay(100);  // piccola pausa per evitare busy loop
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("✅ WiFi connected!");
        Serial.print("📡 IP address: ");
        Serial.println(WiFi.localIP());
        sta_ssid_message = String(sta_ssid);
        sta_ip_message = WiFi.localIP().toString();
        //connection happened with success... therefore store in eeprom
        EEPR_WriteCh32(STA_SSID_EEPROM_ADDR, sta_ssid);
        EEPR_WriteCh32(STA_PWD_EEPROM_ADDR, sta_pass);
        return true;
      }

    }
  }

  Serial.println("❌ Failed to connect to WiFi after max attempts.");
  sta_ssid_message = String(sta_ssid) + " (failed)";
  sta_ip_message = "Not connected";
  return false;
}

/**

 */
void WSER_handleSaveEmailSender()
{
  Serial.println("[handleSaveEmailSender] POST request received.");

  // --- Controllo parametri richiesti ---
  if (!server.hasArg("email") || !server.hasArg("password")) 
  {
    Serial.println("[handleSaveEmailSender] ❌ Missing parameters (email/password).");
    server.send(400, "application/json", "{\"error\":\"Missing parameters\"}");
    return;
  }

  // --- Prelievo, normalizzazione ---
  String email = server.arg("email");  email.trim(); email.toLowerCase();
  String pwd   = server.arg("password"); pwd.trim();

  EEPR_WriteCh32(SENDER_EMAIL_ADDR, email.c_str());
  EEPR_WriteCh32(SENDER_PWD_ADDR, pwd.c_str());

    // --- Read-back per conferma sui tuoi buffer globali ---
  EEPR_ReadCh32(SENDER_EMAIL_ADDR, author_email);
  EEPR_ReadCh32(SENDER_PWD_ADDR , author_pwd);

  // --- Reconnect email client ---
  EML_EmailReconnect();

  if (strcmp(email.c_str(), AUTHOR_EMAIL_DEFAULT) == 0) {
    email_sender_message = String(email) + " (Limited)";
  } else {
    email_sender_message = String(email);
  }

  // --- Tutti i parametri presenti ---
  Serial.print("[handleSaveEmailSender] Email: ");
  Serial.println(server.arg("email"));
  Serial.println("[handleSaveEmailSender] Transferring sender credentials...");

  server.send(200, "text/plain", "Transferring sender credentials");
}


/**

 */
void WSER_handleSaveAP() {

  String sap_ssid = server.arg("ssid");   // Parametro SSID
  String sap_pass = server.arg("password"); // Parametro Password
  
  sap_ssid.toCharArray(ap_ssid, 32);
  sap_pass.toCharArray(ap_pass, 32);

  Serial.println("Save access Point with crdentials");
  Serial.println (sap_ssid);
  Serial.println (sap_pass);

  EEPR_WriteCh32(AP_SSID_EEPROM_ADDR, ap_ssid);   // Salva SSID
  EEPR_WriteCh32(AP_PWD_EEPROM_ADDR, ap_pass);   // Salva password
  
  server.send(200, "text/plain", "Configuration saved successfully.");

  delay(1000);
  WSER_startAP(); 
}



/**

 */
void WSER_handleSaveEmail() {
  Serial.println("[WSER_handleSaveEmail] Entrato in save email");

  // --- Verifica presenza parametro ---
  if (!server.hasArg("email")) {
    Serial.println("[WSER_handleSaveEmail] Errore: parametro 'email' mancante");
    server.send(400, "text/plain", "Error: missing 'email' parameter");
    return;
  }

  String email = server.arg("email");

  EEPR_WriteCh32(RECEIVER_EMAIL_ADDR, email.c_str());
  Serial.printf("[WSER_handleSaveEmail] Email salvata: %s\n", email.c_str());

  // --- Gestione messaggio email ---
  if (email.length() == 0 || email == EMAIL_RECEIVER_DEFAULT || email == "none") {
    email_message = "Contact email missing";
    Serial.println("[WSER_handleSaveEmail] Nessuna email impostata (default)");
  } else {
    email_message = email;
    Serial.printf("[WSER_handleSaveEmail] Email attiva: %s\n", email.c_str());
  }

  strncpy(email_receiver, email.c_str(), sizeof(email_receiver) - 1);
  email_receiver[sizeof(email_receiver) - 1] = '\0'; // garantisce terminatore

  // --- Risposta OK ---
  server.send(200, "text/plain", "Email saved successfully");
}


/**

 */
void WSER_handleSaveSTA() {
  Serial.println("🔧 [WSER_handleSaveSTA] Called");

  // 1. Store the current working credentials in temporary strings
  String sta_ssid_temp = sta_ssid;
  String sta_pass_temp = sta_pass;

  // 2. Get the new credentials from the HTTP POST request
  String sta_ssid_new = server.arg("station_ssid");
  String sta_pass_new = server.arg("station_password");

  // Update the working variables with the new ones (used in connection)
  strcpy(sta_ssid, sta_ssid_new.c_str());
  strcpy(sta_pass, sta_pass_new.c_str());

  // 3. Check if the new SSID is visible and strong enough
  Serial.println("📡 Scanning for networks...");
  if (!WSER_checkWiFiConditions()) {
    Serial.println("❌ New SSID not found or signal too weak. Keeping previous credentials.");

    // Restore previous credentials
    strcpy(sta_ssid, sta_ssid_temp.c_str());
    strcpy(sta_pass, sta_pass_temp.c_str());

    // Se siamo già connessi in STA, aggiorna i messaggi visibili
    if (WiFi.status() == WL_CONNECTED) {
      sta_ssid_message = sta_ssid_temp;
      sta_ip_message   = WiFi.localIP().toString();
    } 

    server.send(200, "application/json", "{\"status\":\"success\",\"result\":\"fail\",\"message\":\"New SSID not found or too weak\"}");
    return;
  }

    // 5. SSID found and signal ok – inform client that connection will proceed
  server.send(200, "application/json", 
    "{\"status\":\"success\",\"result\":\"ok\",\"message\":\"SSID found. Connecting...\"}");


  // 4. SSID found with acceptable signal, notify and start connection
  if (!WSER_connectToWiFi()) {
    Serial.println("❌ Connection failed, restoring previous credentials...");
    strcpy(sta_ssid, sta_ssid_temp.c_str());
    strcpy(sta_pass, sta_pass_temp.c_str());
  }
}

/**

 */
void WSER_handleForgetSTA(){
  String ssidAP = WiFi.softAPSSID();  // ottiene SSID AP corrente
  Serial.println("Entrato in forget station!");
  server.send(200, "text/plain", ssidAP);
  delay(150); // chiude bene la HTTP

  Serial.println(F("[EEPROM] Clearing STA credentials..."));
  EEPR_WriteCh32(STA_SSID_EEPROM_ADDR, "");
  EEPR_WriteCh32(STA_PWD_EEPROM_ADDR, "");
  // Aggiorna anche le copie in RAM
  strcpy(sta_ssid, "");
  strcpy(sta_pass, "");
  sta_ssid_message = "Cleared";
  sta_ip_message   = "Not connected";

  Serial.println(F("[WiFi] Disconnecting STA..."));
  WiFi.disconnect();            // non serve cancellare la radio, le credenziali sono già vuote
  delay(100);

  // (resti in WIFI_AP_STA: l’AP continua a servire la pagina)
  Serial.print(F("[WiFi] AP still active: "));
  Serial.print(WiFi.softAPSSID());
  Serial.print(F("  IP: "));
  Serial.println(WiFi.softAPIP());

}


/**

 */
void WSER_startAP(){

    WiFi.softAPConfig(apIP, gateway, subnet);

    WiFi.softAP(ap_ssid, ap_pass);

    delay(100); // Stabilizza AP
    Serial.print("ACCESS POINT ssid: " + String(ap_ssid) + "; Password: " + String(ap_pass) + "; IP: ");
    Serial.println(WiFi.softAPIP());
  }

//-----------------------------------------------------------------------------
//--- Private Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void WSER_WiFiConfigPage() {
  Serial.print("WIFICONFIG: Heap available before send page: ");
  Serial.println(ESP.getFreeHeap());

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send_P(200, "text/html", WSER_WIFI_CONFIG_PAGE_HEAD);

  server.sendContent(WSER_COMMON_PAGES_STYLE);
  server.sendContent_P(WSER_WIFI_CONFIG_PAGE_STYLE); // in RAM

  // Navbar dinamica
  String temp = WSER_GenerateNavBar("wificonfig");
  server.sendContent(temp);

  // Contenuto statico HTML + form AP e Station
  server.sendContent_P(WSER_HTML_WIFI_CONFIG_MODAL);
  server.sendContent_P(WSER_HTML_WIFI_BODY);

  String html = FPSTR(WSER_SCRIPT_WIFI_CONFIG_STATIC);

  {
    // Genera ID univoco hardware breve (4 caratteri)
    String shortID = getShortID();

    // Sostituisce il placeholder SSID predefinito nella pagina HTML

    html.replace("{DEFAULT_AP_SSID}", String(SSID_AP_DEFAULT) + shortID);

    html.replace("{DEFAULT_AP_PASS}", String(SSID_PWD_DEFAULT));
    html.replace("{DEFAULT_AP_IP}", apIP.toString());

    html.replace("{DEFAULT_EMAIL}", String(EMAIL_RECEIVER_DEFAULT));

    html.replace("{DEFAULT_EMAIL_SENDER}", String(AUTHOR_EMAIL_DEFAULT));
    html.replace("{DEFAULT_PASSWORD_SENDER}", String(AUTHOR_PASSWORD_DEFAULT));

  }

  // Invia la versione con i valori reali
  server.sendContent(html);

  Serial.print("WIFICONFIG: Heap available after send page: ");
  Serial.println(ESP.getFreeHeap());
}


/**

 */
// Funzione per ottenere lo stato della connessione
void WSER_handleGetStatus() {

  String jsonResponse = "{";
  jsonResponse += "\"ssid\":\""       + String(WiFi.softAPSSID())           + "\",";
  jsonResponse += "\"ip\":\""         + String(WiFi.softAPIP().toString())  + "\",";
  jsonResponse += "\"sta_ssid\":\""   + String(sta_ssid_message)            + "\",";
  jsonResponse += "\"sta_ip\":\""     + String(sta_ip_message)              + "\",";
  jsonResponse += "\"email_con\":\""  + String(email_message)               + "\",";
  jsonResponse += "\"email_sen\":\""  + String(email_sender_message)       + "\"";
  jsonResponse += "}";

  server.send(200, "application/json", jsonResponse);
}


/**

 */
String getShortID() {
  uint64_t chipid = ESP.getEfuseMac();
  char idStr[13]; // 12 + terminatore
  sprintf(idStr, "%04X%08X",
          (uint16_t)(chipid >> 32),
          (uint32_t)chipid);
  // restituisce solo i primi 4 caratteri
  return String(idStr).substring(0, 4);
}

/**

 */
bool WSER_IsEmailDefined() {
  return (strlen(email_receiver) > 0 && strcmp(email_receiver, EMAIL_RECEIVER_DEFAULT) != 0);
}

/**

 */
bool WSER_IsAccessPointMode() {
    bool isAccessPointMode = (WiFi.getMode() & WIFI_AP) && (WiFi.status() != WL_CONNECTED);
    
    return isAccessPointMode; // Restituisce true se è in modalità Access Point
}