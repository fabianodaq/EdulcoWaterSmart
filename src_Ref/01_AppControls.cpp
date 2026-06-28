#include "01_AppControls.h"
#include "05_AppHW.h"

//--- Private Variables -------------------------------------------------------

/**
 * @brief Tempo dell'ultimo aggiornamento della pianificazione dei controlli.
 */
unsigned long CtrlSch_lastUpdate = 0;

/**
 * @brief Array di controlli per ciascun controllo possibile.
 */
uint8_t CtrlSch_Idx = 0;

//--- Public Variables --------------------------------------------------------
t_ctrl	s_Ctrl[NUMER_OF_CONTROLS];	// controls for each possible control are defined

t_timer PumpTimers[NUM_PUMPS];  // ✅ DICHIARAZIONE

//--- Private Function Declarations -------------------------------------------
void Ctrl_HandleControl(unsigned char CtrlIdx);
void Ctrl_HandleThresholdControl(uint8_t CtrlIdx);

void PumpTimer_Handle(uint8_t idx);

//--- Public Function Declarations -------------------------------------------
void Ctrl_HanldeControls();
void Ctrl_InitControls();
String WSER_ExtCmdGetStatus(uint8_t ctrlIndex);

//-----------------------------------------------------------------------------
//--- PUBLIC Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
void Ctrl_InitControls(){

    unsigned char idx;

    uint8_t FlagEErpmCtrl     = EEPR_ReadByte(EEPROM_INIT_FLAG_CTRL_ADDR);
    uint8_t FlagEErpmCtrlNeg  = EEPR_ReadByte(EEPROM_INIT_FLAG_CTRL_NEG_ADDR); 

    if ((FlagEErpmCtrl != EEPROM_INIT_CTRL_FLAG ) || (FlagEErpmCtrlNeg != EEPROM_INIT_FLAG_CTRL_NEG)){

        EEPR_WriteByte(CTRL_1_TYPE      ,_PH    ); //---init sens 1----
        EEPR_WriteByte(CTRL_1_ACTIVE    ,true   );
        EEPR_WriteByte(CTRL_1_NUM       ,0      );
        EEPR_WriteByte(CTRL_1_LOGIK     ,true   );

        EEPR_WriteInt16 (CTRL_1_CLOCK   , 0     );
        EEPR_WriteInt16 (CTRL_1_SET_PT  , 700   );
        EEPR_WriteInt16 (CTRL_1_HYSTER  , 100   );

        EEPR_WriteInt16 (SENS_1_CAL_X1, 709);
        EEPR_WriteInt16 (SENS_1_CAL_Y1, 100);
        EEPR_WriteInt16 (SENS_1_CAL_X2, -644);
        EEPR_WriteInt16 (SENS_1_CAL_Y2, 1245);

        EEPR_WriteByte(CTRL_2_TYPE      ,_DS18   ); //---init sens 2----
        EEPR_WriteByte(CTRL_2_ACTIVE    ,true  );
        EEPR_WriteByte(CTRL_2_NUM       ,1      );
        EEPR_WriteByte(CTRL_2_LOGIK     ,true  );

        EEPR_WriteInt16 (CTRL_2_CLOCK   , 20    );
        EEPR_WriteInt16 (CTRL_2_SET_PT  , 250   );
        EEPR_WriteInt16 (CTRL_2_HYSTER  , 50    );

        EEPR_WriteByte(CTRL_3_TYPE      ,_EC    ); //---init sens 3----
        EEPR_WriteByte(CTRL_3_ACTIVE    ,true  );
        EEPR_WriteByte(CTRL_3_NUM       ,2      );
        EEPR_WriteByte(CTRL_3_LOGIK     ,true  );

        EEPR_WriteInt16 (CTRL_3_CLOCK   , 30    );
        EEPR_WriteInt16 (CTRL_3_SET_PT  , 10000 );
        EEPR_WriteInt16 (CTRL_3_HYSTER  , 100   );

        EEPR_WriteUint16 (SENS_3_CAL_X1, 500);
        EEPR_WriteUint16 (SENS_3_CAL_Y1, 500);
        EEPR_WriteUint16 (SENS_3_CAL_X2, 2000);
        EEPR_WriteUint16 (SENS_3_CAL_Y2, 2000);

        EEPR_WriteByte(CTRL_4_TYPE      ,_RX    ); //---init sens 4----
        EEPR_WriteByte(CTRL_4_ACTIVE    ,true   );
        EEPR_WriteByte(CTRL_4_NUM       ,3      );
        EEPR_WriteByte(CTRL_4_LOGIK     ,true   );

        EEPR_WriteInt16 (CTRL_4_CLOCK   , 0     );
        EEPR_WriteInt16 (CTRL_4_SET_PT  , 390   );
        EEPR_WriteInt16 (CTRL_4_HYSTER  , 10    );

        EEPR_WriteInt16 (SENS_4_CAL_X1, -1798);
        EEPR_WriteInt16 (SENS_4_CAL_Y1, -1800);
        EEPR_WriteInt16 (SENS_4_CAL_X2, 1798);
        EEPR_WriteInt16 (SENS_4_CAL_Y2, 1800);

        EEPR_WriteByte(EEPROM_INIT_FLAG_CTRL_ADDR       ,EEPROM_INIT_CTRL_FLAG      ); //---- Init EEProm as initialized----
        EEPR_WriteByte(EEPROM_INIT_FLAG_CTRL_NEG_ADDR   ,EEPROM_INIT_FLAG_CTRL_NEG  );
    }
  
	s_Ctrl[0].type            = EEPR_ReadByte(CTRL_1_TYPE);     //initi control 0
	s_Ctrl[0].Active          = EEPR_ReadByte(CTRL_1_ACTIVE);	
	s_Ctrl[0].SensNum         = EEPR_ReadByte(CTRL_1_NUM);	
	s_Ctrl[0].CtrlSt          = false; 	//Control output set to zero
	s_Ctrl[0].ActReadVal      = 0;   //Cafter sensors		
	s_Ctrl[0].Clock           = EEPR_ReadInt16(CTRL_1_CLOCK);	
	s_Ctrl[0].Set_Point       = EEPR_ReadInt16(CTRL_1_SET_PT);				
	s_Ctrl[0].Set_PosLogik    = EEPR_ReadByte(CTRL_1_LOGIK);				
	s_Ctrl[0].Set_Hyster      = EEPR_ReadInt16(CTRL_1_HYSTER);	

    s_Ctrl[0].CalX1           =  EEPR_ReadInt16(SENS_1_CAL_X1);
    s_Ctrl[0].CalY1           =  EEPR_ReadInt16(SENS_1_CAL_Y1);
    s_Ctrl[0].CalX2           =  EEPR_ReadInt16(SENS_1_CAL_X2);
    s_Ctrl[0].CalY2           =  EEPR_ReadInt16(SENS_1_CAL_Y2);

    s_Ctrl[0].ssid            = "N/A";
    s_Ctrl[0].CtrlStPlug      = false;

	s_Ctrl[1].type            = EEPR_ReadByte(CTRL_2_TYPE);    //initi control 1
	s_Ctrl[1].Active          = EEPR_ReadByte(CTRL_2_ACTIVE);	
	s_Ctrl[1].SensNum         = EEPR_ReadByte(CTRL_2_NUM);	
	s_Ctrl[1].CtrlSt          = false; 	//Control output set to zero
	s_Ctrl[1].ActReadVal      = 0;   //Cafter sensors		
	s_Ctrl[1].Clock           = EEPR_ReadInt16(CTRL_2_CLOCK);	
	s_Ctrl[1].Set_Point       = EEPR_ReadInt16(CTRL_2_SET_PT);				
	s_Ctrl[1].Set_PosLogik    = EEPR_ReadByte(CTRL_2_LOGIK);				
	s_Ctrl[1].Set_Hyster      = EEPR_ReadInt16(CTRL_2_HYSTER);

    s_Ctrl[1].ssid            = "N/A";
    s_Ctrl[1].CtrlStPlug      = false;

	s_Ctrl[2].type            = EEPR_ReadByte(CTRL_3_TYPE);    //initi control 2
	s_Ctrl[2].Active          = EEPR_ReadByte(CTRL_3_ACTIVE);	
	s_Ctrl[2].SensNum         = EEPR_ReadByte(CTRL_3_NUM);	
	s_Ctrl[2].CtrlSt          = false; 	//Control output set to zero
	s_Ctrl[2].ActReadVal      = 0;   //Cafter sensors		
	s_Ctrl[2].Clock           = EEPR_ReadInt16(CTRL_3_CLOCK);	
	s_Ctrl[2].Set_Point       = EEPR_ReadInt16(CTRL_3_SET_PT);				
	s_Ctrl[2].Set_PosLogik    = EEPR_ReadByte(CTRL_3_LOGIK);				
	s_Ctrl[2].Set_Hyster      = EEPR_ReadInt16(CTRL_3_HYSTER);

    s_Ctrl[2].CalX1           =  EEPR_ReadInt16(SENS_3_CAL_X1);
    s_Ctrl[2].CalY1           =  EEPR_ReadInt16(SENS_3_CAL_Y1);
    s_Ctrl[2].CalX2           =  EEPR_ReadInt16(SENS_3_CAL_X2);
    s_Ctrl[2].CalY2           =  EEPR_ReadInt16(SENS_3_CAL_Y2);

    s_Ctrl[2].ssid            = "N/A";
    s_Ctrl[2].CtrlStPlug      = false;

	s_Ctrl[3].type            = EEPR_ReadByte(CTRL_4_TYPE);     //initi control 3
	s_Ctrl[3].Active          = EEPR_ReadByte(CTRL_4_ACTIVE);	
	s_Ctrl[3].SensNum         = EEPR_ReadByte(CTRL_4_NUM);	
	s_Ctrl[3].CtrlSt          = false; 	//Control output set to zero
	s_Ctrl[3].ActReadVal      = 0;   //Cafter sensors		
	s_Ctrl[3].Clock           = EEPR_ReadInt16(CTRL_4_CLOCK);	
	s_Ctrl[3].Set_Point       = EEPR_ReadInt16(CTRL_4_SET_PT);				
	s_Ctrl[3].Set_PosLogik    = EEPR_ReadByte(CTRL_4_LOGIK);				
	s_Ctrl[3].Set_Hyster      = EEPR_ReadInt16(CTRL_4_HYSTER);

    s_Ctrl[3].CalX1           =  EEPR_ReadInt16(SENS_4_CAL_X1);
    s_Ctrl[3].CalY1           =  EEPR_ReadInt16(SENS_4_CAL_Y1);
    s_Ctrl[3].CalX2           =  EEPR_ReadInt16(SENS_4_CAL_X2);
    s_Ctrl[3].CalY2           =  EEPR_ReadInt16(SENS_4_CAL_Y2);

    s_Ctrl[3].ssid            = "N/A";
    s_Ctrl[3].CtrlStPlug      = false;

    // timers initialization
    uint8_t FlagEEPROMPumpTimer;
    uint8_t FlagEEPROMPumpTimerNeg;

    // Lettura flags dalla EEPROM
    FlagEEPROMPumpTimer    = EEPR_ReadByte(EEPROM_INIT_FLAG_PUMP_TIMER_ADDR);
    FlagEEPROMPumpTimerNeg = EEPR_ReadByte(EEPROM_INIT_FLAG_PUMP_TIMER_NEG_ADDR);

    if ((FlagEEPROMPumpTimer != EEPROM_INIT_PUMP_TIMER_FLAG) || (FlagEEPROMPumpTimerNeg != EEPROM_INIT_PUMP_TIMER_NEG)) {
        
        EEPR_WriteByte(PUMP_1_TIMER_ACTIVE, false); // ---- Init Pump Timer 1 ----
        EEPR_WriteUint16(PUMP_1_TON_SECONDS_ADDR, 30);   // 00:30 sec
        EEPR_WriteUint16(PUMP_1_TOFF_SECONDS_ADDR, 10);  // 00:10 sec
        
        EEPR_WriteByte(PUMP_2_TIMER_ACTIVE, false); // ---- Init Pump Timer 2 ----
        EEPR_WriteUint16(PUMP_2_TON_SECONDS_ADDR, 30);   // 00:30 sec
        EEPR_WriteUint16(PUMP_2_TOFF_SECONDS_ADDR, 10);  // 00:10 sec
        
        EEPR_WriteByte(PUMP_3_TIMER_ACTIVE, false); // ---- Init Pump Timer 1 ----
        EEPR_WriteUint16(PUMP_3_TON_SECONDS_ADDR, 30);   // 00:30 sec
        EEPR_WriteUint16(PUMP_3_TOFF_SECONDS_ADDR, 10);  // 00:10 sec
        
        EEPR_WriteByte(PUMP_4_TIMER_ACTIVE, false); // ---- Init Pump Timer 2 ----
        EEPR_WriteUint16(PUMP_4_TON_SECONDS_ADDR, 30);   // 00:30 sec
        EEPR_WriteUint16(PUMP_4_TOFF_SECONDS_ADDR, 10);  // 00:10 sec
        
        EEPR_WriteByte(EEPROM_INIT_FLAG_PUMP_TIMER_ADDR    , EEPROM_INIT_PUMP_TIMER_FLAG); // Scrittura flags di inizializzazione
        EEPR_WriteByte(EEPROM_INIT_FLAG_PUMP_TIMER_NEG_ADDR, EEPROM_INIT_PUMP_TIMER_NEG);
    }

    PumpTimers[0].Active      = EEPR_ReadByte(PUMP_1_TIMER_ACTIVE); // Pump 1
    PumpTimers[0].OnSeconds   = EEPR_ReadUint16(PUMP_1_TON_SECONDS_ADDR);
    PumpTimers[0].OffSeconds  = EEPR_ReadUint16(PUMP_1_TOFF_SECONDS_ADDR);
    PumpTimers[0].OnPhase     = true;
    PumpTimers[0].ElapsedTime = 0;

    PumpTimers[1].Active      = EEPR_ReadByte(PUMP_2_TIMER_ACTIVE); // Pump 2
    PumpTimers[1].OnSeconds   = EEPR_ReadUint16(PUMP_2_TON_SECONDS_ADDR);
    PumpTimers[1].OffSeconds  = EEPR_ReadUint16(PUMP_2_TOFF_SECONDS_ADDR);
    PumpTimers[1].OnPhase     = true;
    PumpTimers[1].ElapsedTime = 0;

    PumpTimers[2].Active      = EEPR_ReadByte(PUMP_3_TIMER_ACTIVE); // Pump 3
    PumpTimers[2].OnSeconds   = EEPR_ReadUint16(PUMP_3_TON_SECONDS_ADDR);
    PumpTimers[2].OffSeconds  = EEPR_ReadUint16(PUMP_3_TOFF_SECONDS_ADDR);
    PumpTimers[2].OnPhase     = true;
    PumpTimers[2].ElapsedTime = 0;

    PumpTimers[3].Active      = EEPR_ReadByte(PUMP_4_TIMER_ACTIVE); // Pump 4
    PumpTimers[3].OnSeconds   = EEPR_ReadUint16(PUMP_4_TON_SECONDS_ADDR);
    PumpTimers[3].OffSeconds  = EEPR_ReadUint16(PUMP_4_TOFF_SECONDS_ADDR);
    PumpTimers[3].OnPhase     = true;
    PumpTimers[3].ElapsedTime = 0;

}

