
#include "01_AppControls.h"
#include "03_EEprom.h"
#include "04_Emailer.h"

#include <FS.h>
#include <LittleFS.h>

#include <time.h>

#include "10_WebServerMain.h"
#include "14_ConfigPage.h"
#include "18_SentinelPage.h"
#include "19_ReporterPage_private.h" // se split in header

// ====== Variabili GLOBALI (template) ========================================

// ---- GLOBALI ----
bool g_timeReady_ = false;
static uint8_t ntpRetryScaler = 0;   // retry NTP nel loop     

unsigned long ulReporterLastUpdateTime = 0; // tick per loop

t_Reporter Reporter[NUM_REPORTERS];

File dataFile;   // <--- 🔹 dichiarazione globale

// Prossimo tick di campionamento
static unsigned long gReporterNextSampleTick = 0;

// ====== Dichiarazioni funzioni ==============================================
void WSER_InitReporterSection();
void WSER_WiFiReporterPage();
void WSER_ReporterLoop();

void WSER_handleReporterGetStatus();   
void WSER_handleSetReporter();         

// Smart Home (stub)
bool WSER_ExtCmdSetReporter(uint8_t ctrl, bool active, uint8_t period);
String WSER_ExtCmdReporterGetStatus(uint8_t ctrl);

//files functions 

bool WSER_ReporterInitDataFile(uint8_t number, uint8_t type);
bool WSER_ReporterAppendValueDataFile(uint8_t number, uint8_t type, int16_t value, int16_t valmin, int16_t valmax);
bool WSER_ReporterResetDataFile(uint8_t number);
void WSER_GenerateReporterStrings(uint8_t reportIndex);
void WSER_InitReporter(uint8_t reportIndex);

/**

 */
