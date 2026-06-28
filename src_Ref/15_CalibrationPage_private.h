#ifndef WSER_SENSOR_PRIVATE_H
#define WSER_SENSOR_PRIVATE_H

//----defines
#define WSER_CAL_TIMEOUT_INTERVAL 500

//---- Constants
const char WSER_HTML_CALIBRATION_HEAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>WiFi Calibration</title>
)rawliteral";

const char WSER_HTML_CALIBRATION_STYLE[] PROGMEM = R"rawliteral(
<style>

  .sensorForm-inner { display: flex; justify-content: center; gap: 10px; }
  .select-button-size { width: 130px; height: 40px; font-size: 14px; border-radius: 5px; }
  .submit-type-button{ padding: 8px 16px; font-size: 14px; height: 40px; width: 120px; cursor: pointer; border: none;
                      border-radius: 5px; background-color: #2574FF; color: white; }

  .links a { display: inline-block; margin-right: 15px; text-decoration: none; color: #2a85cc; font-weight: bold; }

  .links a:hover { text-decoration: underline; }

  #sensorType1,
  #sensorType2 { width: 200px; height: 42px; padding-left: 6px; font-size: 16px; font-weight: bold; }

  .calPointSect { display:flex; justify-content:center; gap:20px; flex-wrap:nowrap; overflow-x:auto;  }

  .calPointSectLow,
  .calPointSectHigh { display:flex; flex-direction:column; gap:20px;          margin:10px 0; 
                      padding:10px; border-radius:10px;    text-align:center; width:fit-content;  
                      align-items: center; }
  .calPointSectLow   {border:2px solid blue;}
  .calPointSectHigh  {border:2px solid red;}

  .calibrationPointLabel { font-weight: bold; font-size: 16px; margin-bottom: 10px; }

  .calibration-range-note { font-size: 12px; color: #555; margin: 0; text-align: center; display: block; } 
  
  .calibration-adjust-buttons { display: flex; justify-content: center; align-items: center; gap: 10px; margin-bottom: 10px; } 
  
  .arrow-button { background-color: #f0f0f0; color: #555; width: 190px; height: 40px; border: none; border-radius: 5px; font-size: 20px; } 
  
  #calPointLow1, #calPointHigh1,
  #calPointLow3, #calPointHigh3,
  #calPointLow4, #calPointHigh4 { width: 350px; }
  
  #rangeValuePointLow1, #rangeValuePointHigh1,
  #rangeValuePointLow3, #rangeValuePointHigh3,
  #rangeValuePointLow4, #rangeValuePointHigh4 { font-size: 14px; color: black; font-weight: bold; }
  
  .calibration-actions { display: flex; justify-content: space-between; gap: 33px; margin-top: 5px; } 
  
  .btn-enter { background-color: #00BFFF; } 
  .btn-reset { background-color: Tomato; } 
  .btn-enter, 
  .btn-reset { color: white; width: 170px; padding: 10px 15px; border: none; border-radius: 5px; cursor: pointer; transition: background-color 0.3s ease; } 
  
  #confirmationDialog, 
  #confirmationDialog2 { display: none; position: fixed; top: 50%; left: 50%; transform: translate(-50%, -50%); padding: 20px; 
                          background-color: white; border-radius: 8px; box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.2); }
  
  #confirmationDialog { top: 25%; } 
  
  #confirmationDialog2 { top: 55%; } 
  
  #confirmationDialog button, 
  #confirmationDialog2 button { padding: 10px 20px; border: none; border-radius: 5px; color: white; cursor: pointer; } 
  
  #confirmationDialog .yes-button, 
  #confirmationDialog2 .yes-button { background-color: #4CAF50; margin-right: 100px; } 
  

  #confirmationDialog .no-button,
  #confirmationDialog2 .no-button { background-color: #f44336; }

  .status-text { font-size: 14px; color: black; font-weight: bold; }

  #calibrationStatusText { font-weight: bold; display: inline-block; min-height: 2em; }

</style>
)rawliteral";

const char WSER_HTML_CALIBRATION_HEAD_OPEN[] PROGMEM = R"rawliteral(
</head>
<body>
)rawliteral";

const char WSER_HTML_CALIBRATION_BODY_OPEN[] PROGMEM = R"rawliteral(
<div style='text-align: center; font-size: 32px; font-weight: bold;'>🧪 Sensors</div>
)rawliteral";

