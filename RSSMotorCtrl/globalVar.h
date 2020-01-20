#pragma once

#ifndef GLOBALVAR_H
#define GLOBALVAR_H


//ECM template project
#include "getopt.h"
#include "ecm.h"
#include "stdint.h"

//#include "ecmDrive.h"
#include "ecmTest.h"

//Thread related
#include <Windows.h>
#include <strsafe.h>
#include <tchar.h>

#define NUMCOMMANDS 2 //How many acyclic commands you want to send;

//To pass command line execution arguments to a Windows Thread
typedef struct MyData {
	int argc;
	char ** argv;
} MYDATA, *PMYDATA;

//EtherCAT command types, numerically identified
typedef enum ethercatCommandTypes
{
	NOP = 0, APRD, APWR, APRW,
	FPRD, FPWR, FPRW,
	BRD, BWR, BRW,
	LRD, LWR, LRW,
	ARMW
}ethercatCommandTypes;


struct cmd
{
	ECM_SLAVE_ADDR			address;
	uint16_t				numBytes;
	ethercatCommandTypes	cmdType;
	const char *			Description;
};

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
};
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