void WSER_InitReporterSection(){

  //----- open data file------------------
  // 1) Monta FS (con fallback format)
  if (!LittleFS.begin()) {
    Serial.println("LittleFS.begin() FAILED. Provo a formattare...");
    if (!LittleFS.begin(true)) {
      Serial.println("LittleFS format/mount FAILED.");
      return;
    }
  }
  Serial.println("LittleFS montato.");
  //----- open data file------------------


  uint8_t FlagEEPromRep     = EEPR_ReadByte(EEPROM_INIT_FLAG_REPORTER_ADDR);
  uint8_t FlagEEPromRepNeg  = EEPR_ReadByte(EEPROM_INIT_FLAG_REPORTER_NEG_ADDR);

  // ---- se non inizializzato → scrivi defaults
  if ((FlagEEPromRep != EEPROM_INIT_REPORTER_FLAG) || (FlagEEPromRepNeg != EEPROM_INIT_REPORTER_NEG)) {

    // Defaults: tutti NOT ACTIVE (0)
    EEPR_WriteByte(EEPROM_REP0_ACTIVE_ADDR, 0);
    EEPR_WriteByte(EEPROM_REP1_ACTIVE_ADDR, 0);
    EEPR_WriteByte(EEPROM_REP2_ACTIVE_ADDR, 0);
    EEPR_WriteByte(EEPROM_REP3_ACTIVE_ADDR, 0);

    // Defaults: period = 7 giorni per tutti
    EEPR_WriteByte(EEPROM_REP0_PERIOD_ADDR, REPORTER_DEFAULT_PERIOD_DAYS);
    EEPR_WriteByte(EEPROM_REP1_PERIOD_ADDR, REPORTER_DEFAULT_PERIOD_DAYS);
    EEPR_WriteByte(EEPROM_REP2_PERIOD_ADDR, REPORTER_DEFAULT_PERIOD_DAYS);
    EEPR_WriteByte(EEPROM_REP3_PERIOD_ADDR, REPORTER_DEFAULT_PERIOD_DAYS);

    // Marca area Reporter come inizializzata
    EEPR_WriteByte(EEPROM_INIT_FLAG_REPORTER_ADDR,     EEPROM_INIT_REPORTER_FLAG);
    EEPR_WriteByte(EEPROM_INIT_FLAG_REPORTER_NEG_ADDR, EEPROM_INIT_REPORTER_NEG);
  }

// ---- init report 0
  Reporter[0].active       = EEPR_ReadByte(EEPROM_REP0_ACTIVE_ADDR) ? true : false;
  Reporter[0].periodReport = EEPR_ReadByte(EEPROM_REP0_PERIOD_ADDR);
  Reporter[0].periodSet = (REPORTER_FAKTOR_60_SAMPLES * Reporter[0].periodReport); //minutes to be waited to have a repero
  Reporter[0].periodcnt = 0; 
  Reporter[0].samplesCnt = 0;
  Reporter[0].type = s_Ctrl[0].type;

  Reporter[0].AvgPeriod = 0;    //avarage value in the period considererd
  Reporter[0].MinPeriod = MAX_VALUE_REPO;    //initialized at worse possible
  Reporter[0].MaxPeriod = MIN_VALUE_REPO;    

  // Init runtime
  Reporter[0].Row1 = "Loading row1 ";
  Reporter[0].Row2 = "Loading row2 ";
  Reporter[0].currentSecondIndex = 0;
  Reporter[0].currentMinuteIndex = 0;
  Reporter[0].currentHourIndex   = 0;

  for (uint8_t j = 0; j < VALUES_ARRAY_DIM_MINUTE_HOUR; j++) {
    Reporter[0].valuesSeconds[j] = 0;   // 60 secondi
    Reporter[0].valuesMinutes[j] = 0;   // 60 minuti
  }
  for (uint8_t j = 0; j < VALUES_ARRAY_DIM_DAY; j++) {
    Reporter[0].valuesHours[j] = 0;     // 24 ore
  }

// ---- init report 1
  Reporter[1].active       = EEPR_ReadByte(EEPROM_REP1_ACTIVE_ADDR) ? true : false;
  Reporter[1].periodReport = EEPR_ReadByte(EEPROM_REP0_PERIOD_ADDR);
  Reporter[1].periodSet = (REPORTER_FAKTOR_60_SAMPLES * Reporter[1].periodReport); //number of seconds oto have 60 samples
  Reporter[1].periodcnt = 0; 
  Reporter[1].samplesCnt = 0;
  Reporter[1].type = s_Ctrl[1].type;

  Reporter[1].AvgPeriod = 0;    //avarage value in the period considererd
  Reporter[1].MinPeriod = MAX_VALUE_REPO;    //initialized at worse possible
  Reporter[1].MaxPeriod = MIN_VALUE_REPO;    

  Reporter[1].Row1 = "Loading row1 ";
  Reporter[1].Row2 = "Loading row2 ";
  Reporter[1].currentSecondIndex = 0;
  Reporter[1].currentMinuteIndex = 0;
  Reporter[1].currentHourIndex   = 0;
  for (uint8_t j = 0; j < VALUES_ARRAY_DIM_MINUTE_HOUR; j++) {
    Reporter[1].valuesSeconds[j] = 0;
    Reporter[1].valuesMinutes[j] = 0;
  }
  for (uint8_t j = 0; j < VALUES_ARRAY_DIM_DAY; j++) {
    Reporter[1].valuesHours[j] = 0;
  }

// ---- init report 2
  Reporter[2].active       = EEPR_ReadByte(EEPROM_REP2_ACTIVE_ADDR) ? true : false;
  Reporter[2].periodReport = EEPR_ReadByte(EEPROM_REP0_PERIOD_ADDR);
  Reporter[2].periodSet = (REPORTER_FAKTOR_60_SAMPLES * Reporter[2].periodReport); //number of seconds oto have 60 samples
  Reporter[2].periodcnt = 0; 
  Reporter[2].samplesCnt = 0;
  Reporter[2].type = s_Ctrl[2].type;

  Reporter[2].AvgPeriod = 0;    //avarage value in the period considererd
  Reporter[2].MinPeriod = MAX_VALUE_REPO;    //initialized at worse possible
  Reporter[2].MaxPeriod = MIN_VALUE_REPO;    

  Reporter[2].Row1 = "Loading row1 ";
  Reporter[2].Row2 = "Loading row2 ";
  Reporter[2].currentSecondIndex = 0;
  Reporter[2].currentMinuteIndex = 0;
  Reporter[2].currentHourIndex   = 0;
  for (uint8_t j = 0; j < VALUES_ARRAY_DIM_MINUTE_HOUR; j++) {
    Reporter[2].valuesSeconds[j] = 0;
    Reporter[2].valuesMinutes[j] = 0;
  }
  for (uint8_t j = 0; j < VALUES_ARRAY_DIM_DAY; j++) {
    Reporter[2].valuesHours[j] = 0;
  }

  // ---- init report 3
  Reporter[3].active       = EEPR_ReadByte(EEPROM_REP3_ACTIVE_ADDR) ? true : false;
  Reporter[3].periodReport = EEPR_ReadByte(EEPROM_REP0_PERIOD_ADDR);
  Reporter[3].periodSet = (REPORTER_FAKTOR_60_SAMPLES * Reporter[3].periodReport); //number of seconds oto have 60 samples
  Reporter[3].periodcnt = 0; 
  Reporter[3].samplesCnt = 0;
  Reporter[3].type = s_Ctrl[3].type;

  Reporter[3].AvgPeriod = 0;    //avarage value in the period considererd
  Reporter[3].MinPeriod = MAX_VALUE_REPO;    //initialized at worse possible
  Reporter[3].MaxPeriod = MIN_VALUE_REPO;  

  Reporter[3].Row1 = "Loading row1 ";
  Reporter[3].Row2 = "Loading row2 ";
  Reporter[3].currentSecondIndex = 0;
  Reporter[3].currentMinuteIndex = 0;
  Reporter[3].currentHourIndex   = 0;
  for (uint8_t j = 0; j < VALUES_ARRAY_DIM_MINUTE_HOUR; j++) {
    Reporter[3].valuesSeconds[j] = 0;
    Reporter[3].valuesMinutes[j] = 0;
  }
  for (uint8_t j = 0; j < VALUES_ARRAY_DIM_DAY; j++) {
    Reporter[3].valuesHours[j] = 0;
  }

  ulReporterLastUpdateTime = millis();

  WSER_ReporterInitDataFile(0, s_Ctrl[0].type );
  WSER_ReporterInitDataFile(1, s_Ctrl[1].type );
  WSER_ReporterInitDataFile(2, s_Ctrl[2].type );
  WSER_ReporterInitDataFile(3, s_Ctrl[3].type );

}

/**

 */
// ---- NTP: prova a settare l'ora se WiFi è connesso ----
bool tryInitTime(uint32_t timeout_ms = 10000) {
  if (WiFi.status() != WL_CONNECTED) return false;

  // TZ Europe/Berlin con DST
  setenv("TZ", "CET-1CEST,M3.5.0/2,M10.5.0/3", 1);
  tzset();

  configTime(0, 0, "pool.ntp.org", "time.google.com", "time.cloudflare.com");

  uint32_t start = millis();
  while (millis() - start < timeout_ms) {
    time_t now = time(nullptr);
    if (now > 1609459200) return true; // > 2021-01-01
    delay(200);
    yield();
  }
  return false;
}

