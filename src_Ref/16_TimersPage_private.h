#ifndef WSER_TIMERS_PRIVATE_H
#define WSER_TIMERS_PRIVATE_H

#define PUMP_TIMERS_CHECK_INTERVAL 1000

//---- Constants
const char WSER_HTML_PUMP_TIMERS_HEAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>WiFi Calibration</title>
)rawliteral";

const char WSER_HTML_PUMP_TIMERS_STYLE[] PROGMEM = R"rawliteral(
<style>


.ton-toff-input-boxes-div {display: flex; justify-content: center; gap: 20px; margin-top: 10px;}

.select-activation { height: 40px; font-size: 16px; padding: 5px 10px; font-weight: bold; }
.activation-mode-box { display: flex; align-items: center; justify-content: center; gap: 12px; margin-top: 10px; }

.timer-container-on,
.timer-container-off { padding: 10px; border-radius: 10px; text-align: center; width: fit-content; }
.timer-container-on { border: 2px solid blue; }
.timer-container-off { border: 2px solid red; }
.timer-setup-label { font-weight: bold; font-size: 16px; margin-bottom: 10px; }
.timer-button-container { margin-bottom: 5px; }
.timer-button-decrease, .timer-button-increase { padding: 4px 10px; width: 120px; font-size: 16px; }
.timer-button-decrease { margin-right: 5px; }
.timer-slider { width: 250px; margin-bottom: 5px; }
.timer-display { font-weight: bold; font-family: monospace; }
.timer-set-buttons-and-slider { display: flex; flex-direction: column; align-items: center; }

.dividerSetTonToffButton {display: flex; justify-content: center; margin-top: 10px;}

.SetTonToffButt { padding: 15px 40px; font-size: 20px; font-weight: bold; border-radius: 8px;
                  background-color: #00BFFF; color: white; border: none; cursor: pointer; }

</style>
)rawliteral";

const char WSER_HTML_PUMP_TIMERS_HEAD_OPEN[] PROGMEM = R"rawliteral(
</head>
<body>
)rawliteral";

const char WSER_HTML_PUMP_TIMERS_BODY_OPEN[] PROGMEM = R"rawliteral(
<div style='text-align: center; font-size: 32px; font-weight: bold;'>🕒 Timers</div>
)rawliteral";

const char WSER_HTML_PUMP_TIMERS_BODY_CONTENT[] PROGMEM = R"rawliteral(
<hr class="line-page-sub-section">
<div class="section-title">🕒 Timer 1</div>
<div class="activation-mode-box">
  <select id="TimerActivation1" class="select-activation">
    <option value="ACTIVE">✅ Active</option>
    <option value="NOT_ACTIVE">❌ Not Active</option>
  </select>
  <input id="TimerActivationButton1" type="submit" value="Timer Activation" onclick="SetTimerActivation(1)" class="submit-activation">
</div>
<div class="ton-toff-input-boxes-div">
  <!-- TON -->
  <div class="timer-container-on">
    <div class="timer-setup-label">Ton 🟢</div>
    <div class="timer-set-buttons-and-slider">
      <div class="timer-button-container">
        <button type="button" onclick="adjustTonValue(-1,1)" class="timer-button-decrease">←</button>
        <button type="button" onclick="adjustTonValue(1,1)" class="timer-button-increase">→</button>
      </div>
      <input type="range" id="tonRange1" min="0" max="1080" value="0" class="timer-slider">
    </div>
    <!-- Secondi aggiuntivi -->
    <div class="timer-set-buttons-and-slider">
      <div class="timer-button-container">
        <button type="button" onclick="adjustTonSeconds(-1,1)" class="timer-button-decrease">←</button>
        <button type="button" onclick="adjustTonSeconds(1,1)" class="timer-button-increase">→</button>
      </div>
      <input type="range" id="tonSecRange1" min="1" max="59" value="30" class="timer-slider">
      <span id="tonDisplay1" class="timer-display">00:00:30</span>
    </div>
  </div>
  <!-- TOFF -->
  <div class="timer-container-off">
    <div class="timer-setup-label">Toff 🛑</div>
    <div class="timer-set-buttons-and-slider">
      <div class="timer-button-container">
        <button type="button" onclick="adjustToffValue(-1,1)" class="timer-button-decrease">←</button>
        <button type="button" onclick="adjustToffValue(1,1)" class="timer-button-increase">→</button>
      </div>
      <input type="range" id="toffRange1" min="0" max="1080" value="0" class="timer-slider">
    </div>
    <!-- Secondi aggiuntivi -->
    <div class="timer-set-buttons-and-slider">
      <div class="timer-button-container">
        <button type="button" onclick="adjustToffSeconds(-1,1)" class="timer-button-decrease">←</button>
        <button type="button" onclick="adjustToffSeconds(1,1)" class="timer-button-increase">→</button>
      </div>
      <input type="range" id="toffSecRange1" min="1" max="59" value="30" class="timer-slider">
      <span id="toffDisplay1" class="timer-display">00:00:30</span>
    </div>
  </div>