const char WSER_HTML_SENSOR_BODY_CONTENT[] PROGMEM = R"rawliteral(
<!-- ===== SENSOR 1 ===== -->
<hr class="line-page-sub-section">
<div class="section-title">🧪 Sensor 1</div>

<div class="sensorForm-inner">
  <select id="ControlType1" name="sensorType1" class="select-button-size">
    <option value="ph">💧 PH</option>
    <option value="orp">⚗️ ORP</option>
  </select>
  <input type="submit" value="Set Type" class="submit-type-button" onclick="SetCtrlType(1)">
</div>

<div class="calPointSect">
  <div class="calPointSectLow">
    <div class="calibrationPointLabel">Calibration Point 🔻</div>
    <div class="calibration-adjust-buttons">
      <button type="button" onclick="adjustValue(1, 1, -1)" class="arrow-button">←</button>
      <button type="button" onclick="adjustValue(1, 1,  1)" class="arrow-button">→</button>
    </div>

    <input type="range" id="calPointLow1" min="{S1_MIN}" max="{S1_MAX}" value="{S1_VALUE}">
    <span id="rangeValuePointLow1" class="status-text">{S1_LOW_LABEL}</span>

    <div class="calibration-actions">
      <button type="button" onclick="sendCalEnter(1, 1)" class="btn-enter">✅ Enter</button>
      <button type="button" onclick="resetCalPoint(1, 1)" class="btn-reset">🛑 Reset</button>
    </div>
  </div>

  <div class="calPointSectHigh">
    <div class="calibrationPointLabel">Calibration Point 🔺</div>
    <div class="calibration-adjust-buttons">
      <button type="button" onclick="adjustValue(1, 2, -1)" class="arrow-button">←</button>
      <button type="button" onclick="adjustValue(1, 2,  1)" class="arrow-button">→</button>
    </div>

    <input type="range" id="calPointHigh1" min="{S1_MIN}" max="{S1_MAX}" value="{S1_VALUE}">
    <span id="rangeValuePointHigh1" class="status-text">{S1_HIGH_LABEL}</span>

    <div class="calibration-actions">
      <button type="button" onclick="sendCalEnter(1, 2)" class="btn-enter">✅ Enter</button>
      <button type="button" onclick="resetCalPoint(1, 2)" class="btn-reset">🛑 Reset</button>
    </div>
  </div>
</div>

<div class="status-box">
  <strong id="CalibrStatusSens1Row1">Loading...</strong><br>
  <strong id="CalibrStatusSens1Row2">Loading...</strong>
</div>

<!-- ===== SENSOR 2 (Temp placeholder) ===== -->
<hr class="line-page-sub-section">
<div class="section-title">🧪 Sensor 2</div>
<div class="sensorForm-inner">
  <select id="ControlType2" name="sensorType2" class="select-button-size">
    <!-- ===== <option value="ntc">🌡️ Temp</option>===== -->
    <option value="ds18">🌡️ Temp</option>
  </select>
</div>
<!-- Placeholder: simple TEMP section (no calibration UI yet) -->

<!-- ===== SENSOR 3 ===== -->
<hr class="line-page-sub-section">
<div class="section-title">🧪 Sensor 3</div>

<div class="sensorForm-inner">
  <select id="ControlType3" name="sensorType3" class="select-button-size">
    <option value="ec">⚡ EC</option>
    <option value="tds">💎 TDS</option>
    <option value="sal">🧂 Salinity</option>
    <option value="ntc">🌡️ Temp</option>
    <option value="diff">🌡️↕️ Diff Temp</option>
    <option value="avg">🌡️∅ Avg Temp</option>
  </select>
  <input type="submit" value="Set Type" class="submit-type-button" onclick="SetCtrlType(3)">
</div>

