#include "01_AppControls.h"

#include "03_EEprom.h"
#include "04_Emailer.h"

#include "10_WebServerMain.h"
#include "14_ConfigPage.h"

#include "18_SentinelPage_private.h"

SMTP_Message message;
bool mailSent = false;

//------------------------------------------------------------------------------
// Variabile globale: indica se il dispositivo è attualmente in modalità Access Point pura
//------------------------------------------------------------------------------
static bool not_connected = true;

Sentinel_t sentinel[MAX_NUMBER_SENTINEL];

//--- Private Variables -------------------------------------------------------

//--- Private Variables -------------------------------------------------------

//--- Public Variables --------------------------------------------------------
uint32_t lastCheckTimeSentinel;
String gSentinelStatusRow2[4];

//--- Private Function Declarations ------------------------------------------

//--- Public Function Declarations -------------------------------------------
void WSER_InitSentinelSection();
void WSER_SentinelLoop();
void WSER_handleSentinelGetStatus();
void WSER_handleSetSentinel();

//web page fucntions

//get fucntions
bool WSER_ExtCmdSetSentinel(uint8_t control, bool active, int16_t minV, int16_t maxV, uint16_t pers, uint16_t occ);

//internal fucntions
void WSER_SentinelStringStatus(uint8_t sentIdx, uint8_t ctrlIndex, const String& stateRow2, String& outRow1, String& outRow2);


String WSER_GetSentinelValueDisplay(uint8_t index);

//get fucntions

