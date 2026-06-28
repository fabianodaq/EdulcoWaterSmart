#ifndef WSER_SETTINGS_PRIVATE_H
#define WSER_SETTINGS_PRIVATE_H

#define REMOTE_Settings_INTERVAL 1000

/* 
 * 🧠 NOTE DI MIGLIORAMENTO:
 * Si potrebbe implementare un controllo di tipo "automatico" che aggiorni dinamicamente
 * i parametri visuali (colori, range min/max, label, ecc.) solo quando necessario,
 * invece di caricarli tutti all’inizio della pagina.
 * In questo modo, se il tipo di controllo viene modificato da un comando esterno
 * (es. via bash, API o altro), il sistema ricaricherebbe automaticamente i dati
 * aggiornati senza bisogno di ricaricare manualmente la pagina o di inizializzare
 * i valori al primo load.
 */

//---- Constants
const char WSER_SETTINGS_HEAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Settings</title>
)rawliteral";

const char WSER_SETTINGS_STYLE[] PROGMEM = R"rawliteral(
<style>

  .line-page-section { border-top: 2px solid black; width: 40%; margin: 10px auto; }
  .group-logic-hyster-target { display: flex; flex-direction: column; gap: 20px; margin-bottom: 10px; padding: 20px; }
  .group-logic-hyster { display: flex; justify-content: center; align-items: flex-start; gap: 40px; }
  .logic-hyster-box { border: 1px solid #ccc; padding: 15px; display: flex; flex-direction: column; align-items: center; border-radius: 6px; }
  .select-logic { width: 150px; height: 40px; padding: 6px 12px; font-size: 16px; margin-bottom: 10px; margin-top: 5px }
  .submit-logic { width: 130px; height: 30px; padding: 6px 12px; font-size: 14px; color: white; border: none; border-radius: 4px; cursor: pointer; margin-top: 20px; }
  .buttons-hyster { background-color: #f0f0f0; color: #555; width: 120px; height: 30px; line-height: 15px; border: none; border-radius: 5px; font-size: 20px; }
  .range-hyster {width: 240px; margin-bottom: 5px;}
  .span-hyster {margin-bottom: 5px; font-weight: bold;}
  .button-set-hyster {width: 240px; padding: 6px 12px; font-size: 14px; color: white; border: none; border-radius: 4px;}
  .group-target {border: 1px solid #ccc; width: 410px; margin: 0 auto; display: flex; flex-direction: column; align-items: center; gap: 8px; border-radius: 6px; padding: 15px;}
  .group-buttons-incr-decr-target {display: flex; justify-content: center; align-items: center; gap: 10px; margin-top: 5px;}
  .buttons-incr-decr-target {background-color: #f0f0f0; color: #555; width: 190px; height: 40px; border: none; border-radius: 5px; font-size: 20px;}
  .range-target{width: 380px;}
  .span-target{ font-size: 16px; font-weight: bold;}
  .button-set-target { color: white; border: none; border-radius: 5px; padding: 8px 12px; cursor: pointer; font-size: 14px; transition: background-color 0.3s ease; width: 380px; height: 35px;}
  .status-panel { display: flex; justify-content: center; gap: 10px; margin-top: 20px; }

</style>
)rawliteral";


const char WSER_SETTINGS_HEAD_OPEN[] PROGMEM = R"rawliteral(
</head>
<body>
)rawliteral";

const char WSER_SETTINGS_BODY_OPEN[] PROGMEM = R"rawliteral(
<div style='text-align: center; font-size: 32px; font-weight: bold;'>⚙️ Controls</div>
)rawliteral";

const char WSER_SETTINGS_SENSOR1_BODY_CONTENT[] PROGMEM = R"rawliteral(

<hr class="line-page-section ">

<div class="section-title">⚙️ Control 1</div>

<div class="group-logic-hyster-target">
  <div class="group-logic-hyster">

    <div class="logic-hyster-box">
      <select id="logic1" class="select-logic">
        <option value="NEG">➖ Negative</option>
        <option value="POS">➕ Positive</option>
      </select>
      <input id="logicButton1" type="submit" value="Set Logic 🧠" onclick="SetControlLogik(1)" class="submit-logic" style="background-color:{COLOR_CTRL_1};">
    </div>

    <div class="logic-hyster-box">
      <div style="margin-bottom: 5px;">
        <button type="button" onclick="adjustHysteresis(1,-1)" class="buttons-hyster">←</button>
        <button type="button" onclick="adjustHysteresis(1, 1)"  class="buttons-hyster">→</button>
      </div>
      <input type="range" id="hysteresis1" min="{HYST_MIN_1}" max="{HYST_MAX_1}" value="{HYST_VALUE_1}" class="range-hyster">
      <span id="hysteresisRangeValue1" class="span-hyster">{HYST_LABEL_1}</span>
      <button id="hysteresisButton1" type="button" onclick="SetControlHysteresis(1)" class="button-set-hyster" style="background-color:{COLOR_CTRL_1};">
        Set Hysteresis 🔁
      </button>
    </div>
  </div>
</div>

<div class="group-target ">
  <div class="group-buttons-incr-decr-target">
    <button type="button" onclick="adjustSetPoint(1,-1)" class="buttons-incr-decr-target">←</button>
    <button type="button" onclick="adjustSetPoint(1, 1)"  class="buttons-incr-decr-target">→</button>
  </div>
  <input type="range" id="setPoint1" min="{SET_MIN_1}" max="{SET_MAX_1}" value="{SET_VALUE_1}" class="range-target">
  <span id="setPointRangeValue1" class="span-target">{SET_LABEL_1}</span>
  <button id="targetButton1" type="button" onclick="SetControlTarget(1)" class ="button-set-target" style="background-color:{COLOR_CTRL_1};">Set Target 🎯</button>
</div>

<div class="status-panel">
  <div class="status-box">
    <strong id="SettingsStatusSens1">---</strong>
  </div>
</div>

<hr class="line-page-section ">

<div class="section-title">⚙️ Control 2</div>

<div class="group-logic-hyster-target">
  <div class="group-logic-hyster">

    <div class="logic-hyster-box">
      <select id="logic2" class="select-logic">
        <option value="NEG">➖ Negative</option>
        <option value="POS">➕ Positive</option>
      </select>
      <input id="logicButton2" type="submit" value="Set Logic 🧠" onclick="SetControlLogik(2)" class="submit-logic" style="background-color:{COLOR_CTRL_2};">
    </div>

    <div class="logic-hyster-box">
      <div style="margin-bottom: 5px;">
        <button type="button" onclick="adjustHysteresis(2,-1)" class="buttons-hyster">←</button>
        <button type="button" onclick="adjustHysteresis(2, 1)"  class="buttons-hyster">→</button>
      </div>
      <input type="range" id="hysteresis2" min="{HYST_MIN_2}" max="{HYST_MAX_2}" value="{HYST_VALUE_2}" class="range-hyster">
      <span id="hysteresisRangeValue2" class="span-hyster">{HYST_LABEL_2}</span>
      <button id="hysteresisButton2" type="button" onclick="SetControlHysteresis(2)" class="button-set-hyster" style="background-color:{COLOR_CTRL_2};">
        Set Hysteresis 🔁
      </button>
    </div>
  </div>
</div>

<div class="group-target ">
  <div class="group-buttons-incr-decr-target">
    <button type="button" onclick="adjustSetPoint(2,-1)" class="buttons-incr-decr-target">←</button>
    <button type="button" onclick="adjustSetPoint(2, 1)"  class="buttons-incr-decr-target">→</button>
  </div>
  <input type="range" id="setPoint2" min="{SET_MIN_2}" max="{SET_MAX_2}" value="{SET_VALUE_2}" class="range-target">
  <span id="setPointRangeValue2" class="span-target">{SET_LABEL_2}</span>
  <button id="targetButton2" type="button" onclick="SetControlTarget(2)" class ="button-set-target" style="background-color:{COLOR_CTRL_2};">Set Target 🎯</button>
</div>

<div class="status-panel">
  <div class="status-box">
    <strong id="SettingsStatusSens2">---</strong>
  </div>
</div>

<hr class="line-page-section ">

<div class="section-title">⚙️ Control 3</div>

<div class="group-logic-hyster-target">
  <div class="group-logic-hyster">

    <div class="logic-hyster-box">
      <select id="logic3" class="select-logic">
        <option value="NEG">➖ Negative</option>
        <option value="POS">➕ Positive</option>
      </select>
      <input id="logicButton3" type="submit" value="Set Logic 🧠" onclick="SetControlLogik(3)" class="submit-logic" style="background-color:{COLOR_CTRL_3};">
    </div>

    <div class="logic-hyster-box">
      <div style="margin-bottom: 5px;">
        <button type="button" onclick="adjustHysteresis(3,-1)" class="buttons-hyster">←</button>
        <button type="button" onclick="adjustHysteresis(3, 1)"  class="buttons-hyster">→</button>
      </div>
      <input type="range" id="hysteresis3" min="{HYST_MIN_3}" max="{HYST_MAX_3}" value="{HYST_VALUE_3}" class="range-hyster">
      <span id="hysteresisRangeValue3" class="span-hyster">{HYST_LABEL_3}</span>
      <button id="hysteresisButton3" type="button" onclick="SetControlHysteresis(3)" class="button-set-hyster" style="background-color:{COLOR_CTRL_3};">
        Set Hysteresis 🔁
      </button>
    </div>
  </div>
</div>

<div class="group-target ">
  <div class="group-buttons-incr-decr-target">
    <button type="button" onclick="adjustSetPoint(3,-1)" class="buttons-incr-decr-target">←</button>
    <button type="button" onclick="adjustSetPoint(3, 1)"  class="buttons-incr-decr-target">→</button>
  </div>
  <input type="range" id="setPoint3" min="{SET_MIN_3}" max="{SET_MAX_3}" value="{SET_VALUE_3}" class="range-target">
  <span id="setPointRangeValue3" class="span-target">{SET_LABEL_3}</span>
  <button id="targetButton3" type="button" onclick="SetControlTarget(3)" class ="button-set-target" style="background-color:{COLOR_CTRL_3};">Set Target 🎯</button>
</div>

<div class="status-panel">
  <div class="status-box">
    <strong id="SettingsStatusSens3">---</strong>
  </div>
</div>

<hr class="line-page-section ">

<div class="section-title">⚙️ Control 4</div>

<div class="group-logic-hyster-target">
  <div class="group-logic-hyster">

    <div class="logic-hyster-box">
      <select id="logic4" class="select-logic">
        <option value="NEG">➖ Negative</option>
        <option value="POS">➕ Positive</option>
      </select>
      <input id="logicButton4" type="submit" value="Set Logic 🧠" onclick="SetControlLogik(4)" class="submit-logic" style="background-color:{COLOR_CTRL_4};">
    </div>

    <div class="logic-hyster-box">
      <div style="margin-bottom: 5px;">
        <button type="button" onclick="adjustHysteresis(4,-1)" class="buttons-hyster">←</button>
        <button type="button" onclick="adjustHysteresis(4, 1)"  class="buttons-hyster">→</button>
      </div>
      <input type="range" id="hysteresis4" min="{HYST_MIN_4}" max="{HYST_MAX_4}" value="{HYST_VALUE_4}" class="range-hyster">
      <span id="hysteresisRangeValue4" class="span-hyster">{HYST_LABEL_4}</span>
      <button id="hysteresisButton4" type="button" onclick="SetControlHysteresis(4)" class="button-set-hyster" style="background-color:{COLOR_CTRL_4};">
        Set Hysteresis 🔁
      </button>
    </div>
  </div>
</div>

<div class="group-target ">
  <div class="group-buttons-incr-decr-target">
    <button type="button" onclick="adjustSetPoint(4,-1)" class="buttons-incr-decr-target">←</button>
    <button type="button" onclick="adjustSetPoint(4, 1)"  class="buttons-incr-decr-target">→</button>
  </div>
  <input type="range" id="setPoint4" min="{SET_MIN_4}" max="{SET_MAX_4}" value="{SET_VALUE_4}" class="range-target">
  <span id="setPointRangeValue4" class="span-target">{SET_LABEL_4}</span>
  <button id="targetButton4" type="button" onclick="SetControlTarget(4)" class ="button-set-target" style="background-color:{COLOR_CTRL_4};">Set Target 🎯</button>
</div>

<div class="status-panel">
  <div class="status-box">
    <strong id="SettingsStatusSens4">---</strong>
  </div>
</div>

)rawliteral";

