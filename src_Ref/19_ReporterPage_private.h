#ifndef WSER_REPORTER_PRIVATE_H
#define WSER_REPORTER_PRIVATE_H

// ----------- Extern Global variables (template) -----------
/**

 */
#define VALUES_ARRAY_DIM_MINUTE_HOUR 60u // number of samples for each arrray of values
#define VALUES_ARRAY_DIM_DAY 24u // number of samples for each arrray of values


#define REPORTER_PERIOD_DAYS_DAY      1
#define REPORTER_PERIOD_DAYS_WEEK     7
#define REPORTER_PERIOD_DAYS_MONTH    30

#define REPORTER_DEFAULT_PERIOD_DAYS  REPORTER_PERIOD_DAYS_DAY

#define REPORTER_SAMPLES_NUMBER       60u

#define REPORTER_FAKTOR_60_SAMPLES    (24u*60u)  //facktor to moltiply for the days of reporting to get how many seconds1 needs to be waited

#define MAX_VALUE_REPO                32767;   // valore massimo 
#define MIN_VALUE_REPO                -32768;   // valore minimo

typedef struct {
  //strings
  String Row1;
  String Row2;

  //section collect data 
  uint8_t type;

  int16_t valuesSeconds[VALUES_ARRAY_DIM_MINUTE_HOUR]; // 60 secondi
  uint8_t currentSecondIndex; // Indice per i secondi

  int16_t valuesMinutes[VALUES_ARRAY_DIM_MINUTE_HOUR];  
  uint8_t currentMinuteIndex; 

  int16_t valuesHours[VALUES_ARRAY_DIM_DAY];  
  uint8_t currentHourIndex; 

  //section reporter handling
  bool      active;   //if the reporter is active or not

  uint8_t periodReport;

  uint32_t   periodSet; // wich is the actual period set for sendinge the report
  uint32_t   periodcnt; // actual counter in seconds1
  uint8_t   samplesCnt; // actual counter in seconds1

  int16_t AvgPeriod;    //avarage value in the period considererd
  int16_t MinPeriod;    //max value
  int16_t MaxPeriod;    //min value 

  //section reporter handling
  String filepath;

} t_Reporter;


#define NUM_REPORTERS 4


//timing defines 
#define WSER_REPORTER_TIMEOUT_INTERVAL   250  // ms //it should be 1 second / number of reports 

#define WSER_REPORTER_SAMPLE_PERIOD_S   60UL  

// ---- HTML HEAD
const char WSER_HTML_REPORTER_HEAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>WiFi Reporter</title>
)rawliteral";

// ---- CSS (una riga per classe)
const char WSER_HTML_REPORTER_STYLE[] PROGMEM = R"rawliteral(
<style>

  .chart-wrap{display:flex;justify-content:center;align-items:center;margin:8px 0;}

  .activation-mode-box { display: flex; align-items: center; justify-content: center; gap: 4px; margin: 0px auto; max-width: 600px; flex-wrap: nowrap; }

  .select-activation { height: 36px; font-size: 16px; padding: 5px 10px; font-weight: bold; }

  .status-inline{padding:10px 14px;border:1px solid #ccc;background:#f9f9f9;border-radius:8px;font-size:14px;min-width:260px;text-align:center;}
  
  .status-inline strong{display:block;}


  .wrap{max-width:760px;margin:24px auto;padding:0 16px}
  .card{background:#fff;border:1px solid #e6e6e9;border-radius:12px;box-shadow:0 1px 2px rgba(0,0,0,.03);padding:16px;margin-bottom:16px}
  
  .row  {display:flex;   gap:8px;  align-items:center; flex-wrap:wrap;
          justify-content: space-between; margin-bottom:10px  }

  .legend { margin-left: auto; font-size: 16px; text-align: right;  margin-top: 8px; padding: 5px; 
            border-radius: 5px; }

</style>
)rawliteral";

// ---- HEAD OPEN
const char WSER_HTML_REPORTER_HEAD_OPEN[] PROGMEM = R"rawliteral(
</head>
<body>
)rawliteral";

// ---- BODY OPEN (titolo pagina)
const char WSER_HTML_REPORTER_BODY_OPEN[] PROGMEM = R"rawliteral(

<div style='text-align:center;font-size:32px;font-weight:bold;'>📈 Reporter</div>

)rawliteral";

// ---- BODY CONTENT (TEMPLATE VUOTO con placeholder base)
const char WSER_HTML_REPORTER_BODY_CONTENT[] PROGMEM = R"rawliteral(

<hr class="line-page-sub-section">

<div class="section-title">📈 Reporter 1</div>

<div class="activation-mode-box">
  <select id="ReporterAction1" class="select-activation">
    <option value="ACTIVE">✅ Set</option>
    <option value="NOT_ACTIVE">❌ Reset</option>
  </select>
  <select id="ReporterInterval1" class="select-activation">
    <option value="1">Day</option>
    <option value="7">Week</option>
    <option value="30">Month</option>
  </select>
  <button class="submit-activation" onclick="performReporterAction(1)">Set Reporter</button>
</div>

<div class="chart-wrap">
  <div class="card">
    <!-- Row: bottoni -->
    <div class="row">
      <button id="btnMinute1" onclick="setMinuteValues(1)">Minute</button>
      <button id="btnHour1" onclick="setHourValues(1)">Hour</button>
      <button id="btnDay1" onclick="setDayValues(1)">Day</button>

      <div id="legend1" class="legend">EC (µS/cm)</div>

    </div>
    <!-- Canvases sovrapposti -->
    <div class="canvas-stack" style="position: relative; width:850px; height:360px;">
      <canvas id="ReporterChartBG1"
              width="850" height="360"
              style="position:absolute; inset:0; z-index:0;"></canvas>

      <canvas id="ReporterChart1"
              width="850" height="360"
              style="position:absolute; inset:0; z-index:1; pointer-events:none;"></canvas>
    </div>
  </div>
</div>

<div class="status-box">
  <strong id="ReporterInlineRow1_1">Loading...</strong><br>
  <strong id="ReporterInlineRow2_1">Loading...</strong>
</div>

<hr class="line-page-sub-section">

<div class="section-title">📈 Reporter 2</div>

