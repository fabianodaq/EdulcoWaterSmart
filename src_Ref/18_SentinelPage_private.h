#ifndef WSER_WIFI_SENTINEL_PRIVATE_H
#define WSER_WIFI_SENTINEL_PRIVATE_H





//----------- Extern Global variables-----------------
#define MAX_NUMBER_SENTINEL 4

#define SENTINEL_EMAIL_MAX_LEN 32

#define REMOTE_SENTINEL_INTERVAL 1000UL  // ms

typedef enum { SENT_OK=0, SENT_VIOLATING, SENT_ALARM } SentinelState_t;

typedef struct {
    // Config
    bool     active;
    int16_t  minVal;
    int16_t  maxVal;
    int16_t  ActVal;            // valore corrente
    uint16_t persistence_min;   // minuti
    uint16_t occurrence_min;    // minuti

    // Runtime
    bool     oofl;              // out-of-limits flag
    SentinelState_t state;      // SENT_OK / SENT_VIOLATING / SENT_ALARM
    float persist_ticks;     // conteggio verso persistenza
    float snooze_ticks;      // countdown per email periodiche
} Sentinel_t;


//---- Constants
const char WSER_SENTINEL_HEAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Monitor</title>
)rawliteral";

const char WSER_SENTINEL_STYLE[] PROGMEM = R"rawliteral(
<style>

.flex-container { display: flex; justify-content: center; flex-wrap: wrap; gap: 10px; align-items: stretch; }


.sentinel-main-container { display: flex; justify-content: center; align-items: flex-start; gap: 8px; flex-wrap: nowrap;         
                          margin: 20px auto; width: fit-content;  align-items: stretch; max-width: 580px; }

.SentinelRangeSet { display: flex;  flex-direction: column; align-items: center;  justify-content: center;  gap: 10px;  margin: 10px auto;     
                    padding: 10px;  border-radius: 10px;  text-align: center; width: fit-content; border: 2px solid blue; box-sizing: border-box; }

.sentinel-timer-container { padding: 10px; border-radius: 10px; text-align: center; width: fit-content; border: 2px solid grey; box-sizing: border-box; 
                            gap: 10px;  margin: 0px auto; padding: 10px;}

.SentinelLabel { font-weight: bold; font-size: 16px; margin-bottom: 10px; }
  
.adjust-buttons { display: flex; justify-content: center; align-items: center; gap: 10px; margin-bottom: 10px; } 

.arrow-button { background-color: #f0f0f0; color: #555; width: 190px; height: 40px; border: none; border-radius: 5px; font-size: 20px; } 
  
.dual-slider-container { position: relative; width: 400px; margin: 5px auto; height: 30px; }

.dual-slider { width: 100%; height: 8px; margin: 6px 0; border-radius: 5px; appearance: none;
                outline: none; pointer-events: all; background: #aaa; }

.dual-range-values {  text-align: center; font-size: 0.9rem; margin-top: 2px; }

.sentinel-setup-label { font-weight: bold; font-size: 16px; text-align: center; margin-top: 10px; color: #222;   margin-bottom: 6px; }

.value-strong { font-weight: bold; }

.sentinel-timer-display { font-weight: bold; font-family: monospace; }

.sentinel-timer-slider { width: 250px; margin-bottom: 5px; }

.sentinel-timer-set-slider { display: flex; flex-direction: column; align-items: center; }


.select-activation { height: 36px; font-size: 16px; padding: 5px 10px; font-weight: bold; }

.sentinel-timer-activation-box { display:flex; gap:4px; align-items:stretch; justify-content:center; 
                                  flex-wrap:wrap; margin:4px auto; width:fit-content; max-width:600px; }
.activation-mode-box { display: flex; align-items: center; justify-content: center; gap: 4px; margin: 0px auto; max-width: 600px; flex-wrap: nowrap; }


</style>
)rawliteral";

const char WSER_SENTINEL_HEAD_OPEN[] PROGMEM = R"rawliteral(
</head>
<body>
)rawliteral";

const char WSER_SENTINEL_BODY_OPEN[] PROGMEM = R"rawliteral(
<div style='text-align: center; font-size: 32px; font-weight: bold;'>🛡️ Sentinel</div>
)rawliteral";



