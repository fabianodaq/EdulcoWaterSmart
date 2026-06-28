#ifndef APP_SENSORS_H
#define APP_SENSORS_H

typedef enum            
{        
    _PH		=	1,    
    _RX		=	2,  
	_EC		=	3,
	_TDS	=	4,
	_SAL	=	5
}	e_SensType;

#define NUM_SENS_PH_RX 2


typedef struct
{
    e_SensType  type;
    long        valueMcp3424;
    uint8_t     ui8Err;

    int         valueFiltTemp;
    int         valueFilt;
    int         valueIst;

    long        valueBuff[4];
    uint8_t     idxLast;

    int         CalX1;
    int         CalY1;
    int         CalX2;
    int         CalY2;
} t_PhOrpSensor;


typedef struct	 // EC/TDS/SAL
{
	e_SensType 	type;		

	long	  valueMcp3424;
    int		  valueFiltTemp;
    int       valueFilt;
	int       value;	
	uint8_t   ui8Err;

	long	  valueBuff[4];
	uint8_t	  idxLast;

	int		  CalX1;   
	int		  CalY1;
	int		  CalX2;   
	int		  CalY2;

}	t_SensorEc;


typedef struct	 // EC/TDS/SAL
{

	long	  valueMcp3424;
    int		  valueFiltTemp;
    int       valueFilt;
	int       value;	
	uint8_t   ui8Err;

	long	  valueBuff[4];
	uint8_t	  idxLast;

}	t_SensorNtc;

typedef struct	 // EC/TDS/SAL
{		

    float Ds18Value;

}	t_SensorDs18;

extern t_PhOrpSensor   SensPhRx[NUM_SENS_PH_RX];
extern t_SensorEc      SensEc;
extern t_SensorNtc     SensNtc;
extern t_SensorDs18    SensDs18;

//------- Global variables---------------
extern void Sens_InitSens();
extern float Sens_DS18_Get();
extern float Sens_Ntc_Get();
extern float Sens_Ph_Get(uint8_t channel);
extern float Sens_Orp_Get(uint8_t channel);
extern float Sens_Ec_Get();


#endif 