<div class="activation-mode-box">
  <select id="ReporterAction2" class="select-activation">
    <option value="ACTIVE">✅ Set</option>
    <option value="NOT_ACTIVE">❌ Reset</option>
  </select>
  <select id="ReporterInterval2" class="select-activation">
    <option value="1">Day</option>
    <option value="7">Week</option>
    <option value="30">Month</option>
  </select>
  <button class="submit-activation" onclick="performReporterAction(2)">Set Reporter</button>
</div>

<div class="chart-wrap">
  <div class="card">
    <div class="row">
      <button id="btnMinute2" onclick="setMinuteValues(2)">Minute</button>
      <button id="btnHour2"   onclick="setHourValues(2)">Hour</button>
      <button id="btnDay2"    onclick="setDayValues(2)">Day</button>

      <div id="legend2" class="legend">EC (µS/cm)</div>
    </div>

    <div class="canvas-stack" style="position: relative; width:850px; height:360px;">
      <canvas id="ReporterChartBG2"
              width="850" height="360"
              style="position:absolute; inset:0; z-index:0;"></canvas>

      <canvas id="ReporterChart2"
              width="850" height="360"
              style="position:absolute; inset:0; z-index:1; pointer-events:none;"></canvas>
    </div>
  </div>
</div>

<div class="status-box">
  <strong id="ReporterInlineRow1_2">Loading...</strong><br>
  <strong id="ReporterInlineRow2_2">Loading...</strong>
</div>


<hr class="line-page-sub-section">

<div class="section-title">📈 Reporter 3</div>

<div class="activation-mode-box">
  <select id="ReporterAction3" class="select-activation">
    <option value="ACTIVE">✅ Set</option>
    <option value="NOT_ACTIVE">❌ Reset</option>
  </select>
  <select id="ReporterInterval3" class="select-activation">
    <option value="1">Day</option>
    <option value="7">Week</option>
    <option value="30">Month</option>
  </select>
  <button class="submit-activation" onclick="performReporterAction(3)">Set Reporter</button>
</div>

<div class="chart-wrap">
  <div class="card">
    <div class="row">
      <button id="btnMinute3" onclick="setMinuteValues(3)">Minute</button>
      <button id="btnHour3"   onclick="setHourValues(3)">Hour</button>
      <button id="btnDay3"    onclick="setDayValues(3)">Day</button>

      <div id="legend3" class="legend">EC (µS/cm)</div>
    </div>

    <div class="canvas-stack" style="position: relative; width:850px; height:360px;">
      <canvas id="ReporterChartBG3"
              width="850" height="360"
              style="position:absolute; inset:0; z-index:0;"></canvas>

      <canvas id="ReporterChart3"
              width="850" height="360"
              style="position:absolute; inset:0; z-index:1; pointer-events:none;"></canvas>
    </div>
  </div>
</div>

<div class="status-box">
  <strong id="ReporterInlineRow1_3">Loading...</strong><br>
  <strong id="ReporterInlineRow2_3">Loading...</strong>
</div>


<hr class="line-page-sub-section">

<div class="section-title">📈 Reporter 4</div>

<div class="activation-mode-box">
  <select id="ReporterAction4" class="select-activation">
    <option value="ACTIVE">✅ Set</option>
    <option value="NOT_ACTIVE">❌ Reset</option>
  </select>
  <select id="ReporterInterval4" class="select-activation">
    <option value="1">Day</option>
    <option value="7">Week</option>
    <option value="30">Month</option>
  </select>
  <button class="submit-activation" onclick="performReporterAction(4)">Set Reporter</button>
</div>

<div class="chart-wrap">
  <div class="card">
    <div class="row">
      <button id="btnMinute4" onclick="setMinuteValues(4)">Minute</button>
      <button id="btnHour4"   onclick="setHourValues(4)">Hour</button>
      <button id="btnDay4"    onclick="setDayValues(4)">Day</button>

      <div id="legend4" class="legend">EC (µS/cm)</div>
    </div>

    <div class="canvas-stack" style="position: relative; width:850px; height:360px;">
      <canvas id="ReporterChartBG4"
              width="850" height="360"
              style="position:absolute; inset:0; z-index:0;"></canvas>

      <canvas id="ReporterChart4"
              width="850" height="360"
              style="position:absolute; inset:0; z-index:1; pointer-events:none;"></canvas>
    </div>
  </div>
</div>

<div class="status-box">
  <strong id="ReporterInlineRow1_4">Loading...</strong><br>
  <strong id="ReporterInlineRow2_4">Loading...</strong>
</div>

)rawliteral";

