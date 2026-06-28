#include <ESP_Mail_Client.h>
#include <time.h>

#include "01_AppControls.h"
#include "04_Emailer_private.h"

#include "10_WebServerMain.h"
#include "14_ConfigPage.h"
#include "18_SentinelPage.h"
#include "19_ReporterPage.h"

#include "21_AboutPage.h"

static unsigned long emailer_start_time = 0;   // tempo d’avvio
static bool email_server_not_connected;

SMTPSession smtp;
ESP_Mail_Session session;

uint32_t email_default_cnt;
uint8_t email_default_remaining_email;

//-------------------------------- fucnntions definition --------------
void EML_Emailer_Setup();  // inizializzazioni emailer
void EML_Emailer_Loop();   // chiamalo spesso nel loop

void EML_SentinelSendResolvedAlarm( int16_t actual_val, int16_t min_val,  int16_t max_val);
void EML_SentinelSendNewAlarm(int16_t actual_val, int16_t min_val, int16_t max_val, uint16_t persistence_min, uint16_t occurrance_min );
void EML_SentinelSetMail();
void EML_EmailReconnect ();




// --- Forward declaration
bool EML_SendCsvReport(uint8_t number, String filepath);

void EML_CopilotSendMessage(String messageToCopilot );
String EML_extractJsonValue(const String& json, const String& key);
String EML_CopilotDeviceInfoMessage();
String EML_CopilotCtrlConfigMessage(uint8_t ctrlIndex);


//-------------------------------- fucnntions definition --------------

/**

 */
void EML_Emailer_Setup() {

  email_default_cnt = 0; 
  email_default_remaining_email = SENDER_LIMIT_EMAILS_DEFAULT;

  EML_EmailReconnect ();

}


/**

 */
void EML_Emailer_Loop() {
  if (millis() - emailer_start_time >= EMAILER_INTERVAL) {
    emailer_start_time = millis();

    if (strcmp(author_email, AUTHOR_EMAIL_DEFAULT) == 0) {
        email_default_cnt++;
    }

    if (email_default_cnt >= SENDER_WATCHDOG_RESET_TIME) {
      email_default_cnt = 0; // Resetta il contatore delle email inviate
      email_default_remaining_email = SENDER_LIMIT_EMAILS_DEFAULT; // Ripristina le email rimanenti
    }

    if ((WiFi.status() == WL_CONNECTED) && email_server_not_connected) {
      EML_SentinelSetMail();
    }
  }
}

/**

 */
void EML_SentinelSetMail(){
  email_server_not_connected = false;
  // sync orario
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  for (int i = 0; i < 50 && now < 8 * 3600 * 2; ++i) {
    delay(2000);
    now = time(nullptr);
  }


  session.server.host_name  = SMTP_HOST;
  session.server.port       = SMTP_PORT;
  session.login.email       = author_email;    // Utilizza la variabile author_email
  session.login.password    = author_pwd;      // Utilizza la variabile author_pwd
  session.login.user_domain = "";

  Serial.println("✅ Sessione email pronta.");
}

/**

 */