const char WSER_SENTINEL_BODY_CONTENT[] PROGMEM = R"rawliteral(
<hr class="line-page-sub-section">

<div class="section-title">🛡️ Sentinel 1</div>

<div class="sentinel-main-container">

  <div class="SentinelRangeSet">
    <div class="SentinelLabel">👇 Min 👆 Max Limits </div>
    <div class="adjust-buttons">
      <button type="button" onclick="adjustValue(1,-1)" class="arrow-button">←</button>
      <button type="button" onclick="adjustValue(1, 1)"  class="arrow-button">→</button>
    </div>
    <div class="dual-slider-container">
      <input type="range" id="sliderLow1"
             min="{SENT1_MIN}" max="{SENT1_MAX}" value="{SENT1_VAL_MIN}"
             class="dual-slider" oninput="updateDualSlider(1)">
      <input type="range" id="sliderHigh1"
             min="{SENT1_MIN}" max="{SENT1_MAX}" value="{SENT1_VAL_MAX}"
             class="dual-slider" oninput="updateDualSlider(1)">
    </div>
    <div class="dual-range-values">
      Min: <span id="lowValue1" class="value-strong">{S1_MIN_LABEL}</span>
        &nbsp;<span class="range-arrow"><b>👇⟷👆</b></span>&nbsp;
      Max: <span id="highValue1" class="value-strong">{S1_MAX_LABEL}</span>
    </div>
  </div>

  <div class="sentinel-timer-activation-box">
    <div class="sentinel-timer-container">
      <div class="sentinel-setup-label">⚠️ Warning time</div>
      <div class="sentinel-timer-set-slider">
        <input type="range" id="tPersistenceRange1" min="1" max="60" value="5" class="sentinel-timer-slider" oninput="updateSentinelTimes(1)">
        <span id="tPersistenceDisplay1" class="sentinel-timer-display">00:05</span>
      </div>

      <div class="sentinel-setup-label">🚨 Notification Interval</div>
      <div class="sentinel-timer-set-slider">
        <input type="range" id="tOccurranceRange1" min="15" max="1440" value="30" class="sentinel-timer-slider" oninput="updateSentinelTimes(1)">
        <span id="tOccurranceDisplay1" class="sentinel-timer-display">00:30</span>
      </div>
    </div>

    <div class="activation-mode-box">
      <select id="SentinelActivation1" class="select-activation" aria-label="Attivazione Sentinel">
        <option value="ACTIVE">✅ Set</option>
        <option value="NOT_ACTIVE">❌ Reset</option>
      </select>

      <input id="SentinelActivationButton1" type="button"
             value="Set Sentinel" onclick="SetSentinelActivation(1)"
             class="submit-activation">
    </div>
  </div>
</div>

<div class="status-box">
  <strong id="Sentinel1StatusRow1">Loading...</strong><br>
  <strong id="Sentinel1StatusRow2">Loading...</strong>
</div>

<hr class="line-page-sub-section">

<div class="section-title">🛡️ Sentinel 2</div>

