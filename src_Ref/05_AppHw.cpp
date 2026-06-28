#include "05_AppHW.h"
#include "01_AppControls.h"
#include "16_TimersPage.h"
#include "15_CalibrationPage.h"
#include "12_SettingsPage.h"
#include <Arduino.h>

void HW_HanldeOutputs();
void HW_InitHw();
void HW_HandleLcdPicCommunication();
bool tryReceiveMessage(uint8_t* buffer, uint8_t length, uint16_t timeoutMs);
void HandleReceivedCommand(uint8_t* incomingData);
void UpdateNewSet_F(unsigned char CtrlNumb, unsigned int NewSet, unsigned char PosNewSet);

void SensEc_SetMidRange();
void SensEc_SetLowRange();
void SensEc_SetOff();
void SensEc_Depolarize();
void SensEc_SetHighRange();


HardwareSerial PicSerial(1);  // Usa UART1 (ce ne sono 3: 0,1,2)

typedef struct	
	{
  unsigned char Address;      // frame sync / display address

	unsigned char MessType;		//Type of message

	unsigned char Clock0H;			//Clocks Actual Values
	unsigned char Clock0L;			//
	unsigned char Clock1H;			//
	unsigned char Clock1L;			//
	unsigned char Clock2H;			//
	unsigned char Clock2L;			//
	unsigned char Clock3H;			//
	unsigned char Clock3L;			//

	unsigned char ValueH;			//Control Actual Value
	unsigned char ValueL;			//

	unsigned char SetTargetH;			//Control Settings
	unsigned char SetTargetL;			//
  unsigned char SetHysterH;	//
  unsigned char SetHysterL;	//
	unsigned char SetLogik ;	//

	unsigned char SetTonH;		// Timer Settings
	unsigned char SetTonL;		//
	unsigned char SetToffH;		//
	unsigned char SetToffL;		//

	unsigned char CtrlSt;			// Control status

  unsigned char Relay;

	unsigned char CheckSum;			// check sum of the information
}	t_NormOpMess;

typedef struct	
	{
  unsigned char Address;      // frame sync / display address

	unsigned char MessType;		//Type of message

	unsigned char AnalogH;			//Clock
	unsigned char AnalogL;			//

	unsigned char Clock0H;			//Clocks Actual Values
	unsigned char Clock0L;			//
	unsigned char Clock1H;			//
	unsigned char Clock1L;			//
	unsigned char Clock2H;			//
	unsigned char Clock2L;			//
	unsigned char Clock3H;			//
	unsigned char Clock3L;			//

	unsigned char X1_L;			//Channel Type
	unsigned char X1_H;			//
	unsigned char Y1_L;			//
	unsigned char Y1_H;			//
	unsigned char X2_L;			//Channel Type
	unsigned char X2_H;			//
	unsigned char Y2_L;			//
	unsigned char Y2_H;			//

	unsigned char CtrlSt;			// Control status

  unsigned char Empty0;
  unsigned char Empty1;

	unsigned char CheckSum;			// check sum of the information
}	t_KalMess;

typedef struct	
	{
	unsigned char Address;		//Address of the I2c device
	unsigned char DeviceType;	//Type of device (blackchick or edulcowater...or...)
	unsigned char MessType;		//Type of message
	unsigned char CommandType;		//Address of the I2c device
	unsigned char Val1H;			//
	unsigned char Val1L;			//
	unsigned char Pos1;			//
	unsigned char Val2H;			//
	unsigned char Val2L;			//
	unsigned char Pos2;			//
	unsigned char CheckSum;			// check sum of the information
}	t_ChangeValMess;

t_NormOpMess s_NormOpMess[4];
t_KalMess s_KalMess[4];
t_ChangeValMess s_ChangeValMess;
t_ChangeValMess s_ChangeValMessOld;

uint8_t incomingData[MESSAGE_LENGHT_SEND];


/**
 
 */