void EML_SentinelSendNewAlarm (  uint8_t sentidx, int16_t actual_val, int16_t min_val,  int16_t max_val,  uint16_t persistence_min, uint16_t occurrance_min ){

    // ✅ EMAIL SENDER --- START
    if (ESP.getFreeHeap() > 25000) {
      Serial.println("📨 Preparazione email...");

      SMTP_Message m;
      m.sender.name  = WiFi.softAPSSID();

      m.sender.email = author_email;

      String ctrlType;
      String unit;

    uint8_t ctrlNumber = sentidx + 1; // numero umano (1-based)

    String valAct, valMin, valMax;

    switch (s_Ctrl[sentidx].type) {
      case _EC:
        ctrlType = String(ctrlNumber) + " (⚡ EC)";
        unit = "µS/cm";
        valAct = String(actual_val);
        valMin = String(min_val);
        valMax = String(max_val);
      break;

      case _TDS:
        ctrlType = String(ctrlNumber) + " (💎 TDS)";
        unit = "ppm";
        valAct = String(actual_val);
        valMin = String(min_val);
        valMax = String(max_val);
      break;

      case _SAL:
        ctrlType = String(ctrlNumber) + " (🧂 SAL)";
        unit = "ppm";
        valAct = String(actual_val);
        valMin = String(min_val);
        valMax = String(max_val);
      break;

      case _NTC:
        ctrlType = String(ctrlNumber) + " (🌡️ Temp)";
        unit = "°C";
        valAct = String((float)actual_val / 10.0f, 1);
        valMin = String((float)min_val   / 10.0f, 1);
        valMax = String((float)max_val   / 10.0f, 1);
      break;

      case _DS18:
          ctrlType = String(ctrlNumber) + " (🌡️ Temp)";
          unit = "°C";
          valAct = String((float)actual_val / 10.0f, 1);
          valMin = String((float)min_val   / 10.0f, 1);
          valMax = String((float)max_val   / 10.0f, 1);
      break;

      case _PH:
        ctrlType = String(ctrlNumber) + " (💧 pH)";
        unit = ""; // niente unità
        valAct = String((float)actual_val / 100.0f, 2);
        valMin = String((float)min_val   / 100.0f, 2);
        valMax = String((float)max_val   / 100.0f, 2);
      break;

      case _RX:
        ctrlType = String(ctrlNumber) + " (⚗️ ORP)";
        unit = "mV";
        valAct = String(actual_val);
        valMin = String(min_val);
        valMax = String(max_val);
      break;

      default:
        ctrlType = String(ctrlNumber) + " (❓ Unknown)";
        unit = "";
        valAct = String(actual_val);
        valMin = String(min_val);
        valMax = String(max_val);
      break;
    }

      m.subject = "🛡️ Sentinel " + ctrlType + ": 🚨 Alarm!";
      m.addRecipient("Receiver", email_receiver);

      String msg = FPSTR(MSG_ALARM_SENTINEL_TEMPLATE);
      msg.replace("{VALUE}", valAct);
      msg.replace("{MIN}", valMin);
      msg.replace("{MAX}", valMax);
      msg.replace("{UNIT}", unit);
      msg.replace("{OCC}", String(occurrance_min));
      msg.replace("{PERS}", String(persistence_min));

      m.text.content = msg;

      if (!smtp.connect(&session)) {
          Serial.println("❌ Connessione SMTP fallita: " + smtp.errorReason());
      } else {
        if (!MailClient.sendMail(&smtp, &m)) {
         Serial.println("❌ Errore invio mail: " + smtp.errorReason());
        } else {
          Serial.println("✅ Email inviata con successo.");
          email_default_remaining_email--;
        }
        smtp.closeSession();
      }

    } else {
      Serial.printf("⚠️ Heap insufficiente per invio email! Heap attuale: %u bytes\n", ESP.getFreeHeap());
    }
}// ✅ EMAIL SENDER --- END

/**

 */