<div class="sentinel-main-container">

  <div class="SentinelRangeSet">
    <div class="SentinelLabel">👇 Min 👆 Max Limits </div>
    <div class="adjust-buttons">
      <button type="button" onclick="adjustValue(2,-1)" class="arrow-button">←</button>
      <button type="button" onclick="adjustValue(2, 1)"  class="arrow-button">→</button>
    </div>
    <div class="dual-slider-container">
      <input type="range" id="sliderLow2"  min="{SENT2_MIN}" max="{SENT2_MAX}" value="{SENT2_VAL_MIN}" class="dual-slider" oninput="updateDualSlider(2)">
      <input type="range" id="sliderHigh2" min="{SENT2_MIN}" max="{SENT2_MAX}" value="{SENT2_VAL_MAX}" class="dual-slider" oninput="updateDualSlider(2)">
    </div>
    <div class="dual-range-values">
      Min: <span id="lowValue2" class="value-strong">{S2_MIN_LABEL}</span>
        &nbsp;<span class="range-arrow"><b>👇⟷👆</b></span>&nbsp;
      Max: <span id="highValue2" class="value-strong">{S2_MAX_LABEL}</span>
    </div>
  </div>

  <div class="sentinel-timer-activation-box">
    <div class="sentinel-timer-container">
      <div class="sentinel-setup-label">⚠️ Warning time</div>
      <div class="sentinel-timer-set-slider">
        <input type="range" id="tPersistenceRange2" min="1" max="60" value="5" class="sentinel-timer-slider" oninput="updateSentinelTimes(2)">
        <span id="tPersistenceDisplay2" class="sentinel-timer-display">00:05</span>
      </div>
      <div class="sentinel-setup-label">🚨 Notification Interval</div>
      <div class="sentinel-timer-set-slider">
        <input type="range" id="tOccurranceRange2" min="15" max="1440" value="30" class="sentinel-timer-slider" oninput="updateSentinelTimes(2)">
        <span id="tOccurranceDisplay2" class="sentinel-timer-display">00:30</span>
      </div>
    </div>

    <div class="activation-mode-box">
      <select id="SentinelActivation2" class="select-activation" aria-label="Attivazione Sentinel 2">
        <option value="ACTIVE">✅ Set</option>
        <option value="NOT_ACTIVE">❌ Reset</option>
      </select>

      <input id="SentinelActivationButton2" type="button" value="Set Sentinel" onclick="SetSentinelActivation(2)" class="submit-activation">
    </div>
  </div>

</div>

<div class="status-box">
  <strong id="Sentinel2StatusRow1">Loading...</strong><br>
  <strong id="Sentinel2StatusRow2">Loading...</strong>
</div>

<hr class="line-page-sub-section">

<div class="section-title">🛡️ Sentinel 3</div>

<div class="sentinel-main-container">

  <div class="SentinelRangeSet">
    <div class="SentinelLabel">👇 Min 👆 Max Limits </div>
    <div class="adjust-buttons">
      <button type="button" onclick="adjustValue(3,-1)" class="arrow-button">←</button>
      <button type="button" onclick="adjustValue(3, 1)"  class="arrow-button">→</button>
    </div>
    <div class="dual-slider-container">
      <input type="range" id="sliderLow3"  min="{SENT3_MIN}" max="{SENT3_MAX}" value="{SENT3_VAL_MIN}" class="dual-slider" oninput="updateDualSlider(3)">
      <input type="range" id="sliderHigh3" min="{SENT3_MIN}" max="{SENT3_MAX}" value="{SENT3_VAL_MAX}" class="dual-slider" oninput="updateDualSlider(3)">
    </div>
    <div class="dual-range-values">
      Min: <span id="lowValue3" class="value-strong">{S3_MIN_LABEL}</span>
        &nbsp;<span class="range-arrow"><b>👇⟷👆</b></span>&nbsp;
      Max: <span id="highValue3" class="value-strong">{S3_MAX_LABEL}</span>
    </div>
  </div>

  <div class="sentinel-timer-activation-box">
    <div class="sentinel-timer-container">
      <div class="sentinel-setup-label">⚠️ Warning time</div>
      <div class="sentinel-timer-set-slider">
        <input type="range" id="tPersistenceRange3" min="1" max="60" value="5" class="sentinel-timer-slider" oninput="updateSentinelTimes(3)">
        <span id="tPersistenceDisplay3" class="sentinel-timer-display">00:05</span>
      </div>
      <div class="sentinel-setup-label">🚨 Notification Interval</div>
      <div class="sentinel-timer-set-slider">
        <input type="range" id="tOccurranceRange3" min="15" max="1440" value="30" class="sentinel-timer-slider" oninput="updateSentinelTimes(3)">
        <span id="tOccurranceDisplay3" class="sentinel-timer-display">00:30</span>
      </div>
    </div>

    <div class="activation-mode-box">
      <select id="SentinelActivation3" class="select-activation" aria-label="Attivazione Sentinel 3">
        <option value="ACTIVE">✅ Set</option>
        <option value="NOT_ACTIVE">❌ Reset</option>
      </select>

      <input id="SentinelActivationButton3" type="button" value="Set Sentinel" onclick="SetSentinelActivation(3)" class="submit-activation">
    </div>
  </div>

</div>

<div class="status-box">
  <strong id="Sentinel3StatusRow1">Loading...</strong><br>
  <strong id="Sentinel3StatusRow2">Loading...</strong>
</div>

