#pragma once

#ifndef GLOBALVAR_H
#define GLOBALVAR_H

//ECM template project
#include <getopt.h>
#include <ecm.h>
#include <stdint.h>

//EtherCAT comms
#include "ecmW.h"

//EtherCAT defines
#include "bethercat.h"

//Thread related
#include <Windows.h>
#include <strsafe.h>
#include <tchar.h>

//Log with PLOG
#include <plog/Log.h>// include the header for LOG! (https://github.com/SergiusTheBest/plog)

//map
#include <map>

//LOG control
#ifdef _DEBUG
#define LOG_LEVEL	plog::verbose
#else
#define LOG_LEVEL	plog::info
#endif

#define NUMCOMMANDS 2 //How many acyclic commands you want to send;



//BITRACK ETHERCAT SLAVES ID's
#define BR1_M1	0	//Robotic Arm #1, Motor #1. First slave connected to Master ETH0
#define BR1_M2	1	//Robotic Arm #1, Motor #2
#define BR1_M3	2	//Robotic Arm #1, Motor #3
#define BR1_M4	3	//Robotic Arm #1, Motor #4
#define BR2_M1	4	//Robotic Arm #2, Motor #1. Connected to BP1_M4
#define BR2_M2	5	//Robotic Arm #2, Motor #2
#define BR2_M3	6	//Robotic Arm #2, Motor #3
#define BR2_M4	7	//Robotic Arm #2, Motor #4
#define BR3_M1	8	//Robotic Arm #3, Motor #1. Connected to BP2_M4
#define BR3_M2	9	//Robotic Arm #3, Motor #2
#define BR3_M3	10	//Robotic Arm #3, Motor #3
#define BR3_M4	11	//Robotic Arm #3, Motor #4
#define BL1_M1	12	//Laparoscopic Arm #1, Motor #1
#define BL1_M2	13	//Laparoscopic Arm #1, Motor #2
#define BL1_M3	14	//Laparoscopic Arm #1, Motor #3
#define BL1_M4	15	//Laparoscopic Arm #1, Motor #4. Last slave connected to Master ETH0, connected directly to Master ETH1 (redundancy)

//SUMMIT ERROR CODES
typedef struct _ErrorCodes {
	uint32_t	hex;
	CString		description;
}ErrorCodes, *pErrorCodes;


/*
ErrorCodes const err[]{
	"Drive is working correctly"	,									0x00000000,
	"Communications watchdog error"	,									0x00001001,
	"Fault - Power stage shutdown. STO is enabled"	,					0x00003280,
	"Fault - Power stage shutdown. STO supply fault",					0x00003281,
	"Critical - Power stage shutdown. STO1 and STO2 inputs mismatch",	0x00003282,
	"Fault - Power stage shutdown. Over-current (int. drive limit)",	0x00002280,
	"Fault - Power stage shutdown. I2T detected",						0x00002288,
	"Fault - Power stage shutdown. Current reading > conf. max.current",0x00002289,
	"Fault - Power stage shutdown. Bus voltage loading error",			0x00003290,
	"Fault - Power stage shutdown. Over-voltage (internal limit)",		0x00003210,
	"Fault - Power stage shutdown. Over-voltage (red. int. limit)",		0x00003211,
	"Fault - Power stage shutdown. Under-voltage (internal limit)",		0x00003221,
	"Fault - Power stage shutdown. Over-voltage (user limit)",			0x00003231,
	"Fault - Power stage shutdown. Under-voltage (user limit)",			0x00003241,
	"Fault - Power stage shutdown. Over-temperature (user limit)",		0x00004303,
	"Fault - Power stage shutdown. Under-temperature (user limit)",		0x00004304,
	"Fault - Power stage shutdown. Over Temperature (internal limit)",	0x00004300,
	"Fault - Power stage shutdown. Under Temperature (internal limit)",	0x00004301,
	"Fault - Power stage shutdown. Motor Over Temperature",				0x00004400,
	"Fault - Power stage shutdown. External fault",						0x00004500,
	"Fault - Power stage shutdown. Interpolation time overflow & PVT",	0x00007390,
	"Fault - Power stage shutdown. Profiler parameters not valid (check sign)",0x00007391,
	"Fault - Power stage shutdown. Following error > vel. fol. error window",0x00007388	,
	"Fault - Power stage shutdown. Following error > pos. fol. error window",0x00007387,
	"Fault - Power stage shutdown. Velocity out of limits",				0x00007386,
	"Fault - Power stage shutdown. Position out of limits",				0x00007385,
	"Fault - Power stage shutdown. CRC checks overflow in SSI readings",0x00007382,
	"Fault - Power stage shutdown. Position flags overflow in SSI readings",0x00007380,
	"Fault - Power stage shutdown. Halls sequence error",				0x00007370,
	"Fault - Power stage shutdown. Halls combination error",			0x00007371,
	"Fault - Power stage shutdown. Digital encoder runaway error",		0x00007372,
	//More to be done soon
};*/