const char WSER_SETTINGS_SENSOR2_BODY_CONTENT[] PROGMEM = R"rawliteral(
)rawliteral";

const char WSER_SETTINGS_TYPE_DEF_CONTENT[] PROGMEM = R"rawliteral(
<script>
var sensor1SettingType = "{SENSOR1_SETTING_TYPE}";
var sensor2SettingType = "{SENSOR2_SETTING_TYPE}";
var sensor3SettingType = "{SENSOR3_SETTING_TYPE}";
var sensor4SettingType = "{SENSOR4_SETTING_TYPE}";
</script>
)rawliteral";

const char WSER_SETTINGS_JAVA_FUNTIONS_SCRIPT[] PROGMEM = R"rawliteral(
<script>

var sliderHysteresis1 = document.getElementById('hysteresis1');
var valueHysteresis1 = document.getElementById('hysteresisRangeValue1');

var sliderTarget1 = document.getElementById('setPoint1');
var valueTarget1 = document.getElementById('setPointRangeValue1');

if (typeof sliderHysteresis1 !== "undefined" && sliderHysteresis1 && valueHysteresis1) {
  sliderHysteresis1.addEventListener('input', function () {
    valueHysteresis1.textContent = formatSensorValue(sliderHysteresis1.value, sensor1SettingType);
  });
}

