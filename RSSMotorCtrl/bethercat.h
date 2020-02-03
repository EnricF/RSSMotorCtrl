#pragma once

/**************************************************************************
*                                                                         *
* bethercat.h -- BETHERCAT declarations and constant definitions		  *
*                                                                         *
* Copyright (c) 2020, Rob Surgical Systems S.L. All rights reserved.      *
*                                                                         *
**************************************************************************/

#ifndef _bethercat_h_
#define _bethercat_h_

//OPERATION MODE REGISTER VALUES
#define POS_M			0x01//To select "Position Mode" in Operation mode CiA register
#define VEL_M			0x03//To select "Velocity Mode" in Operation mode CiA register
#define PROFILE_VEL_M	0x13//To select "Profiler Velocity Mode" in Operation mode Axis1 register
#define PROFILE_POS_M	0x14//To select "Profiler Position Mode" in Operation mode Axis1 register

//CONTROL WORD REGISTER - CHECK AS SOME OF THEM ARE RE-DEFINED AT THE END OF THIS FILE
#define	CTRLW_SHUTDOWN	0x00
#define	CTRLW_SWITCHOFF	0x06
#define	CTRLW_SWITCHON	0x07
#define	CTRLW_OPERATION	0x0F
#define	CTRLW_POSMOTION	0x1F
#define	CTRLW_VELMOTION 0x08//MSByte. It's LSByte is 0x0F (CTRLW_XXXXXX). TWO BYTES in total!
#define	CTRLW_FAULT_RST	0x80

////////////////////////////////////////////////////
// ETHERCAT Commands - PENDING REVIEW!!!!

//ControlWordAxis1 & ControlWord(CiA) - LSBYTE
#define	BETH_CMD_SWITCH_OFF			0x00	//POWER ON and POWER OFF states (standby)
#define	BETH_CMD_SWITCH_ON			0x06	//Second state after POWER ON (standby)
#define	BETH_CMD_VOLTAGE_ON			0x07	//Motor has power, ready to receive MOTION parameters
#define	BETH_CMD_OPERATION_ENABLE	0x0F	//MOTOR has STRENGTH but is NOT IN MOTION yet
//ControlWordAxis1 only - MSBYTE! (2/2)
#define	BETH_CMD_LATCH				0x02	//LATCH command (new position, velocity ...)
#define	BETH_CMD_CLEARLATCHBIT		0x00	//LATCH bit CLEAR command
//ControlWord (CiA) only - LSBYTE
#define	BETH_CMD_MOTION_ENABLE		0x1F	//MOTOR is in MOTION- NO PROFILER!											//MOTOR is in MOTION






/*------------------ Defines --------------------------------------------*/

	//ESD NETS
#define BETH_NET0								0
#define BETH_NET1								1
#define BETH_NET2								2
#define BETH_NET3								3
#define BETH_NET4								4

////////////////////////////////////////////////////
// Can IDENTIFIERS
#define BETH_ETHID_CMDACK					0xA0
#define BETH_ETHID_CMDGET 					0xC0
#define BETH_ETHID_CMDPUT 					0xE0

/*
#define BETH_CMD_RESET							0x00
#define BETH_CMD_HOME							0x01
#define BETH_CMD_HALT							0x02
#define BETH_CMD_BRAKE                          0x03
#define BETH_CMD_GEXT							0x0A
#define BETH_CMD_SEXT							0x08
#define BETH_CMD_MOTION							0x0B
#define BETH_CMD_WRITE_EEPROM                   0x10
#define BETH_CMD_WRITEEEPROM					0xB0
#define BETH_CMD_RESTORE_DEFAULTS				0xB1
#define BETH_CMD_REFRESH_TID                    0xB2
#define BETH_CMD_CLIENT_REQUEST					0x40
*/

////////////////////////////////////////////////////
// ETHERCAT Parameters ID's