//internal fucntions
void WSER_WiFiSentinelPage();
//-----------------------------------------------------------------------------
//--- PUBLIC Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void WSER_InitSentinelSection(){

  uint8_t idx = 0;

  uint8_t FlagEEPromSent;
  uint8_t FlagEEPromSentNeg;


  String debugMessageSent = "";

  gSentinelStatusRow2[0] = "—---";
  gSentinelStatusRow2[1] = "—---";
  gSentinelStatusRow2[2] = "—---";
  gSentinelStatusRow2[3] = "—---";

  lastCheckTimeSentinel = 0;

  FlagEEPromSent    = EEPR_ReadByte(EEPROM_INIT_FLAG_SENTINEL_ADDR);
  FlagEEPromSentNeg = EEPR_ReadByte(EEPROM_INIT_FLAG_SENTINEL_NEG_ADDR);

  // Se i flag NON sono corretti -> inizializza defaults
  if ((FlagEEPromSent != EEPROM_INIT_SENTINEL_FLAG) || (FlagEEPromSentNeg != EEPROM_INIT_SENTINEL_NEG)) {

    debugMessageSent = F("[Sentinel] EEPROM flags MANCANTI/ERRATI (non inizializzata). Inizializzo con defaults...");

    // --- sentinel 0 ---
    EEPR_WriteByte   (EEPROM_SENT0_ACTIVE_ADDR      , 0);    // false
    EEPR_WriteInt16  (EEPROM_SENT0_MINVAL_ADDR      , 0);
    EEPR_WriteInt16  (EEPROM_SENT0_MAXVAL_ADDR      , 1000);
    EEPR_WriteUint16 (EEPROM_SENT0_PERSIST_MIN_ADDR , 5);    // minuti
    EEPR_WriteUint16 (EEPROM_SENT0_OCCUR_MIN_ADDR   , 1);    // minuti

    // --- sentinel 1 ---
    EEPR_WriteByte   (EEPROM_SENT1_ACTIVE_ADDR      , 0);
    EEPR_WriteInt16  (EEPROM_SENT1_MINVAL_ADDR      , 0);
    EEPR_WriteInt16  (EEPROM_SENT1_MAXVAL_ADDR      , 1000);
    EEPR_WriteUint16 (EEPROM_SENT1_PERSIST_MIN_ADDR , 5);
    EEPR_WriteUint16 (EEPROM_SENT1_OCCUR_MIN_ADDR   , 1);
 
    // --- sentinel 2 ---
    EEPR_WriteByte   (EEPROM_SENT2_ACTIVE_ADDR      , 0);
    EEPR_WriteInt16  (EEPROM_SENT2_MINVAL_ADDR      , 0);
    EEPR_WriteInt16  (EEPROM_SENT2_MAXVAL_ADDR      , 1000);
    EEPR_WriteUint16 (EEPROM_SENT2_PERSIST_MIN_ADDR , 5);
    EEPR_WriteUint16 (EEPROM_SENT2_OCCUR_MIN_ADDR   , 1);
 
    // --- sentinel 3 ---
    EEPR_WriteByte   (EEPROM_SENT3_ACTIVE_ADDR      , 0);
    EEPR_WriteInt16  (EEPROM_SENT3_MINVAL_ADDR      , 0);
    EEPR_WriteInt16  (EEPROM_SENT3_MAXVAL_ADDR      , 1000);
    EEPR_WriteUint16 (EEPROM_SENT3_PERSIST_MIN_ADDR , 5);
    EEPR_WriteUint16 (EEPROM_SENT3_OCCUR_MIN_ADDR   , 1);
  
    // ---- marca EEPROM come inizializzata (Sentinel) ----
    EEPR_WriteByte(EEPROM_INIT_FLAG_SENTINEL_ADDR,     EEPROM_INIT_SENTINEL_FLAG);
    EEPR_WriteByte(EEPROM_INIT_FLAG_SENTINEL_NEG_ADDR, EEPROM_INIT_SENTINEL_NEG);

    } 

    // --- init / read Sentinel 0 ---
    idx = 0;
    sentinel[idx].active          = EEPR_ReadByte(EEPROM_SENT0_ACTIVE_ADDR);
    sentinel[idx].minVal          = EEPR_ReadInt16(EEPROM_SENT0_MINVAL_ADDR);
    sentinel[idx].maxVal          = EEPR_ReadInt16(EEPROM_SENT0_MAXVAL_ADDR);
    sentinel[idx].persistence_min = EEPR_ReadUint16(EEPROM_SENT0_PERSIST_MIN_ADDR);
    sentinel[idx].occurrence_min  = EEPR_ReadUint16(EEPROM_SENT0_OCCUR_MIN_ADDR);

    // Runtime init
    sentinel[idx].ActVal          = 0;   // verrà aggiornato dopo la lettura sensori
    sentinel[idx].oofl          = false;
    sentinel[idx].state         = SENT_OK;
    sentinel[idx].persist_ticks = 0;
    sentinel[idx].snooze_ticks  = 0;

    // --- init / read Sentinel 1 ---
    idx = 1;
    sentinel[idx].active          = EEPR_ReadByte(EEPROM_SENT1_ACTIVE_ADDR);
    sentinel[idx].minVal          = EEPR_ReadInt16(EEPROM_SENT1_MINVAL_ADDR);
    sentinel[idx].maxVal          = EEPR_ReadInt16(EEPROM_SENT1_MAXVAL_ADDR);
    sentinel[idx].persistence_min = EEPR_ReadUint16(EEPROM_SENT1_PERSIST_MIN_ADDR);
    sentinel[idx].occurrence_min  = EEPR_ReadUint16(EEPROM_SENT1_OCCUR_MIN_ADDR);

    // Runtime init
    sentinel[idx].ActVal          = 0;   // verrà aggiornato dopo la lettura sensori
    sentinel[idx].oofl            = false;
    sentinel[idx].state           = SENT_OK;
    sentinel[idx].persist_ticks   = 0;
    sentinel[idx].snooze_ticks    = 0;
 
    // --- init / read Sentinel 2 ---
    idx = 2;
    sentinel[idx].active          = EEPR_ReadByte(EEPROM_SENT2_ACTIVE_ADDR);
    sentinel[idx].minVal          = EEPR_ReadInt16(EEPROM_SENT2_MINVAL_ADDR);
    sentinel[idx].maxVal          = EEPR_ReadInt16(EEPROM_SENT2_MAXVAL_ADDR);
    sentinel[idx].persistence_min = EEPR_ReadUint16(EEPROM_SENT2_PERSIST_MIN_ADDR);
    sentinel[idx].occurrence_min  = EEPR_ReadUint16(EEPROM_SENT2_OCCUR_MIN_ADDR);

    // Runtime init
    sentinel[idx].ActVal          = 0;   // verrà aggiornato dopo la lettura sensori
    sentinel[idx].oofl            = false;
    sentinel[idx].state           = SENT_OK;
    sentinel[idx].persist_ticks   = 0;
    sentinel[idx].snooze_ticks    = 0;
 
    // --- init / read Sentinel 3 ---
    idx = 3;
    sentinel[idx].active          = EEPR_ReadByte(EEPROM_SENT3_ACTIVE_ADDR);
    sentinel[idx].minVal          = EEPR_ReadInt16(EEPROM_SENT3_MINVAL_ADDR);
    sentinel[idx].maxVal          = EEPR_ReadInt16(EEPROM_SENT3_MAXVAL_ADDR);
    sentinel[idx].persistence_min = EEPR_ReadUint16(EEPROM_SENT3_PERSIST_MIN_ADDR);
    sentinel[idx].occurrence_min  = EEPR_ReadUint16(EEPROM_SENT3_OCCUR_MIN_ADDR);

    // Runtime init
    sentinel[idx].ActVal          = 0;   // verrà aggiornato dopo la lettura sensori
    sentinel[idx].oofl            = false;
    sentinel[idx].state           = SENT_OK;
    sentinel[idx].persist_ticks   = 0;
    sentinel[idx].snooze_ticks    = 0;

}

/**

 */