</div>
<div class="dividerSetTonToffButton">
  <input type="button" value="⏲️ Set Ton/Toff" id="timerSubmitButton1" onclick="setTimer(1)" class="SetTonToffButt">
</div>
<div class="status-box">
  <strong id="PumpTimer1StatusRow1">Loading...</strong><br>
  <strong id="PumpTimer1StatusRow2">Loading...</strong>
</div>

<hr class="line-page-sub-section">
<div class="section-title">🕒 Timer 2</div>
<div class="activation-mode-box">
  <select id="TimerActivation2" class="select-activation">
    <option value="ACTIVE">✅ Active</option>
    <option value="NOT_ACTIVE">❌ Not Active</option>
  </select>
  <input id="TimerActivationButton2" type="submit" value="Timer Activation" onclick="SetTimerActivation(2)" class="submit-activation">
</div>
<div class="ton-toff-input-boxes-div">
  <!-- TON -->
  <div class="timer-container-on">
    <div class="timer-setup-label">Ton 🟢</div>
    <div class="timer-set-buttons-and-slider">
      <div class="timer-button-container">
        <button type="button" onclick="adjustTonValue(-1,2)" class="timer-button-decrease">←</button>
        <button type="button" onclick="adjustTonValue(1,2)" class="timer-button-increase">→</button>
      </div>
      <input type="range" id="tonRange2" min="0" max="1080" value="0" class="timer-slider">
    </div>
    <!-- Secondi aggiuntivi -->
    <div class="timer-set-buttons-and-slider">
      <div class="timer-button-container">
        <button type="button" onclick="adjustTonSeconds(-1,2)" class="timer-button-decrease">←</button>
        <button type="button" onclick="adjustTonSeconds(1,2)" class="timer-button-increase">→</button>
      </div>
      <input type="range" id="tonSecRange2" min="1" max="59" value="30" class="timer-slider">
      <span id="tonDisplay2" class="timer-display">00:00:30</span>
    </div>
  </div>
  <!-- TOFF -->
  <div class="timer-container-off">
    <div class="timer-setup-label">Toff 🛑</div>
    <div class="timer-set-buttons-and-slider">
      <div class="timer-button-container">
        <button type="button" onclick="adjustToffValue(-1,2)" class="timer-button-decrease">←</button>
        <button type="button" onclick="adjustToffValue(1,2)" class="timer-button-increase">→</button>
      </div>
      <input type="range" id="toffRange2" min="0" max="1080" value="0" class="timer-slider">
    </div>
    <!-- Secondi aggiuntivi -->
    <div class="timer-set-buttons-and-slider">
      <div class="timer-button-container">
        <button type="button" onclick="adjustToffSeconds(-1,2)" class="timer-button-decrease">←</button>
        <button type="button" onclick="adjustToffSeconds(1,2)" class="timer-button-increase">→</button>
      </div>
      <input type="range" id="toffSecRange2" min="1" max="59" value="30" class="timer-slider">
      <span id="toffDisplay2" class="timer-display">00:00:30</span>
    </div>
  </div>