if (typeof sliderTarget1 !== "undefined" && sliderTarget1 && valueTarget1) {
  sliderTarget1.addEventListener('input', function () {
    valueTarget1.textContent = formatSensorValue(sliderTarget1.value, sensor1SettingType);
  });
}

var sliderHysteresis2 = document.getElementById('hysteresis2');
var valueHysteresis2 = document.getElementById('hysteresisRangeValue2');

var sliderTarget2 = document.getElementById('setPoint2');
var valueTarget2 = document.getElementById('setPointRangeValue2');

if (typeof sliderHysteresis2 !== "undefined" && sliderHysteresis2 && valueHysteresis2) {
  sliderHysteresis2.addEventListener('input', function () {
    valueHysteresis2.textContent = formatSensorValue(sliderHysteresis2.value, sensor2SettingType);
  });
}

if (typeof sliderTarget2 !== "undefined" && sliderTarget2 && valueTarget2) {
  sliderTarget2.addEventListener('input', function () {
    valueTarget2.textContent = formatSensorValue(sliderTarget2.value, sensor2SettingType);
  });
}

var sliderHysteresis3 = document.getElementById('hysteresis3');
var valueHysteresis3 = document.getElementById('hysteresisRangeValue3');

var sliderTarget3 = document.getElementById('setPoint3');
var valueTarget3 = document.getElementById('setPointRangeValue3');