void WSER_SentinelLoop(void) {
  unsigned long now = millis();
  static uint8_t ui8IndexSentinel = 0;

  static unsigned long last250ms = 0;                // tick sub-loop 250 ms
  static unsigned long lastUiMs[MAX_NUMBER_SENTINEL] = {0}; // throttle UI 1 Hz



// --- micro-routine: ogni 250 ms aggiorna contatori e UI (in base allo stato) ---
if ((uint32_t)(now - last250ms) >= 250UL) {
  last250ms = now;

  for (uint8_t i = 0; i < MAX_NUMBER_SENTINEL; i++) {
    // incrementi dei contatori SOLO qui, in base allo stato
    if (sentinel[i].state == SENT_VIOLATING && sentinel[i].oofl)  sentinel[i].persist_ticks += 0.25f;
    if (sentinel[i].state == SENT_ALARM     && sentinel[i].oofl)  sentinel[i].snooze_ticks  += 0.25f;

    // throttle UI: aggiorna una volta al secondo
    if ((uint32_t)(now - lastUiMs[i]) >= 1000UL) {
      lastUiMs[i] = now;

      String valDispI = WSER_GetSentinelValueDisplay(i);

      // seleziona quale timer mostrare in base allo stato
      float secs = 0.0f;
      if (sentinel[i].state == SENT_VIOLATING) secs = sentinel[i].persist_ticks;
      else if (sentinel[i].state == SENT_ALARM) secs = sentinel[i].snooze_ticks;

      unsigned long totalSeconds = (unsigned long)(secs + 0.5f);
      unsigned long hh = totalSeconds / 3600UL;
      unsigned long mm = (totalSeconds % 3600UL) / 60UL;
      unsigned long ss = totalSeconds % 60UL;

      char tbuf[10];
      snprintf(tbuf, sizeof(tbuf), "%02lu:%02lu:%02lu", hh, mm, ss);

      if (!sentinel[i].active) {    //if is inactive dont write anything 
        gSentinelStatusRow2[i] = valDispI + " | --- | ❌ Inactive";
        continue;
      }

      // UI per stato
      switch (sentinel[i].state) {
        case SENT_OK:
          gSentinelStatusRow2[i] = valDispI + " | ✅ | ---";
          break;
        case SENT_VIOLATING:
          gSentinelStatusRow2[i] = WSER_IsAccessPointMode()
            ? valDispI + " | ⚠️ | 📡 Ap Mode"
            : (WSER_IsEmailDefined() ? valDispI + " | ⚠️ | " + String(tbuf) + " h"
                                     : valDispI + " | ⚠️ | 📭 Email not defined");
          break;
        case SENT_ALARM:
          gSentinelStatusRow2[i] = WSER_IsAccessPointMode()
            ? valDispI + " | 🚨 | 📡 Ap Mode"
            : (WSER_IsEmailDefined()
                ? (email_default_remaining_email > 0
                    ? valDispI + " | 🚨 | " + String(tbuf) + " h"
                    : valDispI + " | 🚨 | 🔔 Default Email Limits reached")
                : valDispI + " | 🚨 | 📭 Email not defined");
          break;
      }
    }
  }
}






  if ( ( uint32_t ) ( now - lastCheckTimeSentinel ) < REMOTE_SENTINEL_INTERVAL ) return;
  lastCheckTimeSentinel = now;

  ui8IndexSentinel = (ui8IndexSentinel + 1) % MAX_NUMBER_SENTINEL;

  sentinel[ui8IndexSentinel].ActVal = s_Ctrl[ui8IndexSentinel].ActReadVal;

  
  String valDisp = WSER_GetSentinelValueDisplay(ui8IndexSentinel); //get the value current in string 

  // Flag fuori-limite
  sentinel[ui8IndexSentinel].oofl = ( sentinel[ui8IndexSentinel].ActVal < sentinel[ui8IndexSentinel].minVal ) || ( sentinel[ui8IndexSentinel].ActVal > sentinel[ui8IndexSentinel].maxVal );

  if (!sentinel[ui8IndexSentinel].active) {  
    sentinel[ui8IndexSentinel].state         = SENT_OK;
    sentinel[ui8IndexSentinel].persist_ticks = 0;
    sentinel[ui8IndexSentinel].snooze_ticks  = 0;
    gSentinelStatusRow2[ui8IndexSentinel] = valDisp + " | --- | " + "❌ Inactive";
    return;
  }

  switch (sentinel[ui8IndexSentinel].state) {

    case SENT_OK:
      if (sentinel[ui8IndexSentinel].oofl) {
        sentinel[ui8IndexSentinel].state = SENT_VIOLATING;
        sentinel[ui8IndexSentinel].persist_ticks = 0; // inizia a contare
      } 
      break;

    case SENT_VIOLATING:
      if (!sentinel[ui8IndexSentinel].oofl) {
        sentinel[ui8IndexSentinel].state = SENT_OK;
        sentinel[ui8IndexSentinel].persist_ticks = 0;
      } else if (sentinel[ui8IndexSentinel].persist_ticks >= (sentinel[ui8IndexSentinel].persistence_min * 60)) {
        sentinel[ui8IndexSentinel].state = SENT_ALARM;
        sentinel[ui8IndexSentinel].snooze_ticks = 0;
        Serial.println("Warning end ");
        if ((!WSER_IsAccessPointMode()) && WSER_IsEmailDefined() && (email_default_remaining_email > 0)) {
          EML_SentinelSendNewAlarm(ui8IndexSentinel,
                                  sentinel[ui8IndexSentinel].ActVal,
                                  sentinel[ui8IndexSentinel].minVal,
                                  sentinel[ui8IndexSentinel].maxVal,
                                  sentinel[ui8IndexSentinel].persistence_min,
                                  sentinel[ui8IndexSentinel].occurrence_min);
          Serial.println("Allarm sent at the end of warning ");
        }
      }
    break;

    case SENT_ALARM:
      if (!sentinel[ui8IndexSentinel].oofl) {
        // risolto
        // TODO: opzionale: invia "ALARM RESOLVED"
        sentinel[ui8IndexSentinel].state = SENT_OK;
        sentinel[ui8IndexSentinel].persist_ticks = 0;
        sentinel[ui8IndexSentinel].snooze_ticks  = 0;
        Serial.println("✅ Allarme rientrato. Stato ripristinato.");

        if ((!WSER_IsAccessPointMode())&& ( WSER_IsEmailDefined() && ( email_default_remaining_email > 0 ) ) ) {
          EML_SentinelSendResolvedAlarm   ( ui8IndexSentinel, sentinel[ui8IndexSentinel].ActVal, sentinel[ui8IndexSentinel].minVal, sentinel[ui8IndexSentinel].maxVal);
        }

      } else {
        
        if (sentinel[ui8IndexSentinel].snooze_ticks >= (sentinel[ui8IndexSentinel].occurrence_min*60)) {

          Serial.println("Allarm detect ");
          if ((!WSER_IsAccessPointMode())&& ( WSER_IsEmailDefined() ) && ( email_default_remaining_email > 0 ) ) {
            EML_SentinelSendNewAlarm ( ui8IndexSentinel, sentinel[ui8IndexSentinel].ActVal, sentinel[ui8IndexSentinel].minVal, sentinel[ui8IndexSentinel].maxVal, sentinel[ui8IndexSentinel].persistence_min, sentinel[ui8IndexSentinel].occurrence_min );
            Serial.println("Allarm sent ");
          }


          sentinel[ui8IndexSentinel].snooze_ticks = 0;     // <<<<<<<<<< ASSIGN!
        } 

    }

    break; // ⬅️ IMPORTANTISSIMO: il break resta DENTRO il case
    } // ⬅️ chiusura switch
}   // ⬅️ chiusura funzione