//---- SHUNK
#define BETH_PARAM_DR_HOMEOFFSET				0x00	// Home offset [encoder counts] - inner or outter?
#define BETH_PARAM_DR_GEARRATIO                 0x01	// DR: Gear Ratio [#]
#define BETH_PARAM_DR_MINPOS					0x03	// DR: Module Min. Position	[rev] - outter
#define BETH_PARAM_DR_MAXPOS					0x04	// DR: Module Max. Position	[rev] - outter
#define BETH_PARAM_DR_MAXDELTAPOS				0x05
#define BETH_PARAM_DR_MAXDELTAVEL				0x06
#define BETH_PARAM_DR_TORQUERATIO				0x07
		//Are those specifications?
#define BETH_PARAM_DR_MINVEL					0x09	// DR: Motor Min. Vel.	[rev/s]		--> Must be a negative value
#define BETH_PARAM_DR_MAXVEL					0x0A	// DR: Motor Max. Vel.	[rev/s]
#define BETH_PARAM_DR_MINACC					0x0B	// DR: Motor Min. Acc.	[rev/s^2]	--> Must be a negative Value
#define BETH_PARAM_DR_MAXACC					0x0C	// DR: Motor Max. Acc.	[rev/s^2]	--> Min/Max Deceleration? Where?
#define BETH_PARAM_DR_MINCUR					0x0D	// DR: Motor Min. Current [A]
#define BETH_PARAM_DR_MAXCUR					0x0E	// DR: Motor Max. Current [A]
#define BETH_PARAM_DR_HOMEVEL					0x0F
#define BETH_PARAM_DR_HOMEACC					0x10
#define BETH_PARAM_DR_RSSSERIAL                 0x1A
#define BETH_PARAM_DR_CONFIG					0x1B
#define BETH_PARAM_DR_PULSESPERTURN             0x1C
#define BETH_PARAM_DR_RSSVERSION				0x1D
#define BETH_PARAM_DR_BRAKETIMEOUT				0x1F
#define BETH_PARAM_DR_ADDRESS					0x20
#define BETH_PARAM_DR_SECONDARYADDRESS			0x21
#define BETH_PARAM_DR_PRIMEBAUD					0x22


#define BETH_PARAM_AR_POSCOUNT                  0X24
#define BETH_PARAM_AR_REFPOSCOUNT               0x25
#define BETH_PARAM_AW_DIOSETUP                  0x26
#define BETH_PARAM_AR_RSSSTATE                  0x27
#define BETH_PARAM_AW_TARGETPOSINC              0x28
#define BETH_PARAM_AW_TARGETVELINC              0x29
#define BETH_PARAM_AW_TARGETACCINC              0x2a
#define BETH_PARAM_AR_STEPINC                   0x2b
#define BETH_PARAM_AR_HOMEOFFSETINC             0x2c
#define BETH_PARAM_AW_RAWCUR                    0x35
#define BETH_PARAM_AW_CONFIG                    0x39
#define BETH_PARAM_AR_MOVEMODE                  0x3a
#define BETH_PARAM_AR_ACTPOS                    0x3c	//Motor Actual Position [encoder counts]
#define BETH_PARAM_AR_ACTPOS_                   0x3d	//¿?
#define BETH_PARAM_AR_IPOLPOS                   0x3e
#define BETH_PARAM_AR_DELTAPOS                  0x3f
#define BETH_PARAM_AW_MAXDELTAPOS               0x40
#define BETH_PARAM_AR_ACTVEL                    0x41	//Motor Actual Velocity [encoder counts/s]
#define BETH_PARAM_AR_IPOLVEL                   0x42
#define BETH_PARAM_AR_ACTACC                    0x44	//Motor Actual Acceleration [encoder counts/s^2]
#define BETH_PARAM_AW_MINPOS                    0x45	//Motor Minimum Position reached [encoder counts]
#define BETH_PARAM_AW_MAXPOS                    0x46	//Motor Maximum Position reached [encoder counts]
#define BETH_PARAM_AW_MAXVEL                    0x48	//Motor Maximum Velocity reached[encoder counts/s]
#define BETH_PARAM_AW_MAXACC                    0x4a	//Motor Maximum Acceleration reached [encoder counts/s^2]
#define BETH_PARAM_AW_MAXCUR                    0x4c	//Motor Maximum Current reached[A?]
#define BETH_PARAM_AW_CUR                       0x4d	//Motor Actual Current [A?]????????????
#define BETH_PARAM_AW_TARGETPOS                 0x4e
#define BETH_PARAM_AW_TARGETVEL                 0x4f
#define BETH_PARAM_AW_TARGETACC                 0x50
#define BETH_PARAM_AW_CURA                      0x51	//Motor Actual Current phase-A [A]
#define BETH_PARAM_AW_CURB                      0x52	//Motor Actual Current phase-B [A]
#define BETH_PARAM_AW_CURC	                    0x53	//Motor Actual Current phase-C [A]
//mod (two previous ones are a test)
#define BETH_PARAM_AR_DEFC0                     0x51
#define BETH_PARAM_AR_DEFDAMP                   0x52
#define BETH_PARAM_AR_DEFA0                     0x53
#define BETH_PARAM_AW_ACTC0                     0x54
#define BETH_PARAM_AW_ACTDAMP                   0x55
#define BETH_PARAM_AW_ACTA0                     0x56
#define BETH_PARAM_AR_DEFBURNCOUNT				0x57
#define BETH_PARAM_AW_HOMEOFFSET				0x59
#define BETH_PARAM_AR_ACTIPOS                   0x5a
#define BETH_PARAM_AW_ACTIMAXDELTAPOS	        0x5b
#define BETH_PARAM_AW_ACTIMINPOS				0x5c
#define BETH_PARAM_AW_ACTIMAXPOS				0x5d
#define BETH_PARAM_AW_ACTIMAXVEL				0x5e
#define BETH_PARAM_AW_ACTIMAXACC				0x5f
#define BETH_PARAM_AR_ACTIVEL                   0x60
#define BETH_PARAM_AR_ACTIDELTAPOS				0x61
#define BETH_PARAM_AR_ACTFPOSSTATEDIO	        0x62
#define BETH_PARAM_AR_ACTFSAVEDPOS				0x63
#define BETH_PARAM_AR_ACTFHOMEVEL				0x64
#define BETH_PARAM_AR_ACTIHOMEVEL				0x65
#define BETH_PARAM_AR_ACTIIPOLVEL				0x6A