</div>
<div class="dividerSetTonToffButton">
  <input type="button" value="⏲️ Set Ton/Toff" id="timerSubmitButton2" onclick="setTimer(2)" class="SetTonToffButt">
</div>
<div class="status-box">
  <strong id="PumpTimer2StatusRow1">Loading...</strong><br>
  <strong id="PumpTimer2StatusRow2">Loading...</strong>
</div>

<hr class="line-page-sub-section">
<div class="section-title">🕒 Timer 3</div>
<div class="activation-mode-box">
  <select id="TimerActivation3" class="select-activation">
    <option value="ACTIVE">✅ Active</option>
    <option value="NOT_ACTIVE">❌ Not Active</option>
  </select>
  <input id="TimerActivationButton3" type="submit" value="Timer Activation" onclick="SetTimerActivation(3)" class="submit-activation">
</div>
<div class="ton-toff-input-boxes-div">
  <!-- TON -->
  <div class="timer-container-on">
    <div class="timer-setup-label">Ton 🟢</div>
    <div class="timer-set-buttons-and-slider">
      <div class="timer-button-container">
        <button type="button" onclick="adjustTonValue(-1,3)" class="timer-button-decrease">←</button>
        <button type="button" onclick="adjustTonValue(1,3)" class="timer-button-increase">→</button>
      </div>
      <input type="range" id="tonRange3" min="0" max="1080" value="0" class="timer-slider">
    </div>
    <!-- Secondi aggiuntivi -->
    <div class="timer-set-buttons-and-slider">
      <div class="timer-button-container">
        <button type="button" onclick="adjustTonSeconds(-1,3)" class="timer-button-decrease">←</button>
        <button type="button" onclick="adjustTonSeconds(1,3)" class="timer-button-increase">→</button>
      </div>
      <input type="range" id="tonSecRange3" min="1" max="59" value="30" class="timer-slider">
      <span id="tonDisplay3" class="timer-display">00:00:30</span>
    </div>
  </div>
  <!-- TOFF -->
  <div class="timer-container-off">
    <div class="timer-setup-label">Toff 🛑</div>
    <div class="timer-set-buttons-and-slider">
      <div class="timer-button-container">
        <button type="button" onclick="adjustToffValue(-1,3)" class="timer-button-decrease">←</button>
        <button type="button" onclick="adjustToffValue(1,3)" class="timer-button-increase">→</button>
      </div>
      <input type="range" id="toffRange3" min="0" max="1080" value="0" class="timer-slider">
    </div>
    <!-- Secondi aggiuntivi -->
    <div class="timer-set-buttons-and-slider">
      <div class="timer-button-container">
        <button type="button" onclick="adjustToffSeconds(-1,3)" class="timer-button-decrease">←</button>
        <button type="button" onclick="adjustToffSeconds(1,3)" class="timer-button-increase">→</button>
      </div>
      <input type="range" id="toffSecRange3" min="1" max="59" value="30" class="timer-slider">
      <span id="toffDisplay3" class="timer-display">00:00:30</span>
    </div>
  </div>
</div>
<div class="dividerSetTonToffButton">
  <input type="button" value="⏲️ Set Ton/Toff" id="timerSubmitButton3" onclick="setTimer(3)" class="SetTonToffButt">
</div>
<div class="status-box">
  <strong id="PumpTimer3StatusRow1">Loading...</strong><br>
  <strong id="PumpTimer3StatusRow2">Loading...</strong>
</div>

<hr class="line-page-sub-section">
<div class="section-title">🕒 Timer 4</div>
<div class="activation-mode-box">
  <select id="TimerActivation4" class="select-activation">
    <option value="ACTIVE">✅ Active</option>
    <option value="NOT_ACTIVE">❌ Not Active</option>
  </select>
  <input id="TimerActivationButton4" type="submit" value="Timer Activation" onclick="SetTimerActivation(4)" class="submit-activation">