//-----------------------------------------------------------------------------
//--- PRIVATE Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
String WSER_GetSentinelValueDisplay(uint8_t index) {
  String valDisp;

  if (index >= MAX_NUMBER_SENTINEL) return "---";

  switch (s_Ctrl[index].type) {
    case _PH:
      valDisp = String((float)sentinel[index].ActVal / 100.0f, 2);
      break;
    case _RX:  // Redox / ORP
      valDisp = String(sentinel[index].ActVal) + " mV";
      break;
    case _EC:
      valDisp = String(sentinel[index].ActVal) + " µS/cm";
      break;
    case _TDS:
    case _SAL:
      valDisp = String(sentinel[index].ActVal) + " ppm";
      break;
    case _NTC:
      valDisp = String((float)sentinel[index].ActVal / 10.0f, 1) + " °C";
      break;
    case _DS18:
      valDisp = String((float)sentinel[index].ActVal / 10.0f, 1) + " °C";
      break;
    default:
      valDisp = String(sentinel[index].ActVal);
      break;
  }
  return valDisp;
}

/**

 */
void WSER_WiFiSentinelPage() {

  Serial.print("SENTINEL: Heap available before send page: ");
  Serial.println(ESP.getFreeHeap());

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send_P(200, "text/html", WSER_SENTINEL_HEAD);

  // 🔧 CSS con colori dinamici
  server.sendContent_P(WSER_COMMON_PAGES_STYLE);          // CSS
  String styleBlock = FPSTR(WSER_SENTINEL_STYLE);

  server.sendContent(styleBlock);

  server.sendContent_P(WSER_SENTINEL_HEAD_OPEN);
  server.sendContent(WSER_GenerateNavBar("sentinel"));
  server.sendContent_P(WSER_SENTINEL_BODY_OPEN);

String temp = FPSTR(WSER_SENTINEL_BODY_CONTENT);

// Sentinel 1..4: default
String SentMin1="0", SentMax1="100", SentActMin1="25", SentActMax1="75", SentMinLabel1="min", SentMaxLabel1="max", SentCtrl1Type="ph";
//String SentMin2="0", SentMax2="100", SentActMin2="25", SentActMax2="75", SentMinLabel2="min", SentMaxLabel2="max", SentCtrl2Type="ntc";
String SentMin2="0", SentMax2="100", SentActMin2="25", SentActMax2="75", SentMinLabel2="min", SentMaxLabel2="max", SentCtrl2Type="ds18";
String SentMin3="0", SentMax3="100", SentActMin3="25", SentActMax3="75", SentMinLabel3="min", SentMaxLabel3="max", SentCtrl3Type="ec";
String SentMin4="0", SentMax4="100", SentActMin4="25", SentActMax4="75", SentMinLabel4="min", SentMaxLabel4="max", SentCtrl4Type="ph";

/* ---------------- Sentinel #1 (control 0): PH / ORP ---------------- */
if (s_Ctrl[0].type == _PH) {
  // pH 0..14.00 → 0..1400 (centesimi)
  SentMin1 = "0";
  SentMax1 = "1400";
  SentActMin1 = "600";   // pH 6.00
  SentActMax1 = "800";   // pH 8.00
  SentMinLabel1 = "pH 6.00";
  SentMaxLabel1 = "pH 8.00";
  SentCtrl1Type = "ph";
} else if (s_Ctrl[0].type == _RX) {
  // ORP -2000..2000 mV
  SentMin1 = "-2000";
  SentMax1 = "2000";
  SentActMin1 = "0";
  SentActMax1 = "800";
  SentMinLabel1 = "0 mV";
  SentMaxLabel1 = "800 mV";
  SentCtrl1Type = "orp";
}

/* ---------------- Sentinel #2 (control 1): NTC only ---------------- */
if ((s_Ctrl[1].type == _NTC)||(s_Ctrl[1].type == _DS18)) {
  // NTC 50..700 (decimi °C)
  SentMin2 = "50";
  SentMax2 = "700";
  SentActMin2 = "250";   // 25.0 °C
  SentActMax2 = "400";   // 40.0 °C
  SentMinLabel2 = "25.0 °C";
  SentMaxLabel2 = "40.0 °C";
  //SentCtrl2Type = "ntc";
  SentCtrl2Type = "ds18";
}

/* ---------------- Sentinel #3 (control 2): EC / TDS / SAL ---------------- */
if (s_Ctrl[2].type == _EC) {

  SentMin3 = "10";
  SentMax3 = "20000";
  SentActMin3 = "1000";
  SentActMax3 = "2000";
  SentMinLabel3 = "1000 µS/cm";
  SentMaxLabel3 = "2000 µS/cm";
  SentCtrl3Type = "ec";
} else if (s_Ctrl[2].type == _TDS) {
  // TDS 3..1920 ppm
  SentMin3 = "6";
  SentMax3 = "12800";
  SentActMin3 = "500";
  SentActMax3 = "1000";
  SentMinLabel3 = "500 ppm";
  SentMaxLabel3 = "1000 ppm";
  SentCtrl3Type = "tds";
} else if (s_Ctrl[2].type == _SAL) {
  // SAL 3..1650 ppm
  SentMin3 = "6";
  SentMax3 = "11000";
  SentActMin3 = "35";
  SentActMax3 = "100";
  SentMinLabel3 = "35 ppm";
  SentMaxLabel3 = "100 ppm";
  SentCtrl3Type = "sal";
}else if (s_Ctrl[2].type == _NTC) {
  SentMin3 = "50";
  SentMax3 = "700";
  SentActMin3 = "250";   // 25.0 °C
  SentActMax3 = "400";   // 40.0 °C
  SentMinLabel3 = "25.0 °C";
  SentMaxLabel3 = "40.0 °C";
  SentCtrl3Type = "ntc";
} else if (s_Ctrl[2].type == _DIFF) {
  // SAL 3..1650 ppm
  SentMin3 = "50";
  SentMax3 = "1000";
  SentActMin3 = "250";   // 25.0 °C
  SentActMax3 = "400";   // 40.0 °C
  SentMinLabel3 = "25.0 °C";
  SentMaxLabel3 = "40.0 °C";
  SentCtrl3Type = "diff";
} else if (s_Ctrl[2].type == _AVG) {
  SentMin3 = "50";
  SentMax3 = "700";
  SentActMin3 = "250";   // 25.0 °C
  SentActMax3 = "400";   // 40.0 °C
  SentMinLabel3 = "25.0 °C";
  SentMaxLabel3 = "40.0 °C";
  SentCtrl3Type = "avg";
}
/* ---------------- Sentinel #4 (control 3): PH / ORP ---------------- */
if (s_Ctrl[3].type == _PH) {
  SentMin4 = "0";
  SentMax4 = "1400";
  SentActMin4 = "600";
  SentActMax4 = "800";
  SentMinLabel4 = "pH 6.00";
  SentMaxLabel4 = "pH 8.00";
  SentCtrl4Type = "ph";
} else if (s_Ctrl[3].type == _RX) {
  SentMin4 = "-2000";
  SentMax4 = "2000";
  SentActMin4 = "0";
  SentActMax4 = "800";
  SentMinLabel4 = "0 mV";
  SentMaxLabel4 = "800 mV";
  SentCtrl4Type = "orp";
}

// --- Sostituzione placeholder nel template (Sentinel 1..4) ---
// 1
temp.replace("{SENT1_MIN}",     SentMin1);
temp.replace("{SENT1_MAX}",     SentMax1);
temp.replace("{SENT1_VAL_MIN}", SentActMin1);
temp.replace("{SENT1_VAL_MAX}", SentActMax1);
temp.replace("{S1_MIN_LABEL}",  SentMinLabel1);
temp.replace("{S1_MAX_LABEL}",  SentMaxLabel1);
// 2
temp.replace("{SENT2_MIN}",     SentMin2);
temp.replace("{SENT2_MAX}",     SentMax2);
temp.replace("{SENT2_VAL_MIN}", SentActMin2);
temp.replace("{SENT2_VAL_MAX}", SentActMax2);
temp.replace("{S2_MIN_LABEL}",  SentMinLabel2);
temp.replace("{S2_MAX_LABEL}",  SentMaxLabel2);
// 3
temp.replace("{SENT3_MIN}",     SentMin3);
temp.replace("{SENT3_MAX}",     SentMax3);
temp.replace("{SENT3_VAL_MIN}", SentActMin3);
temp.replace("{SENT3_VAL_MAX}", SentActMax3);
temp.replace("{S3_MIN_LABEL}",  SentMinLabel3);
temp.replace("{S3_MAX_LABEL}",  SentMaxLabel3);
// 4
temp.replace("{SENT4_MIN}",     SentMin4);
temp.replace("{SENT4_MAX}",     SentMax4);
temp.replace("{SENT4_VAL_MIN}", SentActMin4);
temp.replace("{SENT4_VAL_MAX}", SentActMax4);
temp.replace("{S4_MIN_LABEL}",  SentMinLabel4);
temp.replace("{S4_MAX_LABEL}",  SentMaxLabel4);

server.sendContent(temp);

// --- SCRIPT ---
temp = FPSTR(WSER_SENTINEL_SCRIPT);
temp.replace("{SENSOR1_SENTINEL_TYPE}", SentCtrl1Type);
temp.replace("{SENSOR2_SENTINEL_TYPE}", SentCtrl2Type);
temp.replace("{SENSOR3_SENTINEL_TYPE}", SentCtrl3Type);
temp.replace("{SENSOR4_SENTINEL_TYPE}", SentCtrl4Type);
temp.replace("{IS_AP_FLAG}", WSER_IsAccessPointMode() ? "true" : "false");
server.sendContent(temp);

  server.sendContent_P(WSER_SENTINEL_CLOSE_PAGE);

  Serial.print("SENTINEL: Heap available after send page: ");
  Serial.println(ESP.getFreeHeap());
}

