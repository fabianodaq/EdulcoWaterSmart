#ifndef WSER_RELAYS_PRIVATE_H
#define WSER_RELAYS_PRIVATE_H

#define REMOTE_RELAYS_CHECK_INTERVAL 500 //every 500ms one connection/command can happen not less

#define REMOTE_RELAY_CONNETION_TIME_REFRESH 5000 // every 5 seconds try to connect if no connection happenend in the while

#define REMOTE_RELAY_TCP_PROBE_TIMEOUT_MS 150   // ms

//---- Constants
const char WSER_REMOTE_REL_HEAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Remote Relays</title>
)rawliteral";

const char WSER_REMOTE_REL_STYLE[] PROGMEM = R"rawliteral(
<style>

.slot-field-row { display: flex; justify-content: center; align-items: flex-start;  gap: 30px; flex-wrap: wrap; margin: 20px auto; }

input[type="text"], select { font-size: 18px; font-weight: bold; padding: 0 10px; border-radius: 6px; border: 1px solid #aaa; min-width: 140px; text-align: center; height: 40px; box-sizing: border-box; }

.slot-button-row { display: flex; justify-content: center; gap: 20px; margin-top: 15px; }

.slot-save-button, 

.slot-save-button { background-color: #00BFFF; color: white; }

.map-row  { display: flex; flex-direction: column; align-items: center; text-align: center; min-width: 150px; font-size: 16px; padding: 10px; 
            border: 2px solid black; border-radius: 8px; }

.slot-radio-inputs { display: flex; justify-content: center; align-items: center; gap: 15px; padding: 10px; margin-top: 7px}

.slot-radio-label { font-size: 18px; font-weight: bold; display: inline-flex; align-items: center; gap: 5px; }

</style>
)rawliteral";


const char WSER_REMOTE_REL_HEAD_OPEN[] PROGMEM = R"rawliteral(
</head>
<body>
)rawliteral";

const char WSER_REMOTE_REL_BODY_CONTENT[] PROGMEM = R"rawliteral(
  <div style='text-align: center; font-size: 32px; font-weight: bold;'>🔌 Relays</div>

  <hr class="line-page-sub-section">
  <div class="section-title">🔌 Relay Control: 1</div>

  <div class="slot-field-row">

      <div class="map-row">
        <div class="slot-radio-inputs">
          <label class="slot-radio-label"><input type="radio" name="ctrl1_relay" id="ctrl1_r0" value="0" checked> 🚫</label>
          <label class="slot-radio-label"><input type="radio" name="ctrl1_relay" id="ctrl1_r1" value="1"        > 1</label>
          <label class="slot-radio-label"><input type="radio" name="ctrl1_relay" id="ctrl1_r2" value="2"        > 2</label>
          <label class="slot-radio-label"><input type="radio" name="ctrl1_relay" id="ctrl1_r3" value="3"        > 3</label>
        </div>
        <div class="slot-button-row">
            <button class="slot-save-button" onclick="setRelayMapping(1)">💾 Save</button>
        </div>
      </div>

  </div>

  <div <div class="status-box">
    <strong id="Relay1StatusRow1">...</strong><br>
    <strong id="Relay1StatusRow2">...</strong>
  </div>

  <hr class="line-page-sub-section">
  <div class="section-title">🔌 Relay Control: 2</div>
<div class="slot-field-row">

  <div class="map-row">
    <div class="slot-radio-inputs">
      <label class="slot-radio-label"><input type="radio" name="ctrl2_relay" id="ctrl2_r0" value="0" checked> 🚫</label>
      <label class="slot-radio-label"><input type="radio" name="ctrl2_relay" id="ctrl2_r1" value="1"> 1</label>
      <label class="slot-radio-label"><input type="radio" name="ctrl2_relay" id="ctrl2_r2" value="2"> 2</label>
      <label class="slot-radio-label"><input type="radio" name="ctrl2_relay" id="ctrl2_r3" value="3"> 3</label>
    </div>
    <div class="slot-button-row">
      <button class="slot-save-button" onclick="setRelayMapping(2)">💾 Save</button>
    </div>
  </div>

</div>

  <div <div class="status-box">
    <strong id="Relay2StatusRow1">...</strong><br>
    <strong id="Relay2StatusRow2">...</strong>
  </div>

  <hr class="line-page-sub-section">
  <div class="section-title">🔌 Relay Control: 3</div>
<div class="slot-field-row">

  <div class="map-row">
    <div class="slot-radio-inputs">
      <label class="slot-radio-label"><input type="radio" name="ctrl3_relay" id="ctrl3_r0" value="0" checked> 🚫</label>
      <label class="slot-radio-label"><input type="radio" name="ctrl3_relay" id="ctrl3_r1" value="1"> 1</label>
      <label class="slot-radio-label"><input type="radio" name="ctrl3_relay" id="ctrl3_r2" value="2"> 2</label>
      <label class="slot-radio-label"><input type="radio" name="ctrl3_relay" id="ctrl3_r3" value="3"> 3</label>
    </div>
    <div class="slot-button-row">
      <button class="slot-save-button" onclick="setRelayMapping(3)">💾 Save</button>
    </div>
  </div>