<div class="calPointSect">
  <div class="calPointSectLow">
    <div class="calibrationPointLabel">Calibration Point 🔻</div>
    <div class="calibration-adjust-buttons">
      <button type="button" onclick="adjustValue(3, 1, -1)" class="arrow-button">←</button>
      <button type="button" onclick="adjustValue(3, 1, 1)"" class="arrow-button">→</button>
    </div>

    <input type="range" id="calPointLow3" min="{S3_MIN}" max="{S3_MAX}" value="{S3_VALUE}">
    <span id="rangeValuePointLow3" class="status-text">{S3_LOW_LABEL}</span>

    <div class="calibration-actions">
      <button type="button" onclick="sendCalEnter(3, 1)" class="btn-enter">✅ Enter</button>
      <button type="button" onclick="resetCalPoint(3, 1)" class="btn-reset">🛑 Reset</button>
    </div>
  </div>

  <div class="calPointSectHigh">
    <div class="calibrationPointLabel">Calibration Point 🔺</div>
    <div class="calibration-adjust-buttons">
      <button type="button" onclick="adjustValue(3, 2, -1)" class="arrow-button">←</button>
      <button type="button" onclick="adjustValue(3, 2, 1)" class="arrow-button">→</button>
    </div>

    <input type="range" id="calPointHigh3" min="{S3_MIN}" max="{S3_MAX}" value="{S3_VALUE}">
    <span id="rangeValuePointHigh3" class="status-text">{S3_HIGH_LABEL}</span>

    <div class="calibration-actions">
      <button type="button" onclick="sendCalEnter(3, 2)" class="btn-enter">✅ Enter</button>
      <button type="button" onclick="resetCalPoint(3, 2)" class="btn-reset">🛑 Reset</button>
    </div>
  </div>
</div>

<div class="status-box">
  <strong id="CalibrStatusSens3Row1">Loading...</strong><br>
  <strong id="CalibrStatusSens3Row2">Loading...</strong>
</div>

<!-- ===== SENSOR 4 ===== -->
<hr class="line-page-sub-section">
<div class="section-title">🧪 Sensor 4</div>

<div class="sensorForm-inner">
  <select id="ControlType4" name="sensorType4" class="select-button-size">
    <option value="ph">💧 PH</option>
    <option value="orp">⚗️ ORP</option>
  </select>
  <input type="submit" value="Set Type" class="submit-type-button" onclick="SetCtrlType(4)">
</div>

<div class="calPointSect">
  <div class="calPointSectLow">
    <div class="calibrationPointLabel">Calibration Point 🔻</div>
    <div class="calibration-adjust-buttons">
      <button type="button" onclick="adjustValue(4, 1, -1)" class="arrow-button">←</button>
      <button type="button" onclick="adjustValue(4, 1, 1)" class="arrow-button">→</button>
    </div>

    <input type="range" id="calPointLow4" min="{S4_MIN}" max="{S4_MAX}" value="{S4_VALUE}">
    <span id="rangeValuePointLow4" class="status-text">{S4_LOW_LABEL}</span>

    <div class="calibration-actions">
      <button type="button" onclick="sendCalEnter(4, 1)" class="btn-enter">✅ Enter</button>
      <button type="button" onclick="resetCalPoint(4, 1)" class="btn-reset">🛑 Reset</button>
    </div>
  </div>

  <div class="calPointSectHigh">
    <div class="calibrationPointLabel">Calibration Point 🔺</div>
    <div class="calibration-adjust-buttons">
      <button type="button" onclick="adjustValue(4, 2, -1)" class="arrow-button">←</button>
      <button type="button" onclick="adjustValue(4, 2, 1)" class="arrow-button">→</button>
    </div>

    <input type="range" id="calPointHigh4" min="{S4_MIN}" max="{S4_MAX}" value="{S4_VALUE}">
    <span id="rangeValuePointHigh4" class="status-text">{S4_HIGH_LABEL}</span>

    <div class="calibration-actions">
      <button type="button" onclick="sendCalEnter(4, 2)" class="btn-enter">✅ Enter</button>
      <button type="button" onclick="resetCalPoint(4, 2)" class="btn-reset">🛑 Reset</button>
    </div>
  </div>
</div>