if (typeof sliderHysteresis3 !== "undefined" && sliderHysteresis3 && valueHysteresis3) {
  sliderHysteresis3.addEventListener('input', function () {
    valueHysteresis3.textContent = formatSensorValue(sliderHysteresis3.value, sensor3SettingType);
  });
}

if (typeof sliderTarget3 !== "undefined" && sliderTarget3 && valueTarget3) {
  sliderTarget3.addEventListener('input', function () {
    valueTarget3.textContent = formatSensorValue(sliderTarget3.value, sensor3SettingType);
  });
}

var sliderHysteresis4 = document.getElementById('hysteresis4');
var valueHysteresis4 = document.getElementById('hysteresisRangeValue4');

var sliderTarget4 = document.getElementById('setPoint4');
var valueTarget4 = document.getElementById('setPointRangeValue4');

if (typeof sliderHysteresis4 !== "undefined" && sliderHysteresis4 && valueHysteresis4) {
  sliderHysteresis4.addEventListener('input', function () {
    valueHysteresis4.textContent = formatSensorValue(sliderHysteresis4.value, sensor4SettingType);
  });
}

if (typeof sliderTarget4 !== "undefined" && sliderTarget4 && valueTarget4) {
  sliderTarget4.addEventListener('input', function () {
    valueTarget4.textContent = formatSensorValue(sliderTarget4.value, sensor4SettingType);
  });
}