void HW_HanldeOutputs(){

  uint8_t static idx = 0;

  bool relayState = false;

  relayState = s_Ctrl[idx].CtrlSt && PumpTimers[idx].OnPhase;

  if (s_Ctrl[idx].HwRelay == 1) {
      digitalWrite(RELAY_1, relayState ? HIGH : LOW);
  }
  else if (s_Ctrl[idx].HwRelay == 2) {
      digitalWrite(RELAY_2, relayState ? HIGH : LOW);
  }
  else if (s_Ctrl[idx].HwRelay == 3) {
      digitalWrite(RELAY_3, relayState ? HIGH : LOW);
  }

  idx = (idx + 1) % 4;

  HW_HandleLcdPicCommunication(); //always there, in casethe controller doesnt use the dispaly simply is not connected

}

 //---- conflict to be solved in the premium
/**

 */
void HW_InitHw(){
	
  EEPROM.begin(EEPROM_SIZE); // o 1024, in base alla dimensione necessaria

  pinMode(RELAY_1, OUTPUT);    // imposta GPIO per RELAY_1 come output
  digitalWrite(RELAY_1, LOW);  // stato iniziale OFF

  pinMode(RELAY_2, OUTPUT);    // imposta GPIO per RELAY_2 come output
  digitalWrite(RELAY_2, LOW);  // stato iniziale OFF

  pinMode(RELAY_3, OUTPUT);    // imposta GPIO per RELAY_3 come output
  digitalWrite(RELAY_3, LOW);  // stato iniziale OFF

  //for ec
  pinMode(EC_PD_470_PIN, INPUT );
  pinMode(EC_PD_33K_PIN, INPUT);    // to be deleted fi the new hw doenst need those circuitry
  pinMode(EC_PD_3K3_PIN, INPUT); 

  // for DS18B20
  pinMode(DS18B20_PIN, INPUT); //in other module it is initialized for one wire functions

  PicSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

}


/**

 */
