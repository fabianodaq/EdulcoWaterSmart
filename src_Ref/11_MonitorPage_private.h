#ifndef WSER_MONITOR_PRIVATE_H
#define WSER_MONITOR_PRIVATE_H

//---- Constants
const char WSER_MONITOR_HEAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Monitor</title>
)rawliteral";

const char WSER_MONITOR_STYLE[] PROGMEM = R"rawliteral(
<style>

.flex-container { display: flex; justify-content: center; flex-wrap: wrap; gap: 10px; align-items: stretch; }

.cardMainPanel1, 
.cardMainPanel2, 
.cardMainPanel3, 
.cardMainPanel4 { font-size: 18px; width: 240px; border-radius: 8px; overflow: hidden; box-shadow: 0 2px 5px rgba(0,0,0,0.2); color: white; text-align: center; 
                  font-weight: bold; display: flex; flex-direction: column; justify-content: space-between; padding: 0; }

.cardMainPanel1 { background-color: {CARD_TYPE_COLOR_1}; }
.cardMainPanel2 { background-color: {CARD_TYPE_COLOR_2}; }
.cardMainPanel3 { background-color: {CARD_TYPE_COLOR_3}; }
.cardMainPanel4 { background-color: {CARD_TYPE_COLOR_4}; }

.cardActPanel1, 
.cardActPanel2, 
.cardActPanel3, 
.cardActPanel4 { font-size: 18px; width: 240px; border-radius: 8px; overflow: hidden; box-shadow: 0 2px 5px rgba(0,0,0,0.2); color: white; text-align: center; 
                  font-weight: bold; display: flex; flex-direction: column; justify-content: space-between; padding: 0; }

.cardActPanel1 { background-color: {CARD_TYPE_COLOR_1}; }
.cardActPanel2 { background-color: {CARD_TYPE_COLOR_2}; }
.cardActPanel3 { background-color: {CARD_TYPE_COLOR_3}; }
.cardActPanel4 { background-color: {CARD_TYPE_COLOR_4}; }

.cardMainPanel-header { font-size: 30px; padding: 15px; }

.cardMainPanel-label { background-color: white; color: #333; font-size: 16px; padding: 15px; font-weight: normal; }

.cardActPanel-header { font-size: 20px; padding: 15px; }

.cardActPanel-ssid {  font-size: 14px; padding: 8px; background-color: rgb(186, 182, 182); color: #333; font-weight: bold; font-family: monospace;
                      font-style: italic; flex-shrink: 0; }

.cardActPanel-label { font-size: 14px; padding: 8px; background-color: white; color: #333; font-weight: bold; flex-shrink: 0; }

.cardInfoBox1, 
.cardInfoBox2,
.cardInfoBox3,
.cardInfoBox4 { font-size: 10px; width: 160px; border-radius: 8px; overflow: hidden; box-shadow: 0 2px 5px rgba(0,0,0,0.2); text-align: center; font-weight: bold; 
              color: white; }

.cardInfoBox1 { background-color: {CARD_TYPE_COLOR_1}; }
.cardInfoBox2 { background-color: {CARD_TYPE_COLOR_2}; }
.cardInfoBox3 { background-color: {CARD_TYPE_COLOR_3}; }
.cardInfoBox4 { background-color: {CARD_TYPE_COLOR_4}; }

.cardInfoBox-header { font-size: 20px; padding: 15px; }

.cardInfoBox-label { background-color: white; color: #333; font-size: 14px; padding: 8px; font-weight: normal; }

</style>
)rawliteral";

const char WSER_MONITOR_HEAD_OPEN[] PROGMEM = R"rawliteral(
</head>
<body>
)rawliteral";

const char WSER_MONITOR_BODY_OPEN[] PROGMEM = R"rawliteral(
<div style='text-align: center; font-size: 32px; font-weight: bold;'>📊 Monitor</div>
)rawliteral";

const char WSER_MONITOR_BODY_CONTENT[] PROGMEM = R"rawliteral(
<hr class="line-page-sub-section">

<div class="section-title">📊 Control 1</div>