/**

 */
// ---- Timestamp ISO locale (Europe/Berlin) ----
String isoNowLocal() {
  time_t now = time(nullptr);
  struct tm t;
  localtime_r(&now, &t);
  char buf[24];
  // YYYY-MM-DD HH:MM:SS
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &t);
  return String(buf);
}

/**

 */
void WSER_ReporterLoop() {

  static unsigned long lastSampleTime = 0;
  unsigned long now = millis();
  static uint8_t reportIndex = 0;  // mantiene il numero file corrente


  if (now - lastSampleTime >= WSER_REPORTER_TIMEOUT_INTERVAL) { // every second
    lastSampleTime = now;

    reportIndex = (reportIndex + 1) % NUM_REPORTERS;


    if (Reporter[reportIndex].type != s_Ctrl[reportIndex].type) WSER_InitReporter(reportIndex);

if (WiFi.isConnected()) {

    // 1) Se l’ora non è pronta, ritenta ogni 30 cicli (~30s)
    if (!g_timeReady_) {
      ntpRetryScaler++;
      if (ntpRetryScaler >= 30) {
        ntpRetryScaler = 0;
        g_timeReady_ = tryInitTime(12000);
        if (g_timeReady_) {
          Serial.printf("⏰ Time OK: %s (Europe/Berlin)\n", isoNowLocal().c_str());
        } else {
          Serial.println(F("⏰ NTP retry: time not set yet."));
        }
      }
    }

    Reporter[reportIndex].periodcnt++;

      Reporter[reportIndex].AvgPeriod = Reporter[reportIndex].AvgPeriod + s_Ctrl[reportIndex].ActReadVal; 
      if ( s_Ctrl[reportIndex].ActReadVal > Reporter[reportIndex].MaxPeriod) {
        Reporter[reportIndex].MaxPeriod = s_Ctrl[reportIndex].ActReadVal;
      }
      if ( s_Ctrl[reportIndex].ActReadVal < Reporter[reportIndex].MinPeriod) {
        Reporter[reportIndex].MinPeriod = s_Ctrl[reportIndex].ActReadVal;
      }
    
      
    if (Reporter[reportIndex].periodcnt >= ((Reporter[reportIndex].periodSet)/300)) { 

      Reporter[reportIndex].AvgPeriod = (Reporter[reportIndex].AvgPeriod/Reporter[reportIndex].periodcnt); //calculate the avarage
      Reporter[reportIndex].periodcnt = 0;

      if (!g_timeReady_) {
        Serial.println(F("⏳ Skip log: time not ready."));
      } else {
        WSER_ReporterAppendValueDataFile(reportIndex, s_Ctrl[reportIndex].type, Reporter[reportIndex].AvgPeriod, Reporter[reportIndex].MinPeriod, Reporter[reportIndex].MaxPeriod);
        Reporter[reportIndex].samplesCnt++;
        if (Reporter[reportIndex].samplesCnt >= REPORTER_SAMPLES_NUMBER){
          
           if ((!WSER_IsAccessPointMode())&& ( WSER_IsEmailDefined() && ( email_default_remaining_email > 0 ) ) ){
            EML_SendCsvReport(reportIndex, Reporter[reportIndex].filepath);
           }

          WSER_ReporterResetDataFile(reportIndex);
          Reporter[reportIndex].samplesCnt=0;

        }
      }
 
      Reporter[reportIndex].AvgPeriod = 0;
      Reporter[reportIndex].MinPeriod =  MAX_VALUE_REPO;
      Reporter[reportIndex].MaxPeriod =  MIN_VALUE_REPO;

    }

} else {
  Serial.println(F("Reporter not active in not connected mode"));
}

    // store data (secondi)
    Reporter[reportIndex].valuesSeconds[Reporter[reportIndex].currentSecondIndex] = s_Ctrl[reportIndex].ActReadVal;
    Reporter[reportIndex].currentSecondIndex++;

    if (Reporter[reportIndex].currentSecondIndex >= VALUES_ARRAY_DIM_MINUTE_HOUR) {
      // fine minuto
      Reporter[reportIndex].currentSecondIndex = 0;

      int32_t sumMin = 0;
      for (uint8_t i = 0; i < VALUES_ARRAY_DIM_MINUTE_HOUR; i++) {
        sumMin += Reporter[reportIndex].valuesSeconds[i];
      }
      int avgMin = (int)((sumMin + (VALUES_ARRAY_DIM_MINUTE_HOUR / 2)) / VALUES_ARRAY_DIM_MINUTE_HOUR);
      Serial.print("Media valori ultimo minuto controllo: "); Serial.print(reportIndex); Serial.print(" valore: "); Serial.println(avgMin);


      Reporter[reportIndex].valuesMinutes[Reporter[reportIndex].currentMinuteIndex] = avgMin;
      Reporter[reportIndex].currentMinuteIndex++;

      if (Reporter[reportIndex].currentMinuteIndex >= VALUES_ARRAY_DIM_MINUTE_HOUR) {

        Reporter[reportIndex].currentMinuteIndex = 0;

        int32_t sumHour = 0;
        for (uint8_t i = 0; i < VALUES_ARRAY_DIM_MINUTE_HOUR; i++) {
          sumHour += Reporter[reportIndex].valuesMinutes[i]; 
        }
        int avgHour = (int)((sumHour + (VALUES_ARRAY_DIM_MINUTE_HOUR / 2)) / VALUES_ARRAY_DIM_MINUTE_HOUR);
        Serial.print("Media valori ultima ora: ");
        Serial.println(avgHour);


        Reporter[reportIndex].valuesHours[Reporter[reportIndex].currentHourIndex] = avgHour;
        Reporter[reportIndex].currentHourIndex++;
        if (Reporter[reportIndex].currentHourIndex >= VALUES_ARRAY_DIM_DAY) {
          Reporter[reportIndex].currentHourIndex = 0;
        }
      }
    }

      WSER_GenerateReporterStrings(reportIndex);

  }
}