// ---- SCRIPT (TEMPLATE funzioni)
const char WSER_SCRIPT_REPORTER[] PROGMEM = R"rawliteral(
<script>

  // variables 
  var typeControl1 = "{SENSOR1_TYPE}";
  var oldType1     = "{SENSOR1_TYPE}";

  var typeControl2 = "{SENSOR2_TYPE}";
  var oldType2     = "{SENSOR2_TYPE}";

  var typeControl3 = "{SENSOR3_TYPE}";
  var oldType3     = "{SENSOR3_TYPE}";

  var typeControl4 = "{SENSOR4_TYPE}";
  var oldType4     = "{SENSOR4_TYPE}";

  const isAPMode = {IS_AP_FLAG};

  var seconds1 = [
    "{secVal0_1}", "{secVal1_1}", "{secVal2_1}", "{secVal3_1}", "{secVal4_1}",
    "{secVal5_1}", "{secVal6_1}", "{secVal7_1}", "{secVal8_1}", "{secVal9_1}",
    "{secVal10_1}", "{secVal11_1}", "{secVal12_1}", "{secVal13_1}", "{secVal14_1}",
    "{secVal15_1}", "{secVal16_1}", "{secVal17_1}", "{secVal18_1}", "{secVal19_1}",
    "{secVal20_1}", "{secVal21_1}", "{secVal22_1}", "{secVal23_1}", "{secVal24_1}",
    "{secVal25_1}", "{secVal26_1}", "{secVal27_1}", "{secVal28_1}", "{secVal29_1}",
    "{secVal30_1}", "{secVal31_1}", "{secVal32_1}", "{secVal33_1}", "{secVal34_1}",
    "{secVal35_1}", "{secVal36_1}", "{secVal37_1}", "{secVal38_1}", "{secVal39_1}",
    "{secVal40_1}", "{secVal41_1}", "{secVal42_1}", "{secVal43_1}", "{secVal44_1}",
    "{secVal45_1}", "{secVal46_1}", "{secVal47_1}", "{secVal48_1}", "{secVal49_1}",
    "{secVal50_1}", "{secVal51_1}", "{secVal52_1}", "{secVal53_1}", "{secVal54_1}",
    "{secVal55_1}", "{secVal56_1}", "{secVal57_1}", "{secVal58_1}", "{secVal59_1}"
  ];

  var seconds2 = [
      "{secVal0_2}", "{secVal1_2}", "{secVal2_2}", "{secVal3_2}", "{secVal4_2}",
      "{secVal5_2}", "{secVal6_2}", "{secVal7_2}", "{secVal8_2}", "{secVal9_2}",
      "{secVal10_2}", "{secVal11_2}", "{secVal12_2}", "{secVal13_2}", "{secVal14_2}",
      "{secVal15_2}", "{secVal16_2}", "{secVal17_2}", "{secVal18_2}", "{secVal19_2}",
      "{secVal20_2}", "{secVal21_2}", "{secVal22_2}", "{secVal23_2}", "{secVal24_2}",
      "{secVal25_2}", "{secVal26_2}", "{secVal27_2}", "{secVal28_2}", "{secVal29_2}",
      "{secVal30_2}", "{secVal31_2}", "{secVal32_2}", "{secVal33_2}", "{secVal34_2}",
      "{secVal35_2}", "{secVal36_2}", "{secVal37_2}", "{secVal38_2}", "{secVal39_2}",
      "{secVal40_2}", "{secVal41_2}", "{secVal42_2}", "{secVal43_2}", "{secVal44_2}",
      "{secVal45_2}", "{secVal46_2}", "{secVal47_2}", "{secVal48_2}", "{secVal49_2}",
      "{secVal50_2}", "{secVal51_2}", "{secVal52_2}", "{secVal53_2}", "{secVal54_2}",
      "{secVal55_2}", "{secVal56_2}", "{secVal57_2}", "{secVal58_2}", "{secVal59_2}"
  ];

  var seconds3 = [
    "{secVal0_3}", "{secVal1_3}", "{secVal2_3}", "{secVal3_3}", "{secVal4_3}",
    "{secVal5_3}", "{secVal6_3}", "{secVal7_3}", "{secVal8_3}", "{secVal9_3}",
    "{secVal10_3}", "{secVal11_3}", "{secVal12_3}", "{secVal13_3}", "{secVal14_3}",
    "{secVal15_3}", "{secVal16_3}", "{secVal17_3}", "{secVal18_3}", "{secVal19_3}",
    "{secVal20_3}", "{secVal21_3}", "{secVal22_3}", "{secVal23_3}", "{secVal24_3}",
    "{secVal25_3}", "{secVal26_3}", "{secVal27_3}", "{secVal28_3}", "{secVal29_3}",
    "{secVal30_3}", "{secVal31_3}", "{secVal32_3}", "{secVal33_3}", "{secVal34_3}",
    "{secVal35_3}", "{secVal36_3}", "{secVal37_3}", "{secVal38_3}", "{secVal39_3}",
    "{secVal40_3}", "{secVal41_3}", "{secVal42_3}", "{secVal43_3}", "{secVal44_3}",
    "{secVal45_3}", "{secVal46_3}", "{secVal47_3}", "{secVal48_3}", "{secVal49_3}",
    "{secVal50_3}", "{secVal51_3}", "{secVal52_3}", "{secVal53_3}", "{secVal54_3}",
    "{secVal55_3}", "{secVal56_3}", "{secVal57_3}", "{secVal58_3}", "{secVal59_3}"
  ];

  var seconds4 = [
    "{secVal0_4}", "{secVal1_4}", "{secVal2_4}", "{secVal3_4}", "{secVal4_4}",
    "{secVal5_4}", "{secVal6_4}", "{secVal7_4}", "{secVal8_4}", "{secVal9_4}",
    "{secVal10_4}", "{secVal11_4}", "{secVal12_4}", "{secVal13_4}", "{secVal14_4}",
    "{secVal15_4}", "{secVal16_4}", "{secVal17_4}", "{secVal18_4}", "{secVal19_4}",
    "{secVal20_4}", "{secVal21_4}", "{secVal22_4}", "{secVal23_4}", "{secVal24_4}",
    "{secVal25_4}", "{secVal26_4}", "{secVal27_4}", "{secVal28_4}", "{secVal29_4}",
    "{secVal30_4}", "{secVal31_4}", "{secVal32_4}", "{secVal33_4}", "{secVal34_4}",
    "{secVal35_4}", "{secVal36_4}", "{secVal37_4}", "{secVal38_4}", "{secVal39_4}",
    "{secVal40_4}", "{secVal41_4}", "{secVal42_4}", "{secVal43_4}", "{secVal44_4}",
    "{secVal45_4}", "{secVal46_4}", "{secVal47_4}", "{secVal48_4}", "{secVal49_4}",
    "{secVal50_4}", "{secVal51_4}", "{secVal52_4}", "{secVal53_4}", "{secVal54_4}",
    "{secVal55_4}", "{secVal56_4}", "{secVal57_4}", "{secVal58_4}", "{secVal59_4}"
  ];

  var minutes1 = [
    "{minVal0_1}", "{minVal1_1}", "{minVal2_1}", "{minVal3_1}", "{minVal4_1}",
    "{minVal5_1}", "{minVal6_1}", "{minVal7_1}", "{minVal8_1}", "{minVal9_1}",
    "{minVal10_1}", "{minVal11_1}", "{minVal12_1}", "{minVal13_1}", "{minVal14_1}",
    "{minVal15_1}", "{minVal16_1}", "{minVal17_1}", "{minVal18_1}", "{minVal19_1}",
    "{minVal20_1}", "{minVal21_1}", "{minVal22_1}", "{minVal23_1}", "{minVal24_1}",
    "{minVal25_1}", "{minVal26_1}", "{minVal27_1}", "{minVal28_1}", "{minVal29_1}",
    "{minVal30_1}", "{minVal31_1}", "{minVal32_1}", "{minVal33_1}", "{minVal34_1}",
    "{minVal35_1}", "{minVal36_1}", "{minVal37_1}", "{minVal38_1}", "{minVal39_1}",
    "{minVal40_1}", "{minVal41_1}", "{minVal42_1}", "{minVal43_1}", "{minVal44_1}",
    "{minVal45_1}", "{minVal46_1}", "{minVal47_1}", "{minVal48_1}", "{minVal49_1}",
    "{minVal50_1}", "{minVal51_1}", "{minVal52_1}", "{minVal53_1}", "{minVal54_1}",
    "{minVal55_1}", "{minVal56_1}", "{minVal57_1}", "{minVal58_1}", "{minVal59_1}"
  ];

  var minutes2 = [
    "{minVal0_2}", "{minVal1_2}", "{minVal2_2}", "{minVal3_2}", "{minVal4_2}",
    "{minVal5_2}", "{minVal6_2}", "{minVal7_2}", "{minVal8_2}", "{minVal9_2}",
    "{minVal10_2}", "{minVal11_2}", "{minVal12_2}", "{minVal13_2}", "{minVal14_2}",
    "{minVal15_2}", "{minVal16_2}", "{minVal17_2}", "{minVal18_2}", "{minVal19_2}",
    "{minVal20_2}", "{minVal21_2}", "{minVal22_2}", "{minVal23_2}", "{minVal24_2}",
    "{minVal25_2}", "{minVal26_2}", "{minVal27_2}", "{minVal28_2}", "{minVal29_2}",
    "{minVal30_2}", "{minVal31_2}", "{minVal32_2}", "{minVal33_2}", "{minVal34_2}",
    "{minVal35_2}", "{minVal36_2}", "{minVal37_2}", "{minVal38_2}", "{minVal39_2}",
    "{minVal40_2}", "{minVal41_2}", "{minVal42_2}", "{minVal43_2}", "{minVal44_2}",
    "{minVal45_2}", "{minVal46_2}", "{minVal47_2}", "{minVal48_2}", "{minVal49_2}",
    "{minVal50_2}", "{minVal51_2}", "{minVal52_2}", "{minVal53_2}", "{minVal54_2}",
    "{minVal55_2}", "{minVal56_2}", "{minVal57_2}", "{minVal58_2}", "{minVal59_2}"
  ];

  var minutes3 = [
    "{minVal0_3}", "{minVal1_3}", "{minVal2_3}", "{minVal3_3}", "{minVal4_3}",
    "{minVal5_3}", "{minVal6_3}", "{minVal7_3}", "{minVal8_3}", "{minVal9_3}",
    "{minVal10_3}", "{minVal11_3}", "{minVal12_3}", "{minVal13_3}", "{minVal14_3}",
    "{minVal15_3}", "{minVal16_3}", "{minVal17_3}", "{minVal18_3}", "{minVal19_3}",
    "{minVal20_3}", "{minVal21_3}", "{minVal22_3}", "{minVal23_3}", "{minVal24_3}",
    "{minVal25_3}", "{minVal26_3}", "{minVal27_3}", "{minVal28_3}", "{minVal29_3}",
    "{minVal30_3}", "{minVal31_3}", "{minVal32_3}", "{minVal33_3}", "{minVal34_3}",
    "{minVal35_3}", "{minVal36_3}", "{minVal37_3}", "{minVal38_3}", "{minVal39_3}",
    "{minVal40_3}", "{minVal41_3}", "{minVal42_3}", "{minVal43_3}", "{minVal44_3}",
    "{minVal45_3}", "{minVal46_3}", "{minVal47_3}", "{minVal48_3}", "{minVal49_3}",
    "{minVal50_3}", "{minVal51_3}", "{minVal52_3}", "{minVal53_3}", "{minVal54_3}",
    "{minVal55_3}", "{minVal56_3}", "{minVal57_3}", "{minVal58_3}", "{minVal59_3}"
  ];

  var minutes4 = [
    "{minVal0_4}", "{minVal1_4}", "{minVal2_4}", "{minVal3_4}", "{minVal4_4}",
    "{minVal5_4}", "{minVal6_4}", "{minVal7_4}", "{minVal8_4}", "{minVal9_4}",
    "{minVal10_4}", "{minVal11_4}", "{minVal12_4}", "{minVal13_4}", "{minVal14_4}",
    "{minVal15_4}", "{minVal16_4}", "{minVal17_4}", "{minVal18_4}", "{minVal19_4}",
    "{minVal20_4}", "{minVal21_4}", "{minVal22_4}", "{minVal23_4}", "{minVal24_4}",
    "{minVal25_4}", "{minVal26_4}", "{minVal27_4}", "{minVal28_4}", "{minVal29_4}",
    "{minVal30_4}", "{minVal31_4}", "{minVal32_4}", "{minVal33_4}", "{minVal34_4}",
    "{minVal35_4}", "{minVal36_4}", "{minVal37_4}", "{minVal38_4}", "{minVal39_4}",
    "{minVal40_4}", "{minVal41_4}", "{minVal42_4}", "{minVal43_4}", "{minVal44_4}",
    "{minVal45_4}", "{minVal46_4}", "{minVal47_4}", "{minVal48_4}", "{minVal49_4}",
    "{minVal50_4}", "{minVal51_4}", "{minVal52_4}", "{minVal53_4}", "{minVal54_4}",
    "{minVal55_4}", "{minVal56_4}", "{minVal57_4}", "{minVal58_4}", "{minVal59_4}"
  ];

  var hours1 = [
    "{hourVal0_1}", "{hourVal1_1}", "{hourVal2_1}", "{hourVal3_1}", "{hourVal4_1}",
    "{hourVal5_1}", "{hourVal6_1}", "{hourVal7_1}", "{hourVal8_1}", "{hourVal9_1}",
    "{hourVal10_1}", "{hourVal11_1}", "{hourVal12_1}", "{hourVal13_1}", "{hourVal14_1}",
    "{hourVal15_1}", "{hourVal16_1}", "{hourVal17_1}", "{hourVal18_1}", "{hourVal19_1}",
    "{hourVal20_1}", "{hourVal21_1}", "{hourVal22_1}", "{hourVal23_1}"
  ];

  var hours2 = [
    "{hourVal0_2}", "{hourVal1_2}", "{hourVal2_2}", "{hourVal3_2}", "{hourVal4_2}",
    "{hourVal5_2}", "{hourVal6_2}", "{hourVal7_2}", "{hourVal8_2}", "{hourVal9_2}",
    "{hourVal10_2}", "{hourVal11_2}", "{hourVal12_2}", "{hourVal13_2}", "{hourVal14_2}",
    "{hourVal15_2}", "{hourVal16_2}", "{hourVal17_2}", "{hourVal18_2}", "{hourVal19_2}",
    "{hourVal20_2}", "{hourVal21_2}", "{hourVal22_2}", "{hourVal23_2}"
  ];

  var hours3 = [
    "{hourVal0_3}", "{hourVal1_3}", "{hourVal2_3}", "{hourVal3_3}", "{hourVal4_3}",
    "{hourVal5_3}", "{hourVal6_3}", "{hourVal7_3}", "{hourVal8_3}", "{hourVal9_3}",
    "{hourVal10_3}", "{hourVal11_3}", "{hourVal12_3}", "{hourVal13_3}", "{hourVal14_3}",
    "{hourVal15_3}", "{hourVal16_3}", "{hourVal17_3}", "{hourVal18_3}", "{hourVal19_3}",
    "{hourVal20_3}", "{hourVal21_3}", "{hourVal22_3}", "{hourVal23_3}"
  ];

  var hours4 = [
    "{hourVal0_4}", "{hourVal1_4}", "{hourVal2_4}", "{hourVal3_4}", "{hourVal4_4}",
    "{hourVal5_4}", "{hourVal6_4}", "{hourVal7_4}", "{hourVal8_4}", "{hourVal9_4}",
    "{hourVal10_4}", "{hourVal11_4}", "{hourVal12_4}", "{hourVal13_4}", "{hourVal14_4}",
    "{hourVal15_4}", "{hourVal16_4}", "{hourVal17_4}", "{hourVal18_4}", "{hourVal19_4}",
    "{hourVal20_4}", "{hourVal21_4}", "{hourVal22_4}", "{hourVal23_4}"
  ];

  var displayTime1 = "seconds1";
  var displayTime2 = "seconds2";
  var displayTime3 = "seconds3";
  var displayTime4 = "seconds4";

