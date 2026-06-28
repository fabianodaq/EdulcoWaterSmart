#include "10_WebServerMain.h"
#include "21_AboutPage_private.h"

//--- Private Variables -------------------------------------------------------

//--- Public Variables --------------------------------------------------------

//--- Private Function Declarations ------------------------------------------


//--- Public Function Declarations -------------------------------------------
void WSER_InitAboutSection();

//internal fucntions
void WSER_WiFiAboutPage();

String WSER_ExtCmdGetDeviceAbout();

extern String getShortID();
//-----------------------------------------------------------------------------
//--- PUBLIC Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void WSER_InitAboutSection(){ // << Chiamata al nuovo modulo separato

}

//-----------------------------------------------------------------------------
//--- PRIVATE Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void WSER_WiFiAboutPage() {
  Serial.print("ABOUT: Heap available before send page: ");
  Serial.println(ESP.getFreeHeap());

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send_P(200, "text/html", WSER_HTML_ABOUT_HEAD);       // <!DOCTYPE html><html><head>...

  server.sendContent_P(WSER_COMMON_PAGES_STYLE);          // CSS
  server.sendContent_P(WSER_HTML_ABOUT_STYLE);                 // <style> contenuto in PROGMEM
  server.sendContent_P(WSER_HTML_ABOUT_BODY_OPEN);             // </head><body>

  String temp = WSER_GenerateNavBar("about");                  // Navbar attiva su tab "about"
  server.sendContent(temp);

  // Corpo principale con sostituzioni dinamiche
  temp = FPSTR(WSER_HTML_ABOUT_BODY_CONTENT);
  temp.replace("{DEVICE_TYPE}", DEVICE_TYPE_NAME);
  temp.replace("{HW_VERSION}", HW_VERSION_STRING);
  temp.replace("{SW_VERSION}", SW_VERSION_STRING);

  String deviceID = getShortID();  // genera ID breve e univoco del chip
  temp.replace("{DEVICE_ID}", deviceID);

  server.sendContent(temp);

  // Script statici finali + chiusura </body></html>
  server.sendContent_P(WSER_SCRIPT_ABOUT_STATIC);
  
  server.sendContent_P(WSER_ABOUT_CLOSE_PAGE);

  Serial.print("ABOUT: Heap available after send page: ");
  Serial.println(ESP.getFreeHeap());
}

/**

 */
String WSER_ExtCmdGetDeviceAbout() {
  String deviceID = getShortID();

  char ssid[33];
  char password[33];
  EEPR_ReadCh32(AP_SSID_EEPROM_ADDR, ssid);
  EEPR_ReadCh32(AP_PWD_EEPROM_ADDR, password);

  String json = "{";
  json += "\"deviceType\":\"" DEVICE_TYPE_NAME "\",";     // es. "EdulcoWater-Premium"
  json += "\"hwVersion\":\"" HW_VERSION_STRING "\",";     // es. "01.00"
  json += "\"swVersion\":\"" SW_VERSION_STRING "\",";     // es. "01.00"
  json += "\"deviceID\":\"" + deviceID + "\",";           // MAC finale
  json += "\"ssid\":\"" + String(ssid) + "\",";           // SSID AP
  json += "\"password\":\"" + String(password) + "\",";   // PWD AP
  json += "\"ip\":\"" + WiFi.softAPIP().toString() + "\"";
  json += "}";
  return json;
}
