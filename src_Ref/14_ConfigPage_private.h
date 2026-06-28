#ifndef WSER_CONFIGPAGE_PRIVATE_H
#define WSER_CONFIGPAGE_PRIVATE_H

#include "03_EEprom.h"
#include "04_Emailer.h"

//---- Constants

//--ACCESS POINT definitions
#define SSID_AP_DEFAULT 	"EdulcoAP_"
#define SSID_PWD_DEFAULT 	"123456789"

//--STATION definitions
#define SSID_STA_DEFAULT        "default"
#define SSID_STA_PWD_DEFAULT    "111122223"




const char WSER_WIFI_CONFIG_PAGE_HEAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>WiFi Configurator</title>
)rawliteral";

/**
 * @brief Specific WiFi Configurator page style (form layout, input fields, panels, etc.)
 * 
 * This style is used only on the WiFi Configurator page and customizes the layout
 * of form inputs, status panels, and section titles.
 */
const char WSER_WIFI_CONFIG_PAGE_STYLE[] PROGMEM = R"rawliteral(
  <style>

    .page-title { text-align: center; font-size: 32px; font-weight: bold; margin-top: 10px; 
                    margin-bottom: 15px; }

    .section-block { display:flex; flex-direction:column; align-items:center; gap:10px; margin-top:15px; }

     .section-input-line { display:flex; align-items:center; justify-content:center; gap:10px; }

    .input-field { padding:6px; width:220px; font-size:16px; }

    .input-submit { padding:8px 20px; font-size:16px; font-weight:bold; cursor:pointer; }

    .status-panel { margin-top:10px; padding:15px; border:1px solid #ccc; background:#f9f9f9; 
                    border-radius:8px; font-size:16px; text-align:center; width:40%; margin-left:auto; 
                    margin-right:auto; }

    .section-line { border-top:2px solid black; width:40%; margin:10px auto; }

    .label-inline { font-weight:bold; margin-right:8px; }

  </style>

</head>
<body>
)rawliteral";

const char WSER_HTML_WIFI_CONFIG_MODAL[] PROGMEM = R"rawliteral(
<!-- Modal di conferma Access Point -->

<div id="confirmationModal" style=" display: none; position: fixed; top: 0; left: 0; right: 0; bottom: 0;
                                    background-color: rgba(0,0,0,0.6); justify-content: center; 
                                    align-items: center; z-index: 9999; ">

  <div style=" background-color: white; padding: 20px 30px; border-radius: 10px; text-align: center;
              max-width: 400px; box-shadow: 0 0 10px rgba(0,0,0,0.3); ">

    <div id="popupMessage" style="margin-bottom: 20px; font-size: 16px;"></div> 

     <button onclick="confirmSave()" style=" margin-right: 10px; padding: 8px 16px; font-size: 16px;
                                            border: none; border-radius: 5px; background-color: #4CAF50;
                                            color: white; cursor: pointer; ">Yes</button>

    <button onclick="cancelSave()" style=" padding: 8px 16px; font-size: 16px; border: none;
                                          border-radius: 5px; background-color: #f44336; color: white;
                                          cursor: pointer; ">No</button>

  </div>
</div>
)rawliteral";

const char WSER_HTML_WIFI_BODY[] PROGMEM = R"rawliteral(

<div class="page-title">📡 WiFi Configurator</div>
<hr class="section-line">

<!-- Section AP 🔗 -->
<div class="section-block">
  
  <div class="section-title">🔗 Access Point</div>

  <div class="section-input-line">

    <label for="ssid" class="label-inline">📶 SSID:</label>
    <input type="text" id="ssid" name="ssid" required class="input-field">

    <label for="password" class="label-inline">🔐 Pass:</label>
    <input type="password" id="password" class="input-field" name="password" required minlength="8" placeholder="Enter password" onfocus="this.type='text'" onblur="if(this.value.length >= 8) this.type='password';">

    <button class="input-submit" onclick="saveAP()">💾 </button>
    <button class="input-submit" onclick="forgetAP()">🧹 </button>

  </div>

  <div id="statusPanel" class="status-panel">
    📶 SSID:&nbsp;<strong id="ssidValue">Loading...</strong><br>
    IP:&nbsp;<strong id="ipValue">Loading...</strong>
  </div>

  <hr class="section-line">