void HW_HandleLcdPicCommunication() {
  static uint32_t lastCommTime = 0;
  static bool transferModeNormal = true;  // true = NORMAL, false = KALIBRA
  static uint8_t uci = 0;     // indice NORMAL: 0 → 3
  static uint8_t uciKal = 0;  // indice KALIBRA: 0 → 3

  if (millis() - lastCommTime >= 128) {
    lastCommTime = millis();

    if (transferModeNormal) {

      unsigned char* ucPtrMess = (unsigned char*) &s_NormOpMess[uci];

      for (unsigned i = 0; i < MESSAGE_LENGTH; i++) {
        PicSerial.write(*(ucPtrMess + i));
        delayMicroseconds(300);
      }

      bool received = tryReceiveMessage(incomingData, MESSAGE_LENGHT_SEND, 50);

      if (received) {
        Serial.print("📥 Ricevuto [");
        for (uint8_t i = 0; i < MESSAGE_LENGHT_SEND; i++) {
          if (incomingData[i] < 0x10) Serial.print("0");
          Serial.print(incomingData[i], HEX);
          if (i < MESSAGE_LENGHT_SEND - 1) Serial.print(" ");
        }
        Serial.println("]");

      HandleReceivedCommand(&incomingData[0]);
      }

    uci = (uci + 1) & 0x03;

    t_KalMess* msg = &s_KalMess[uciKal];   // 🔧 Prepara buffer KALIBRA → s_KalMess[uciKal]

    msg->Address = MESSAGE_DISPLAY_I2C_ADDRESS;               // SYNC

    msg->MessType = MESSAGE_KALIBRATION + uciKal;               // Message type

    msg->AnalogH = (s_Ctrl[uciKal].ActReadVal >> 8) & 0xFF;            // Raw analog value
    msg->AnalogL = s_Ctrl[uciKal].ActReadVal & 0xFF;
  
    msg->Clock0H = (PumpTimers[0].ElapsedTime >> 8) & 0xFF;     // Timers status
    msg->Clock0L = PumpTimers[0].ElapsedTime & 0xFF;
    msg->Clock1H = (PumpTimers[1].ElapsedTime >> 8) & 0xFF;
    msg->Clock1L = PumpTimers[1].ElapsedTime & 0xFF;
    msg->Clock2H = (PumpTimers[2].ElapsedTime >> 8) & 0xFF;
    msg->Clock2L = PumpTimers[2].ElapsedTime & 0xFF;
    msg->Clock3H = (PumpTimers[3].ElapsedTime >> 8) & 0xFF;
    msg->Clock3L = PumpTimers[3].ElapsedTime & 0xFF;
    
    msg->X1_L = s_Ctrl[uciKal].CalX1 & 0xFF;          // Calibration points
    msg->X1_H = (s_Ctrl[uciKal].CalX1 >> 8) & 0xFF;
    msg->Y1_L = s_Ctrl[uciKal].CalY1 & 0xFF;
    msg->Y1_H = (s_Ctrl[uciKal].CalY1 >> 8) & 0xFF;
    msg->X2_L = s_Ctrl[uciKal].CalX2 & 0xFF;
    msg->X2_H = (s_Ctrl[uciKal].CalX2 >> 8) & 0xFF;
    msg->Y2_L = s_Ctrl[uciKal].CalY2 & 0xFF;
    msg->Y2_H = (s_Ctrl[uciKal].CalY2 >> 8) & 0xFF;
    
    msg->CtrlSt = 0x00;                                         // Status byte: same logic as NORMAL
    msg->CtrlSt |= (s_Ctrl[uciKal].type & 0x0F);                // bit 0..3: control type
    if (PumpTimers[uciKal].Active) {                            // bit 4: timer active
      msg->CtrlSt |= (1 << 4);
    }
    if (PumpTimers[uciKal].OnPhase) {                           // bit 5: timer phase
      msg->CtrlSt |= (1 << 5);
    }
    if (s_Ctrl[uciKal].CtrlSt) {                                // bit 6: control request
      msg->CtrlSt |= (1 << 6);
    }
    if (s_Ctrl[uciKal].CtrlSt && PumpTimers[uciKal].OnPhase) {  // bit 7: real output state
      msg->CtrlSt |= (1 << 7);
    }

    msg->Empty0 = uciKal;
    msg->Empty1 = uciKal;

    uint8_t* ptr = (uint8_t*)msg;  // Checksum KAL
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < MESSAGE_LENGTH - 1; i++) {
      checksum += ptr[i];
    }
    msg->CheckSum = checksum;

  } else {
    
      unsigned char* ucPtrMess = (unsigned char*) &s_KalMess[uciKal]; // 📤 Invia KALIBRA → s_KalMess[uciKal]

      for (unsigned i = 0; i < MESSAGE_LENGTH; i++) {
        PicSerial.write(*(ucPtrMess + i));
        delayMicroseconds(300);
      }

      bool received = tryReceiveMessage(incomingData, MESSAGE_LENGHT_SEND, 50);

      if (received) {
        Serial.print("📥 Ricevuto [");
        for (uint8_t i = 0; i < MESSAGE_LENGHT_SEND; i++) {
          if (incomingData[i] < 0x10) Serial.print("0");
          Serial.print(incomingData[i], HEX);
          if (i < MESSAGE_LENGHT_SEND - 1) Serial.print(" ");
        }
        Serial.println("]");

        HandleReceivedCommand(&incomingData[0]);

      }

      uciKal = (uciKal + 1) & 0x03;

      t_NormOpMess* msg = &s_NormOpMess[uci];

      msg->Address = MESSAGE_DISPLAY_I2C_ADDRESS;               // SYNC

      msg->MessType   = MESSAGE_NORMAL_OP_ZERO + uci; // Message type

      msg->Clock0H = (PumpTimers[0].ElapsedTime >> 8) & 0xFF; // Timers status
      msg->Clock0L = PumpTimers[0].ElapsedTime & 0xFF;
      msg->Clock1H = (PumpTimers[1].ElapsedTime >> 8) & 0xFF;      
      msg->Clock1L = PumpTimers[1].ElapsedTime & 0xFF;
      msg->Clock2H = (PumpTimers[2].ElapsedTime >> 8) & 0xFF;     
      msg->Clock2L = PumpTimers[2].ElapsedTime & 0xFF;
      msg->Clock3H = (PumpTimers[3].ElapsedTime >> 8) & 0xFF;      
      msg->Clock3L = PumpTimers[3].ElapsedTime & 0xFF;

      msg->ValueH = (s_Ctrl[uci].ActReadVal >> 8) & 0xFF;  // Control actual value
      msg->ValueL = s_Ctrl[uci].ActReadVal & 0xFF;

      msg->SetTargetH = (s_Ctrl[uci].Set_Point >> 8) & 0xFF; // Control settings
      msg->SetTargetL = s_Ctrl[uci].Set_Point & 0xFF;
      msg->SetHysterH = (s_Ctrl[uci].Set_Hyster >> 8) & 0xFF;
      msg->SetHysterL = s_Ctrl[uci].Set_Hyster & 0xFF;
      msg->SetLogik = s_Ctrl[uci].Set_PosLogik;

      msg->SetTonH = (PumpTimers[uci].OnSeconds >> 8) & 0xFF; // timer settings
      msg->SetTonL = PumpTimers[uci].OnSeconds & 0xFF;
      msg->SetToffH = (PumpTimers[uci].OffSeconds >> 8) & 0xFF;
      msg->SetToffL = PumpTimers[uci].OffSeconds & 0xFF;

      msg->CtrlSt = 0x00;
      msg->CtrlSt |= (s_Ctrl[uci].type & 0x0F);             // bit 0..3: control type
      if (PumpTimers[uci].Active) {                         // bit 4: timer active
        msg->CtrlSt |= (1 << 4);
      }
      if (PumpTimers[uci].OnPhase) {                        // bit 5: timer phase, 1 = TON / ON phase, 0 = TOFF / OFF phase
        msg->CtrlSt |= (1 << 5);
      }
      if (s_Ctrl[uci].CtrlSt) {                             // bit 6: control request/status
        msg->CtrlSt |= (1 << 6);
      }
      if (s_Ctrl[uci].CtrlSt && PumpTimers[uci].OnPhase) {  // bit 7: real output relay state
        msg->CtrlSt |= (1 << 7);
      }

      msg->Relay = s_Ctrl[uci].HwRelay;

      uint8_t* ptr2 = (uint8_t*)msg;     // checksum
      uint8_t checksum2 = 0;
      for (uint8_t i = 0; i < MESSAGE_LENGTH - 1; i++) {
        checksum2 += ptr2[i];
      }
      msg->CheckSum = checksum2;

    }
    transferModeNormal = !transferModeNormal;// 🔄 Alterna modalità
  }
}
//--------------------------------------------------------------------
/**

 */
