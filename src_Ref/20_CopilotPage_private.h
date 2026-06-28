#ifndef WSER_COPILOT_PRIVATE_H
#define WSER_COPILOT_PRIVATE_H




// ----------- Extern Global variables (template) -----------

#define WSER_COPILOT_TIMEOUT_INTERVAL 1000


// ---- HTML HEAD
const char WSER_HTML_COPILOT_HEAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>WiFi NetSky</title>
)rawliteral";

// ---- CSS (una riga per classe)
const char WSER_HTML_COPILOT_STYLE[] PROGMEM = R"rawliteral(
<style>

  .activation-mode-box { display: flex; align-items: center; justify-content: center; gap: 4px; margin: 0px auto; max-width: 600px; flex-wrap: nowrap; }

  .text-input-box { margin: 20px 0; }

  .text-area { width: 40%; padding: 10px; border: 1px solid #ccc; border-radius: 8px; font-size: 16px; 
    box-sizing: border-box; margin-left: auto; margin-right: auto; display: block; }

</style>
)rawliteral";

// ---- HEAD OPEN
const char WSER_HTML_COPILOT_HEAD_OPEN[] PROGMEM = R"rawliteral(
</head>
<body>
)rawliteral";

// ---- BODY OPEN (titolo pagina)
const char WSER_HTML_COPILOT_BODY_OPEN[] PROGMEM = R"rawliteral(

<div style='text-align:center;font-size:32px;font-weight:bold;'>🤖 NetSky</div>

)rawliteral";

// ---- BODY CONTENT (TEMPLATE VUOTO con placeholder base)
const char WSER_HTML_COPILOT_BODY_CONTENT[] PROGMEM = R"rawliteral(

<hr class="line-page-sub-section">

<div class="activation-mode-box">
  <button class="submit-activation" onclick="CopilotSendMessage()">Send Message</button>
</div>

<!-- Spazio per inserire il testo -->
<div class="text-input-box" style="margin: 20px 0;">
  <textarea id="clientMessage" rows="4"  maxlength="200" class="text-area" placeholder="type here ur message, max 200 charachters...."></textarea>
</div>

<hr class="line-page-sub-section">

<div class="activation-mode-box">
  <button class="submit-activation" onclick="CopilotExeMessage()">Execute Commands</button>
</div>

<!-- Spazio per inserire il testo -->
<div class="text-input-box" style="margin: 20px 0;">
  <textarea id="clientMessageBack" rows="4"  class="text-area" placeholder="copy and paste here ur commands message..."></textarea>
</div>

<hr class="line-page-sub-section">

<div class="status-box">
  <strong id="CopilotPanel1Row1">Loading...</strong><br>
  <strong id="CopilotPanel1Row2">Loading...</strong>
</div>

)rawliteral";

// ---- SCRIPT (TEMPLATE funzioni)
const char WSER_SCRIPT_COPILOT[] PROGMEM = R"rawliteral(
<script>

function CopilotSendMessage() {
    const message = document.getElementById('clientMessage').value;

    // Messaggio di conferma per l'invio
  const confirmMsg = "Press OK to send the message. The answer will come in a few minutes at 📭 the receiver's email address (see in the WiFiConfig page).\n\n" +
                   "Please be assured that no sensitive data will be collected by the system. The system will request explicit permission to access information via an email consent request.";
  const confirmProceed = confirm(confirmMsg);
    if (!confirmProceed) {
        console.log("⛔ Invio del messaggio annullato dall'utente.");
        return;
    }

    // Invia il messaggio tramite fetch
    fetch('/sendmessage', {
        method: 'POST',
        body: new URLSearchParams({
            'message': message
        })
    })
    .then(response => response.text())
    .then(result => {
        console.log(`Messaggio inviato:`, result);
    })
    .catch(error => {
        console.error('Errore nell\'invio del messaggio:', error);
    });
}


function CopilotExeMessage() {
  const message = document.getElementById('clientMessageBack').value.trim();

  // ✅ Controllo rapido di validità
  if (message.length < 3 ) {
    alert("⚠️ Invalid message format.\n\n");
    console.log("⛔ Message rejected: too short.");
    return;
  }

  const confirmMsg = "Press OK to execute the command message.\n\n";

  const confirmProceed = confirm(confirmMsg);
    if (!confirmProceed) {
        console.log("⛔ Invio del messaggio annullato dall'utente.");
        return;
    }

    // Invia il messaggio tramite fetch
    fetch('/executemessage', {
        method: 'POST',
        body: new URLSearchParams({
            'message': message
        })
    })
    .then(response => response.text())
    .then(result => {
        console.log(`Messaggio inviato:`, result);
    })
    .catch(error => {
        console.error('Errore nell\'invio del messaggio:', error);
    });
}

function updateStatusCopilot() {
    fetch('/wificopilotGetStatus')
        .then(r => r.json())
        .then(data => {
            // Aggiorna i contenuti delle righe del pannello
            document.getElementById('CopilotPanel1Row1').textContent = data.panel1row1 || "—";
            document.getElementById('CopilotPanel1Row2').textContent = data.panel1row2 || "—";

        })
        .catch(err => {
            console.error('Errore nel recuperare lo stato:', err);
        });
}

window.addEventListener('load', function() {
    updateStatusCopilot();
    setInterval(updateStatusCopilot, 1000);
});

</script>
)rawliteral";

// ---- CLOSE PAGE
const char WSER_COPILOT_CLOSE_PAGE[] PROGMEM = R"rawliteral(
</body>
</html>
)rawliteral";


#endif // APP_WEBBROWSER_COPILOT_H