/**

 */
void Ctrl_HanldeControls(){
    unsigned long currentMillis = millis();
    if (currentMillis - CtrlSch_lastUpdate >= TIME_INTERVAL) {
        CtrlSch_lastUpdate = currentMillis;
        Ctrl_HandleControl(CtrlSch_Idx);

        CtrlSch_Idx++;
        if (CtrlSch_Idx > CONTROLS_MAX_IDX) {
          CtrlSch_Idx = 0; // Reset ciclo
        }

        for (uint8_t i = 0; i <= CONTROLS_MAX_IDX; i++) {
            PumpTimer_Handle(i);
        } 
    }
}

//-----------------------------------------------------------------------------
//--- Private Functions Definitions -------------------------------------------
//-----------------------------------------------------------------------------
/**

 */
 void PumpTimer_Handle(uint8_t idx)
{
    if (!PumpTimers[idx].Active) return;
    uint16_t timeSecOn  = PumpTimers[idx].OnSeconds;
    uint16_t timeSecOff = PumpTimers[idx].OffSeconds;
    PumpTimers[idx].ElapsedTime += TIME_INTERVAL_SEC;
    if (PumpTimers[idx].OnPhase == true) {
        // Fase ON
        if (PumpTimers[idx].ElapsedTime >= timeSecOn) {
            PumpTimers[idx].OnPhase = false;
            PumpTimers[idx].ElapsedTime = 0;

        }
    } else {
        // Fase OFF
            if (PumpTimers[idx].ElapsedTime >= timeSecOff) {
                PumpTimers[idx].OnPhase = true;
                PumpTimers[idx].ElapsedTime = 0;

        }
    }
}

