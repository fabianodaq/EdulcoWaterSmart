#ifndef WSER_ABOUTPAGE_PRIVATE_H
#define WSER_ABOUTPAGE_PRIVATE_H

#include "03_EEprom.h"


#define DEVICE_TYPE_NAME   "EdulcoWaterPlattform"
#define HW_VERSION_STRING  "04.00"
#define SW_VERSION_STRING  "01.00"


const char WSER_HTML_ABOUT_HEAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>About</title>
)rawliteral";

const char WSER_HTML_ABOUT_BODY_OPEN[] PROGMEM = R"rawliteral(
</head>
<body>
)rawliteral";

const char WSER_HTML_ABOUT_STYLE[] PROGMEM  = R"rawliteral(
  <style>

    .container { max-width: 800px; margin: 40px auto; padding: 20px; background: #ffffff; border-radius: 8px; box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1); }

    .info-block { margin-bottom: 20px; padding: 15px; background-color: #f0f0f0; border-radius: 6px; border-left: 4px solid #2a85cc; }
    .info-block a { display: block; margin-bottom: 5px; color: #2a85cc; font-weight: bold; text-decoration: none; }
    .info-block a:hover { text-decoration: underline; }

    p { text-decoration: none; }

  </style>
)rawliteral";

const char WSER_HTML_ABOUT_BODY_CONTENT[] PROGMEM = R"rawliteral(
<div class="container">

<h1>ℹ️ EdulcoWater</h1>

<div class="info-block">
  <p>
    <strong>EdulcoWater Platform</strong> is a next-generation modular system for <strong>smart water quality management</strong>, combining <strong>precision</strong>, <strong>scalability</strong>, and <strong>connectivity</strong>.<br><br>

    Through its various controller units, the platform can measure and regulate 
    <strong>pH</strong>, <strong>Redox (ORP)</strong>, <strong>Temperature</strong>, 
    <strong>Electrical Conductivity (EC)</strong>, <strong>Salinity</strong>, and 
    <strong>Total Dissolved Solids (TDS)</strong> — delivering a complete 
    <strong>6-in-1 solution</strong> for professional water automation.<br><br>

    Each module communicates via <strong>Wi-Fi</strong> in both 
    <strong>Access Point</strong> and <strong>Station</strong> modes, enabling 
    <strong>remote control</strong>, <strong>automatic dosing</strong>, and 
    <strong>real-time monitoring</strong>.<br><br>

    Advanced features include <strong>Sentinel protection</strong> (threshold monitoring and alerts), 
    <strong>report generation</strong> for trend analysis, and integration with an 
    <strong>AI assistant</strong> for smart recommendations and predictive maintenance.<br><br>

    Designed for <strong>hydroponics</strong>, <strong>aquariums</strong>, 
    <strong>pools</strong>, and <strong>industrial water systems</strong>, 
    <strong>EdulcoWater Platform</strong> brings together 
    <strong>innovation</strong>, <strong>reliability</strong>, and 
    <strong>intelligence</strong> — making water management truly 
    <strong>connected and autonomous</strong>.
  <p>
    🔎 <strong>Device Information</strong><br>
    Device Type: {DEVICE_TYPE}<br>
    HW: {HW_VERSION}<br>
    SW: {SW_VERSION}<br>
    Device ID: {DEVICE_ID}
  </p>

  <p>🛒 You can purchase EdulcoWater devices at:</p>
  <a href="https://www.ebay.de/usr/studios_de" target="_blank">EdulcoWater on Ebay</a>

  <p>🌐 Visit the Studios official website:</p>
  <a href="https://www.controllerstudios.com/" target="_blank">www.controllerstudios.com</a>

  <p>👥 EdulcoWater is also on Facebook:</p>
  <a href="https://www.facebook.com/EdulcoWater" target="_blank">facebook.com/EdulcoWater</a>

  <p>📺 Watch tutorials and product videos on YouTube:</p>
  <a href="https://www.youtube.com/@EdulcoWater" target="_blank">youtube.com/@EdulcoWater</a>

  <p>📧 Send support request (please keep the default subject: edulco-support):</p>
  <a href="mailto:edulcowater.mailer@gmail.com?subject=edulco-support" target="_blank" >edulcowater.mailer@gmail.com</a>

</div>

)rawliteral";

const char WSER_SCRIPT_ABOUT_STATIC[] PROGMEM = R"rawliteral(
<script>

  window.onload = function() {
    const isAPMode = window.location.hostname === "192.168.4.1";

    if (isAPMode) {
      alert("⚠️ System is in Access Point Mode. External links are not available.");

      const links = document.querySelectorAll("a[target='_blank']");
      links.forEach(link => {
        link.style.pointerEvents = "none";
        link.style.opacity = "0.5";
        link.style.textDecoration = "line-through";
        link.title = "Link not available in AP mode";
      });
    }
  };
</script>
)rawliteral";

const char WSER_ABOUT_CLOSE_PAGE[] PROGMEM = R"rawliteral(
</body>
</html>
)rawliteral";


#endif 