function setMinuteValues(idx) {
  window["displayTime" + idx] = "seconds" + idx;
  updateButtonStyles("btnMinute" + idx, idx);
  drawBackGroung(idx);
  drawReporterChart(window["seconds" + idx], window["typeControl" + idx], idx);
}

function setHourValues(idx) {
  window["displayTime" + idx] = "minutes" + idx;
  updateButtonStyles("btnHour" + idx, idx);
  drawBackGroung(idx);
  drawReporterChart(window["minutes" + idx], window["typeControl" + idx], idx);
}

function setDayValues(idx) {
  window["displayTime" + idx] = "hours" + idx;
  updateButtonStyles("btnDay" + idx, idx);
  drawBackGroung(idx);
  drawReporterChart(window["hours" + idx], window["typeControl" + idx], idx);
}

function resetBuffers(idx) {
  if (idx === 1) {
    for (let i = 0; i < 60; i++) {
      seconds1[i] = 0;
      minutes1[i] = 0;
    }
    for (let i = 0; i < 24; i++) {
      hours1[i] = 0;
    }

  } else if (idx === 2) {
    for (let i = 0; i < 60; i++) {
      seconds2[i] = 0;
      minutes2[i] = 0;
    }
    for (let i = 0; i < 24; i++) {
      hours2[i] = 0;
    }

  } else if (idx === 3) {
    for (let i = 0; i < 60; i++) {
      seconds3[i] = 0;
      minutes3[i] = 0;
    }
    for (let i = 0; i < 24; i++) {
      hours3[i] = 0;
    }

  } else if (idx === 4) {
    for (let i = 0; i < 60; i++) {
      seconds4[i] = 0;
      minutes4[i] = 0;
    }
    for (let i = 0; i < 24; i++) {
      hours4[i] = 0;
    }

  } else {
    console.warn("resetBuffers: indice non valido:", idx);
  }
}