//RSS Parameters
#define BETH_PARAM_AR_ACTMODULEPOS              0xE0	//Module Actual Position [encoder counts]			- Absolute!
#define BETH_PARAM_AR_ACTMODULEVEL              0xE1	//Module Actual Velocity [encoder counts/s]			- Absolute!
#define BETH_PARAM_AR_ACTMODULEACC              0xE2	//Module Actual Acceleration [encoder counts/s^2]	- Absolute!
#define BETH_PARAM_AW_TARGETMODULEPOS			0xE3	//Module TARGET Position [encoder counts]			- Absolute!
#define BETH_PARAM_AW_TARGETMODULEVEL			0xE4	//Module TARGET Velocity [encoder counts/s]			- Absolute!
#define BETH_PARAM_AW_TARGETMODULEACC			0xE5	//Module TARGET Acceleration [encoder counts/s^2]	- Absolute!

//old (new above)
#define BETH_PARAM_AR_ACTROTORPOS               0xE0
#define BETH_PARAM_AR_ACTROTORVEL               0xE1
#define BETH_PARAM_AR_ACTROTORACC               0xE2
#define BETH_PARAM_AW_ACTROTORTARGETPOS         0xE3
#define BETH_PARAM_AW_ACTROTORTARGETVEL         0xE4
#define BETH_PARAM_AW_ACTROTORTARGETACC         0xE5
#define BETH_PARAM_AR_ACTROTORDELTAPOS          0xE6
#define BETH_PARAM_AW_ACTROTORAMPLITUDE         0xE7
#define BETH_PARAM_AR_ACTROTORPARAM1            0xE8
#define BETH_PARAM_AR_ACTROTORPARAM2            0xE9
#define BETH_PARAM_AR_ACTROTORPARAM3            0xEA
#define BETH_PARAM_AR_ACTROTORDELTAVEL          0xEB
#define BETH_PARAM_AW_ACTROTOROFFSET            0xEC
#define BETH_PARAM_AW_ACTMODULEOFFSET           0xED
#define BETH_PARAM_AW_ACTROTORMAXAMPLITUDE      0xEE