/**

 */
void WSER_handleSentinelGetStatus() {   // <-- nome corretto


  String Sent1row1, Sent1row2;
  WSER_SentinelStringStatus(0, 0, gSentinelStatusRow2[0], Sent1row1, Sent1row2);

  String Sent2row1, Sent2row2;
  WSER_SentinelStringStatus(1, 1, gSentinelStatusRow2[1], Sent2row1, Sent2row2);

  String Sent3row1, Sent3row2;
  WSER_SentinelStringStatus(2, 2, gSentinelStatusRow2[2], Sent3row1, Sent3row2);

  String Sent4row1, Sent4row2;
  WSER_SentinelStringStatus(3, 3, gSentinelStatusRow2[3], Sent4row1, Sent4row2);


  // JSON out
  String jsonResponse = "{";
  jsonResponse += "\"SentinelStatus1Row1\":\"" + Sent1row1 + "\",";
  jsonResponse += "\"SentinelStatus1Row2\":\"" + Sent1row2 + "\",";
  jsonResponse += "\"SentinelStatus2Row1\":\"" + Sent2row1 + "\",";
  jsonResponse += "\"SentinelStatus2Row2\":\"" + Sent2row2 + "\",";
  jsonResponse += "\"SentinelStatus3Row1\":\"" + Sent3row1 + "\",";
  jsonResponse += "\"SentinelStatus3Row2\":\"" + Sent3row2 + "\",";
  jsonResponse += "\"SentinelStatus4Row1\":\"" + Sent4row1 + "\",";
  jsonResponse += "\"SentinelStatus4Row2\":\"" + Sent4row2 + "\"";
  jsonResponse += "}";

  server.send(200, "application/json", jsonResponse);
}
                               