ErrorCodes const err[]{
	0x00000000,	"Drive is working correctly"	,
	0x00001001,	"Communications watchdog error"	,
	0x00003280,	"Fault - Power stage shutdown. STO is enabled"	,
	0x00003281,	"Fault - Power stage shutdown. STO supply fault",
	0x00003282,	"Critical - Power stage shutdown. STO1 and STO2 inputs mismatch",
	0x00002280,	"Fault - Power stage shutdown. Over-current (int. drive limit)",
	0x00002288,	"Fault - Power stage shutdown. I2T detected",
	0x00002289,	"Fault - Power stage shutdown. Current reading > conf. max.current",
	0x00003290,	"Fault - Power stage shutdown. Bus voltage loading error",
	0x00003210,	"Fault - Power stage shutdown. Over-voltage (internal limit)",
	0x00003211,	"Fault - Power stage shutdown. Over-voltage (red. int. limit)",
	0x00003221,	"Fault - Power stage shutdown. Under-voltage (internal limit)",
	0x00003231,	"Fault - Power stage shutdown. Over-voltage (user limit)",
	0x00003241,	"Fault - Power stage shutdown. Under-voltage (user limit)",
	0x00004303,	"Fault - Power stage shutdown. Over-temperature (user limit)",
	0x00004304,	"Fault - Power stage shutdown. Under-temperature (user limit)",
	0x00004300,	"Fault - Power stage shutdown. Over Temperature (internal limit)",
	0x00004301,	"Fault - Power stage shutdown. Under Temperature (internal limit)",
	0x00004400,	"Fault - Power stage shutdown. Motor Over Temperature",
	0x00004500,	"Fault - Power stage shutdown. External fault",
	0x00007390,	"Fault - Power stage shutdown. Interpolation time overflow & PVT",
	0x00007391,	"Fault - Power stage shutdown. Profiler parameters not valid (check sign)",
	0x00007388,	"Fault - Power stage shutdown. Following error > vel. fol. error window",
	0x00007387,	"Fault - Power stage shutdown. Following error > pos. fol. error window",
	0x00007386,	"Fault - Power stage shutdown. Velocity out of limits",
	0x00007385,	"Fault - Power stage shutdown. Position out of limits",
	0x00007382,	"Fault - Power stage shutdown. CRC checks overflow in SSI readings",
	0x00007380,	"Fault - Power stage shutdown. Position flags overflow in SSI readings",
	0x00007370,	"Fault - Power stage shutdown. Halls sequence error",
	0x00007371,	"Fault - Power stage shutdown. Halls combination error",
	0x00007372,	"Fault - Power stage shutdown. Digital encoder runaway error",
	//More to be done soon
};

//----------------------------
//--------- PARAMETERS -------
//----------------------------
/*
typedef struct sParameter {
	float actual;
	float target;
	float max;
	float min;
	bool enabled;
}sParameter;

typedef struct sDeviceParameters {
	sParameter pos;//Position
	sParameter vel;//Velocity
	sParameter acc;//Acceleration
}sDeviceParameters;

typedef struct sDeviceInfo {
	short				status;
	short				prev_status = 0;//Previous status
	sDeviceParameters	params;
}sDeviceInfo;*/
//---------------------------

/*
static struct cmd const newCommands[] =
{
	//{Address_slave ADP (uint16_t), Address_slave ADO (uint16_t), numBytes, EtherCAT command type, Description}
	{ 0x0000, 0x2010, sizeof(uint16_t),	BRD,	"Reads 0x010 axis-1 (Control Word - RW)"},//Writes first ECM_SLAVE_ADDR type union variables (two uint16_t), so uint32_t is indirectly set too (they form a union).
	{ 0x0000, 0x2011, sizeof(uint16_t),	BRD,	"Reads 0x011 axis-1 (Status Word - RO)"},
	{ 0x0000, 0x2014, sizeof(int8_t),	BWR,	"Writes 0x014 axis-1 (Operation mode - RW)"},
	{ 0x03E9, 0x2014, sizeof(int8_t),	FPRD,	"Reads 0x014 axis-1 (Operation mode - RW)"},//Slave addressing (Configured Address Read mode)
	{ 0x0000, 0x2014, sizeof(int8_t),	BRD,	"Reads 0x014 axis-1 (Operation mode - RW)"},//Broadcast Read. 0x015 (Operation mode display - RO) NOT WORKING YET! So it is changed to 0x2014
	{ 0x0006, 0x0100, sizeof(uint8_t),	LRW,	"Logical Read/Write 0x01000006 (Operation Mode - RW)"},//Test!!!
	{ 0x0006, 0x0100, sizeof(uint8_t),	LRD,	"Logical Read 0x01000006 (Operation Mode - RW)"},//Test!!!
	//{ 0x03E9, 0x2010, sizeof(uint16_t),	FPWR,	"Writes 0x010 axis-1 (Control Word - RW)"},//Shutdown
	{ 0x03E9, 0x2010, sizeof(uint16_t),	FPWR,	"Writes 0x010 axis-1 (Control Word - RW)"},//Switch on
	{ 0x03E9, 0x2010, sizeof(uint16_t),	FPWR,	"Writes 0x010 axis-1 (Control Word - RW)"},//Switch on + Enable operation
	{ 0x0000, 0x2010, sizeof(uint16_t),	BRD,	"Reads 0x010 axis-1 (Control Word - RW)"},
	//{ 0x0000, 0x2030, sizeof(int32_t),	BRD,	"Reads 0x030 axis-1 (Actual Position - RO)"},
	//{ 0x0000, 0x6064, sizeof(int32_t),	BRD,	"Reads 0x6064 (Position Actual Value - RO)"},
	{ 0x0000, 0x2011, sizeof(uint16_t),	BRD,	"Reads 0x011 axis-1 (Status Word - RO)"},
};*/

/*
uint16_t	value		= 0;		//This is the value
void		*pData		= &value;	//Pointer to the value
uint16_t	pucCnt_val	= 0;		//These are WC values
uint16_t	*pucCnt		= &pucCnt_val;		//Pointer to the counter value
//uint16_t	pucCnt_val[NUMCOMMANDS]	= { 0,0,0,0,0,0,0,0 };	//These are WC values
//uint16_t	*pucCnt			= &pucCnt_val[0];		//Pointer to the counter value

int ecmAsyncRequest_cnt = 0;		//Counter of ecmAsyncRequest() done
*/

#endif