<div class="status-box">
  <strong id="CalibrStatusSens4Row1">Loading...</strong><br>
  <strong id="CalibrStatusSens4Row2">Loading...</strong>
</div>
)rawliteral";

const char WSER_SCRIPT_SENSOR_CALIBRATION[] PROGMEM = R"rawliteral(
<script>

var sensor1Type = "{SENSOR1_TYPE}";
var sensor2Type = "{SENSOR2_TYPE}";
var sensor3Type = "{SENSOR3_TYPE}";
var sensor4Type = "{SENSOR4_TYPE}";

// --------- listener functions 

function format(val, sensorType){
  const n = Number(val);
  const t = (sensorType||'').toLowerCase();
  if (t === "sal" || t === "tds") return parseInt(n,10) + " ppm";
  if (t === "ec")  return parseInt(n,10) + " µS/cm";
  if ((t === "ntc") || (t === "diff") || (t === "avg") )return (n/10).toFixed(1) + " °C";
  if (t === "ds18") return (n/10).toFixed(1) + " °C";
  if (t === "ph")  return (n/100).toFixed(2);
  if (t === "rx" || t === "orp") return parseInt(n,10) + " mV";
  return String(n);
}

document.addEventListener('DOMContentLoaded', function () {

  {
    const sliderLow = document.getElementById('calPointLow1');
    const sliderHigh = document.getElementById('calPointHigh1');
    const rangeLow   = document.getElementById('rangeValuePointLow1');
    const rangeHigh  = document.getElementById('rangeValuePointHigh1');

    sliderLow.addEventListener('input',  () => { rangeLow.textContent  = format(sliderLow.value, sensor1Type); });
    sliderHigh.addEventListener('input', () => { rangeHigh.textContent = format(sliderHigh.value, sensor1Type); });
  }

  {
    const sliderLow = document.getElementById('calPointLow3');
    const sliderHigh = document.getElementById('calPointHigh3');
    const rangeLow   = document.getElementById('rangeValuePointLow3');
    const rangeHigh  = document.getElementById('rangeValuePointHigh3');

    sliderLow.addEventListener('input',  () => { rangeLow.textContent  = format(sliderLow.value, sensor3Type); });
    sliderHigh.addEventListener('input', () => { rangeHigh.textContent = format(sliderHigh.value, sensor3Type); });
  }

  {
    const sliderLow = document.getElementById('calPointLow4');
    const sliderHigh = document.getElementById('calPointHigh4');
    const rangeLow   = document.getElementById('rangeValuePointLow4');
    const rangeHigh  = document.getElementById('rangeValuePointHigh4');

  sliderLow.addEventListener('input',  () => { rangeLow.textContent  = format(sliderLow.value, sensor4Type); });
  sliderHigh.addEventListener('input', () => { rangeHigh.textContent = format(sliderHigh.value, sensor4Type); });
  }

});

function adjustValue(sensor, calpt, dir){

  let sliderId; 
  let labelId;
  let sensortype;

  if (sensor === 1){
    sliderId = (calpt===1) ? 'calPointLow1'  : 'calPointHigh1';
    labelId  = (calpt===1) ? 'rangeValuePointLow1' : 'rangeValuePointHigh1';
    sensortype = sensor1Type;
  } else if (sensor === 3){
    sliderId = (calpt===1) ? 'calPointLow3'  : 'calPointHigh3';
    labelId  = (calpt===1) ? 'rangeValuePointLow3' : 'rangeValuePointHigh3';
    sensortype = sensor3Type;
  } else if (sensor === 4){
    sliderId = (calpt===1) ? 'calPointLow4'  : 'calPointHigh4';
    labelId  = (calpt===1) ? 'rangeValuePointLow4' : 'rangeValuePointHigh4';
    sensortype = sensor4Type;
  }

  const s   = document.getElementById(sliderId);
  const lbl = document.getElementById(labelId);
  if(!s || !lbl) return;

  const min  = parseFloat(s.min);
  const max  = parseFloat(s.max);

  let v = parseFloat(s.value) + dir ;

  if (v < min) v = min;
  if (v > max) v = max;

  s.value = v;

  lbl.textContent = format(v, sensortype);
}