/**

 */
void WSER_SentinelStringStatus(uint8_t sentIdx,
                               uint8_t ctrlIndex,
                               const String& stateRow2,
                               String& outRow1,
                               String& outRow2)
{
  // Safety guard su indici
  if (sentIdx > 3) sentIdx = 0;
  if (ctrlIndex > 3) ctrlIndex = 0;

  const bool isActive = (sentinel[sentIdx].active != 0);
  const String activeStr = isActive ? F("✅ Active") : F("❌ Inactive");

  // --- min/max con unità corrette in base al tipo controllo ---
  String minLabel, maxLabel;

  switch (s_Ctrl[ctrlIndex].type) {
    case _EC:   // µS/cm (interi)
      minLabel = String(sentinel[sentIdx].minVal) + F(" µS/cm");
      maxLabel = String(sentinel[sentIdx].maxVal) + F(" µS/cm");
      break;

    case _TDS:  // ppm
    case _SAL:  // ppm
      minLabel = String(sentinel[sentIdx].minVal) + F(" ppm");
      maxLabel = String(sentinel[sentIdx].maxVal) + F(" ppm");
      break;

    case _NTC:  // decimi di °C → °C con 1 decimale
      minLabel = String((float)sentinel[sentIdx].minVal / 10.0f, 1) + F(" °C");
      maxLabel = String((float)sentinel[sentIdx].maxVal / 10.0f, 1) + F(" °C");
      break;

    case _DS18:  // decimi di °C → °C con 1 decimale
      minLabel = String((float)sentinel[sentIdx].minVal / 10.0f, 1) + F(" °C");
      maxLabel = String((float)sentinel[sentIdx].maxVal / 10.0f, 1) + F(" °C");
      break;

    case _PH: {  // centesimi di pH → pH x.xx
      float minPh = (float)sentinel[sentIdx].minVal / 100.0f;
      float maxPh = (float)sentinel[sentIdx].maxVal / 100.0f;

      minLabel = String("pH ") + String(minPh, 2);
      maxLabel = String("pH ") + String(maxPh, 2);
    } break;

    case _RX:   // mV
      minLabel = String(sentinel[sentIdx].minVal) + F(" mV");
      maxLabel = String(sentinel[sentIdx].maxVal) + F(" mV");
      break;

    default:
      minLabel = String(sentinel[sentIdx].minVal);
      maxLabel = String(sentinel[sentIdx].maxVal);
      break;
  }

  // --- persistenza / occorrenza: minuti -> "HH:MM h" ---
  auto fmtHHMM = [](uint16_t mins) -> String {
    char buf[6]; // "HH:MM"
    uint16_t h = mins / 60;
    uint16_t m = mins % 60;
    snprintf(buf, sizeof(buf), "%02u:%02u", h, m);
    return String(buf) + F(" h");
  };

  const String persistStr = fmtHHMM(sentinel[sentIdx].persistence_min);
  const String occurStr   = fmtHHMM(sentinel[sentIdx].occurrence_min);

  const String minShow     = isActive ? minLabel   : F("---");
  const String maxShow     = isActive ? maxLabel   : F("---");
  const String persistShow = isActive ? persistStr : F("---");
  const String occurShow   = isActive ? occurStr   : F("---");

  // --- Riga 1 (settings) ---
  outRow1  = activeStr;
  outRow1 += F(" | 👇 ");
  outRow1 += minShow;
  outRow1 += F(" | 👆 ");
  outRow1 += maxShow;
  outRow1 += F(" | ⚠️ ");
  outRow1 += persistShow;
  outRow1 += F(" | 🚨 ");
  outRow1 += occurShow;

  // --- Riga 2 (stato runtime, passata dal chiamante) ---
  outRow2 = stateRow2;
}