<hr class="line-page-sub-section">

<div class="section-title">🛡️ Sentinel 4</div>

<div class="sentinel-main-container">

  <div class="SentinelRangeSet">
    <div class="SentinelLabel">👇 Min 👆 Max Limits </div>
    <div class="adjust-buttons">
      <button type="button" onclick="adjustValue(4,-1)" class="arrow-button">←</button>
      <button type="button" onclick="adjustValue(4, 1)"  class="arrow-button">→</button>
    </div>
    <div class="dual-slider-container">
      <input type="range" id="sliderLow4"  min="{SENT4_MIN}" max="{SENT4_MAX}" value="{SENT4_VAL_MIN}" class="dual-slider" oninput="updateDualSlider(4)">
      <input type="range" id="sliderHigh4" min="{SENT4_MIN}" max="{SENT4_MAX}" value="{SENT4_VAL_MAX}" class="dual-slider" oninput="updateDualSlider(4)">
    </div>
    <div class="dual-range-values">
      Min: <span id="lowValue4" class="value-strong">{S4_MIN_LABEL}</span>
        &nbsp;<span class="range-arrow"><b>👇⟷👆</b></span>&nbsp;
      Max: <span id="highValue4" class="value-strong">{S4_MAX_LABEL}</span>
    </div>
  </div>

  <div class="sentinel-timer-activation-box">
    <div class="sentinel-timer-container">
      <div class="sentinel-setup-label">⚠️ Warning time</div>
      <div class="sentinel-timer-set-slider">
        <input type="range" id="tPersistenceRange4" min="1" max="60" value="5" class="sentinel-timer-slider" oninput="updateSentinelTimes(4)">
        <span id="tPersistenceDisplay4" class="sentinel-timer-display">00:05</span>
      </div>
      <div class="sentinel-setup-label">🚨 Notification Interval</div>
      <div class="sentinel-timer-set-slider">
        <input type="range" id="tOccurranceRange4" min="15" max="1440" value="30" class="sentinel-timer-slider" oninput="updateSentinelTimes(4)">
        <span id="tOccurranceDisplay4" class="sentinel-timer-display">00:30</span>
      </div>
    </div>

    <div class="activation-mode-box">
      <select id="SentinelActivation4" class="select-activation" aria-label="Attivazione Sentinel 4">
        <option value="ACTIVE">✅ Set</option>
        <option value="NOT_ACTIVE">❌ Reset</option>
      </select>

      <input id="SentinelActivationButton4" type="button" value="Set Sentinel" onclick="SetSentinelActivation(4)" class="submit-activation">
    </div>
  </div>

</div>

<div class="status-box">
  <strong id="Sentinel4StatusRow1">Loading...</strong><br>
  <strong id="Sentinel4StatusRow2">Loading...</strong>
</div>


)rawliteral";

const char WSER_SENTINEL_SCRIPT[] PROGMEM = R"rawliteral(
<script>

var sensor1Type = "{SENSOR1_SENTINEL_TYPE}";  // es. "ph" o "orp"
var sensor2Type = "{SENSOR2_SENTINEL_TYPE}";  // es. "ntc" or ds18
var sensor3Type = "{SENSOR3_SENTINEL_TYPE}";  // es. "ec", "tds" o "sal"
var sensor4Type = "{SENSOR4_SENTINEL_TYPE}";  // es. "ph" o "orp"

const isAPMode = {IS_AP_FLAG};