</div>
<div class="ton-toff-input-boxes-div">
  <!-- TON -->
  <div class="timer-container-on">
    <div class="timer-setup-label">Ton 🟢</div>
    <div class="timer-set-buttons-and-slider">
      <div class="timer-button-container">
        <button type="button" onclick="adjustTonValue(-1,4)" class="timer-button-decrease">←</button>
        <button type="button" onclick="adjustTonValue(1,4)" class="timer-button-increase">→</button>
      </div>
      <input type="range" id="tonRange4" min="0" max="1080" value="0" class="timer-slider">
    </div>
    <!-- Secondi aggiuntivi -->
    <div class="timer-set-buttons-and-slider">
      <div class="timer-button-container">
        <button type="button" onclick="adjustTonSeconds(-1,4)" class="timer-button-decrease">←</button>
        <button type="button" onclick="adjustTonSeconds(1,4)" class="timer-button-increase">→</button>
      </div>
      <input type="range" id="tonSecRange4" min="1" max="59" value="30" class="timer-slider">
      <span id="tonDisplay4" class="timer-display">00:00:30</span>
    </div>
  </div>
  <!-- TOFF -->
  <div class="timer-container-off">
    <div class="timer-setup-label">Toff 🛑</div>
    <div class="timer-set-buttons-and-slider">
      <div class="timer-button-container">
        <button type="button" onclick="adjustToffValue(-1,4)" class="timer-button-decrease">←</button>
        <button type="button" onclick="adjustToffValue(1,4)" class="timer-button-increase">→</button>
      </div>
      <input type="range" id="toffRange4" min="0" max="1080" value="0" class="timer-slider">
    </div>
    <!-- Secondi aggiuntivi -->
    <div class="timer-set-buttons-and-slider">
      <div class="timer-button-container">
        <button type="button" onclick="adjustToffSeconds(-1,4)" class="timer-button-decrease">←</button>
        <button type="button" onclick="adjustToffSeconds(1,4)" class="timer-button-increase">→</button>
      </div>
      <input type="range" id="toffSecRange4" min="1" max="59" value="30" class="timer-slider">
      <span id="toffDisplay4" class="timer-display">00:00:30</span>
    </div>
  </div>
</div>
<div class="dividerSetTonToffButton">
  <input type="button" value="⏲️ Set Ton/Toff" id="timerSubmitButton4" onclick="setTimer(4)" class="SetTonToffButt">
</div>
<div class="status-box">
  <strong id="PumpTimer4StatusRow1">Loading...</strong><br>
  <strong id="PumpTimer4StatusRow2">Loading...</strong>
</div>
)rawliteral";


const char WSER_PUMP_TIMERS_CALIBRATION2[] PROGMEM = R"rawliteral(
<script>

//slider move functions 
document.addEventListener('DOMContentLoaded', function () {

  for (let i = 1; i <= 4; i++) {
    const tonMin = document.getElementById(`tonRange${i}`);
    const tonSec = document.getElementById(`tonSecRange${i}`);
    const toffMin = document.getElementById(`toffRange${i}`);
    const toffSec = document.getElementById(`toffSecRange${i}`);

    tonMin.addEventListener('input',  () => updateTonDisplay(i));
    tonSec.addEventListener('input',  () => updateTonDisplay(i));
    toffMin.addEventListener('input', () => updateToffDisplay(i));
    toffSec.addEventListener('input', () => updateToffDisplay(i));
  }

});

function updateTonDisplay(timer) {

  let minutes;
  let seconds;
  let totalSec;

  let tonSlider;
  let tonSecSlider ;
  let tonDisplay;

  if (timer === 1) {
    tonSlider = document.getElementById('tonRange1');
    tonSecSlider = document.getElementById('tonSecRange1');
    tonDisplay = document.getElementById('tonDisplay1');
  } else if (timer === 2) {
    tonSlider = document.getElementById('tonRange2');
    tonSecSlider = document.getElementById('tonSecRange2');
    tonDisplay = document.getElementById('tonDisplay2');
  } else if (timer === 3) {
    tonSlider = document.getElementById('tonRange3');
    tonSecSlider = document.getElementById('tonSecRange3');
    tonDisplay = document.getElementById('tonDisplay3');
  } else if (timer === 4) {
    tonSlider = document.getElementById('tonRange4');
    tonSecSlider = document.getElementById('tonSecRange4');
    tonDisplay = document.getElementById('tonDisplay4');
  } else {
    alert("❌ Errore: timer non definito!");
    console.error("Timer index non valido:", timer);
    return; // interrompe la funzione chiamante
  }

  minutes = parseInt(tonSlider.value);
  seconds = parseInt(tonSecSlider.value);
  totalSec = minutes * 60 + seconds;

  const h = Math.floor(totalSec / 3600);
  const m = Math.floor((totalSec % 3600) / 60);
  const s = totalSec % 60;

  tonDisplay.textContent = `${String(h).padStart(2, '0')}:${String(m).padStart(2, '0')}:${String(s).padStart(2, '0')}`;
}