<div class="flex-container">
  <div class="cardMainPanel1">
    <div id="MainValue1"  class="cardMainPanel-header">----</div>
    <div                  class="cardMainPanel-label">{SENSOR1_LABEL}</div>
  </div>

  <div class="cardActPanel1">
    <div id="ActuatorValue1"  class="cardActPanel-header">OFF</div>
    <div id="PlugSSID1"       class="cardActPanel-ssid">🕒 Timer: ---</div>
    <div                      class="cardActPanel-label">🔌 Actuator</div>
  </div>
</div>

<div class="flex-container" style="margin-top: 20px;">
  <div class="cardInfoBox1">
    <div id="LogicValue1"   class="cardInfoBox-header">POS</div>
    <div                    class="cardInfoBox-label">🧠 Logic</div>
  </div>
  <div class="cardInfoBox1">
    <div id="HysteresisValue1"  class="cardInfoBox-header">92mV</div>
    <div                        class="cardInfoBox-label">🔁 Hysteresis</div>
  </div>
  <div class="cardInfoBox1">
    <div id="TargetValue1"  class="cardInfoBox-header">-1682mV</div>
    <div                    class="cardInfoBox-label">🎯 Target</div>
  </div>
</div>

<hr class="line-page-sub-section">

<div class="section-title">📊 Control 2</div>

<div class="flex-container">
  <div class="cardMainPanel2">
    <div id="MainValue2"  class="cardMainPanel-header">----</div>
    <div                  class="cardMainPanel-label">{SENSOR2_LABEL}</div>
  </div>

  <div class="cardActPanel2">
    <div id="ActuatorValue2"  class="cardActPanel-header">OFF</div>
    <div id="PlugSSID2"       class="cardActPanel-ssid">🕒 Timer: ---</div>
    <div                      class="cardActPanel-label">🔌 Actuator</div>
  </div>
</div>

<div class="flex-container" style="margin-top: 20px;">
  <div class="cardInfoBox2">
    <div id="LogicValue2"   class="cardInfoBox-header">POS</div>
    <div                    class="cardInfoBox-label">🧠 Logic</div>
  </div>
  <div class="cardInfoBox2">
    <div id="HysteresisValue2"  class="cardInfoBox-header">92mV</div>
    <div                        class="cardInfoBox-label">🔁 Hysteresis</div>
  </div>
  <div class="cardInfoBox2">
    <div id="TargetValue2"  class="cardInfoBox-header">-1682mV</div>
    <div                    class="cardInfoBox-label">🎯 Target</div>
  </div>
</div>

<hr class="line-page-sub-section">

<div class="section-title">📊 Control 3</div>

<div class="flex-container">
  <div class="cardMainPanel3">
    <div id="MainValue3"  class="cardMainPanel-header">----</div>
    <div                  class="cardMainPanel-label">{SENSOR3_LABEL}</div>
  </div>

  <div class="cardActPanel3">
    <div id="ActuatorValue3"  class="cardActPanel-header">OFF</div>
    <div id="PlugSSID3"       class="cardActPanel-ssid">🕒 Timer: ---</div>
    <div                      class="cardActPanel-label">🔌 Actuator</div>
  </div>
</div>

<div class="flex-container" style="margin-top: 20px;">
  <div class="cardInfoBox3">
    <div id="LogicValue3"   class="cardInfoBox-header">POS</div>
    <div                    class="cardInfoBox-label">🧠 Logic</div>
  </div>
  <div class="cardInfoBox3">
    <div id="HysteresisValue3"  class="cardInfoBox-header">92mV</div>
    <div                        class="cardInfoBox-label">🔁 Hysteresis</div>
  </div>
  <div class="cardInfoBox3">
    <div id="TargetValue3"  class="cardInfoBox-header">-1682mV</div>
    <div                    class="cardInfoBox-label">🎯 Target</div>
  </div>
</div>

<hr class="line-page-sub-section">

<div class="section-title">📊 Control 4</div>