void SensEc_SetHighRange()
{
    pinMode(EC_PD_470_PIN, INPUT);
    pinMode(EC_PD_3K3_PIN, INPUT);
    pinMode(EC_PD_33K_PIN, OUTPUT);
    digitalWrite(EC_PD_33K_PIN, LOW);
}


/**

 */
void SensEc_SetMidRange()
{
    pinMode(EC_PD_470_PIN, INPUT);
    pinMode(EC_PD_3K3_PIN, OUTPUT);
    digitalWrite(EC_PD_3K3_PIN, LOW);
    pinMode(EC_PD_33K_PIN, INPUT);
}


/**

 */
void SensEc_SetLowRange()
{
    pinMode(EC_PD_470_PIN, OUTPUT);
    digitalWrite(EC_PD_470_PIN, LOW);
    pinMode(EC_PD_3K3_PIN, INPUT);
    pinMode(EC_PD_33K_PIN, INPUT);
}

/**

 */
void SensEc_SetOff()
{
    pinMode(EC_PD_470_PIN, INPUT);
    pinMode(EC_PD_3K3_PIN, INPUT);
    pinMode(EC_PD_33K_PIN, INPUT);
}


/**

 */
void SensEc_Depolarize()
{
    pinMode(EC_PD_3K3_PIN, OUTPUT);
    digitalWrite(EC_PD_3K3_PIN, HIGH);
    delay(5);
    pinMode(EC_PD_3K3_PIN, INPUT);
}