#define BETH_PARAM_DR_CUBE_SERIAL				0x1A
#define BETH_PARAM_DR_VERSION					0x1D
#define BETH_PARAM_AR_CUBE_STATE				0x27
#define BETH_PARAM_AR_ACT_POS					0x3C
#define BETH_PARAM_AR_ACT_VEL					0x41

#define BETH_PARAM_AW_ACT_C0					0x54
#define BETH_PARAM_AW_ACT_DAMP					0x55
#define BETH_PARAM_AW_ACT_A0					0x56

#define BETH_PARAM_AR_ACT_IPOS					0x5A	
#define BETH_PARAM_AR_ACT_FPOS_STATE_DIO        0x62

#define BETH_PARAM_AR_ACT_RAWTEMP				0x6E
#define BETH_PARAM_AR_ACT_FMOTOR_CUR            0x70
#define BETH_PARAM_AR_ACT_FMOTOR_SUPPLY         0x71
#define BETH_PARAM_AR_ACT_FTEMP					0x72	//Motor Temperature [ºC]
#define BETH_PARAM_AR_ACT_FLOGIC_SUPPLY         0x73
#define BETH_PARAM_AR_ACT_MIN_LOGIC_SUPPLY  	0x74
#define BETH_PARAM_AR_ACT_MAX_LOGIC_SUPPLY		0x75
#define BETH_PARAM_AR_ACT_MIN_MOTOR_SUPPLY		0x76
#define BETH_PARAM_AR_ACT_MAX_MOTOR_SUPPLY      0x77

#define BETH_PARAM_AW_ACT_MAX_TEMP              0x84
#define BETH_PARAM_AW_ACT_MIN_TEMP              0x85


#define BETH_PARAM_AW_TARGET_POS				0x4E	//Motor Target Position [encoder counts]
#define BETH_PARAM_AW_TARGET_VEL				0x4F	//Motor Target Velocity [rev/s]
#define BETH_PARAM_AW_TARGET_ACC				0x50	//Motor Target Velocity [rev/s^2]


//---- RSS
#define BETH_PARAM_AW_OFFSET					0xB1
#define BETH_PARAM_AW_LEDS						0xB2
#define BETH_PARAM_AW_HYSTERESIS				0xB3		
#define BETH_PARAM_AR_COMPASS					0xB4
#define BETH_PARAM_AW_OFFSETS4					0xB5
#define BETH_PARAM_AW_OFFSETBETATELE			0xB5
#define BETH_PARAM_AR_HOMEMINMAX                0xB6
#define BETH_PARAM_AW_OSLIN                     0xB7

#define BETH_PARAM_AR_ACT_IPOS123				0xB8
#define BETH_PARAM_AR_ACT_POS123				0xB9        
#define BETH_PARAM_AR_ACT_IPOSBETAS4            0xBA
#define BETH_PARAM_AR_ACT_IALFABETATELE			0xBA	
#define BETH_PARAM_AR_ACT_POSBETAS4             0xBB
#define BETH_PARAM_AR_ACT_ALFABETATELE			0xBB
#define BETH_PARAM_AW_ETEEPROM                  0xBC
#define BETH_PARAM_AR_PWMVALUES                 0xBD
#define BETH_PARAM_AW_EEPROM					0xBE
#define BETH_PARAM_AR_DEVID						0xBF

//---- RSS-PORTAPINCES BCUServer-PP
#define BETH_PARAM_AW_PP_LEDS                   0xB9
#define BETH_PARAM_AR_PP_DEVID                  0xBA
#define BETH_PARAM_AR_PP_TOOLID_INFO            0xBB
#define BETH_PARAM_AR_PP_ACT_JOINT123           0xBE
#define BETH_PARAM_AR_PP_ACT_POSBETAS4          0xB3
#define BETH_PARAM_AR_PP_ACT_TARGETJ123         0xBF
#define BETH_MOTION_PP_JOINTS123_MODE           0xB3
#define BETH_MOTION_PP_IRAMP4_MODE				0xB1



#define BETH_PARAM_AW_CTRLKPKIKD               	0xC0
#define BETH_PARAM_AW_CTRLPWMMIN               	0xC1

#define BETH_PARAM_AR_ACT_FSENSORXYZ			0xC5
#define BETH_PARAM_AR_ACT_CURSENSOR             0xC6