/**

 */
void WSER_handleSetSentinel() {

  // --- Controllo parametri obbligatori ---
  if (!server.hasArg("control")       ||
      !server.hasArg("status")        ||
      !server.hasArg("min")           ||
      !server.hasArg("max")           ||
      !server.hasArg("time_persist")  ||
      !server.hasArg("time_psnooze")  ) 
  {
    server.send(400, "application/json", "{\"error\":\"Missing parameters\"}");
    return;
  }

  // --- Lettura parametri ---
  uint8_t control     = server.arg("control").toInt();
  String  statusStr   = server.arg("status");
  String  minStr      = server.arg("min");
  String  maxStr      = server.arg("max");
  String  timePers    = server.arg("time_persist");
  String  timeSnooze  = server.arg("time_psnooze");

  bool active   = (statusStr == "ACTIVE");
  int16_t  minV = minStr.toInt();
  int16_t  maxV = maxStr.toInt();
  uint16_t pers = timePers.toInt();     // minuti di persistenza
  uint16_t occ  = timeSnooze.toInt();   // minuti intervallo notifiche

   bool result = WSER_ExtCmdSetSentinel(control, active, minV, maxV, pers, occ);

  // --- Risposta HTTP ---
  if (result) {
    server.send(200, "application/json", "{\"result\":\"OK\",\"message\":\"Sentinel updated successfully\"}");
  } else {
    server.send(500, "application/json", "{\"error\":\"Failed to update sentinel\"}");
  }
}

/**

 */
