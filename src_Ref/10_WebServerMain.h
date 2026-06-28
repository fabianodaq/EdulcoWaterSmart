#ifndef APP_WEBBROWSER_H
#define APP_WEBBROWSER_H

#include <Arduino.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP_Mail_Client.h>
#include <time.h>
#include <WebServer.h>
#include <pgmspace.h>

//--Email sender definitions
#define EMAIL_RECEIVER_DEFAULT   "name@domain.com"
#define AUTHOR_EMAIL_DEFAULT     "edulcowater.mailer@gmail.com"
#define AUTHOR_PASSWORD_DEFAULT  "bjtvoxojjdmehzcw"   // <-- la password per app generata da Google

#define WIFI_MIN_RSSI_THRESHOLD -75  // Soglia minima per una connessione considerata stabile
#define WIFI_MAX_ATTEMPTS 5
#define WIFI_ATTEMPT_INTERVAL 2000  // in ms

//----------- Constants definitions-----------------

// Barra di navigazione HTML riutilizzabile
/**

 */
const char WSER_COMMON_PAGES_STYLE[] PROGMEM  = R"rawliteral(
<style>

    /* ================================
      Common style
    ================================ */

    body { font-family: Arial, sans-serif; margin: 0; padding: 16px; }

    .line-page-sub-section { border-top: 2px solid black; width: 70%; margin: 10px auto; }

    .status-box { margin-top: 10px; padding: 15px; border: 1px solid #ccc; background: #f9f9f9; 
                  border-radius: 8px; font-size: 16px; text-align: center; width: 60%; 
                  margin-left: auto; margin-right: auto; }

    .section-title { text-align: center; font-size: 18px; margin-top: 10px; font-weight: bold; 
                      margin-bottom: 20px; }

    .submit-activation { padding: 10px 20px; font-size: 16px; font-weight: bold; border-radius: 8px; background-color: #00BFFF; color: white; border: none; 
                        cursor: pointer; }

    /* ================================
      Navigation Bar Style
    ================================ */

    nav { background: #f6f6f6; border: 1px solid #ccc; width: 100%; margin: auto; 
          border-radius: 6px; overflow: hidden; box-shadow: 0 2px 5px rgba(0,0,0,0.1); }

    nav ul { list-style: none; display: flex; padding: 0; margin: 0;   justify-content: flex-start; overflow-x: auto; white-space: nowrap; -webkit-overflow-scrolling: touch; }
    
    nav ul::-webkit-scrollbar { display: none; }

    nav li { margin: 0; }

    nav a { display: block; padding: 6px; color: #555; text-decoration: none;
            font-weight: bold; background: #f6f6f6; transition: background 0.3s, color 0.3s; }

    nav a:hover { background: #e0e0e0; }

    nav a.active { background: #2a85cc; color: white !important; }

</style>
)rawliteral";


//----------- Struct defintions-----------------
String WSER_GenerateNavBar(const String& activeTab);

//----------- Struct defintions-----------------

//------- Global variables---------------
extern WebServer server;

//------- Global Functions---------------
extern void WSER_InitWebServer();
extern void WSER_HandleWebServer();

#endif 