// java fucntion internal to the page 
function updateDualSlider(id){
  let low, high, fillLow, fillOk, fillHigh;

 if (id === 1) {
    low  = document.getElementById('sliderLow1');
    high = document.getElementById('sliderHigh1');
    fillLow  = document.getElementById('rangeFilllow1');
    fillOk   = document.getElementById('rangeFillok1');
    fillHigh = document.getElementById('rangeFillHigh1');
  }

  else if (id === 2) {
    low  = document.getElementById('sliderLow2');
    high = document.getElementById('sliderHigh2');
    fillLow  = document.getElementById('rangeFilllow2');
    fillOk   = document.getElementById('rangeFillok2');
    fillHigh = document.getElementById('rangeFillHigh2');
  }

  else if (id === 3) {
    low  = document.getElementById('sliderLow3');
    high = document.getElementById('sliderHigh3');
    fillLow  = document.getElementById('rangeFilllow3');
    fillOk   = document.getElementById('rangeFillok3');
    fillHigh = document.getElementById('rangeFillHigh3');
  }

  else if (id === 4) {
    low  = document.getElementById('sliderLow4');
    high = document.getElementById('sliderHigh4');
    fillLow  = document.getElementById('rangeFilllow4');
    fillOk   = document.getElementById('rangeFillok4');
    fillHigh = document.getElementById('rangeFillHigh4');
  }

  else {
    console.error("Invalid Sentinel ID:", id);
    return;
  }

  // valori correnti
  let lowVal  = parseFloat(low.value);
  let highVal = parseFloat(high.value);
  const min = parseFloat(low.min);
  const max = parseFloat(low.max);

// distanza minima tra low e high
const GAP = 10;

// normalizza entro min/max
lowVal  = Math.max(min, Math.min(lowVal,  max));
highVal = Math.max(min, Math.min(highVal, max));

// se si incrociano o violano il gap, trascina l’altro
const activeEl = document.activeElement;

if (activeEl === low) {
  // sto muovendo il minimo
  if (lowVal + GAP > highVal) {
    highVal = Math.min(lowVal + GAP, max);
    // se abbiamo toccato il max, riaggiusta low per garantire il gap
    lowVal  = Math.max(min, Math.min(highVal - GAP, lowVal));
  }
} else if (activeEl === high) {
  // sto muovendo il massimo
  if (highVal - GAP < lowVal) {
    lowVal  = Math.max(highVal - GAP, min);
    // se abbiamo toccato il min, riaggiusta high per garantire il gap
    highVal = Math.min(max, Math.max(lowVal + GAP, highVal));
  }
} else {
  // fallback (aggiornamenti programmati): centra e imponi gap
  if (highVal - lowVal < GAP) {
    const mid = (lowVal + highVal) / 2;
    lowVal  = Math.max(min, Math.min(mid - GAP / 2, max - GAP));
    highVal = Math.min(max, lowVal + GAP);
  }
}

// applica i valori eventualmente corretti agli input
low.value  = String(lowVal);
high.value = String(highVal);

  // percentuali aggiornate
  const lowPct  = ((lowVal  - min) / (max - min)) * 100;
  const highPct = ((highVal - min) / (max - min)) * 100;

  // 🔴 sotto range
  if (fillLow) {
    fillLow.style.left  = "0%";
    fillLow.style.width = lowPct + "%";
  }
  // 🟢 range valido
  if (fillOk) {
    fillOk.style.left  = lowPct + "%";
    fillOk.style.width = (highPct - lowPct) + "%";
  }
  // 🔴 sopra range
  if (fillHigh) {
    fillHigh.style.left  = highPct + "%";
    fillHigh.style.width = (100 - highPct) + "%";
  }

// etichette (formattate in base al tipo)
if (id === 1) {
  const lowLabel  = document.getElementById('lowValue1');
  const highLabel = document.getElementById('highValue1');
  if (lowLabel)  lowLabel.textContent  = formatSentinelLabel(lowVal,  sensor1Type);
  if (highLabel) highLabel.textContent = formatSentinelLabel(highVal, sensor1Type);
}

else if (id === 2) {
  const lowLabel  = document.getElementById('lowValue2');
  const highLabel = document.getElementById('highValue2');
  if (lowLabel)  lowLabel.textContent  = formatSentinelLabel(lowVal,  sensor2Type);
  if (highLabel) highLabel.textContent = formatSentinelLabel(highVal, sensor2Type);
}

else if (id === 3) {
  const lowLabel  = document.getElementById('lowValue3');
  const highLabel = document.getElementById('highValue3');
  if (lowLabel)  lowLabel.textContent  = formatSentinelLabel(lowVal,  sensor3Type);
  if (highLabel) highLabel.textContent = formatSentinelLabel(highVal, sensor3Type);
}

else if (id === 4) {
  const lowLabel  = document.getElementById('lowValue4');
  const highLabel = document.getElementById('highValue4');
  if (lowLabel)  lowLabel.textContent  = formatSentinelLabel(lowVal,  sensor4Type);
  if (highLabel) highLabel.textContent = formatSentinelLabel(highVal, sensor4Type);
}

else {
  console.error("Invalid Sentinel ID:", id);
  return;
}

// icone
if (id === 1) {
  const lowIcon  = document.getElementById('lowIcon1');
  const highIcon = document.getElementById('highIcon1');
  if (lowIcon)  lowIcon.style.left  = lowPct + "%";
  if (highIcon) highIcon.style.left = highPct + "%";
}

else if (id === 2) {
  const lowIcon  = document.getElementById('lowIcon2');
  const highIcon = document.getElementById('highIcon2');
  if (lowIcon)  lowIcon.style.left  = lowPct + "%";
  if (highIcon) highIcon.style.left = highPct + "%";
}

else if (id === 3) {
  const lowIcon  = document.getElementById('lowIcon3');
  const highIcon = document.getElementById('highIcon3');
  if (lowIcon)  lowIcon.style.left  = lowPct + "%";
  if (highIcon) highIcon.style.left = highPct + "%";
}

else if (id === 4) {
  const lowIcon  = document.getElementById('lowIcon4');
  const highIcon = document.getElementById('highIcon4');
  if (lowIcon)  lowIcon.style.left  = lowPct + "%";
  if (highIcon) highIcon.style.left = highPct + "%";
}

else {
  console.error("Invalid Sentinel ID:", id);
  return;
}

}