//------------Module internal functions------------------------


/**

 */
 void WSER_InitReporter(uint8_t reportIndex)
{
    // Allinea tipo al controllo (assumi già verificato esternamente)
    Reporter[reportIndex].type = s_Ctrl[reportIndex].type;

    // Reset indici temporali
    Reporter[reportIndex].currentSecondIndex = 0;
    Reporter[reportIndex].currentMinuteIndex = 0;
    Reporter[reportIndex].currentHourIndex   = 0;

    // Reset array valori secondi e minuti
    for (int i = 0; i < VALUES_ARRAY_DIM_MINUTE_HOUR; i++)
    {
        Reporter[reportIndex].valuesSeconds[i] = 0;
        Reporter[reportIndex].valuesMinutes[i] = 0;
    }

    // Reset array valori ore
    for (int i = 0; i < VALUES_ARRAY_DIM_DAY; i++)
    {
        Reporter[reportIndex].valuesHours[i] = 0;
    }

    // Reset contatori e statistiche
    Reporter[reportIndex].periodcnt  = 0;
    Reporter[reportIndex].samplesCnt = 0;
    Reporter[reportIndex].AvgPeriod  = 0;
    Reporter[reportIndex].MinPeriod  = MAX_VALUE_REPO;
    Reporter[reportIndex].MaxPeriod  = MIN_VALUE_REPO;

    // Reset file dati associato al reporter
    WSER_ReporterResetDataFile(reportIndex);

    // Messaggio diagnostico
    Serial.printf("♻️ Reporter[%u] inizializzato → file: %s\n",
                  reportIndex, Reporter[reportIndex].filepath.c_str());
}

/**

 */
void WSER_GenerateReporterStrings(uint8_t reportIndex)
{
    // --- Determina il periodo come stringa ---
    String ReportPeriod = "Daily";
    if (Reporter[reportIndex].periodReport == REPORTER_PERIOD_DAYS_DAY)
        ReportPeriod = "Day";
    else if (Reporter[reportIndex].periodReport == REPORTER_PERIOD_DAYS_WEEK)
        ReportPeriod = "Week";
    else if (Reporter[reportIndex].periodReport == REPORTER_PERIOD_DAYS_MONTH)
        ReportPeriod = "Month";

    // --- Se reporter attivo ---
    if (Reporter[reportIndex].active)
    {
        Reporter[reportIndex].Row1 = "✅ ACTIVE | " + ReportPeriod;

        // --- Se WiFi non connesso ---
        if (!WiFi.isConnected())
        {
            Reporter[reportIndex].Row2 = "⚠️ Not working in AP mode";
            return;
        }

        // --- Se limite email raggiunto ---
        if (email_default_remaining_email <= 0)
        {
            Reporter[reportIndex].Row2 = "🔔 Default Email Limits reached";
            return;
        }

        // --- Calcolo percentuale completamento ---
        int percent = (Reporter[reportIndex].samplesCnt * 100) / REPORTER_SAMPLES_NUMBER;

        // --- Calcolo tempo rimanente ---
        uint32_t periodSec = Reporter[reportIndex].periodReport * 86400UL; // giorni → secondi
        uint32_t remainingSec = (uint32_t)((periodSec * (100 - percent)) / 100);
        uint32_t g = remainingSec / 86400UL;        // giorni
        uint32_t h = (remainingSec % 86400UL) / 3600UL; // ore
        uint32_t m = (remainingSec % 3600UL) / 60UL;    // minuti

        // --- Genera stringa stato ---
        Reporter[reportIndex].Row2 =
            "📉 " + String(percent) + "% Completed | 📧 in " +
            (g > 0 ? String(g) + "d " : "") +
            String(h) + "h " + String(m) + "m";
    }
    else
    {
        // --- Reporter non attivo ---
        Reporter[reportIndex].Row1 = "❌ NOT ACTIVE | ";
        Reporter[reportIndex].Row2 = "📧 —";
    }
}

//files section 
/**

 */
bool WSER_ReporterInitDataFile(uint8_t number, uint8_t type) {
  String fileName;
  String typeName;
  String unitName = "";

  switch (type) {
    case _PH:    typeName = "PH";     unitName = "pH";       break;
    case _RX:    typeName = "ORP";    unitName = "mV";       break;
    case _EC:    typeName = "EC";     unitName = "µS/cm";    break;
    case _TDS:   typeName = "TDS";    unitName = "ppm";      break;
    case _SAL:   typeName = "SAL";    unitName = "ppm";      break;
    case _TEMP:  typeName = "TEMP";   unitName = "°C";       break;
    case _HUM:   typeName = "HUM";    unitName = "%";        break;
    case _NTC:   typeName = "NTC";    unitName = "°C";       break;
    case _DS18:  typeName = "DS18";    unitName = "°C";      break;
    case _TIMER: typeName = "TIMER";  unitName = "s";        break;
  }

  fileName = "/Reporter_" + String(number + 1) + ".csv";

  //----- open data file ------------------
  File dataFile = LittleFS.open(fileName, "a");
  if (!dataFile) {
    Serial.println("❌ Errore: impossibile aprire il report file: " + fileName);
    return false;
  }

if (dataFile.size() == 0) {

  dataFile.printf("# TYPE: %s, UNIT: %s\n", typeName.c_str(), unitName.c_str());
  dataFile.println("Timestamp,Average,ValMin,ValMax");
  dataFile.flush();
}

  dataFile.close();
  Serial.println("📁 File CSV creato o già esistente: " + fileName);

  Reporter[number].filepath = fileName;
  //----- open data file ------------------

  return true;
}