#define BETH_PARAM_AR_ACT_IMODULE				0xD0
#define BETH_PARAM_AR_ACT_IMOTOR				0xD1
#define BETH_PARAM_AR_ACT_CONTROL				0xD2
#define BETH_PARAM_AR_ACT_TEMP					0xD3	//Primary/Driver temperature [ºC]
#define BETH_PARAM_AR_TARGET_IMOTOR				0xD4
#define BETH_PARAM_AR_PARAM_IMOTOR				0xD5
#define BETH_PARAM_AR_ACT_VOLT					0xD6

#define BETH_PARAM_DR_DEVICEID                  0xE0


//---- BATTERY CHARGER
#define BETH_PARAM_AR_VOLTAGE					0xF0
#define BETH_PARAM_AR_CURRENT					0xF1
#define BETH_PARAM_AW_BOFFSET					0xF2


////////////////////////////////////////////////////
// ETHERCAT Motion ID's

//---- SCHUNK
#define BETH_MOTION_IPOS_MODE					0x01
#define BETH_MOTION_FRAMP_MODE					0x04
#define BETH_MOTION_FSTEP_MODE					0x06
#define BETH_MOTION_FVEL_MODE					0x07
#define BETH_MOTION_IRAMP_MODE					0x09	
#define BETH_MOTION_FSTEP_ACK					0x10
#define BETH_MOTION_IVEL_MODE					0x0C
#define BETH_MOTION_FRAMP_ACK					0x0E
#define BETH_MOTION_TIME_MODE                   0x20


//---- RSS
#define BETH_MOTION_IRAMP123_MODE				0x20
#define BETH_MOTION_IRAMP4_MODE					0x21
#define BETH_MOTION_IPWM_MODE                   0x0C



//---- RSS-MASTER_DEVICE
#define BETH_PARAM_MD_AR_PWM_VALUES             0xDA
#define BETH_PARAM_MD_AW_ENC_OFFSET             0xDB
#define BETH_PARAM_MD_AW_M_LIMITS               0xDC
#define BETH_PARAM_MD_AW_CTRL_KP_KI_KD          0xDD
#define BETH_PARAM_MD_AW_PWM_MAX                0xDE
#define BETH_PARAM_MD_AW_LEDS                   0xDF
#define BETH_PARAM_MD_AR_PEDALS                 0xE0
#define BETH_PARAM_MD_AR_SENSORS                0xE1
#define BETH_PARAM_MD_AR_ACT_MENCX_INFO			0xE2	

// Internal ETHERCAT BUS 2 (CO3 requests to CO1)
#define BETH_PARAM_AR_GET_HAND_SENSOR           0xE3
#define BETH_PARAM_RW_SEND_MOTORS_GET_ENC       0xE4

#define BETH_PARAM_AR_GET_PID                   0xE5
#define BETH_PARAM_MD_AW_FORCE_FUNC_PARAMS      0xE6

//---- RSS MASTER	
#define BETH_MOTION_MD_IRAMP_MODE               0xB1
#define BETH_MOTION_MD_IPWM_MODE                0xB2
#define BETH_MOTION_MD_GESTUAL                  0xB3


////////////////////////////////////////////////////
// ETHERCAT States ID's

//---- Drive State Machine
#define	GD_STATE_SWITCH_ON_DISABLE				0x00	//POWER ON and POWER OFF states (standby)
#define	GD_STATE_READY_TO_SWITCH_ON				0x06	//Second state after POWER ON (standby)
#define	GD_STATE_SWITCH_ON						0x07	//Motor has power, ready to receive MOTION parameters
#define	GD_STATE_OPERATION_ENABLE				0x0F	//MOTOR has STRENGTH but is NOT IN MOTION yet
#define	GD_STATE_MOTION_ENABLE					0x31	//MOTOR is in MOTION											//MOTOR is in MOTION
/*
enum driveSTATE {
	SHUTDOWN = 0,	//POWER ON and POWER OFF states (standby)
	SWITCHON,		//second state after POWER ON (standby)
	OPERATION,		//Motor has power, ready to receive MOTION parameters
	MOTION			//MOTOR is in MOTION
};*/

#endif /* _bcan_h_ */