document.addEventListener('DOMContentLoaded', () => {
  updateDualSlider(1);
  updateDualSlider(2);
  updateDualSlider(3);
  updateDualSlider(4);
});

function formatSentinelLabel(val, type) {
  switch ((type || "").toLowerCase()) {
    case "ec":   return Math.round(val) + " µS/cm";
    case "tds":
    case "sal":  return Math.round(val) + " ppm";
    case "ntc":  return (val / 10).toFixed(1) + " °C";
    case "diff":  return (val / 10).toFixed(1) + " °C";
    case "avg":  return (val / 10).toFixed(1) + " °C";
    case "ds18":  return (val / 10).toFixed(1) + " °C";
    case "ph":  return (val / 100).toFixed(2);
    case "orp": return Math.round(val) + " mV";
    default:     return String(Math.round(val));
  }
}

function adjustValue(id, dir) {

  let low, high;

  if (id === 1) {
    low  = document.getElementById('sliderLow1');
    high = document.getElementById('sliderHigh1');
  }

  else if (id === 2) {
    low  = document.getElementById('sliderLow2');
    high = document.getElementById('sliderHigh2');
  }

  else if (id === 3) {
    low  = document.getElementById('sliderLow3');
    high = document.getElementById('sliderHigh3');
  }

  else if (id === 4) {
    low  = document.getElementById('sliderLow4');
    high = document.getElementById('sliderHigh4');
  }

  else {
    console.error("Invalid Sentinel ID:", id);
    return;
  }

  if (!low || !high) {
    console.error(`Missing slider elements for Sentinel ${id}`);
    return;
  }

  const min = parseFloat(low.min);
  const max = parseFloat(low.max);
  const GAP = 10; // stesso gap usato in updateDualSlider

  let lowVal  = parseFloat(low.value);
  let highVal = parseFloat(high.value);

  if (dir < 0) {
    // ← riduci SOLO il valore minimo
    let newLow = lowVal - 1;                 // passo unitario
    newLow = Math.max(min, newLow);          // clamp al minimo
    // rispetta gap: non spingere il max, ferma il low al limite
    if (newLow + GAP > highVal) {
      newLow = Math.max(min, highVal - GAP);
    }
    low.value = String(newLow);

  } else if (dir > 0) {
    // → aumenta SOLO il valore massimo
    let newHigh = highVal + 1;               // passo unitario
    newHigh = Math.min(max, newHigh);        // clamp al massimo
    // rispetta gap: non spingere il low, ferma l'high al limite
    if (newHigh - GAP < lowVal) {
      newHigh = Math.min(max, lowVal + GAP);
    }
    high.value = String(newHigh);
  }

  // aggiorna barre, etichette e icone (già formatta in base a sensor1Type)
  updateDualSlider(id);
}