/**

 */
void  Ctrl_HandleControl(unsigned char CtrlIdx){

    if (!s_Ctrl[CtrlIdx].Active) {
        return; // Se non attivo, esci dalla funzione
    }
    // Se attivo, gestisci il tipo di controllo
    switch (s_Ctrl[CtrlIdx].type) {
        case _PH:
        case _RX:
        case _NTC:
        case _DS18:
        case _DIFF:
        case _AVG:
        case _EC:
        case _TDS:
        case _SAL:
            Ctrl_HandleThresholdControl(CtrlIdx);
            break;
    }

    if (s_Ctrl[CtrlIdx].CtrlSt != s_Ctrl[CtrlIdx].CtrlStPlug) {
        s_Ctrl[CtrlIdx].ActivReq = true;        //activate set request
    }

}


/**

 */
void Ctrl_HandleThresholdControl(uint8_t CtrlIdx)
{
    if (s_Ctrl[CtrlIdx].Set_PosLogik) {
        if (s_Ctrl[CtrlIdx].ActReadVal > (s_Ctrl[CtrlIdx].Set_Point + s_Ctrl[CtrlIdx].Set_Hyster)) {
            s_Ctrl[CtrlIdx].CtrlSt = false;
        }

        if (s_Ctrl[CtrlIdx].ActReadVal < (s_Ctrl[CtrlIdx].Set_Point - s_Ctrl[CtrlIdx].Set_Hyster)) {
            s_Ctrl[CtrlIdx].CtrlSt = true;
        }

    } else {
        if (s_Ctrl[CtrlIdx].ActReadVal > (s_Ctrl[CtrlIdx].Set_Point + s_Ctrl[CtrlIdx].Set_Hyster)) {
            s_Ctrl[CtrlIdx].CtrlSt = true;
        }

        if (s_Ctrl[CtrlIdx].ActReadVal < (s_Ctrl[CtrlIdx].Set_Point - s_Ctrl[CtrlIdx].Set_Hyster)) {
            s_Ctrl[CtrlIdx].CtrlSt = false;
        }
    }
}