function updateButtonStyles(activeButtonId, idx) {
  const buttons = [
    'btnMinute' + idx,
    'btnHour'   + idx,
    'btnDay'    + idx
  ];

  buttons.forEach(buttonId => {
    const button = document.getElementById(buttonId);
    if (!button) return;

    if (buttonId === activeButtonId) {
      button.style.backgroundColor = 'blue';   // attivo
      button.style.color = 'white';
    } else {
      button.style.backgroundColor = 'lightgray'; // inattivo
      button.style.color = 'black';
    }
  });
}

function updateLegend(idx) {

  let legend;
  let typeCtrl;

  if (idx === 1) {
    legend = document.getElementById('legend1');
    typeCtrl = typeControl1;
  } else if (idx === 2) {
    legend = document.getElementById('legend2');
    typeCtrl = typeControl2;
  } else if (idx === 3) {
    legend = document.getElementById('legend3');
    typeCtrl = typeControl3;
  } else if (idx === 4) {
    legend = document.getElementById('legend4');
    typeCtrl = typeControl4;
  } else {
    console.warn("updateLegend: indice non valido:", idx);
    return;
  }

  if (!legend) return;

  switch (typeCtrl) {
  case "ph":
    legend.innerHTML = `<div><span style="color:#007BFF; font-weight:700;">💧 pH</span></div>`;
    break;
  case "orp":
    legend.innerHTML = `<div><span style="color:#28a745; font-weight:700;">⚗️ ORP (mV)</span></div>`;
    break;
  case "ec":
    legend.innerHTML = `<div><span style="color:#FFA500; font-weight:700;">⚡ EC (µS/cm)</span></div>`;
    break;
  case "tds":
    legend.innerHTML = `<div><span style="color:#A0522D; font-weight:700;">💎 TDS (ppm)</span></div>`;
    break;
  case "sal":
    legend.innerHTML = `<div><span style="color:#6C757D; font-weight:700;">🧂 SAL (ppm)</span></div>`;
    break;
  case "ntc":
    legend.innerHTML = `<div><span style="color:#dc3545; font-weight:700;">🌡️ Temp (°C)</span></div>`;
    break;
  case "diff":
    legend.innerHTML = `<div><span style="color:#dc3545; font-weight:700;">🌡️ Temp (°C)</span></div>`;
    break;
  case "avg":
    legend.innerHTML = `<div><span style="color:#dc3545; font-weight:700;">🌡️ Temp (°C)</span></div>`;
    break;
  case "ds18":
    legend.innerHTML = `<div><span style="color:#dc3545; font-weight:700;">🌡️ Temp (°C)</span></div>`;
    break;
    default:
      legend.innerHTML = `<div><span style="color:#FFA500; font-weight:700;">⚡ EC</span></div>`;
    break;
  }
  
}