</div>

<!-- Section station 🌐 -->
<div class="section-block">

  <div class="section-title">🌐 Station</div>

  <div class="section-input-line">

    <label for="station_ssid" class="label-inline">📶 SSID:</label>
    <input type="text" id="station_ssid" name="station_ssid" required class="input-field">

    <label for="station_password" class="label-inline">🔐 Pass:</label>
    <input type="password" id="station_password" name="station_password" required minlength="8" placeholder="Enter password"
         onfocus="this.type='text'" onblur="if(this.value.length >= 8) this.type='password';" class="input-field">

    <button class="input-submit" onclick="saveStation()">💾 </button>
    <button class="input-submit" onclick="forgetStation()">🧹 </button>

  </div>


 <div id="stationStatusPanel" class="status-panel">
    📶 SSID:&nbsp;<strong id="station_ssidValue">Loading...</strong><br>
    IP:&nbsp;<strong id="station_ipValue">Loading...</strong>
  </div>

  <hr class="section-line">

</div>

<!-- Section email 📧 -->
<div class="section-block">

  <div class="section-title">📧 Email Contacts</div>

  <div class="section-input-line">
    <label for="EmailSentinel1" class="label-inline">📭 Receiver: </label>
    <input type="email" id="EmailSentinel1" class="input-field" placeholder="name@example.com">
    <button class="input-submit" onclick="saveEmail()">💾 Save</button>
    <button class="input-submit" onclick="forgetEmail()">🧹 Clean</button>
  </div>



  <div class="section-input-line">

    <label for="emailsender" class="label-inline">📧 Sender:</label>
    <input type="email" id="emailsender" class="input-field" placeholder="name@example.com">

    <label for="password_sender" class="label-inline">🔐 Pass:</label>
    <input type="password" id="password_sender" class="input-field" name="password_sender" placeholder="Enter password" onfocus="this.type='text'" onblur="this.type='password'">

    <button class="input-submit" onclick="saveEmailSender()">💾 </button>
    <button class="input-submit" onclick="cleanEmailSender()">🧹 </button>

  </div>


  <div id="emailStatusPanel" class="status-panel">
    📭 Receiver:&nbsp;<strong id="email_contactValue">Loading...</strong><br>
    📧 Sender :&nbsp;<strong id="email_senderValue">Loading...</strong>
  </div>

  <hr class="section-line">
  
</div>

)rawliteral";

const char WSER_SCRIPT_WIFI_CONFIG_STATIC[] PROGMEM = R"rawliteral(
<script>
 

  // Funzione per aggiornare lo stato WiFi
  function updateStatus() {
    fetch('/WiFiConfigGetStatus')
      .then(response => response.json())
      .then(data => {
        document.getElementById("ssidValue").textContent            = data.ssid       || "Loading...";
        document.getElementById("ipValue").textContent              = data.ip         || "Loading...";
        document.getElementById("station_ssidValue").textContent    = data.sta_ssid   || "Loading...";
        document.getElementById("station_ipValue").textContent      = data.sta_ip     || "Loading...";
        document.getElementById("email_contactValue").textContent   = data.email_con  || "Loading...";
        document.getElementById("email_senderValue").textContent    = data.email_sen  || "Loading...";
        
      })
      .catch(error => {
        console.error('Errore nel recuperare lo stato:', error);
      });
  }

  window.onload = function() {
    updateStatus();
    setInterval(updateStatus, 1000); // Aggiorna ogni secondo
  };

//----access point fucnions------ 
function forgetAP() {

// Scrive i valori di default nei campi input
  document.getElementById("ssid").value = "{DEFAULT_AP_SSID}";
  document.getElementById("password").value = "{DEFAULT_AP_PASS}";

  // Mostra popup di conferma
  document.getElementById("popupMessage").innerHTML = 
    `Press <strong>Yes</strong> to restore factory default Access Point credentials.<br>
     The new network will be <strong>"{DEFAULT_AP_SSID}"</strong> with password <strong>"{DEFAULT_AP_PASS}"</strong>.<br>
     After reset, reconnect using <strong>{DEFAULT_AP_IP}</strong>.`;

  // Mostra il popup di conferma
  document.getElementById("confirmationModal").style.display = "flex";
}