/**

 */
bool WSER_ReporterResetDataFile(uint8_t number) {

  File f = LittleFS.open(Reporter[number].filepath, "w");  // "w" = write → cancella e ricrea
  if (!f) {
    Serial.println(F("❌ Errore: impossibile aprire .csv per reinizializzazione."));
    return false;
  }

  // Recupera tipo e unità dal Reporter per ricreare header coerente
  String typeName = "UNKNOWN";
  String unitName = "";

  switch (Reporter[number].type) {
    case _PH:    typeName = "PH";     unitName = "pH";     break;
    case _RX:    typeName = "ORP";    unitName = "mV";     break;
    case _EC:    typeName = "EC";     unitName = "uS/cm";  break;
    case _TDS:   typeName = "TDS";    unitName = "ppm";    break;
    case _SAL:   typeName = "SAL";    unitName = "ppm";    break;
    case _TEMP:  typeName = "TEMP";   unitName = "°C";     break;
    case _HUM:   typeName = "HUM";    unitName = "%";      break;
    case _NTC:   typeName = "NTC";    unitName = "°C";     break;
    case _DS18:   typeName = "DS18";   unitName = "°C";     break;
    case _TIMER: typeName = "TIMER";  unitName = "s";      break;
    default:                                 break;
  }

  // Header descrittivo
  f.print("# TYPE: ");
  f.print(typeName);
  f.print(", UNIT: ");
  f.println(unitName);
  f.println("Timestamp,Average,ValMin,ValMax");
  f.close();
  yield();

  Serial.println(F("🧾 File .csv resettato e intestazione riscritta."));
  return true;
}

//files section 
/**

 */
bool WSER_ReporterAppendValueDataFile(uint8_t number, uint8_t type, int16_t value, int16_t valmin, int16_t valmax) {

    if (Reporter[number].filepath.length() == 0) {
      Serial.print(F("❌ Reporter[")); Serial.print(number);
      Serial.println(F("]: filepath non impostato."));
      return false;
    }

  File fw = LittleFS.open(Reporter[number].filepath, "a");
  if (!fw) {
    Serial.println(F("❌ Errore apertura file .csv in scrittura: "));
    Serial.println(Reporter[number].filepath);
    return false;
  }

  String ts = isoNowLocal(); // es. "2025-10-27 22:45:01"

  // Riga CSV: Timestamp,Average,ValMin,ValMax
  fw.printf("%s,%d,%d,%d\n", ts.c_str(), value, valmin, valmax);
  fw.close();


  return true;
}

// ====== FETCH HANDLERS ======================================================
/**

 */