// the area where the drwaing will happen is limited by those 2 points origin and end
const OFFSET_ORIGIN_X = 20; 
const OFFSET_ORIGIN_Y = 20; 

const OFFSET_END_X = 20; 
const OFFSET_END_Y = 20; 

const GRID_TICK = 3;
const GRID_TICK_AXIS = 6;
const XY_AXIS_LABEL_OFFSET = 15;

function drawBackGroung(idx){

  let cv;
  let typeCtrl;
  let displayTime;

  if (idx === 1) {
    cv = document.getElementById('ReporterChartBG1');
    typeCtrl    = (typeControl1 || "").toLowerCase();
    displayTime = displayTime1;
  } else if (idx === 2) {
    cv = document.getElementById('ReporterChartBG2');
    typeCtrl    = (typeControl2 || "").toLowerCase();
    displayTime = displayTime2;
  } else if (idx === 3) {
    cv = document.getElementById('ReporterChartBG3');
    typeCtrl    = (typeControl3 || "").toLowerCase();
    displayTime = displayTime3;
  } else if (idx === 4) {
    cv = document.getElementById('ReporterChartBG4');
    typeCtrl    = (typeControl4 || "").toLowerCase();
    displayTime = displayTime4;
  } else {
    console.warn('drawBackGroung: idx non valido:', idx);
    return;
  }

    if(!cv) return;
    var ctx = cv.getContext('2d');
    var W = cv.width, H = cv.height;
        
    var w_net = W - OFFSET_ORIGIN_X - OFFSET_END_X; 
    var h_net = H - OFFSET_ORIGIN_Y - OFFSET_END_Y; 

    var w_zero = W - OFFSET_END_X, h_zero = H - OFFSET_END_Y;

    ctx.clearRect(0,0,W,H); //clear all 

    // assi.... draw axes
    ctx.strokeStyle = '#000';
    ctx.lineWidth = 2;
    ctx.beginPath();

    ctx.moveTo(OFFSET_ORIGIN_X, OFFSET_ORIGIN_Y);
    ctx.lineTo(OFFSET_ORIGIN_X, H - OFFSET_END_Y + GRID_TICK_AXIS);
    ctx.moveTo(OFFSET_ORIGIN_X - GRID_TICK_AXIS, H - OFFSET_END_Y);
    ctx.lineTo(W - OFFSET_END_X, H - OFFSET_END_Y);
    ctx.stroke();

    // draw grid vertical
    var numLines; 

  switch (displayTime) {
    case "seconds" + idx:
      numLines = 60;
      break;
    case "minutes" + idx:
      numLines = 60;
      break;
    case "hours" + idx:
      numLines = 24;
      break;
    default:
      numLines = 60;
      break;
  }

    var spacing = w_net / (numLines); 

    ctx.strokeStyle = '#eee'; 
    ctx.lineWidth = 1; 

    for (var i = 0; i < numLines; i++) {
        var x = OFFSET_ORIGIN_X + spacing * (i); 
        ctx.beginPath();
        ctx.moveTo(x, OFFSET_ORIGIN_Y); 
        ctx.lineTo(x, H - OFFSET_END_Y + GRID_TICK);
        ctx.stroke();

        // Labels
        ctx.fillStyle = '#000';
        ctx.font = '10px Arial';
        var labelValue = i; // Calculate label value
        
      if (displayTime === "seconds" + idx || displayTime === "minutes" + idx) {
        if ((i % 5 === 0) || (i === 59)) { // Ogni 5 step o ultimo
          ctx.fillText(labelValue.toString(), x - 5, H - OFFSET_END_Y + XY_AXIS_LABEL_OFFSET);
        }
      } else if (displayTime === "hours" + idx) {
        ctx.fillText(labelValue.toString(), x - 5, H - OFFSET_END_Y + XY_AXIS_LABEL_OFFSET);
      }
    }

    // draw grid horizontal
    var numLinesHor; 
    var spacing;
    var minValue, maxValue;
    var interval;

    switch (typeCtrl) {
    case "ec":
      numLinesHor = 20;
      minValue = 10;
      maxValue = 25000;   // µS/cm
      interval = maxValue / numLinesHor;
      break;

    case "tds":
      numLinesHor = 20;
      minValue = 10;
      maxValue = 16000;   // ppm circa (25000 * 0.64)
      interval = maxValue / numLinesHor;
      break;

    case "sal":
      numLinesHor = 20;
      minValue = 10;
      maxValue = 14000;   // ppm circa (25000 * 0.55)
      interval = maxValue / numLinesHor;
      break;

      case "ntc":
        minValue = 0;
        maxValue = 70;
        interval = 5;
        numLinesHor = Math.floor((maxValue - minValue) / interval);
        break;

      case "diff":
        minValue = 0;
        maxValue = 120;
        interval = 5;
        numLinesHor = Math.floor((maxValue - minValue) / interval);
        break;

      case "avg":
        minValue = 0;
        maxValue = 70;
        interval = 5;
        numLinesHor = Math.floor((maxValue - minValue) / interval);
        break;

      case "ds18":
        minValue = 0;
        maxValue = 70;
        interval = 5;
        numLinesHor = Math.floor((maxValue - minValue) / interval);
        break;

      case "ph":
        minValue = 0;
        maxValue = 14;
        interval = 1;
        numLinesHor = Math.floor((maxValue - minValue) / interval);
        break;

      case "orp":
        minValue = -2000;
        maxValue = 2000;
        interval = 200;
        numLinesHor = Math.floor((maxValue - minValue) / interval);
        break;

      default:
        // Default fallback (EC-like)
        numLinesHor = 10;
        minValue = 10;
        maxValue = 20000;
        interval = maxValue / numLinesHor;
        break;
    }

    spacing = h_net / numLinesHor; 

    ctx.strokeStyle = '#eee'; 
    ctx.lineWidth = 1; 

    for (var i = 0; i <= numLinesHor; i++) {
        var y = OFFSET_ORIGIN_Y + spacing * (i); 
        ctx.beginPath();
        ctx.moveTo(OFFSET_ORIGIN_X - GRID_TICK, y); 
        ctx.lineTo(W - OFFSET_END_X, y);
        ctx.stroke();

        // Labels
        ctx.fillStyle = '#000';
        ctx.font = '10px Arial';

        var labelValue = maxValue - (interval * i);
        ctx.fillText(labelValue.toString(), OFFSET_ORIGIN_X - XY_AXIS_LABEL_OFFSET, y); // Adjust position as needed

    }

}