function saveAP() {
  const ssid = document.getElementById("ssid").value;
  const password = document.getElementById("password").value;

  // --- VALIDAZIONE ---
  if (!ssid) {
    alert("📶 SSID field cannot be empty.");
    document.getElementById("ssid").focus();
    return;
  }
  if (!password) {
    alert("🔐 Password field cannot be empty.");
    document.getElementById("password").focus();
    return;
  }
  if (password.length < 8) {
    alert("🔐 Password must be at least 8 characters long.");
    document.getElementById("password").focus();
    return;
  }

  // Messaggio di conferma nel popup
  document.getElementById("popupMessage").innerHTML = 
    `Press <strong>Yes</strong> to confirm the new credentials.<br>
    After saving, if system is in <strong>Access Point Mode</strong> reconnect to the network 
    <strong>"${ssid}"</strong> with the password <strong>"${password}"</strong> 
    using the address <strong>{DEFAULT_AP_IP}</strong>.`;

  // Mostra il popup di conferma
  document.getElementById("confirmationModal").style.display = "flex";
}

// Annulla e ricarica la pagina
function cancelSave() {
  document.getElementById("confirmationModal").style.display = "none";
  window.location.reload();
}

// Conferma salvataggio AP
function confirmSave() {
  const ssid = document.getElementById("ssid").value;
  const password = document.getElementById("password").value;

  const isAP = checkIfAccessPoint(); // Verifica se il sistema è in modalità Access Point

  if (isAP) {
    // Mostra messaggio a schermo pieno
    document.body.innerHTML = `
      <div style="display:flex;justify-content:center;align-items:center;height:100vh;margin:0;">
        <div style="text-align:center;">
          <h2 style="font-size:38px;">Configuration Saved</h2>
          <p style="font-size:26px;">Connect to "<strong>${ssid}</strong>" with Password "<strong>${password}</strong>"</p>
          <p style="font-size:26px;">Use IP: <strong>192.168.4.1</strong> for further configuration.</p>
        </div>
      </div>
    `;
  } else {
    document.getElementById("confirmationModal").style.display = "none";
    window.location.reload();
  }

  // Invio dei dati via POST
  fetch('/saveap', {
    method: 'POST',
    body: new URLSearchParams({
      'ssid': ssid,
      'password': password
    })
  })
  .then(response => response.text())
  .then(data => {
    console.log("Configurazione salvata:", data);
  })
  .catch(error => console.error('Errore nel salvataggio:', error));
}

//----Station fucnions------ 
function saveStation() {
  if (!checkIfAccessPoint()) {
    alert("⚠️ This action is only allowed in Access Point mode.\nPlease connect to the Access Point SSID via 192.168.4.1 and try again.");
    return;
  }

  const ssid = document.getElementById("station_ssid").value;
  const password = document.getElementById("station_password").value;

  fetch('/savestation', {
    method: 'POST',
    body: new URLSearchParams({
      'station_ssid': ssid,
      'station_password': password
    })
  })
  .then(response => response.json())
  .then(data => {
    console.log("Station credentials saved:", data);
    // Nessun updateStatus o alert: lo status si aggiorna da solo ogni secondo
    if (data.status === "success") {
      // Mostra messaggio ricevuto dal server
      alert(data.message);
    } else {
      alert("❌ An error occurred while saving.");
    }
  })
  .catch(error => {
    console.error("Error saving station credentials:", error);
  });
}