void EML_SentinelSendResolvedAlarm(   uint8_t sentidx, int16_t actual_val, int16_t min_val,  int16_t max_val){ // ✅ EMAIL "ALARM RESOLVED"

    if (ESP.getFreeHeap() > 25000) {
      Serial.println("📨 Preparazione email...");

      SMTP_Message m;
      m.sender.name  = WiFi.softAPSSID();

      m.sender.email = author_email;

      String ctrlType;
      String unit;
      uint8_t ctrlNumber = sentidx + 1; // numero umano (1-based)

    String valAct, valMin, valMax;

    switch (s_Ctrl[sentidx].type) {
      case _EC:
        ctrlType = String(ctrlNumber) + " (⚡ EC)";
        unit = "µS/cm";
        valAct = String(actual_val);
        valMin = String(min_val);
        valMax = String(max_val);
      break;

      case _TDS:
        ctrlType = String(ctrlNumber) + " (💎 TDS)";
        unit = "ppm";
        valAct = String(actual_val);
        valMin = String(min_val);
        valMax = String(max_val);
      break;

      case _SAL:
        ctrlType = String(ctrlNumber) + " (🧂 SAL)";
        unit = "ppm";
        valAct = String(actual_val);
        valMin = String(min_val);
        valMax = String(max_val);
      break;

      case _NTC:
        ctrlType = String(ctrlNumber) + " (🌡️ Temp)";
        unit = "°C";
        valAct = String((float)actual_val / 10.0f, 1);
        valMin = String((float)min_val   / 10.0f, 1);
        valMax = String((float)max_val   / 10.0f, 1);
      break;

      case _DS18:
          ctrlType = String(ctrlNumber) + " (🌡️ Temp)";
          unit = "°C";
          valAct = String((float)actual_val / 10.0f, 1);
          valMin = String((float)min_val   / 10.0f, 1);
          valMax = String((float)max_val   / 10.0f, 1);
      break;

      case _PH:
        ctrlType = String(ctrlNumber) + " (💧 pH)";
        unit = ""; // niente unità
        valAct = String((float)actual_val / 100.0f, 2);
        valMin = String((float)min_val   / 100.0f, 2);
        valMax = String((float)max_val   / 100.0f, 2);
      break;

      case _RX:
        ctrlType = String(ctrlNumber) + " (⚗️ ORP)";
        unit = "mV";
        valAct = String(actual_val);
        valMin = String(min_val);
        valMax = String(max_val);
      break;

      default:
        ctrlType = String(ctrlNumber) + " (❓ Unknown)";
        unit = "";
        valAct = String(actual_val);
        valMin = String(min_val);
        valMax = String(max_val);
      break;
    }
      m.subject = "🛡️ Sentinel n° " + ctrlType + " ✅ Alarm resolved!";
        m.addRecipient("Receiver", email_receiver);


      String msg = FPSTR(MSG_RESOLVED_SENTINEL_TEMPLATE);
      msg.replace("{VALUE}", valAct);
      msg.replace("{MIN}", valMin);
      msg.replace("{MAX}", valMax);
      msg.replace("{UNIT}",  unit);
      m.text.content = msg;

      if (!smtp.connect(&session)) {
        Serial.println("❌ Connessione SMTP fallita: " + smtp.errorReason());
      } else {
        if (!MailClient.sendMail(&smtp, &m)) {
          Serial.println("❌ Errore invio mail: " + smtp.errorReason());
        } else {
          Serial.println("✅ Email RESOLVED inviata con successo.");
          email_default_remaining_email--;
        }
        smtp.closeSession();
      }
    } else {
      Serial.printf("⚠️ Heap insufficiente per invio email RESOLVED! Heap: %u bytes\n", ESP.getFreeHeap());
    }
    // ✅ EMAIL "ALARM RESOLVED" --- END
}

/**

 */
bool EML_SendCsvReport(uint8_t number, String filepath) {

  if (filepath.length() == 0) {
    Serial.println(F("❌ Filepath vuoto: niente invio."));
    return false;
  }

  if (!LittleFS.exists(filepath)) {
    Serial.println(F("❌ .csv non trovato: niente invio."));
    return false;
  }

  if (ESP.getFreeHeap() <= 25000) {
    Serial.printf("⚠️ Heap insufficiente per invio email! Heap: %u bytes\n", ESP.getFreeHeap());
    return false;
  }

  Serial.println(F("📨 Preparazione email CSV..."));

  String fullPath = filepath;
  String fileName = fullPath.substring(fullPath.lastIndexOf('/') + 1);

  SMTP_Message m;
  m.sender.name  = WiFi.softAPSSID();

  m.sender.email = author_email;

  String ctrlType;
  String unit;

  switch (s_Ctrl[number].type) {
    case _EC:  
      ctrlType = " (⚡ EC)";    
      unit = "µS/cm";
    break;
    case _TDS: 
      ctrlType = " (💎 TDS)";  
      unit = "ppm";
    break;
    case _SAL: 
      ctrlType = " (🧂 SAL)";  
      unit = "ppm";
    break;
    case _NTC: 
      ctrlType = " (🌡️ Temp)";  
      unit = "°C";
    break;

    case _DS18:
      ctrlType = " (🌡️ Temp)";  
      unit = "°C";
    break;

    case _RX: 
        ctrlType = " (⚗️ ORP)";   
        unit = "mV";
        break;
    case _PH: 
        ctrlType = " (💧 pH)";      // pH
        unit = "";
        break;
    default:   
      ctrlType = "❓ Unknown";
      unit = "";
    break;
  }

  m.subject  = "📈 Reporter " + String(number + 1) + ctrlType;

  String msg = FPSTR(MSG_TEMPLATE_REPORTER);
  msg.replace("{NUM}",  String(number + 1));
  msg.replace("{TYPE}",  ctrlType);
  msg.replace("{UNIT}",  unit);
  m.text.content = msg;

  m.addRecipient("Receiver", email_receiver);      // stessa email che già usi

  SMTP_Attachment att;
  att.descr.filename    = fileName;
  att.descr.mime        = "text/csv";
  att.file.path         = filepath;
  att.file.storage_type = esp_mail_file_storage_type_flash;  // SPIFFS/LittleFS

  m.addAttachment(att);

  if (!smtp.connect(&session)) {
    Serial.println("❌ Connessione SMTP fallita: " + smtp.errorReason());
    return false;
  }

  if (!MailClient.sendMail(&smtp, &m)) {
    Serial.println("❌ Errore invio mail: " + smtp.errorReason());
    return false;
  } else {
    Serial.println(F("✅ Email CSV inviata con successo."));
    email_default_remaining_email--;
  }

  smtp.closeSession();
  return true;
}