<div class="flex-container">
  <div class="cardMainPanel4">
    <div id="MainValue4"  class="cardMainPanel-header">----</div>
    <div                  class="cardMainPanel-label">{SENSOR4_LABEL}</div>
  </div>

  <div class="cardActPanel4">
    <div id="ActuatorValue4"  class="cardActPanel-header">OFF</div>
    <div id="PlugSSID4"       class="cardActPanel-ssid">🕒 Timer: ---</div>
    <div                      class="cardActPanel-label">🔌 Actuator</div>
  </div>
</div>

<div class="flex-container" style="margin-top: 20px;">
  <div class="cardInfoBox4">
    <div id="LogicValue4"   class="cardInfoBox-header">POS</div>
    <div                    class="cardInfoBox-label">🧠 Logic</div>
  </div>
  <div class="cardInfoBox4">
    <div id="HysteresisValue4"  class="cardInfoBox-header">92mV</div>
    <div                        class="cardInfoBox-label">🔁 Hysteresis</div>
  </div>
  <div class="cardInfoBox4">
    <div id="TargetValue4"  class="cardInfoBox-header">-1682mV</div>
    <div                    class="cardInfoBox-label">🎯 Target</div>
  </div>
</div>

)rawliteral";

const char WSER_MONITOR_SCRIPT[] PROGMEM = R"rawliteral(
<script>

function updateStatusMonitor() {
  fetch('/WiFiMonitorGetStatus')
    .then(response => response.json())
    .then(data => {
        // --- Control 1 ---
        document.getElementById("MainValue1").textContent = data.MainValue1 || "----";
        document.getElementById("ActuatorValue1").textContent = data.Actuator1 || "OFF";
        document.getElementById("LogicValue1").textContent = data.Logic1 || "-";
        document.getElementById("HysteresisValue1").textContent = data.Hysteresis1 || "-";
        document.getElementById("TargetValue1").textContent = data.Target1 || "-";
        document.getElementById("PlugSSID1").textContent = "🕒 Timer: " + (data.PlugSSID1 || "---");

        // --- Control 2 ---
        document.getElementById("MainValue2").textContent = data.MainValue2 || "----";
        document.getElementById("ActuatorValue2").textContent = data.Actuator2 || "OFF";
        document.getElementById("LogicValue2").textContent = data.Logic2 || "-";
        document.getElementById("HysteresisValue2").textContent = data.Hysteresis2 || "-";
        document.getElementById("TargetValue2").textContent = data.Target2 || "-";
        document.getElementById("PlugSSID2").textContent = "🕒 Timer: " + (data.PlugSSID2 || "---");

        // --- Control 3 ---
        document.getElementById("MainValue3").textContent = data.MainValue3 || "----";
        document.getElementById("ActuatorValue3").textContent = data.Actuator3 || "OFF";
        document.getElementById("LogicValue3").textContent = data.Logic3 || "-";
        document.getElementById("HysteresisValue3").textContent = data.Hysteresis3 || "-";
        document.getElementById("TargetValue3").textContent = data.Target3 || "-";
        document.getElementById("PlugSSID3").textContent = "🕒 Timer: " + (data.PlugSSID3 || "---");

        // --- Control 4 ---
        document.getElementById("MainValue4").textContent = data.MainValue4 || "----";
        document.getElementById("ActuatorValue4").textContent = data.Actuator4 || "OFF";
        document.getElementById("LogicValue4").textContent = data.Logic4 || "-";
        document.getElementById("HysteresisValue4").textContent = data.Hysteresis4 || "-";
        document.getElementById("TargetValue4").textContent = data.Target4 || "-";
        document.getElementById("PlugSSID4").textContent = "🕒 Timer: " + (data.PlugSSID4 || "---");
      })
    .catch(error => {
      console.error('Errore nel recuperare lo stato:', error);
    });
}

  window.onload = function() {
    updateStatusMonitor();
    setInterval(updateStatusMonitor, 500); // Aggiorna ogni secondo
  };

</script>
)rawliteral";

const char WSER_MONITOR_CLOSE_PAGE[] PROGMEM = R"rawliteral(
</body>
</html>
)rawliteral";

#endif 