function updateToffDisplay(timer) {

  let minutes;
  let seconds;
  let totalSec;

  let toffSlider;
  let toffSecSlider;
  let toffDisplay;

if (timer === 1) {
  toffSlider = document.getElementById('toffRange1');
  toffSecSlider = document.getElementById('toffSecRange1');
  toffDisplay = document.getElementById('toffDisplay1');
} else if (timer === 2) {
  toffSlider = document.getElementById('toffRange2');
  toffSecSlider = document.getElementById('toffSecRange2');
  toffDisplay = document.getElementById('toffDisplay2');
} else if (timer === 3) {
  toffSlider = document.getElementById('toffRange3');
  toffSecSlider = document.getElementById('toffSecRange3');
  toffDisplay = document.getElementById('toffDisplay3');
} else if (timer === 4) {
  toffSlider = document.getElementById('toffRange4');
  toffSecSlider = document.getElementById('toffSecRange4');
  toffDisplay = document.getElementById('toffDisplay4');

} else {
  alert("❌ Errore: timer non definito!");
  console.error("Timer index non valido:", timer);
  return; // interrompe la funzione chiamante
}

  minutes = parseInt(toffSlider.value);
  seconds = parseInt(toffSecSlider.value);
  totalSec = minutes * 60 + seconds;

  const h = Math.floor(totalSec / 3600);
  const m = Math.floor((totalSec % 3600) / 60);
  const s = totalSec % 60;

  toffDisplay.textContent = `${String(h).padStart(2, '0')}:${String(m).padStart(2, '0')}:${String(s).padStart(2, '0')}`;
}

// adjust values functions..........
function adjustToffValue(delta, timer) {

  let slider;
  let sliderSec;
  let display;

if (timer === 1) {
  slider = document.getElementById('toffRange1');       // minuti
  sliderSec = document.getElementById('toffSecRange1'); // secondi
  display = document.getElementById('toffDisplay1');
} else if (timer === 2) {
  slider = document.getElementById('toffRange2');       // minuti
  sliderSec = document.getElementById('toffSecRange2'); // secondi
  display = document.getElementById('toffDisplay2');
} else if (timer === 3) {
  slider = document.getElementById('toffRange3');       // minuti
  sliderSec = document.getElementById('toffSecRange3'); // secondi
  display = document.getElementById('toffDisplay3');
} else if (timer === 4) {
  slider = document.getElementById('toffRange4');       // minuti
  sliderSec = document.getElementById('toffSecRange4'); // secondi
  display = document.getElementById('toffDisplay4');
} else {
  alert("❌ Errore: timer non definito!");
  console.error("Timer index non valido:", timer);
  return; // interrompe la funzione chiamante
}

if (!slider || !sliderSec || !display) { console.error('IDs toff* mancanti per', timer); return; }


  let val = parseInt(slider.value) + delta;
  val = Math.max(0, Math.min(1080, val)); // range corretto: 0–1080 minuti
  slider.value = val;

  const sec = parseInt(sliderSec.value); // secondi attuali non toccati
  const totalSec = val * 60 + sec;

  const h = Math.floor(totalSec / 3600);
  const m = Math.floor((totalSec % 3600) / 60);
  const s = totalSec % 60;

  display.textContent = `${String(h).padStart(2, '0')}:${String(m).padStart(2, '0')}:${String(s).padStart(2, '0')}`;
}