function drawReporterChart(data, typeCtrl, idx){

  let cv;
  let displayTime;

      if (idx === 1) {
        cv = document.getElementById('ReporterChart1');
         displayTime = displayTime1;
      } else if (idx === 2) {
        cv = document.getElementById('ReporterChart2');
         displayTime = displayTime2;
      } else if (idx === 3) {
       cv = document.getElementById('ReporterChart3');
        displayTime = displayTime3;
      } else if (idx === 4) {
        cv = document.getElementById('ReporterChart4');
         displayTime = displayTime4;
      } 

  if(!cv) return;

    var ctx = cv.getContext('2d');
    var W = cv.width, H = cv.height;
        
    var w_net = W - OFFSET_ORIGIN_X - OFFSET_END_X; 
    var h_net = H - OFFSET_ORIGIN_Y - OFFSET_END_Y; 

    var w_zero = W - OFFSET_END_X, h_zero = H - OFFSET_END_Y;

    ctx.clearRect(0,0,W,H); //clear

    var minValue, maxValue, interval;

    var lineColor = '#000';
    switch (typeCtrl) {
      case 'ec':  
        lineColor = '#FFA500'; 
        minValue = 0; 
        maxValue = 25000; // Modifica in base al tuo contesto
        interval = maxValue - minValue;
      break;
      case 'tds': 
        lineColor = '#A0522D'; 
        minValue = 0; 
        maxValue = 16000; // Modifica in base al tuo contesto
        interval = maxValue - minValue;
      break;
      case 'sal': 
        lineColor = '#6C757D'; 
        minValue = 0; 
        maxValue = 14000; // Modifica in base al tuo contesto
        interval = maxValue - minValue;
      break;
      case 'ntc': 
        lineColor = '#dc3545'; 
        minValue = 0; 
        maxValue = 700; // Modifica in base al tuo contesto
        interval = maxValue - minValue;
      break;
      case 'diff': 
        lineColor = '#dc3545'; 
        minValue = 0; 
        maxValue = 1000; // Modifica in base al tuo contesto
        interval = maxValue - minValue;
      break;
      case 'avg': 
        lineColor = '#dc3545'; 
        minValue = 0; 
        maxValue = 700; // Modifica in base al tuo contesto
        interval = maxValue - minValue;
      break;
      case 'ds18': 
        lineColor = '#dc3545'; 
        minValue = 0; 
        maxValue = 700; // Modifica in base al tuo contesto
        interval = maxValue - minValue;
      break;
      case 'ph': 
          lineColor = '#007BFF'; // Colore per h
          minValue = 0; 
          maxValue = 1400; // Modifica in base al tuo contesto
          interval = maxValue - minValue;
          break;
      case 'orp': 
          lineColor = '#28A745'; // Colore per rx
          minValue = -2000; 
          maxValue = 2000; // Modifica in base al tuo contesto
          interval = maxValue - minValue;
          break;
    }

    let interval_time;

    switch (displayTime) {
      case "seconds" + idx:
        interval_time = 60; // 60 seconds
        break;

      case "minutes" + idx:
        interval_time = 60; // 60 minutes
        break;

      case "hours" + idx:
        interval_time = 24; // 24 hours
        break;

      default:
        interval_time = 60; // default fallback
        break;
    }

    const unitToPixel = h_net / interval;
    const unitTimeToPixel = w_net / interval_time;

    ctx.strokeStyle = lineColor;
    ctx.lineWidth = 2;
    ctx.beginPath();

    for( var i=0; i<(data.length); i++){
      var y1 = H - OFFSET_END_Y - (data[i] - minValue) * unitToPixel;
      var x1 = OFFSET_ORIGIN_X + (i * unitTimeToPixel);
      if(i===0) ctx.moveTo(x1,y1); else ctx.lineTo(x1,y1);
    }
    ctx.stroke();
  }