void WSER_handleReporterGetStatus() {
  // ---- TYPE MAP 0 ----
  String type0 = "none";
  switch (s_Ctrl[0].type) {
    case _RX:  type0 = "orp"; break;
    case _PH:  type0 = "ph";  break;
    default: break;
  }
  // ---- RING INDEX 0 ----
  const uint8_t lastSecIdx0  = (Reporter[0].currentSecondIndex + VALUES_ARRAY_DIM_MINUTE_HOUR - 1) % VALUES_ARRAY_DIM_MINUTE_HOUR;
  const uint8_t lastMinIdx0  = (Reporter[0].currentMinuteIndex + VALUES_ARRAY_DIM_MINUTE_HOUR - 1) % VALUES_ARRAY_DIM_MINUTE_HOUR;
  const uint8_t lastHourIdx0 = (Reporter[0].currentHourIndex   + VALUES_ARRAY_DIM_DAY          - 1) % VALUES_ARRAY_DIM_DAY;
  const int lastSecVal0  = Reporter[0].valuesSeconds[lastSecIdx0];
  const int lastMinVal0  = Reporter[0].valuesMinutes[lastMinIdx0];
  const int lastHourVal0 = Reporter[0].valuesHours[lastHourIdx0];

  // ---- TYPE MAP 1 ----
  //String type1 = "ntc";
  String type1 = "ds18";

  // ---- RING INDEX 1 ----
  const uint8_t lastSecIdx1  = (Reporter[1].currentSecondIndex + VALUES_ARRAY_DIM_MINUTE_HOUR - 1) % VALUES_ARRAY_DIM_MINUTE_HOUR;
  const uint8_t lastMinIdx1  = (Reporter[1].currentMinuteIndex + VALUES_ARRAY_DIM_MINUTE_HOUR - 1) % VALUES_ARRAY_DIM_MINUTE_HOUR;
  const uint8_t lastHourIdx1 = (Reporter[1].currentHourIndex   + VALUES_ARRAY_DIM_DAY          - 1) % VALUES_ARRAY_DIM_DAY;
  const int lastSecVal1  = Reporter[1].valuesSeconds[lastSecIdx1];
  const int lastMinVal1  = Reporter[1].valuesMinutes[lastMinIdx1];
  const int lastHourVal1 = Reporter[1].valuesHours[lastHourIdx1];

  // ---- TYPE MAP 2 ----
  String type2 = "none";
  switch (s_Ctrl[2].type) {
    case _EC:  type2 = "ec";  break;
    case _TDS: type2 = "tds"; break;
    case _SAL: type2 = "sal"; break;
    case _NTC:  type2 = "ntc";  break;
    case _DIFF: type2 = "diff"; break;
    case _AVG: type2 = "avg"; break;
    default: break;
  }
  // ---- RING INDEX 2 ----
  const uint8_t lastSecIdx2  = (Reporter[2].currentSecondIndex + VALUES_ARRAY_DIM_MINUTE_HOUR - 1) % VALUES_ARRAY_DIM_MINUTE_HOUR;
  const uint8_t lastMinIdx2  = (Reporter[2].currentMinuteIndex + VALUES_ARRAY_DIM_MINUTE_HOUR - 1) % VALUES_ARRAY_DIM_MINUTE_HOUR;
  const uint8_t lastHourIdx2 = (Reporter[2].currentHourIndex   + VALUES_ARRAY_DIM_DAY          - 1) % VALUES_ARRAY_DIM_DAY;
  const int lastSecVal2  = Reporter[2].valuesSeconds[lastSecIdx2];
  const int lastMinVal2  = Reporter[2].valuesMinutes[lastMinIdx2];
  const int lastHourVal2 = Reporter[2].valuesHours[lastHourIdx2];

  // ---- TYPE MAP 3 ----
  String type3 = "none";
  switch (s_Ctrl[3].type) {
    case _RX:  type3 = "orp"; break;
    case _PH:  type3 = "ph";  break;
    default: break;
  }
  // ---- RING INDEX 3 ----
  const uint8_t lastSecIdx3  = (Reporter[3].currentSecondIndex + VALUES_ARRAY_DIM_MINUTE_HOUR - 1) % VALUES_ARRAY_DIM_MINUTE_HOUR;
  const uint8_t lastMinIdx3  = (Reporter[3].currentMinuteIndex + VALUES_ARRAY_DIM_MINUTE_HOUR - 1) % VALUES_ARRAY_DIM_MINUTE_HOUR;
  const uint8_t lastHourIdx3 = (Reporter[3].currentHourIndex   + VALUES_ARRAY_DIM_DAY          - 1) % VALUES_ARRAY_DIM_DAY;
  const int lastSecVal3  = Reporter[3].valuesSeconds[lastSecIdx3];
  const int lastMinVal3  = Reporter[3].valuesMinutes[lastMinIdx3];
  const int lastHourVal3 = Reporter[3].valuesHours[lastHourIdx3];


  // ---- JSON unico con 4 blocchi espliciti ----
  String json = "{";

  // blocco 1
  json += "\"status1row1\":\"" + Reporter[0].Row1 + "\",";
  json += "\"status1row2\":\"" + Reporter[0].Row2 + "\",";
  json += "\"type1\":\"" + type0 + "\",";
  json += "\"last_second_value1\":" + String(lastSecVal0) + ",";
  json += "\"last_second_index1\":" + String(lastSecIdx0) + ",";
  json += "\"last_minute_value1\":" + String(lastMinVal0) + ",";
  json += "\"last_minute_index1\":" + String(lastMinIdx0) + ",";
  json += "\"last_hour_value1\":"   + String(lastHourVal0) + ",";
  json += "\"last_hour_index1\":"   + String(lastHourIdx0) + ",";

  // blocco 2
  json += "\"status2row1\":\"" + Reporter[1].Row1 + "\",";
  json += "\"status2row2\":\"" + Reporter[1].Row2 + "\",";
  json += "\"type2\":\"" + type1 + "\",";
  json += "\"last_second_value2\":" + String(lastSecVal1) + ",";
  json += "\"last_second_index2\":" + String(lastSecIdx1) + ",";
  json += "\"last_minute_value2\":" + String(lastMinVal1) + ",";
  json += "\"last_minute_index2\":" + String(lastMinIdx1) + ",";
  json += "\"last_hour_value2\":"   + String(lastHourVal1) + ",";
  json += "\"last_hour_index2\":"   + String(lastHourIdx1) + ",";

  // blocco 3
  json += "\"status3row1\":\"" + Reporter[2].Row1 + "\",";
  json += "\"status3row2\":\"" + Reporter[2].Row2 + "\",";
  json += "\"type3\":\"" + type2 + "\",";
  json += "\"last_second_value3\":" + String(lastSecVal2) + ",";
  json += "\"last_second_index3\":" + String(lastSecIdx2) + ",";
  json += "\"last_minute_value3\":" + String(lastMinVal2) + ",";
  json += "\"last_minute_index3\":" + String(lastMinIdx2) + ",";
  json += "\"last_hour_value3\":"   + String(lastHourVal2) + ",";
  json += "\"last_hour_index3\":"   + String(lastHourIdx2) + ",";

  // blocco 4
  json += "\"status4row1\":\"" + Reporter[3].Row1 + "\",";
  json += "\"status4row2\":\"" + Reporter[3].Row2 + "\",";
  json += "\"type4\":\"" + type3 + "\",";
  json += "\"last_second_value4\":" + String(lastSecVal3) + ",";
  json += "\"last_second_index4\":" + String(lastSecIdx3) + ",";
  json += "\"last_minute_value4\":" + String(lastMinVal3) + ",";
  json += "\"last_minute_index4\":" + String(lastMinIdx3) + ",";
  json += "\"last_hour_value4\":"   + String(lastHourVal3) + ",";
  json += "\"last_hour_index4\":"   + String(lastHourIdx3);

  json += "}";

  server.send(200, "application/json", json);
}