/**

 */
bool tryReceiveMessage(uint8_t* buffer, uint8_t expectedLength, uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t idx = 0;

  // Ricezione dati
  while ((millis() - start) < timeoutMs && idx < expectedLength) {
    if (PicSerial.available()) {
      buffer[idx++] = PicSerial.read();
    }
  }

  // Controllo: se non ho ricevuto tutti i byte richiesti
  if (idx != expectedLength) return false;

  // Verifica del checksum (somma dei primi N-1 byte)
  uint8_t checksum = 0;
  for (uint8_t i = 0; i < expectedLength - 1; i++) {
    checksum += buffer[i];
  }

  return (checksum == buffer[expectedLength - 1]);
}

/**

 */
  void HandleReceivedCommand(uint8_t* incomingData) {
  // Copia i dati ricevuti nella struttura attiva
  memcpy(&s_ChangeValMess, incomingData, MESSAGE_LENGHT_SEND);

  // Verifica se il comando è cambiato
  bool isNewCommand = false;
  uint8_t* curr = (uint8_t*)&s_ChangeValMess;
  uint8_t* prev = (uint8_t*)&s_ChangeValMessOld;
  for (uint8_t i = 0; i < MESSAGE_LENGHT_SEND; i++) {
    if (curr[i] != prev[i]) {
      isNewCommand = true;
      break;
    }
  }

  // Se è un nuovo comando, esegui
  if (isNewCommand) {

    switch (s_ChangeValMess.CommandType) {
      case I2C_NO_COMMAND:
			{

			  break;
			}

		case I2C_COMMAND_NEW_SET_F0:
			{
			unsigned int tempNewSet=0;
			tempNewSet	=	(s_ChangeValMess.Val1H<<8);
			tempNewSet	=	tempNewSet	+ s_ChangeValMess.Val1L;
			UpdateNewSet_F(0, tempNewSet, s_ChangeValMess.Pos1);
			break;
			}
		case I2C_COMMAND_NEW_SET_F1:
			{
			unsigned int tempNewSet=0;
			tempNewSet	=	(s_ChangeValMess.Val1H<<8);
			tempNewSet	=	tempNewSet	+ s_ChangeValMess.Val1L;
			UpdateNewSet_F(1, tempNewSet, s_ChangeValMess.Pos1);
			break;
			}
		case I2C_COMMAND_NEW_SET_F2:
			{
			unsigned int tempNewSet=0;
			tempNewSet	=	(s_ChangeValMess.Val1H<<8);
			tempNewSet	=	tempNewSet	+ s_ChangeValMess.Val1L;
			UpdateNewSet_F(2, tempNewSet, s_ChangeValMess.Pos1);
			break;
			}
		case I2C_COMMAND_NEW_SET_F3:
			{
			unsigned int tempNewSet=0;
			tempNewSet	=	(s_ChangeValMess.Val1H<<8);
			tempNewSet	=	tempNewSet	+ s_ChangeValMess.Val1L;
			UpdateNewSet_F(3, tempNewSet, s_ChangeValMess.Pos1);
			break;
			}

      case I2C_COMMAND_NEW_SET_F4:
			{
			unsigned int tempNewSet=0;
			tempNewSet	=	(s_ChangeValMess.Val1H<<8);
			tempNewSet	=	tempNewSet	+ s_ChangeValMess.Val1L;
			UpdateNewSet_F(0, tempNewSet, s_ChangeValMess.Pos1);
			break;
			}
      case I2C_COMMAND_NEW_SET_F5:
			{
			unsigned int tempNewSet=0;
			tempNewSet	=	(s_ChangeValMess.Val1H<<8);
			tempNewSet	=	tempNewSet	+ s_ChangeValMess.Val1L;
			UpdateNewSet_F(1, tempNewSet, s_ChangeValMess.Pos1);
			break;
			}
      case I2C_COMMAND_NEW_SET_F6:
			{
			unsigned int tempNewSet=0;
			tempNewSet	=	(s_ChangeValMess.Val1H<<8);
			tempNewSet	=	tempNewSet	+ s_ChangeValMess.Val1L;
			UpdateNewSet_F(2, tempNewSet, s_ChangeValMess.Pos1);
			break;
			}
      case I2C_COMMAND_NEW_SET_F7:
			{
			unsigned int tempNewSet=0;
			tempNewSet	=	(s_ChangeValMess.Val1H<<8);
			tempNewSet	=	tempNewSet	+ s_ChangeValMess.Val1L;
			UpdateNewSet_F(3, tempNewSet, s_ChangeValMess.Pos1);
			break;
			}
//--------------KALIBRATION COMMANDS-----------------------
      case I2C_COMMAND_NEW_SET_KALIBRATION:
      {
          uint8_t pos1 = s_ChangeValMess.Pos1;
          uint8_t pos2 = s_ChangeValMess.Pos2;
          int16_t calY = (int16_t)((s_ChangeValMess.Val1H << 8) | s_ChangeValMess.Val1L);

          Serial.printf("🧪 Calibrazione - Sensore %d - Valore %d = %d\n", pos1, pos2, calY);

          WSER_ExtCmdSetCalPoint(pos1, pos2, calY);

          break;
      }

		  case I2C_COMMAND_RESTORE_DEFAULT_KALIB:
			{
          Serial.println("🧪 Caalibration reset sens 0 poin 1");
          WSER_ExtCmdCalReset(0, 1 );
          Serial.println("🧪 Caalibration reset sens 0 poin 1");
          WSER_ExtCmdCalReset(0, 2 );

          Serial.println("🧪 Caalibration reset sens 2 poin 1");
          WSER_ExtCmdCalReset(2, 1 );
          Serial.println("🧪 Caalibration reset sens 2 poin 1");
          WSER_ExtCmdCalReset(2, 2 );
        
          Serial.println("🧪 Caalibration reset sens 3 poin 1");
          WSER_ExtCmdCalReset(3, 1 );
          Serial.println("🧪 Caalibration reset sens 3 poin 1");
          WSER_ExtCmdCalReset(3, 2 );

			  break;
			}

		  default:
			{
			  // Comando non riconosciuto
			  // Potresti aggiungere un log, un blink LED, ecc.
			  break;
			}

    }

    // Aggiorna il backup
    memcpy(&s_ChangeValMessOld, &s_ChangeValMess, MESSAGE_LENGHT_SEND);
  }
}




