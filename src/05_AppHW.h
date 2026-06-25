#ifndef APP_HW_H
#define APP_HW_H

//---------------PIN OUT DEFINITIONS-----------

// Relays
#define RELAY_3 26
#define RELAY_2 5
#define RELAY_1 23  //---- conflict to be solved in the premium

//for EC 
#define EC_PD_470_PIN   27   // io27 → polarità positiva / depolarizzazione
#define EC_PD_33K_PIN   32   // io32 → polarità negativa / misura
#define EC_PD_3K3_PIN   33   // io33 → polarità negativa / misura

#define R_PULL_UP_HIGH  33000.0f
#define R_PULL_UP_MID    3300.0f
#define R_PULL_UP_LOW     470.0f

#define V_PULL_UP_LOW    2500.0f

//for DS18 
#define DS18B20_PIN 16


#define MESSAGE_LENGTH 24
#define MESSAGE_LENGHT_SEND	11			//the structure to be sent has 10 bytes....
#define MESSAGE_DISPLAY_I2C_ADDRESS		0xA0
 #define MESSAGE_DEVICE_TYPE_EDULCOWATER		0xED
  #define MESSAGE_KALIBRATION			0xD0
 #define MESSAGE_NORMAL_OP_ZERO			0xF0
#define MESSAGE_OUTPUT_TYPE_RELAY_MASK    0x10
#define MESSAGE_OUTPUT_TYPE_MOSFET_MASK   0x20
#define MESSAGE_CTRLSTATUS_AUTO_MASK      0x40
#define MESSAGE_CTRLSTATUS_MAN_MASK       0x80
#define RATIO_HOUR	 	2
#define RATIO_MIN	 	  1
#define TIMER_SET5_OFFSET		8
#define NOT_ACT		8
#define ACTIVE		9
#define SLAVE_LCD1602_ADDRESS MESSAGE_DISPLAY_I2C_ADDRESS//Address see above
#define SLAVE_LCD1602	1	//device type
#define I2C_COMMAND_RELAY_1_ON	0x11	//device type
#define I2C_COMMAND_RELAY_2_ON	0x12	//device type
#define I2C_COMMAND_RELAY_3_ON	0x13	//device type
#define I2C_COMMAND_NEW_SET_NORMAL_OPERATION	0x20	//device type
#define I2C_COMMAND_NEW_SET_F0					0x20	//device type
#define I2C_COMMAND_NEW_SET_F1					0x21	//device type
#define I2C_COMMAND_NEW_SET_F2					0x22	//device type
#define I2C_COMMAND_NEW_SET_F3					0x23	//device type
#define I2C_COMMAND_NEW_SET_F4					0x24	//device type
#define I2C_COMMAND_NEW_SET_F5					0x25	//device type
#define I2C_COMMAND_NEW_SET_F6					0x26	//device type
#define I2C_COMMAND_NEW_SET_F7					0x27	//device type
#define I2C_COMMAND_NEW_SET_KALIBRATION			0x30	//device type
#define I2C_COMMAND_RESTORE_DEFAULT_KALIB	0x70	//device type
	#define I2C_NO_COMMAND	0x80	//device type
	#define CTRL_NEW_TYPE		0x10
	#define CTRL_NEW_SET1		0x11
	#define CTRL_NEW_SET2		0x12
	#define CTRL_NEW_SET3		0x13
	#define CTRL_NEW_SET4		0x14
	#define CTRL_NEW_SET5		0x15
  #define CTRL_NEW_SET_TIMER_ON 	0x20
	#define CTRL_NEW_SET_TIMER_OFF  0x21

	#define CTRL_NEW_SET_RELAY  0x22


#define KAL_SENS1_Y1_PH_EEADRR			0xB0
#define KAL_SENS1_X1_PH_EEADRR			0xB2
#define KAL_SENS1_Y2_PH_EEADRR			0xB4
#define KAL_SENS1_X2_PH_EEADRR			0xB6
#define KAL_CHAN1_VAL1_RX_EEADRR		0xB8
#define KAL_CHAN1_READ1_RX_EEADRR		0xBA
#define KAL_CHAN1_VAL2_RX_EEADRR		0xBC
#define KAL_CHAN1_READ2_RX_EEADRR		0xBE
	#define KAL_CHAN2_VAL1_PH_EEADRR		0xE0
	#define KAL_CHAN2_READ1_PH_EEADRR		0xE2
	#define KAL_CHAN2_VAL2_PH_EEADRR		0xE4
	#define KAL_CHAN2_READ2_PH_EEADRR		0xE6
	#define KAL_CHAN2_VAL1_RX_EEADRR		0xE8
	#define KAL_CHAN2_READ1_RX_EEADRR		0xEA
	#define KAL_CHAN2_VAL2_RX_EEADRR		0xEC
	#define KAL_CHAN2_READ2_RX_EEADRR		0xEE

#define RX_PIN 19   // stesso pin RX della vecchia SoftSerial su ESP8266
#define TX_PIN 18   // stesso pin TX della vecchia SoftSerial su ESP8266

#define MESSAGE_OUTPUT_TYPE_RELAY_MASK		0x10
#define MESSAGE_OUTPUT_TYPE_MOSFET_MASK		0x20

#define MESSAGE_CTRLSTATUS_AUTO_MASK	0x40
#define MESSAGE_CTRLSTATUS_MAN_MASK		0x80
#define MESSAGE_TYPE_MASK  0x0F


//limits table definitions for table limits 
#define CTRL_LIMIT_TYPES_NUM     7
#define CTRL_LIMIT_FIELDS_NUM    3
#define CTRL_LIMIT_VALUES_NUM    5

#define CTRL_VAL_MIN             0
#define CTRL_VAL_MAX             1
#define CTRL_VAL_STEP_SMALL      2
#define CTRL_VAL_STEP_MEDIUM     3
#define CTRL_VAL_STEP_LARGE      4

#define CTRL_TYPE_TEMP           0
#define CTRL_TYPE_PH             1
#define CTRL_TYPE_ORP            2
#define CTRL_TYPE_EC             3
#define CTRL_TYPE_TDS            4
#define CTRL_TYPE_SAL            5
#define CTRL_TYPE_TIMER          6

#define CTRL_TARGET_ACTIVATION   0
#define CTRL_HYSTER_TON          1
#define CTRL_LOGIC_TOFF          2




extern void HW_InitHw();


extern void SensEc_SetMidRange();
extern void SensEc_SetLowRange();
extern void SensEc_SetOff();
extern void SensEc_Depolarize();
extern void SensEc_SetHighRange();


#endif 