function performReporterAction(idx) {

  let isActive, interval, confirmMsg, actionText;

  if (idx === 1) {
    isActive = (document.getElementById('ReporterAction1').value === 'ACTIVE') ? '1' : '0';
    interval = document.getElementById('ReporterInterval1').value;
    actionText = (isActive === '1') ? 'activate' : 'deactivate';
    confirmMsg = `Press ok to ${actionText.toUpperCase()} Reporter 1 with selected period?`;
  }

  else if (idx === 2) {
    isActive = (document.getElementById('ReporterAction2').value === 'ACTIVE') ? '1' : '0';
    interval = document.getElementById('ReporterInterval2').value;
    actionText = (isActive === '1') ? 'activate' : 'deactivate';
    confirmMsg = `Press ok to to ${actionText.toUpperCase()} Reporter 2 with selected period?`;
  }

  else if (idx === 3) {
    isActive = (document.getElementById('ReporterAction3').value === 'ACTIVE') ? '1' : '0';
    interval = document.getElementById('ReporterInterval3').value;
    actionText = (isActive === '1') ? 'activate' : 'deactivate';
    confirmMsg = `Press ok to to ${actionText.toUpperCase()} Reporter 3 with selected period?`;
  }

  else if (idx === 4) {
    isActive = (document.getElementById('ReporterAction4').value === 'ACTIVE') ? '1' : '0';
    interval = document.getElementById('ReporterInterval4').value;
    actionText = (isActive === '1') ? 'activate' : 'deactivate';
    confirmMsg = `Press ok to to ${actionText.toUpperCase()} Reporter 4 with selected period?`;
  }

  else {
    console.warn(`⚠️ Invalid reporter index: ${idx}`);
    return;
  }

  if (!confirm(confirmMsg)) {
    console.log(`Reporter ${idx} action canceled by user.`);
    return;
  }

  fetch('/setreporter', {
    method: 'POST',
    body: new URLSearchParams({
      'reporter': idx,
      'active': isActive,
      'period': interval
    })
  })
  .then(r => r.text())
  .then(_ => {
    console.log(`Reporter ${idx} successfully ${isActive === '1' ? 'activated' : 'deactivated'}.`);
    updateStatusReporter();
  })
  .catch(err => {
    console.error(`Reporter ${idx} POST action error:`, err);
  });
}

  function updateStatusReporter() {
    fetch('/wifireporterGetStatus')
      .then(r => r.json())
      .then(data => {

        // ===== CONTROLLO 1 =====
        document.getElementById('ReporterInlineRow1_1').textContent = data.status1row1 || "—";
        document.getElementById('ReporterInlineRow2_1').textContent = data.status1row2 || "—";
        typeControl1 = data.type1;

        seconds1[data.last_second_index1] = data.last_second_value1;
        minutes1[data.last_minute_index1] = data.last_minute_value1;
        hours1  [data.last_hour_index1]   = data.last_hour_value1;

        if (displayTime1 === "seconds1") {
          drawReporterChart(seconds1, typeControl1, 1);
        } else if (displayTime1 === "minutes1") {
          drawReporterChart(minutes1, typeControl1, 1);
        } else if (displayTime1 === "hours1") {
          drawReporterChart(hours1,   typeControl1, 1);
        }

        if (oldType1 !== typeControl1) {
          updateLegend(1);
          resetBuffers(1);
          drawBackGroung(1);
          oldType1 = typeControl1;
        }

        // ===== CONTROLLO 2 =====
        document.getElementById('ReporterInlineRow1_2').textContent = data.status2row1 || "—";
        document.getElementById('ReporterInlineRow2_2').textContent = data.status2row2 || "—";
        typeControl2 = data.type2;

        seconds2[data.last_second_index2] = data.last_second_value2;
        minutes2[data.last_minute_index2] = data.last_minute_value2;
        hours2  [data.last_hour_index2]   = data.last_hour_value2;

        if (displayTime2 === "seconds2") {
          drawReporterChart(seconds2, typeControl2, 2);
        } else if (displayTime2 === "minutes2") {
          drawReporterChart(minutes2, typeControl2, 2);
        } else if (displayTime2 === "hours2") {
          drawReporterChart(hours2,   typeControl2, 2);
        }

        if (oldType2 !== typeControl2) {
          updateLegend(2);
          resetBuffers(2);
          drawBackGroung(2);
          oldType2 = typeControl2;
        }

        // ===== CONTROLLO 3 =====
        document.getElementById('ReporterInlineRow1_3').textContent = data.status3row1 || "—";
        document.getElementById('ReporterInlineRow2_3').textContent = data.status3row2 || "—";
        typeControl3 = data.type3;

        seconds3[data.last_second_index3] = data.last_second_value3;
        minutes3[data.last_minute_index3] = data.last_minute_value3;
        hours3  [data.last_hour_index3]   = data.last_hour_value3;

        if (displayTime3 === "seconds3") {
          drawReporterChart(seconds3, typeControl3, 3);
        } else if (displayTime3 === "minutes3") {
          drawReporterChart(minutes3, typeControl3, 3);
        } else if (displayTime3 === "hours3") {
          drawReporterChart(hours3,   typeControl3, 3);
        }

        if (oldType3 !== typeControl3) {
          updateLegend(3);
          resetBuffers(3);
          drawBackGroung(3);
          oldType3 = typeControl3;
        }

        // ===== CONTROLLO 4 =====
        document.getElementById('ReporterInlineRow1_4').textContent = data.status4row1 || "—";
        document.getElementById('ReporterInlineRow2_4').textContent = data.status4row2 || "—";
        typeControl4 = data.type4;

        seconds4[data.last_second_index4] = data.last_second_value4;
        minutes4[data.last_minute_index4] = data.last_minute_value4;
        hours4  [data.last_hour_index4]   = data.last_hour_value4;

        if (displayTime4 === "seconds4") {
          drawReporterChart(seconds4, typeControl4, 4);
        } else if (displayTime4 === "minutes4") {
          drawReporterChart(minutes4, typeControl4, 4);
        } else if (displayTime4 === "hours4") {
          drawReporterChart(hours4,   typeControl4, 4);
        }

        if (oldType4 !== typeControl4) {
          updateLegend(4);
          resetBuffers(4);
          drawBackGroung(4);
          oldType4 = typeControl4;
        }

      })
      .catch(err => {
        console.error('Errore nel recuperare lo stato:', err);
        drawReporterChart([], typeControl1, 1);
      });
  }

  window.addEventListener('load', function(){  
    updateButtonStyles('btnMinute1', 1);  
    updateLegend(1); 
    drawBackGroung(1);
    drawReporterChart(minutes1, typeControl1, 1);

    updateButtonStyles('btnMinute2', 2);  
    updateLegend(2); 
    drawBackGroung(2);
    drawReporterChart(minutes2, typeControl2, 2);

    updateButtonStyles('btnMinute3', 3);  
    updateLegend(3); 
    drawBackGroung(3);
    drawReporterChart(minutes3, typeControl3, 3);

    updateButtonStyles('btnMinute4', 4);  
    updateLegend(4); 
    drawBackGroung(4);
    drawReporterChart(minutes4, typeControl4, 4);

    updateStatusReporter();

    if (isAPMode) {
      alert("⚠️ System is in Access Point Mode. Mail notifications are not active.");
    }

    setInterval(updateStatusReporter, 1000);
  });

</script>
)rawliteral";

// ---- CLOSE PAGE
const char WSER_REPORTER_CLOSE_PAGE[] PROGMEM = R"rawliteral(
</body>
</html>
)rawliteral";

#endif // APP_WEBBROWSER_REPORTER_H