/**

 */
String WSER_ExtCmdGetStatus(uint8_t ctrlIndex) {

  const t_ctrl& ctrl = s_Ctrl[ctrlIndex];
  const t_timer& timer = PumpTimers[ctrlIndex];

    String json = "{";
    json += "\"ctrl\":" + String(ctrlIndex) + ",";  // 👈 aggiunto
    json += "\"type\":" + String(ctrl.type) + ",";
    json += "\"sensor\":" + String(ctrl.SensNum) + ",";
    json += "\"state\":" + String(ctrl.CtrlSt ? "true" : "false") + ",";
    json += "\"value\":" + String(ctrl.ActReadVal) + ",";
    json += "\"setpoint\":" + String(ctrl.Set_Point) + ",";
    json += "\"logic\":" + String(ctrl.Set_PosLogik ? "true" : "false") + ",";
    json += "\"hysteresis\":" + String(ctrl.Set_Hyster) + ",";

    json += "\"timer\":{";
    json += "\"active\":" + String(timer.Active ? "true" : "false") + ",";
    json += "\"onPhase\":" + String(timer.OnPhase ? "true" : "false") + ",";
    json += "\"elapsed\":" + String(timer.ElapsedTime) + ",";
    json += "\"onSeconds\":" + String(timer.OnSeconds) + ",";
    json += "\"offSeconds\":" + String(timer.OffSeconds);
    json += "}";

    json += "}";

  return json;
}