function forgetStation() {
  if (!confirm("Do you really want to forget the current WiFi network?\nThe device will restart in Access Point mode.")) {
    return;
  }

  fetch('/forgetsta', { method: 'POST', keepalive: true })
    .then(r => r.text())
    .then(apSSID => {
      document.body.innerHTML = `
        <div style="display:flex;justify-content:center;align-items:center;height:100vh;">
          <div style="text-align:center;">
            <h2 style="font-size:32px;">WiFi Station Credentials Deleted</h2>
            <p style="font-size:22px;">The device will restart in <strong>Access Point Mode</strong>.</p>
            <p style="font-size:22px;">Connect to "<strong>${apSSID}</strong>" IP address: <strong>192.168.4.1</strong>.</p>
          </div>
        </div>`;
    })
    .catch(err => console.error("Forget error:", err));
}
// ---- fine station

//----- email functions
function forgetEmail() {
  const emailEl = document.getElementById('EmailSentinel1');

  emailEl.value = "{DEFAULT_EMAIL}";   // imposta la default nel campo
  saveEmail();                     // riusa la stessa rotta /saveEmail
}


function saveEmail() {
  const emailEl = document.getElementById('EmailSentinel1');
  if (!emailEl) {
    console.error("[SaveEmail] Missing email field in DOM");
    alert("Missing email field.");
    return;
  }

    // -------------------- check email ------------------
  const email = (emailEl.value || "").trim();

  if (!email) {
    alert("Please insert one receiver email.");
    emailEl.focus();
    return;
  }

  // Regex semplice e robusta: nome@dominio.tld (tld >= 2)
  const emailPattern = /^[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,}$/i;

  if (!emailPattern.test(email)) {
    alert("Email not valid. Example: name@domain.com");
    emailEl.focus();
    return;
  }

  // normalizza in minuscolo per invio
  const email_ = email.toLowerCase();
  // -------------------- check email ------------------

  // --- Invio al server ---
  fetch('/saveEmail', {
    method: 'POST',
    body: new URLSearchParams({
      'email': email_
    })
  })
  .then(response => response.text())
  .then(data => {
    console.log("[SaveEmail] Success:", data);
    alert("Email saved successfully!");
  })
  .catch(error => {
    console.error("[SaveEmail] Error:", error);
    alert("Error while saving email.");
  });
}

function saveEmailSender() {
  const emailEl = document.getElementById('emailsender');
  const passEl = document.getElementById('password_sender');

  if (!emailEl || !passEl) {
    console.error("[SaveEmailSender] Missing sender fields in DOM");
    alert("Missing sender email or password field.");
    return;
  }

  // -------------------- check email ------------------
  const email = (emailEl.value || "").trim();
  if (!email) {
    alert("Please insert the sender email.");
    emailEl.focus();
    return;
  }

  const emailPattern = /^[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,}$/i;
  if (!emailPattern.test(email)) {
    alert("Email not valid. Example: name@domain.com");
    emailEl.focus();
    return;
  }

  const email_ = email.toLowerCase();
  // -------------------- check email ------------------

  // -------------------- check password ------------------
  const password = (passEl.value || "").trim();
  if (!password) {
    alert("Please insert the sender password.");
    passEl.focus();
    return;
  }
  // -------------------- check password ------------------

  // --- Invio al server ---
  fetch('/saveEmailSender', {
    method: 'POST',
    body: new URLSearchParams({
      'email': email_,
      'password': password
    })
  })
  .then(response => response.text())
  .then(data => {
    console.log("[SaveEmailSender] Success:", data);
    alert("Sender credentials saved successfully!");
  })
  .catch(error => {
    console.error("[SaveEmailSender] Error:", error);
    alert("Error while saving sender credentials.");
  });
}


function cleanEmailSender() {
  const emailEl = document.getElementById('emailsender');
  const passEl = document.getElementById('password_sender');

  emailEl.value = "{DEFAULT_EMAIL_SENDER}";
  passEl.value = "{DEFAULT_PASSWORD_SENDER}";

  // Riutilizza la funzione di salvataggio per aggiornare la EEPROM lato server
  saveEmailSender();
}


// ---------------- end email fucntions

  // Verifica se sei in modalità Access Point
  function checkIfAccessPoint() {
    return window.location.hostname === "{DEFAULT_AP_IP}";
  }

</script>
</body>
</html>
)rawliteral";
//--- Private Variables -------------------------------------------------------


#endif 