function adjustToffSeconds(delta, timer) {

  let sliderMin;
  let sliderSec;
  let displayFull;

if (timer === 1) {
  sliderMin = document.getElementById('toffRange1');
  sliderSec = document.getElementById('toffSecRange1');
  displayFull = document.getElementById('toffDisplay1');
} else if (timer === 2) {
  sliderMin = document.getElementById('toffRange2');
  sliderSec = document.getElementById('toffSecRange2');
  displayFull = document.getElementById('toffDisplay2');
} else if (timer === 3) {
  sliderMin = document.getElementById('toffRange3');
  sliderSec = document.getElementById('toffSecRange3');
  displayFull = document.getElementById('toffDisplay3');
} else if (timer === 4) {
  sliderMin = document.getElementById('toffRange4');
  sliderSec = document.getElementById('toffSecRange4');
  displayFull = document.getElementById('toffDisplay4');
} else {
  alert("❌ Errore: timer non definito!");
  console.error("Timer index non valido:", timer);
  return; // interrompe la funzione chiamante
}

  const min = parseInt(sliderMin.value);
  let sec = parseInt(sliderSec.value) + delta;

  // Imposta limiti condizionati
  if (min > 0) {
    sec = Math.max(0, Math.min(59, sec));
  } else {
    sec = Math.max(2, Math.min(59, sec));
  }

  sliderSec.value = sec;

  // Aggiorna display hh:mm:ss completo
  const totalSec = min * 60 + sec;
  const h = Math.floor(totalSec / 3600);
  const m = Math.floor((totalSec % 3600) / 60);
  const s = totalSec % 60;
  displayFull.textContent = `${String(h).padStart(2, '0')}:${String(m).padStart(2, '0')}:${String(s).padStart(2, '0')}`;
}

function adjustTonValue(delta, timer) {

  let slider;
  let sliderSec;
  let display;

if (timer === 1) {
  slider = document.getElementById('tonRange1');       // minuti
  sliderSec = document.getElementById('tonSecRange1'); // secondi
  display = document.getElementById('tonDisplay1');
} else if (timer === 2) {
  slider = document.getElementById('tonRange2');       // minuti
  sliderSec = document.getElementById('tonSecRange2'); // secondi
  display = document.getElementById('tonDisplay2');
} else if (timer === 3) {
  slider = document.getElementById('tonRange3');       // minuti
  sliderSec = document.getElementById('tonSecRange3'); // secondi
  display = document.getElementById('tonDisplay3');
} else if (timer === 4) {
  slider = document.getElementById('tonRange4');       // minuti
  sliderSec = document.getElementById('tonSecRange4'); // secondi
  display = document.getElementById('tonDisplay4');
} else {
  alert("❌ Errore: timer non definito!");
  console.error("Timer index non valido:", timer);
  return; // interrompe la funzione chiamante
}

  let val = parseInt(slider.value) + delta;
  val = Math.max(0, Math.min(1080, val)); // range corretto: 0–1080 minuti
  slider.value = val;

  const sec = parseInt(sliderSec.value); // secondi attuali
  const totalSec = val * 60 + sec;

  const h = Math.floor(totalSec / 3600);
  const m = Math.floor((totalSec % 3600) / 60);
  const s = totalSec % 60;

  display.textContent = `${String(h).padStart(2, '0')}:${String(m).padStart(2, '0')}:${String(s).padStart(2, '0')}`;
}