function formatSensorValue(value, type) {
  if (type === "ec") {
    return value + " µS/cm";
  } else if (type === "sal") {
    return value + " ppm"; 
  } else if (type === "tds") {
    return value + " ppm";
  } else if (type === "ntc" || type === "avg" || type === "diff") {
    return (value / 10).toFixed(1) + " °C"; // esempio: 253 -> 25.3 °C
  } else if (type === "ds18") {
    return (value / 10).toFixed(1) + " °C"; // esempio: 253 -> 25.3 °C
  } else if (type === "ph") {
    return (value / 100).toFixed(2);
  } else if (type === "orp") {
    return value + " mV";
  }
  return value; // fallback
}

function adjustHysteresis(ctrlNumber, dir) {

    if (ctrlNumber === 1) {
    var slider = document.getElementById('hysteresis1');
    var rangeValue = document.getElementById('hysteresisRangeValue1');
    var rawVal = parseInt(slider.value);
    var type = sensor1SettingType;

    } else if (ctrlNumber === 2) {
    var slider = document.getElementById('hysteresis2');
    var rangeValue = document.getElementById('hysteresisRangeValue2');
    var rawVal = parseInt(slider.value);
    var type = sensor2SettingType;

    } else if (ctrlNumber === 3) {
    var slider = document.getElementById('hysteresis3');
    var rangeValue = document.getElementById('hysteresisRangeValue3');
    var rawVal = parseInt(slider.value);
    var type = sensor3SettingType;

    } else if (ctrlNumber === 4) {
    var slider = document.getElementById('hysteresis4');
    var rangeValue = document.getElementById('hysteresisRangeValue4');
    var rawVal = parseInt(slider.value);
    var type = sensor4SettingType;
    }

  if (!slider || !rangeValue) return;

  if (type === "ec") {
    var newValue = rawVal + dir;
    newValue = Math.max(1, Math.min(100, newValue)); // limiti ipotetici isteresi EC (µS/cm)
    slider.value = newValue;
    rangeValue.textContent = newValue + " µS/cm";
  } else if (type === "sal") {
    var newValue = rawVal + dir;
    newValue = Math.max(1, Math.min(100, newValue)); 
    slider.value = newValue;
    rangeValue.textContent = newValue + " ppm";
  } else if (type === "tds") {
    var newValue = rawVal + dir;
    newValue = Math.max(1, Math.min(100, newValue)); // limiti ipotetici isteresi TDS (ppm)
    slider.value = newValue;
    rangeValue.textContent = newValue + " ppm";
  } else if ((type === "ntc") || (type === "diff") || (type === "avg")){
    var current = rawVal / 10;                // slider lavora in decimi °C
    var newValue = current + (dir * 0.1);     // passo = 0.1 °C
    newValue = Math.max(0.1, Math.min(10.0, newValue)); // limiti ipotetici isteresi °C
    slider.value = Math.round(newValue * 10);
    rangeValue.textContent = newValue.toFixed(1) + " °C";
  } else if (type === "ds18") {
    var current = rawVal / 10;                // slider lavora in decimi °C
    var newValue = current + (dir * 0.1);     // passo = 0.1 °C
    newValue = Math.max(0.1, Math.min(10.0, newValue)); // limiti ipotetici isteresi °C
    slider.value = Math.round(newValue * 10);
    rangeValue.textContent = newValue.toFixed(1) + " °C";
  } else if (type === "ph") {
    var current = rawVal / 100;
    var newValue = current + (dir * 0.01);
    newValue = Math.min(1.00, Math.max(0.01, newValue)); // limiti ipotetici pH
    slider.value = Math.round(newValue * 100);
    rangeValue.textContent = newValue.toFixed(2);
  } else if (type === "orp") {
    var newValue = rawVal + dir;
    newValue = Math.max(1, Math.min(100, newValue)); // limiti ipotetici ORP
    slider.value = newValue;
    rangeValue.textContent = newValue + " mV";
  }

}