bool WSER_ExtCmdSetSentinel(uint8_t control, bool active, int16_t minV, int16_t maxV, uint16_t pers, uint16_t occ) {

  // --- Controllo parametri ---
  if (control < 1 || control > 4) {
    return false;
  }

  // --- Sentinel 1 ---
  if (control == 1) {
    if (active) {
      EEPR_WriteByte   (EEPROM_SENT0_ACTIVE_ADDR      , 1);
      EEPR_WriteInt16  (EEPROM_SENT0_MINVAL_ADDR      , minV);
      EEPR_WriteInt16  (EEPROM_SENT0_MAXVAL_ADDR      , maxV);
      EEPR_WriteUint16 (EEPROM_SENT0_PERSIST_MIN_ADDR , pers);
      EEPR_WriteUint16 (EEPROM_SENT0_OCCUR_MIN_ADDR   , occ);
    } else {
      EEPR_WriteByte   (EEPROM_SENT0_ACTIVE_ADDR      , 0);
      EEPR_WriteInt16  (EEPROM_SENT0_MINVAL_ADDR      , 0);
      EEPR_WriteInt16  (EEPROM_SENT0_MAXVAL_ADDR      , 0);
      EEPR_WriteUint16 (EEPROM_SENT0_PERSIST_MIN_ADDR , 0);
      EEPR_WriteUint16 (EEPROM_SENT0_OCCUR_MIN_ADDR   , 0);
    }

    sentinel[0].active          = (EEPR_ReadByte(EEPROM_SENT0_ACTIVE_ADDR) != 0);
    sentinel[0].minVal          = EEPR_ReadInt16(EEPROM_SENT0_MINVAL_ADDR);
    sentinel[0].maxVal          = EEPR_ReadInt16(EEPROM_SENT0_MAXVAL_ADDR);
    sentinel[0].persistence_min = EEPR_ReadUint16(EEPROM_SENT0_PERSIST_MIN_ADDR);
    sentinel[0].occurrence_min  = EEPR_ReadUint16(EEPROM_SENT0_OCCUR_MIN_ADDR);
  }

  // --- Sentinel 2 ---
  else if (control == 2) {
    if (active) {
      EEPR_WriteByte   (EEPROM_SENT1_ACTIVE_ADDR      , 1);
      EEPR_WriteInt16  (EEPROM_SENT1_MINVAL_ADDR      , minV);
      EEPR_WriteInt16  (EEPROM_SENT1_MAXVAL_ADDR      , maxV);
      EEPR_WriteUint16 (EEPROM_SENT1_PERSIST_MIN_ADDR , pers);
      EEPR_WriteUint16 (EEPROM_SENT1_OCCUR_MIN_ADDR   , occ);
    } else {
      EEPR_WriteByte   (EEPROM_SENT1_ACTIVE_ADDR      , 0);
      EEPR_WriteInt16  (EEPROM_SENT1_MINVAL_ADDR      , 0);
      EEPR_WriteInt16  (EEPROM_SENT1_MAXVAL_ADDR      , 0);
      EEPR_WriteUint16 (EEPROM_SENT1_PERSIST_MIN_ADDR , 0);
      EEPR_WriteUint16 (EEPROM_SENT1_OCCUR_MIN_ADDR   , 0);
    }

    sentinel[1].active          = (EEPR_ReadByte(EEPROM_SENT1_ACTIVE_ADDR) != 0);
    sentinel[1].minVal          = EEPR_ReadInt16(EEPROM_SENT1_MINVAL_ADDR);
    sentinel[1].maxVal          = EEPR_ReadInt16(EEPROM_SENT1_MAXVAL_ADDR);
    sentinel[1].persistence_min = EEPR_ReadUint16(EEPROM_SENT1_PERSIST_MIN_ADDR);
    sentinel[1].occurrence_min  = EEPR_ReadUint16(EEPROM_SENT1_OCCUR_MIN_ADDR);
  }

  // --- Sentinel 3 ---
  else if (control == 3) {
    if (active) {
      EEPR_WriteByte   (EEPROM_SENT2_ACTIVE_ADDR      , 1);
      EEPR_WriteInt16  (EEPROM_SENT2_MINVAL_ADDR      , minV);
      EEPR_WriteInt16  (EEPROM_SENT2_MAXVAL_ADDR      , maxV);
      EEPR_WriteUint16 (EEPROM_SENT2_PERSIST_MIN_ADDR , pers);
      EEPR_WriteUint16 (EEPROM_SENT2_OCCUR_MIN_ADDR   , occ);
    } else {
      EEPR_WriteByte   (EEPROM_SENT2_ACTIVE_ADDR      , 0);
      EEPR_WriteInt16  (EEPROM_SENT2_MINVAL_ADDR      , 0);
      EEPR_WriteInt16  (EEPROM_SENT2_MAXVAL_ADDR      , 0);
      EEPR_WriteUint16 (EEPROM_SENT2_PERSIST_MIN_ADDR , 0);
      EEPR_WriteUint16 (EEPROM_SENT2_OCCUR_MIN_ADDR   , 0);
    }

    sentinel[2].active          = (EEPR_ReadByte(EEPROM_SENT2_ACTIVE_ADDR) != 0);
    sentinel[2].minVal          = EEPR_ReadInt16(EEPROM_SENT2_MINVAL_ADDR);
    sentinel[2].maxVal          = EEPR_ReadInt16(EEPROM_SENT2_MAXVAL_ADDR);
    sentinel[2].persistence_min = EEPR_ReadUint16(EEPROM_SENT2_PERSIST_MIN_ADDR);
    sentinel[2].occurrence_min  = EEPR_ReadUint16(EEPROM_SENT2_OCCUR_MIN_ADDR);
  }

  // --- Sentinel 4 ---
  else if (control == 4) {
    if (active) {
      EEPR_WriteByte   (EEPROM_SENT3_ACTIVE_ADDR      , 1);
      EEPR_WriteInt16  (EEPROM_SENT3_MINVAL_ADDR      , minV);
      EEPR_WriteInt16  (EEPROM_SENT3_MAXVAL_ADDR      , maxV);
      EEPR_WriteUint16 (EEPROM_SENT3_PERSIST_MIN_ADDR , pers);
      EEPR_WriteUint16 (EEPROM_SENT3_OCCUR_MIN_ADDR   , occ);
    } else {
      EEPR_WriteByte   (EEPROM_SENT3_ACTIVE_ADDR      , 0);
      EEPR_WriteInt16  (EEPROM_SENT3_MINVAL_ADDR      , 0);
      EEPR_WriteInt16  (EEPROM_SENT3_MAXVAL_ADDR      , 0);
      EEPR_WriteUint16 (EEPROM_SENT3_PERSIST_MIN_ADDR , 0);
      EEPR_WriteUint16 (EEPROM_SENT3_OCCUR_MIN_ADDR   , 0);
    }

    sentinel[3].active          = (EEPR_ReadByte(EEPROM_SENT3_ACTIVE_ADDR) != 0);
    sentinel[3].minVal          = EEPR_ReadInt16(EEPROM_SENT3_MINVAL_ADDR);
    sentinel[3].maxVal          = EEPR_ReadInt16(EEPROM_SENT3_MAXVAL_ADDR);
    sentinel[3].persistence_min = EEPR_ReadUint16(EEPROM_SENT3_PERSIST_MIN_ADDR);
    sentinel[3].occurrence_min  = EEPR_ReadUint16(EEPROM_SENT3_OCCUR_MIN_ADDR);
  }

  return true;
}