/**

 */
void WSER_WiFiReporterPage(){

  Serial.print("REPORTER: Heap available before send page: ");
  Serial.println(ESP.getFreeHeap());

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send_P(200, "text/html", WSER_HTML_REPORTER_HEAD);

  server.sendContent_P(WSER_COMMON_PAGES_STYLE); 

  server.sendContent_P(WSER_HTML_REPORTER_STYLE);

  server.sendContent_P(WSER_HTML_REPORTER_HEAD_OPEN);

  String nav = WSER_GenerateNavBar("reporter");
  server.sendContent(nav);

  server.sendContent_P(WSER_HTML_REPORTER_BODY_OPEN);
  server.sendContent_P(WSER_HTML_REPORTER_BODY_CONTENT);

  String scriptReporter = FPSTR(WSER_SCRIPT_REPORTER);

// ---- CONTROL 1 ----
for (int i = 0; i < VALUES_ARRAY_DIM_MINUTE_HOUR; i++) {
  scriptReporter.replace("{secVal" + String(i) + "_1}", String(Reporter[0].valuesSeconds[i]));
}
for (int i = 0; i < VALUES_ARRAY_DIM_MINUTE_HOUR; i++) {
  scriptReporter.replace("{minVal" + String(i) + "_1}", String(Reporter[0].valuesMinutes[i]));
}
for (int i = 0; i < VALUES_ARRAY_DIM_DAY; i++) {
  scriptReporter.replace("{hourVal" + String(i) + "_1}", String(Reporter[0].valuesHours[i]));
}

// ---- CONTROL 2 ----
for (int i = 0; i < VALUES_ARRAY_DIM_MINUTE_HOUR; i++) {
  scriptReporter.replace("{secVal" + String(i) + "_2}", String(Reporter[1].valuesSeconds[i]));
}
for (int i = 0; i < VALUES_ARRAY_DIM_MINUTE_HOUR; i++) {
  scriptReporter.replace("{minVal" + String(i) + "_2}", String(Reporter[1].valuesMinutes[i]));
}
for (int i = 0; i < VALUES_ARRAY_DIM_DAY; i++) {
  scriptReporter.replace("{hourVal" + String(i) + "_2}", String(Reporter[1].valuesHours[i]));
}


// ---- CONTROL 3 ----
for (int i = 0; i < VALUES_ARRAY_DIM_MINUTE_HOUR; i++) {
  scriptReporter.replace("{secVal" + String(i) + "_3}", String(Reporter[2].valuesSeconds[i]));
}
for (int i = 0; i < VALUES_ARRAY_DIM_MINUTE_HOUR; i++) {
  scriptReporter.replace("{minVal" + String(i) + "_3}", String(Reporter[2].valuesMinutes[i]));
}
for (int i = 0; i < VALUES_ARRAY_DIM_DAY; i++) {
  scriptReporter.replace("{hourVal" + String(i) + "_3}", String(Reporter[2].valuesHours[i]));
}

// ---- CONTROL 4 ----
for (int i = 0; i < VALUES_ARRAY_DIM_MINUTE_HOUR; i++) {
  scriptReporter.replace("{secVal" + String(i) + "_4}", String(Reporter[3].valuesSeconds[i]));
}
for (int i = 0; i < VALUES_ARRAY_DIM_MINUTE_HOUR; i++) {
  scriptReporter.replace("{minVal" + String(i) + "_4}", String(Reporter[3].valuesMinutes[i]));
}
for (int i = 0; i < VALUES_ARRAY_DIM_DAY; i++) {
  scriptReporter.replace("{hourVal" + String(i) + "_4}", String(Reporter[3].valuesHours[i]));
}

  String typeCtrl1;
  String typeCtrl2;
  String typeCtrl3;
  String typeCtrl4;

  switch (s_Ctrl[0].type) {
    case _PH:  typeCtrl1 = "ph";  break;
    case _RX: typeCtrl1 = "orp"; break;
  }

  //typeCtrl2 = "ntc";
  typeCtrl2 = "ds18";

  switch (s_Ctrl[2].type) {
    case _EC:  typeCtrl3 = "ec";  break;
    case _TDS: typeCtrl3 = "tds"; break;
    case _SAL: typeCtrl3 = "sal"; break;
    case _NTC: typeCtrl3 = "ntc"; break;
    case _DIFF: typeCtrl3 = "diff"; break;
    case _AVG: typeCtrl3 = "avg"; break;
  }

  switch (s_Ctrl[3].type) {
    case _PH:  typeCtrl4 = "ph";  break;
    case _RX: typeCtrl4 = "orp"; break;
  }


  scriptReporter.replace("{SENSOR1_TYPE}", typeCtrl1);
  scriptReporter.replace("{SENSOR2_TYPE}", typeCtrl2);
  scriptReporter.replace("{SENSOR3_TYPE}", typeCtrl3);
  scriptReporter.replace("{SENSOR4_TYPE}", typeCtrl4);

  scriptReporter.replace("{IS_AP_FLAG}", WSER_IsAccessPointMode() ? "true" : "false");

  server.sendContent(scriptReporter);

  server.sendContent_P(WSER_REPORTER_CLOSE_PAGE);

  Serial.print("REPORTER: Heap available after send page: ");
  Serial.println(ESP.getFreeHeap());

}



// ====== SMART HOME (stub) ===================================================
/**

 */