function adjustSetPoint(ctrlNumber, dir) {

    if (ctrlNumber === 1) {
    var slider = document.getElementById('setPoint1');
    var rangeValue = document.getElementById('setPointRangeValue1');
    var type = sensor1SettingType;

    } else if (ctrlNumber === 2) {
    var slider = document.getElementById('setPoint2');
    var rangeValue = document.getElementById('setPointRangeValue2');
    var type = sensor2SettingType;

    } else if (ctrlNumber === 3) {
    var slider = document.getElementById('setPoint3');
    var rangeValue = document.getElementById('setPointRangeValue3');
    var type = sensor3SettingType;

    } else if (ctrlNumber === 4) {
    var slider = document.getElementById('setPoint4');
    var rangeValue = document.getElementById('setPointRangeValue4');
    var type = sensor4SettingType;
    }

  if (!slider || !rangeValue) return;

  var rawVal = parseInt(slider.value);

  if (type === "ec") {
    var newValue = rawVal + dir;
    newValue = Math.max(5, Math.min(20000, newValue));   
    slider.value = newValue;
    rangeValue.textContent = newValue + " µS/cm";

  } else if (type === "sal") {
    var newValue = rawVal + dir;
    newValue = Math.max(6, Math.min(11000, newValue));     
    slider.value = newValue;
    rangeValue.textContent = newValue + " ppm";

  } else if (type === "tds") {
    var newValue = rawVal + dir;
    newValue = Math.max(6, Math.min(12800, newValue));   
    slider.value = newValue;
    rangeValue.textContent = newValue + " ppm";

  } else if ((type === "ntc")||(type === "diff")||(type === "avg")){
    // NTC in decimi di °C
    var minVal = parseInt(slider.min);
    var maxVal = parseInt(slider.max);
    var current = rawVal / 10;
    var newValue = current + (dir * 0.1);        // passo 0.1 °C
    newValue = Math.min(maxVal / 10, Math.max(minVal / 10, newValue));
    slider.value = Math.round(newValue * 10);
    rangeValue.textContent = newValue.toFixed(1) + " °C";

  } else if (type === "ds18") {
    // NTC in decimi di °C
    var minVal = parseInt(slider.min);
    var maxVal = parseInt(slider.max);
    var current = rawVal / 10;
    var newValue = current + (dir * 0.1);        // passo 0.1 °C
    newValue = Math.min(maxVal / 10, Math.max(minVal / 10, newValue));
    slider.value = Math.round(newValue * 10);
    rangeValue.textContent = newValue.toFixed(1) + " °C";

  }
    else   if (type === "ph") {
    var current = rawVal / 100;
    var newValue = current + (dir * 0.01);
    newValue = Math.min(14.00, Math.max(0.01, newValue)); // limiti ipotetici pH
    slider.value = Math.round(newValue * 100);
    rangeValue.textContent = newValue.toFixed(2);

  } else if (type === "orp") {
    var newValue = rawVal + dir;
    newValue = Math.max(1, Math.min(100, newValue)); // limiti ipotetici ORP
    slider.value = newValue;
    rangeValue.textContent = newValue + " mV";

  }
}

  function SetControlLogik(controlNumber) {
  let logikEl;
    if (controlNumber === 1) {
    logikEl = document.getElementById("logic1");
    if (!logikEl) return;

    } else if (controlNumber === 2) {
    logikEl = document.getElementById("logic2");
    if (!logikEl) return;

    } else if (controlNumber === 3) {
    logikEl = document.getElementById("logic3");
    if (!logikEl) return;

    } else if (controlNumber === 4) {
    logikEl = document.getElementById("logic4");
    if (!logikEl) return;
    }

  var logikVal = logikEl.value;

    fetch('/setControlLogik', {
      method: 'POST',
      body: new URLSearchParams({
        'control': controlNumber,
        'logik': logikVal
      })
    })
    .then(response => response.text())
    .then(data => {
      console.log("Tipo sensore aggiornato con successo");
      alert(`🧠 Logik of Control ${controlNumber} updated`);
    })
    .catch(error => console.error("Errore nell'aggiornamento del tipo sensore"));
  }

  function SetControlHysteresis(controlNumber) {
  let inputEl;
    if (controlNumber === 1) {
    inputEl = document.getElementById('hysteresis1');


    } else if (controlNumber === 2) {
    inputEl = document.getElementById('hysteresis2');


    } else if (controlNumber === 3) {
    inputEl = document.getElementById('hysteresis3');


    } else if (controlNumber === 4) {
    inputEl = document.getElementById('hysteresis4');

    }

    if (!inputEl) return;

    var hystresisVal = inputEl.value;

    fetch('/setControlHyster', {
      method: 'POST',
      body: new URLSearchParams({
        'control': controlNumber,
        'hyster': hystresisVal
      })
    })
    .then(response => response.text())
    .then(data => {
      console.log("Isteresi aggiornata");
      alert(`🔁 Hysteresis of Control ${controlNumber} updated`);
    })
    .catch(error => console.error("Errore nell'aggiornamento dell'isteresi"));
  }