// --------- listener functions 
function updateUi(controlNumber, type){

  let low;
  let high;
  let lowLbl;
  let hiLbl;

  if (controlNumber === 1) {
    low    = document.getElementById('calPointLow1');
    high   = document.getElementById('calPointHigh1');
    lowLbl = document.getElementById('rangeValuePointLow1');
    hiLbl  = document.getElementById('rangeValuePointHigh1');

  } else if (controlNumber === 3) {
    low    = document.getElementById('calPointLow3');
    high   = document.getElementById('calPointHigh3');
    lowLbl = document.getElementById('rangeValuePointLow3');
    hiLbl  = document.getElementById('rangeValuePointHigh3');

  } else if (controlNumber === 4) {
    low    = document.getElementById('calPointLow4');
    high   = document.getElementById('calPointHigh4');
    lowLbl = document.getElementById('rangeValuePointLow4');
    hiLbl  = document.getElementById('rangeValuePointHigh4');

  }

  if (type === 'ec') {
    low.min = high.min = 10; low.max = high.max = 20000;
    low.value  = high.value = 1000;
    lowLbl.textContent = hiLbl.textContent  = '1000 µS/cm';

  } else if (type === 'tds') {
    low.min = high.min = 36; low.max = high.max = 12800;
    low.value = high.value = 500;
    lowLbl.textContent = '500 ppm';
    hiLbl.textContent  = '500 ppm';

  } else if (type === 'sal' ) {
    low.min = high.min = 6; low.max = high.max = 11000; // scala ×10
    low.value = high.value = 35;                     
    lowLbl.textContent = '35 ppm';
    hiLbl.textContent  = '35 ppm';

  } else if ((type === 'ntc' ) || (type === 'diff' ) || (type === 'avg' )) {
    low.min = high.min = 50;     // -50.0°C
    low.max = high.max = 1200;     // 150.0°C
    low.value = high.value = 250;  // 25.0°C
    lowLbl.textContent = hiLbl.textContent = '25.0 °C';

  } else if (type === 'ph') {
    low.min = high.min = 1;  low.max = high.max = 1400;
    low.value = high.value = 700;
    lowLbl.textContent = '7.00';
    hiLbl.textContent  = '7.00';

  } else if (type === 'orp') {
    low.min = high.min = -2000;  low.max = high.max = 2000;
    low.value = high.value = 0;
    lowLbl.textContent = '0 mV';
    hiLbl.textContent  = '0 mV';
  } 

}

// set sensor type funtions---- should be in  acommon script part
  function SetCtrlType(controlNumber) {

    let type;

    if (controlNumber === 1 ) {
      type = document.getElementById("ControlType1").value;
    } else if (controlNumber === 3 ) {   
      type = document.getElementById("ControlType3").value;
    } else if (controlNumber === 4 ) {   
      type = document.getElementById("ControlType4").value;
    }

    if (
      (controlNumber === 1 && type === sensor1Type) ||
      (controlNumber === 3 && type === sensor3Type) ||
      (controlNumber === 4 && type === sensor4Type)
    ) {
      alert("Sensor type " + controlNumber  + " is already set as " + type);
      return;
    }

    var confirmed = confirm("ATTENTION!!: Change of Sensor type will reset all the Calibration, Control Type, Control settings to default values.\nPress OK to proceed or Cancel to abort command.");

    if (!confirmed) {
      return;
    }

    fetch('/setControlType', {
      method: 'POST',
      body: new URLSearchParams({
        'control': controlNumber,
        'type': type
      })
    })
    .then(response => response.text())
    .then(data => {
      console.log("Tipo Controllo aggiornato con successo");

      // 🔁 Aggiorna PRIMA la variabile globale
      if (controlNumber === 1) {
        sensor1Type = type;
      } else if (controlNumber === 3) {
        sensor3Type = type;
      } else if (controlNumber === 4) {
        sensor4Type = type;
      }

      updateUi(controlNumber, type); 

    })
    .catch(error => console.error("Errore nell'aggiornamento del tipo sensore"));
  }