function adjustTonSeconds(delta, timer) {

 let sliderMin;
 let sliderSec;
 let display;
  let min;
  let sec;

if (timer === 1) {
  sliderMin = document.getElementById('tonRange1');
  sliderSec = document.getElementById('tonSecRange1');
  display   = document.getElementById('tonDisplay1');

} else if (timer === 2) {
  sliderMin = document.getElementById('tonRange2');
  sliderSec = document.getElementById('tonSecRange2');
  display   = document.getElementById('tonDisplay2');
} else if (timer === 3) {
  sliderMin = document.getElementById('tonRange3');
  sliderSec = document.getElementById('tonSecRange3');
  display   = document.getElementById('tonDisplay3');
} else if (timer === 4) {
  sliderMin = document.getElementById('tonRange4');
  sliderSec = document.getElementById('tonSecRange4');
  display   = document.getElementById('tonDisplay4');
} else {
  alert("❌ Errore: timer non definito!");
  console.error("Timer index non valido:", timer);
  return; // interrompe la funzione chiamante
}

  min = parseInt(sliderMin.value);
  sec = parseInt(sliderSec.value) + delta;

  // Limiti condizionati
  if (min > 0) {
    sec = Math.max(0, Math.min(59, sec));
  } else {
    sec = Math.max(2, Math.min(59, sec));
  }

  sliderSec.value = sec;

  // Aggiorna display hh:mm:ss completo
  const totalSec = min * 60 + sec;
  const h = Math.floor(totalSec / 3600);
  const m = Math.floor((totalSec % 3600) / 60);
  const s = totalSec % 60;
  display.textContent = `${String(h).padStart(2, '0')}:${String(m).padStart(2, '0')}:${String(s).padStart(2, '0')}`;
}

// set fucntions.... for the routes
function SetTimerActivation(timerIndex) {

  let activeValue = "";

  if (timerIndex === 1) {
    activeValue = document.getElementById("TimerActivation1").value;
  } else if (timerIndex === 2) {
    activeValue = document.getElementById("TimerActivation2").value;
  }  else if (timerIndex === 3) {
    activeValue = document.getElementById("TimerActivation3").value;
  }  else if (timerIndex === 4) {
    activeValue = document.getElementById("TimerActivation4").value;
  } else {
    alert("❌ Errore: timerIndex non valido!");
    return;
  }

// Messaggio di conferma per ACTIVE o NOT_ACTIVE
if (activeValue === "ACTIVE") {
  const msg = "👁️ By setting ACTIVE, the pump will run ONLY when:\n\n" +
              "1️⃣ The control (e.g. pH/ORP) requests dosing\n" +
              "2️⃣ AND the timer is in its ON phase\n\n" +
              "⏱️ The timer acts as a time-window limiter.\n\n" +
              "Do you want to proceed?";

  const confirmProceed = confirm(msg);
  if (!confirmProceed) {
    console.log("⛔ Activation cancelled by user.");
    return;
  }

} else if (activeValue === "NOT_ACTIVE") {
  const msg = "👁️ By setting NOT ACTIVE, the pump will run ONLY when:\n\n" +
              "1️⃣ The control (e.g. pH/ORP) requests dosing\n" +
              "❌ The timer will have NO effect in this mode.\n\n" +
              "Do you want to proceed?";

  const confirmProceed = confirm(msg);
  if (!confirmProceed) {
    console.log("⛔ Activation cancelled by user.");
    return;
  }
}

  fetch('/setTimerActivation', {
    method: 'POST',
    body: new URLSearchParams({
      'timer': timerIndex,
      'active': activeValue
    })
  })
  .then(response => response.text())
  .then(result => {
    console.log(`Timer ${timerIndex} activation set:`, result);
  })
  .catch(error => {
    console.error('Errore in SetTimerActivation:', error);
  });
}