// --- Helpers di formattazione ---
function formatHHMM_fromMinutes(m) {
  const h = Math.floor(m / 60);
  const mm = m % 60;
  return String(h).padStart(2,'0') + ':' + String(mm).padStart(2,'0');
}

// --- Aggiorna entrambi i display (persistence + occurrance) per un dato Sentinel ---
function updateSentinelTimes(id) {
  if (id === 1) {
    const rP = document.getElementById('tPersistenceRange1');
    const dP = document.getElementById('tPersistenceDisplay1');
    const rO = document.getElementById('tOccurranceRange1');
    const dO = document.getElementById('tOccurranceDisplay1');
    if (rP && dP) dP.textContent = formatHHMM_fromMinutes(parseInt(rP.value||'5',10)) + " h";
    if (rO && dO) dO.textContent = formatHHMM_fromMinutes(parseInt(rO.value||'30',10)) + " h";
  }
  else if (id === 2) {
    const rP = document.getElementById('tPersistenceRange2');
    const dP = document.getElementById('tPersistenceDisplay2');
    const rO = document.getElementById('tOccurranceRange2');
    const dO = document.getElementById('tOccurranceDisplay2');
    if (rP && dP) dP.textContent = formatHHMM_fromMinutes(parseInt(rP.value||'5',10)) + " h";
    if (rO && dO) dO.textContent = formatHHMM_fromMinutes(parseInt(rO.value||'30',10)) + " h";
  }
  else if (id === 3) {
    const rP = document.getElementById('tPersistenceRange3');
    const dP = document.getElementById('tPersistenceDisplay3');
    const rO = document.getElementById('tOccurranceRange3');
    const dO = document.getElementById('tOccurranceDisplay3');
    if (rP && dP) dP.textContent = formatHHMM_fromMinutes(parseInt(rP.value||'5',10)) + " h";
    if (rO && dO) dO.textContent = formatHHMM_fromMinutes(parseInt(rO.value||'30',10)) + " h";
  }
  else if (id === 4) {
    const rP = document.getElementById('tPersistenceRange4');
    const dP = document.getElementById('tPersistenceDisplay4');
    const rO = document.getElementById('tOccurranceRange4');
    const dO = document.getElementById('tOccurranceDisplay4');
    if (rP && dP) dP.textContent = formatHHMM_fromMinutes(parseInt(rP.value||'5',10)) + " h";
    if (rO && dO) dO.textContent = formatHHMM_fromMinutes(parseInt(rO.value||'30',10)) + " h";
  }
}

document.addEventListener('DOMContentLoaded', () => {
  updateSentinelTimes(1);
  updateSentinelTimes(2);
  updateSentinelTimes(3);
  updateSentinelTimes(4);

  const p1 = document.getElementById('tPersistenceRange1');
  const o1 = document.getElementById('tOccurranceRange1');
  if (p1) p1.addEventListener('input', () => updateSentinelTimes(1));
  if (o1) o1.addEventListener('input', () => updateSentinelTimes(1));

  const p2 = document.getElementById('tPersistenceRange2');
  const o2 = document.getElementById('tOccurranceRange2');
  if (p2) p2.addEventListener('input', () => updateSentinelTimes(2));
  if (o2) o2.addEventListener('input', () => updateSentinelTimes(2));

  const p3 = document.getElementById('tPersistenceRange3');
  const o3 = document.getElementById('tOccurranceRange3');
  if (p3) p3.addEventListener('input', () => updateSentinelTimes(3));
  if (o3) o3.addEventListener('input', () => updateSentinelTimes(3));

  const p4 = document.getElementById('tPersistenceRange4');
  const o4 = document.getElementById('tOccurranceRange4');
  if (p4) p4.addEventListener('input', () => updateSentinelTimes(4));
  if (o4) o4.addEventListener('input', () => updateSentinelTimes(4));
});