/*
void UpdateNewSet_F
*/
void UpdateNewSet_F(unsigned char CtrlNumb, unsigned int NewSet, unsigned char PosNewSet)
{
  switch (PosNewSet)
  {
    case CTRL_NEW_SET1:
    {
      Serial.print("  → CtrlNumb: ");
      Serial.println(CtrlNumb);
      
      Serial.print("  → NewTarget: ");
      Serial.println(NewSet);

      WSER_ExtCmdSetSetpoint(CtrlNumb, NewSet);

      break;
    }
    case CTRL_NEW_SET2:
    {
      
      break;
    }

    case CTRL_NEW_SET3:
    {
      Serial.print("  → CtrlNumb: ");
      Serial.println(CtrlNumb);
      
      Serial.println(" Newt Logk Set");
      Serial.println(NewSet);

      WSER_ExtCmdSetLogic(CtrlNumb, NewSet);
      break;
    }

    case CTRL_NEW_SET4:
    {

      Serial.print("  → CtrlNumb: ");
      Serial.println(CtrlNumb);
      
      Serial.println(" Hyster");
      Serial.println(NewSet);
      
      WSER_ExtCmdSetHysteresis(CtrlNumb, NewSet);

      break;
    }

    case CTRL_NEW_SET5: //set active the related timers 
    {

      Serial.print("  → CtrlNumb: ");
      Serial.println(CtrlNumb);
      Serial.print("  → New Timer activaiton: ");
      Serial.println(NewSet);

      WSER_ExtCmdSetTimerActivation(CtrlNumb, NewSet != 0);

      break;
    }

    case CTRL_NEW_TYPE:
    {
      Serial.print("  → CtrlNumb: ");
      Serial.println(CtrlNumb);

      Serial.print("  → NewSet (Tipo): ");
      Serial.println(NewSet);

      String typeStr = "";

      if (CtrlNumb == 0 || CtrlNumb == 3)
      {
        if (NewSet == _PH)
          typeStr = "ph";
        else if (NewSet == _RX)
          typeStr = "orp";
      }
      else if (CtrlNumb == 2)
      {
        if (NewSet == _EC)
          typeStr = "ec";
        else if (NewSet == _TDS)
          typeStr = "tds";
        else if (NewSet == _SAL)
          typeStr = "sal";
        else if (NewSet == _NTC)
          typeStr = "ntc";
        else if (NewSet == _DIFF)
          typeStr = "diff";
        else if (NewSet == _AVG)
          typeStr = "avg";

      }

      if (typeStr != "")
      {
        WSER_ExtCmdSetType(CtrlNumb, typeStr);
      }

      break;
    }

    case CTRL_NEW_SET_TIMER_ON:
    {

      Serial.print("CtrlNumb: ");
      Serial.println(CtrlNumb);
      Serial.print("New Timer on: ");
      Serial.println(NewSet);

      uint16_t oldToff = PumpTimers[CtrlNumb].OffSeconds;
      WSER_ExtCmdSetTonToff(CtrlNumb, NewSet, oldToff);

      break;
    }

    case CTRL_NEW_SET_TIMER_OFF:
    {
      Serial.print("CtrlNumb: ");
      Serial.println(CtrlNumb);
      Serial.print("NewSet: ");
      Serial.println(NewSet);

      uint16_t oldTon = PumpTimers[CtrlNumb].OnSeconds;
      WSER_ExtCmdSetTonToff(CtrlNumb, oldTon, NewSet);

      break;
    }

    case CTRL_NEW_SET_RELAY:
    {
      Serial.println("=== CTRL_NEW_SET_RELAY ===");
      Serial.print("CtrlNumb: ");
      Serial.println(CtrlNumb);

      Serial.print("RelayIdx: ");
      Serial.println(NewSet);

      if (CtrlNumb > 3 || NewSet > 3)
        break;

      
      if (NewSet != 0) // Se il relay è già assegnato ad altro controllo, liberalo
      {
        for (uint8_t i = 0; i < 4; i++)
        {
          if (i == CtrlNumb)
            continue;

          if (s_Ctrl[i].HwRelay == NewSet)
          {
            s_Ctrl[i].HwRelay = 0;
            EEPR_WriteByte(RELAY_CONTROL_1_ADDR + i, 0);
          }
        }
      }

      
      switch (s_Ctrl[CtrlNumb].HwRelay) // Spegni il vecchio relay associato a questo controllo
      {
        case 1:
          digitalWrite(RELAY_1, LOW);
          break;

        case 2:
          digitalWrite(RELAY_2, LOW);
          break;

        case 3:
          digitalWrite(RELAY_3, LOW);
          break;
      }

      // Salva nuova assegnazione
      EEPR_WriteByte(RELAY_CONTROL_1_ADDR + CtrlNumb, NewSet);
      s_Ctrl[CtrlNumb].HwRelay = EEPR_ReadByte(RELAY_CONTROL_1_ADDR + CtrlNumb);

      break;
    }

  }
}