function setTimer(timerIndex) {

  let tonMin;
  let tonSec;
  let toffMin;
  let toffSec;

  if (timerIndex === 1) {
    tonMin  = parseInt(document.getElementById("tonRange1").value) || 0;
    tonSec  = parseInt(document.getElementById("tonSecRange1").value) || 0;
    toffMin = parseInt(document.getElementById("toffRange1").value) || 0;
    toffSec = parseInt(document.getElementById("toffSecRange1").value) || 0;
  } else if (timerIndex === 2) {
    tonMin  = parseInt(document.getElementById("tonRange2").value) || 0;
    tonSec  = parseInt(document.getElementById("tonSecRange2").value) || 0;
    toffMin = parseInt(document.getElementById("toffRange2").value) || 0;
    toffSec = parseInt(document.getElementById("toffSecRange2").value) || 0;
  }  else if (timerIndex === 3) {
    tonMin  = parseInt(document.getElementById("tonRange3").value) || 0;
    tonSec  = parseInt(document.getElementById("tonSecRange3").value) || 0;
    toffMin = parseInt(document.getElementById("toffRange3").value) || 0;
    toffSec = parseInt(document.getElementById("toffSecRange3").value) || 0;
  }  else if (timerIndex === 4) {
    tonMin  = parseInt(document.getElementById("tonRange4").value) || 0;
    tonSec  = parseInt(document.getElementById("tonSecRange4").value) || 0;
    toffMin = parseInt(document.getElementById("toffRange4").value) || 0;
    toffSec = parseInt(document.getElementById("toffSecRange4").value) || 0;
  } else {
    alert("❌ Errore: timerIndex non valido!");
    return;
  }

  const tonTotalSeconds  = (tonMin * 60) + tonSec;
  const toffTotalSeconds = (toffMin * 60) + toffSec;

  // Validazione: nessun tempo può essere zero
  if ((tonTotalSeconds === 0)||(toffTotalSeconds === 0)) {
    alert("❌ Ton or Toff cannot be zero! Command ignored.");
    return;
  }

  // Calcolo ore, minuti e secondi per il messaggio
  const tonH = Math.floor(tonMin / 60);
  const tonM = tonMin % 60;
  const toffH = Math.floor(toffMin / 60);
  const toffM = toffMin % 60;

  // Updated confirmation message
  const msg = "👁️ By setting new Ton/Toff values, the timer will be RESET if currently active.\n\n" +
              `Ton:  ${tonH}h ${tonM}m ${tonSec}s\n` +
              `Toff: ${toffH}h ${toffM}m ${toffSec}s\n\n` +
              "Do you want to proceed?";

  const confirmProceed = confirm(msg);
  if (!confirmProceed) {
    console.log("⛔ Impostazione timer annullata dall'utente.");
    return;
  }

fetch('/setTonToff', {
  method: 'POST',
  body: new URLSearchParams({
    'timer': timerIndex,
    'tonSeconds': tonTotalSeconds,
    'toffSeconds': toffTotalSeconds
  })
})
  .then(response => response.text())
  .then(result => {
    console.log(`Timer ${timerIndex} Ton/Toff set:`, result);
  })
  .catch(error => {
    console.error('Errore in setTimer:', error);
  });
}

// get status fucntion..........
function updatePumpTimersPageStatus() {
  fetch('/PumpTimersGetStatus')
    .then(response => response.json())
    .then(data => {

      document.getElementById("PumpTimer1StatusRow1").textContent = data.MessStatusPumpTimer1Row1 || "Loading...";
      document.getElementById("PumpTimer1StatusRow2").textContent = data.MessStatusPumpTimer1Row2 || "Loading...";

      document.getElementById("PumpTimer2StatusRow1").textContent = data.MessStatusPumpTimer2Row1 || "Loading...";
      document.getElementById("PumpTimer2StatusRow2").textContent = data.MessStatusPumpTimer2Row2 || "Loading...";

      document.getElementById("PumpTimer3StatusRow1").textContent = data.MessStatusPumpTimer3Row1 || "Loading...";
      document.getElementById("PumpTimer3StatusRow2").textContent = data.MessStatusPumpTimer3Row2 || "Loading...";
      
      document.getElementById("PumpTimer4StatusRow1").textContent = data.MessStatusPumpTimer4Row1 || "Loading...";
      document.getElementById("PumpTimer4StatusRow2").textContent = data.MessStatusPumpTimer4Row2 || "Loading...";

    })
    .catch(error => {
      console.error('Errore nel recuperare lo stato:', error);
    });
}

window.onload = function() {
  for (let i = 1; i <= 4; i++) {
    updateTonDisplay(i);
    updateToffDisplay(i);
  }
  updatePumpTimersPageStatus();
  setInterval(updatePumpTimersPageStatus, 1000); // Aggiorna ogni secondo
};

</script>
)rawliteral";

const char WSER_PUMP_TIMERS_CLOSE_PAGE[] PROGMEM = R"rawliteral(
</body>
</html>
)rawliteral";
//--- Private Variables -------------------------------------------------------


#endif 