String WSER_ExtCmdReporterGetStatus(uint8_t ctrl) {
  // Validazione base: accettiamo solo 1..4
  if (ctrl < 1 || ctrl > 4) {
    return "{\"error\":\"Invalid reporter index (must be 1..4)\"}";
  }

  uint8_t idx = ctrl - 1;

  String json = "{";
  json += "\"active\":"   + String(Reporter[idx].active ? 1 : 0) + ",";
  json += "\"period\":"   + String(Reporter[idx].periodReport);
  json += "}";

  return json;
}


/**

 */
void WSER_handleSetReporter() {

 if (!server.hasArg("reporter") || !server.hasArg("active") || !server.hasArg("period")) {
    server.send(400, "application/json", "{\"error\":\"Missing 'reporter', 'active' or 'period' parameter\"}");
    return;
  }

  int reporterIdx = server.arg("reporter").toInt();
  bool active     = (server.arg("active") == "1");
  int period      = server.arg("period").toInt();


  if (reporterIdx < 1 || reporterIdx > 4) {
    server.send(400, "application/json", "{\"error\":\"Invalid reporter index (must be 1..4)\"}");
    return;
  }

  if ((period != 1 ) && (period != 7 ) && (period != 30 ) ) {
    server.send(400, "application/json", "{\"error\":\"Invalid reporter index (must be 1 7 or 30)\"}");
    return;
  }

  if ( WSER_ExtCmdSetReporter(reporterIdx, active, period)){
    server.send(200, "application/json", "{\"status\":\"ok\",\"message\":\"Reporter updated\"}");
  }else {
    server.send(500, "application/json", "{\"error\":\"Failed to update Reporter\"}");
  }

}

/**

 */
bool WSER_ExtCmdSetReporter(uint8_t ctrl, bool active, uint8_t period) {
  // Validazioni
  if ((period != 1) && (period != 7) && (period != 30)) {
    Serial.println("period not in {1, 7, 30} days");
    return false;
  }
  if ((ctrl >4) || (ctrl == 0 )) {
    return false;
  }

uint8_t ui8Index = ctrl -1;  // solo per log/leggibilità (1..4)

switch (ui8Index) {
  case 0: {
    // ACTIVE
    EEPR_WriteByte(EEPROM_REP0_ACTIVE_ADDR, active ? 1 : 0);
    Reporter[0].active = active;

    // PERIOD
    EEPR_WriteByte(EEPROM_REP0_PERIOD_ADDR, period);
    Reporter[0].periodReport = EEPR_ReadByte(EEPROM_REP0_PERIOD_ADDR);
    Reporter[0].periodSet    = (REPORTER_FAKTOR_60_SAMPLES * Reporter[0].periodReport);

    // Reset runtime counters
    Reporter[0].periodcnt  = 0;
    Reporter[0].samplesCnt = 0;
    Reporter[0].AvgPeriod  = 0;
    Reporter[0].MinPeriod  = MAX_VALUE_REPO;
    Reporter[0].MaxPeriod  = MIN_VALUE_REPO;

    WSER_ReporterResetDataFile(0);

  } break;

  case 1: {
    EEPR_WriteByte(EEPROM_REP1_ACTIVE_ADDR, active ? 1 : 0);
    Reporter[1].active = active;

    EEPR_WriteByte(EEPROM_REP1_PERIOD_ADDR, period);
    Reporter[1].periodReport = EEPR_ReadByte(EEPROM_REP1_PERIOD_ADDR);
    Reporter[1].periodSet    = (REPORTER_FAKTOR_60_SAMPLES * Reporter[1].periodReport);

    Reporter[1].periodcnt  = 0;
    Reporter[1].samplesCnt = 0;
    Reporter[1].AvgPeriod  = 0;
    Reporter[1].MinPeriod  = MAX_VALUE_REPO;
    Reporter[1].MaxPeriod  = MIN_VALUE_REPO;

    WSER_ReporterResetDataFile(1);

  } break;

  case 2: {
    EEPR_WriteByte(EEPROM_REP2_ACTIVE_ADDR, active ? 1 : 0);
    Reporter[2].active = active;

    EEPR_WriteByte(EEPROM_REP2_PERIOD_ADDR, period);
    Reporter[2].periodReport = EEPR_ReadByte(EEPROM_REP2_PERIOD_ADDR);
    Reporter[2].periodSet    = (REPORTER_FAKTOR_60_SAMPLES * Reporter[2].periodReport);

    Reporter[2].periodcnt  = 0;
    Reporter[2].samplesCnt = 0;
    Reporter[2].AvgPeriod  = 0;
    Reporter[2].MinPeriod  = MAX_VALUE_REPO;
    Reporter[2].MaxPeriod  = MIN_VALUE_REPO;

    WSER_ReporterResetDataFile(2);

  } break;

  case 3: {
    EEPR_WriteByte(EEPROM_REP3_ACTIVE_ADDR, active ? 1 : 0);
    Reporter[3].active = active;

    EEPR_WriteByte(EEPROM_REP3_PERIOD_ADDR, period);
    Reporter[3].periodReport = EEPR_ReadByte(EEPROM_REP3_PERIOD_ADDR);
    Reporter[3].periodSet    = (REPORTER_FAKTOR_60_SAMPLES * Reporter[3].periodReport);

    Reporter[3].periodcnt  = 0;
    Reporter[3].samplesCnt = 0;
    Reporter[3].AvgPeriod  = 0;
    Reporter[3].MinPeriod  = MAX_VALUE_REPO;
    Reporter[3].MaxPeriod  = MIN_VALUE_REPO;

    WSER_ReporterResetDataFile(3);

  } break;

}
  return true;
}