/**

 */
void EML_EmailReconnect (){

  emailer_start_time = millis();
  email_server_not_connected = true;

  email_default_cnt = 0; 
  email_default_remaining_email = SENDER_LIMIT_EMAILS_DEFAULT;

  Serial.println(F("📧 AppEmailer setup eseguito."));

}

/**

 */
bool EML_CopilotSendMessage(String messageToCopilot, String fileSystemInfoPath) {
    if (ESP.getFreeHeap() <= 25000) {
        Serial.printf("⚠️ Heap insufficiente per invio email! Heap: %u bytes\n", ESP.getFreeHeap());
        return false;
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println(F("❌ Non connesso a Wi-Fi!"));
        return false;
    }

    if (!LittleFS.exists(fileSystemInfoPath)) {
        Serial.println(F("❌ Il file non esiste!"));
        return false;
    }

    Serial.println(F("📨 Preparazione email to Copilot..."));

    String filepath = fileSystemInfoPath; // Assicurati che questo sia corretto
    String fileName = filepath.substring(filepath.lastIndexOf('/') + 1);

    SMTP_Message m;
    m.sender.name = WiFi.softAPSSID();
    m.sender.email = author_email;
    m.subject = "EdulcoWaterCopilotRequest";

    // Corpo testo email formattato sknt_*
    m.text.content  = "sknt_mess: " + messageToCopilot + "\n";
    m.text.content += "sknt_rec: "  + String(email_receiver) + "\n";
    m.text.content += EML_CopilotDeviceInfoMessage() + "\n";
    m.text.content += EML_CopilotCtrlConfigMessage(0) + "\n";
    m.text.content += EML_CopilotCtrlConfigMessage(1) + "\n";
    m.text.content += EML_CopilotCtrlConfigMessage(2) + "\n";
    m.text.content += EML_CopilotCtrlConfigMessage(3) + "\n";
    m.text.content += "sknt_end.\n";

    m.addRecipient("Receiver", email_Copilot_receiver); // stessa email che già usi

    SMTP_Attachment att;
    att.descr.filename = fileName;
    att.descr.mime = "text/plain"; // Assicurati di specificare il tipo MIME corretto
    att.file.path = filepath;
    att.file.storage_type = esp_mail_file_storage_type_flash; // SPIFFS/LittleFS

    m.addAttachment(att); // Assicurati che questa riga sia presente

    if (!smtp.connect(&session)) {
        Serial.println("❌ Connessione SMTP fallita: " + smtp.errorReason());
        return false;
    }

    if (!MailClient.sendMail(&smtp, &m)) {
        Serial.println("❌ Errore invio mail: " + smtp.errorReason());
        return false;
    } else {
        Serial.println(F("✅ Email xtxt copilot inviata con successo."));
    }

    smtp.closeSession();
    return true;
}

/**

 */
String EML_CopilotDeviceInfoMessage() {

    String json = WSER_ExtCmdGetDeviceAbout();

    String finalMsg = "";
    finalMsg += String("sknt_about: ")     + json;
    
    return finalMsg;
}

/**

 */
String EML_CopilotCtrlConfigMessage(uint8_t ctrlIndex) {

    String json = WSER_ExtCmdGetStatus(ctrlIndex);

    String finalMsg = "";
    finalMsg += String("sknt_config") + String(ctrlIndex + 1 ) + String(": ") +json ;

    return finalMsg;
}