// routes e funzioni esterne 
function SetSentinelActivation(id) {
  let controlNumber = 0;
  let select, lowEl, highEl, pEl, oEl;

  // --- Sentinel 1 ---
  if (id === 1) {
    controlNumber = 1;
    select = document.getElementById('SentinelActivation1');
    lowEl  = document.getElementById('sliderLow1');
    highEl = document.getElementById('sliderHigh1');
    pEl    = document.getElementById('tPersistenceRange1') || document.getElementById('tPersistenceRange');
    oEl    = document.getElementById('tOccurranceRange1')  || document.getElementById('tOccurranceRange');

  // --- Sentinel 2 ---
  } else if (id === 2) {
    controlNumber = 2;
    select = document.getElementById('SentinelActivation2');
    lowEl  = document.getElementById('sliderLow2');
    highEl = document.getElementById('sliderHigh2');
    pEl    = document.getElementById('tPersistenceRange2');
    oEl    = document.getElementById('tOccurranceRange2');

  // --- Sentinel 3 ---
  } else if (id === 3) {
    controlNumber = 3;
    select = document.getElementById('SentinelActivation3');
    lowEl  = document.getElementById('sliderLow3');
    highEl = document.getElementById('sliderHigh3');
    pEl    = document.getElementById('tPersistenceRange3');
    oEl    = document.getElementById('tOccurranceRange3');

  // --- Sentinel 4 ---
  } else if (id === 4) {
    controlNumber = 4;
    select = document.getElementById('SentinelActivation4');
    lowEl  = document.getElementById('sliderLow4');
    highEl = document.getElementById('sliderHigh4');
    pEl    = document.getElementById('tPersistenceRange4');
    oEl    = document.getElementById('tOccurranceRange4');

  } else {
    console.error('Sentinel id non valido:', id);
    return;
  }

  if (!select || !lowEl || !highEl || !pEl || !oEl) {
    console.error(`Elementi mancanti per Sentinel ${id}`);
    return;
  }


  var status_        = select.value;
  var minval_        = lowEl.value;
  var maxval_        = highEl.value; 
  var time_severity_ = pEl.value;
  var timesnooze_    = oEl.value; 

  let msg = ""; // NEW: garantisco che esista sempre
  if (status_ === "NOT_ACTIVE") {
    msg = `Press OK to Reset Sentinel ${controlNumber}`;
  } else if (status_ === "ACTIVE") {
    msg = `Press OK to set Sentinel ${controlNumber} with the actual values`;
  } else {
    alert("Invalid status"); return; // NEW
  }

  if (!confirm(msg)) return;

  fetch('/setsentinel', {
    method: 'POST',
    body: new URLSearchParams({
      'control': controlNumber,
      'status': status_,
      'min': minval_,
      'max': maxval_,
      'time_persist': time_severity_,
      'time_psnooze': timesnooze_
    })
  })
  .then(response => response.text())
  .then(data => {
    console.log("Sentinel aggiornata con successo:", data);
  })
  .catch(error => console.error("Errore nell'aggiornamento del sentinel:", error));
}

//get status
function updateStatusSentinel() {
  fetch('/wifisentinelGetStatus')
    .then(response => response.json())
    .then(data => {
      // Sensore 1
      document.getElementById("Sentinel1StatusRow1").textContent    = data.SentinelStatus1Row1          || "loading";
      document.getElementById("Sentinel1StatusRow2").textContent    = data.SentinelStatus1Row2          || "loading";

      document.getElementById("Sentinel2StatusRow1").textContent    = data.SentinelStatus2Row1          || "loading";
      document.getElementById("Sentinel2StatusRow2").textContent    = data.SentinelStatus2Row2          || "loading";


      document.getElementById("Sentinel3StatusRow1").textContent    = data.SentinelStatus3Row1          || "loading";
      document.getElementById("Sentinel3StatusRow2").textContent    = data.SentinelStatus3Row2          || "loading";


      document.getElementById("Sentinel4StatusRow1").textContent    = data.SentinelStatus4Row1          || "loading";
      document.getElementById("Sentinel4StatusRow2").textContent    = data.SentinelStatus4Row2          || "loading";

    })
    .catch(error => {
      console.error('Errore nel recuperare lo stato:', error);
    });
}

  window.onload = function() {
    updateStatusSentinel();
    setInterval(updateStatusSentinel, 1000); // Aggiorna ogni secondo
    if (isAPMode) {
      alert("⚠️ System is in Access Point Mode. Mail notifications are not active.");
    }
  };

</script>
)rawliteral";

const char WSER_SENTINEL_CLOSE_PAGE[] PROGMEM = R"rawliteral(
</body>
</html>
)rawliteral";

#endif 