</div>

  <div <div class="status-box">
    <strong id="Relay3StatusRow1">...</strong><br>
    <strong id="Relay3StatusRow2">...</strong>
  </div>

  <hr class="line-page-sub-section">
  <div class="section-title">🔌 Relay Control: 4</div>
<div class="slot-field-row">

  <div class="map-row">
    <div class="slot-radio-inputs">
      <label class="slot-radio-label"><input type="radio" name="ctrl4_relay" id="ctrl4_r0" value="0" checked> 🚫</label>
      <label class="slot-radio-label"><input type="radio" name="ctrl4_relay" id="ctrl4_r1" value="1"> 1</label>
      <label class="slot-radio-label"><input type="radio" name="ctrl4_relay" id="ctrl4_r2" value="2"> 2</label>
      <label class="slot-radio-label"><input type="radio" name="ctrl4_relay" id="ctrl4_r3" value="3"> 3</label>
    </div>
    <div class="slot-button-row">
      <button class="slot-save-button" onclick="setRelayMapping(4)">💾 Save</button>
    </div>
  </div>

</div>

  <div <div class="status-box">
    <strong id="Relay4StatusRow1">...</strong><br>
    <strong id="Relay4StatusRow2">...</strong>
  </div>

)rawliteral";

const char WSER_REMOTE_REL_SCRIPT[] PROGMEM = R"rawliteral(
<script>

//------------ routes --------------------------
function setRelayMapping(slotNum) {

  let relayValue;

  if (slotNum == 1) {
    relayValue = document.querySelector(`input[name="ctrl1_relay"]:checked`).value;

  } else if (slotNum == 2) {
    relayValue = document.querySelector(`input[name="ctrl2_relay"]:checked`).value;

  }  else if (slotNum == 3) {
    relayValue = document.querySelector(`input[name="ctrl3_relay"]:checked`).value;

  }  else if (slotNum == 4) {
    relayValue = document.querySelector(`input[name="ctrl4_relay"]:checked`).value;

  }else {
    console.error("❌ Invalid slot number.");
    return;
  }

    const message = `⚠️ Press OK to associate Control ${slotNum} to Hardware Relay ${relayValue}?`;
    if (!confirm(message)) {
      console.log("❌ Operation cancelled by user.");
      return;
    }

    fetch('/saveRelay', {
      method: 'POST',
      body: new URLSearchParams({
        'slot': slotNum,
        'hwrel': relayValue
      })
    })
  .then(response => {
    if (response.ok) {
      console.log(`✅ Control ${slotNum} successfully updated.`);
      alert(`✅ Control ${slotNum} successfully updated.`);
    } else if (response.status === 409) {
      console.warn(`⚠️ Relay already in use by another control.`);
      alert(`⚠️ Relay already in use by another control!`);
    } else {
      console.warn(`❌ Generic server error (${response.status}).`);
      alert(`❌ Server error (code ${response.status}).`);
    }
  })
  .catch(error => {
    console.error(`❌ Communication error for slot ${slotNum}:`, error);
    alert(`❌ Communication error with the server.`);
  });
}

// --- internal fucntion-----  

// --- get funtion-----  
function updateRemoteRelaysStatus() {
  fetch('/RemoteRelaysGetStatus')
    .then(response => response.json())
    .then(data => {
      // 🔹 Relay locali (Ctrl)
      document.getElementById("Relay1StatusRow1").textContent = data.MessStatusRelay1Row1 || "Loading...";
      document.getElementById("Relay1StatusRow2").textContent = data.MessStatusRelay1Row2 || "Loading...";

      document.getElementById("Relay2StatusRow1").textContent = data.MessStatusRelay2Row1 || "Loading...";
      document.getElementById("Relay2StatusRow2").textContent = data.MessStatusRelay2Row2 || "Loading...";

      document.getElementById("Relay3StatusRow1").textContent = data.MessStatusRelay3Row1 || "Loading...";
      document.getElementById("Relay3StatusRow2").textContent = data.MessStatusRelay3Row2 || "Loading...";

      document.getElementById("Relay4StatusRow1").textContent = data.MessStatusRelay4Row1 || "Loading...";
      document.getElementById("Relay4StatusRow2").textContent = data.MessStatusRelay4Row2 || "Loading...";
 
    })
    .catch(error => {
      console.error('❌ Errore nel recuperare lo stato dei Remote Relays:', error);
    });
}

// --- load fuctnion----- 
  window.onload = function() {
    updateRemoteRelaysStatus();
    setInterval(updateRemoteRelaysStatus, 1000); // Aggiorna ogni secondo
  };
</script>
)rawliteral";


const char WSER_REMOTE_REL_CLOSE_PAGE[] PROGMEM = R"rawliteral(
</body>
</html>
)rawliteral";
#endif 