function SetControlTarget(controlNumber) {

  let inputEl;

  if (controlNumber === 1) {
    inputEl = document.getElementById('setPoint1');
  } else if (controlNumber === 2) {
    inputEl = document.getElementById('setPoint2');
  } else if (controlNumber === 3) {
    inputEl = document.getElementById('setPoint3');
  } else if (controlNumber === 4) {
    inputEl = document.getElementById('setPoint4');
  }

  if (!inputEl) {
    console.error("Elemento setPoint non trovato per control", controlNumber);
    return;
  }

  const targetVal = inputEl.value;

  fetch('/setControlTarget', {
    method: 'POST',
    body: new URLSearchParams({
      'control': controlNumber,
      'target': targetVal
    })
  })
  .then(response => response.text())
  .then(data => {
    console.log("Target aggiornato con successo:", data);
    alert(`🎯 Target of Control ${controlNumber} updated`);
  })
  .catch(error => console.error("Errore nell'aggiornamento del target:", error));
}

function updateStatusSettings() {
fetch('/settingsGetStatus')
  .then(response => response.json())
  .then(data => {
    document.getElementById("SettingsStatusSens1").textContent = data.MessSettingsStatusSens1  || "Loading...";
    document.getElementById("SettingsStatusSens2").textContent = data.MessSettingsStatusSens2  || "Loading...";
    document.getElementById("SettingsStatusSens3").textContent = data.MessSettingsStatusSens3  || "Loading...";
    document.getElementById("SettingsStatusSens4").textContent = data.MessSettingsStatusSens4  || "Loading...";
  })
  .catch(error => {
    console.error('Errore nel recuperare lo stato:', error);
  });
}

  window.onload = function() {
    updateStatusSettings();
    setInterval(updateStatusSettings, 1000); // Aggiorna ogni secondo
  };
</script>
)rawliteral";

const char WSER_SETTINGS_CLOSE_PAGE[] PROGMEM = R"rawliteral(
</body>
</html>
)rawliteral";

#endif