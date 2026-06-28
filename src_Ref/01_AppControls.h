#ifndef APP_CONTROLS_H
#define APP_CONTROLS_H

#include "02_AppSensors.h"
#include "03_EEprom.h"
#include <Arduino.h>

//----------- Constants definitions-----------------
typedef enum            
{  
    _TEMP	=	0,  
    _HUM	=	1, 
    _TIMER	=	2,       
    _PH		=	3,    
    _RX		=	4, 
	_NTC	=	5,
	_DIFF	=	6,
	_AVG	=   7,   
	_EC		=	8,
	_TDS	=	9,
	_SAL	=	10, 
   	_DS18	=	11, 
	_EMPTY	=	12 
}	e_CtrlType;

#define NUMER_OF_CONTROLS 4 //usually one Ph one redox and 2 more... maybe NTC and EC
#define CONTROLS_MAX_IDX (NUMER_OF_CONTROLS-1)

#define TIME_REFRESH 4000 //in ms
#define TIME_INTERVAL (TIME_REFRESH/NUMER_OF_CONTROLS) //in ms
#define TIME_INTERVAL_SEC (TIME_INTERVAL/1000) //in ms

//----------- Struct defintions-----------------
typedef struct	
{
	uint8_t 		type;	
	bool 			Active;	
	uint8_t 		SensNum;	
	bool 			CtrlSt;		//On or Off
	int 			ActReadVal;		
	unsigned int 	Clock;			
	int 			Set_Point;			
	bool 			Set_PosLogik;			
	int 			Set_Hyster;
	//calibration valies
	int				CalX1;   
	int				CalY1;
	int				CalX2;   
	int				CalY2;
	//associated slot
	String			ssid;
	bool 			CtrlStPlug;		//On or Off
	bool			ActivReq;
	//associated Relay
	uint8_t			HwRelay;
	//relay web page
 	String messStatusRow1;
  	String messStatusRow2;

}	t_ctrl;

//------- Global variables---------------
extern t_ctrl	s_Ctrl[NUMER_OF_CONTROLS];

//----------- Struct definitions -----------------
typedef struct
{
  bool     Active;         // true = timer attivo
  bool     OnPhase;        // true = fase ON, false = OFF
  uint16_t ElapsedTime;    // tempo trascorso nella fase attuale (in secondi)
  uint16_t OnSeconds;      // durata fase ON (in secondi)
  uint16_t OffSeconds;     // durata fase OFF (in secondi)
} t_timer;

#define NUM_PUMPS 4
extern t_timer PumpTimers[NUM_PUMPS];  // ✅ DICHIARAZIONE

//------- Global Functions---------------
extern void Ctrl_HanldeControls();
extern void Ctrl_InitControls();
extern String WSER_ExtCmdGetStatus(uint8_t ctrlIndex);

#endif 