function sendCalEnter(sensorNumber, calpoint) {

  let sensorType;
  let value;

  if ( sensorNumber === 1 ){
    if (calpoint === 1) {
      value = document.getElementById('calPointLow1').value;
    } else if (calpoint === 2) {
      value = document.getElementById('calPointHigh1').value;
    }
    sensorType = sensor1Type;
  }else if ( sensorNumber === 3 ){
    if (calpoint === 1) {
      value = document.getElementById('calPointLow3').value;
    } else if (calpoint === 2) {
      value = document.getElementById('calPointHigh3').value;
    }
    sensorType = sensor3Type;
  }else if ( sensorNumber === 4 ){
    if (calpoint === 1) {
      value = document.getElementById('calPointLow4').value;
    } else if (calpoint === 2) {
      value = document.getElementById('calPointHigh4').value;
    }
    sensorType = sensor4Type;
  }

  if (sensorType === 'ntc' || sensorType === 'diff' || sensorType === 'avg') {
    alert('Calibration is not available for this sensor type.');
    return;
  }

    const icon = calpoint === 1 ? '🔻' : '🔺';

    const displayValue = format(value, sensorType);

    const msg = `Press OK to set calibration point ${icon} for sensor ${sensorNumber} to value ${displayValue}.`;
    if (!confirm(msg)) return;

    fetch('/setCalPoint', {
      method: 'POST',
      body: new URLSearchParams({
        'sensorNumb': sensorNumber,
        'calPoint': calpoint,
        'calPointValue': value
      })
    })
    .then(r => r.text())
    .then(() => {
      console.log(`Calibration: point ${calpoint} set for Sensor ${sensorNumber}.`);
    })
    .catch(err => {
      console.error(`Error setting calibration point ${calpoint} for Sensor ${sensorNumber}:`, err);
    });
  }

  function resetCalPoint(sensorNumber, calpoint) {

    const icon = calpoint === 1 ? '🔻' : '🔺';

    const msg = `Press OK to ReSet calibration point ${icon} for sensor ${sensorNumber}.`;
    if (!confirm(msg)) return;

    fetch('/resetCalPoint', {
      method: 'POST',
      body: new URLSearchParams({
        'sensorNumb': sensorNumber,
        'calPoint': calpoint
      })
    })
    .then(r => r.text())
    .then(() => {
      console.log(`Calibration: point ${calpoint} reset for Sensor ${sensorNumber}.`);
    })
    .catch(err => {
      console.error(`Error resetting calibration point ${calpoint} for Sensor ${sensorNumber}:`, err);
    });
  }

// ----------------get string function------------------
  function updateCalibrationPageStatus() {
    fetch('/WiFiCalibrationGetStatus')
      .then(response => response.json())
      .then(data => {

        document.getElementById("CalibrStatusSens1Row1").textContent = data.MessStatusSens1Row1  || "Loading...";
        document.getElementById("CalibrStatusSens1Row2").textContent = data.MessStatusSens1Row2  || "Loading...";

        //document.getElementById("CalibrStatusSens2Row1").textContent = data.MessStatusSens2Row1  || "Loading..."; // sensor 2 doesnt have a status
        //document.getElementById("CalibrStatusSens2Row2").textContent = data.MessStatusSens2Row2  || "Loading...";

        document.getElementById("CalibrStatusSens3Row1").textContent = data.MessStatusSens3Row1  || "Loading...";
        document.getElementById("CalibrStatusSens3Row2").textContent = data.MessStatusSens3Row2  || "Loading...";

        document.getElementById("CalibrStatusSens4Row1").textContent = data.MessStatusSens4Row1  || "Loading...";
        document.getElementById("CalibrStatusSens4Row2").textContent = data.MessStatusSens4Row2  || "Loading...";


      })
      .catch(error => {
        console.error('Errore nel recuperare lo stato:', error);
      });
  }

  // ----------------get string function------------------
  window.onload = function() {
    updateCalibrationPageStatus();
    setInterval(updateCalibrationPageStatus, 1000); // Aggiorna ogni secondo
  };
  
</script>
)rawliteral";

const char WSER_CALIBRATION_CLOSE_PAGE[] PROGMEM = R"rawliteral(
</body>
</html>
)rawliteral";

#endif 
