#pragma once

#include "framework.h"
#include "ecm.h"
#include <map>
//#include "RobGUI.h"

#ifndef ECMW_H
#define ECMW_H


#if defined(__cplusplus)
extern "C" {//"C" mode
#endif
//Solved from link: https://stackoverflow.com/questions/30581837/linker-error-when-calling-a-c-function-from-c-code-in-different-vs2010-project
//This file/class must be compiled as "C"


//EtherCAT command types, numerically identified
typedef enum ethercatCommandTypes
{
	NOP = 0, APRD, APWR, APRW,
	FPRD, FPWR, FPRW,
	BRD, BWR, BRW,
	LRD, LWR, LRW,
	ARMW
}ethercatCommandTypes;

typedef struct _PDMapP {
	//TPDO Inputs
	uint8_t *pDio_StatusWord;//"Status mode" pointer				(CiA)
	uint8_t *pDio_PositionActual;//"Position actual" pointer		(CiA) - Motor(inner)
	uint8_t *pDio_VelocityActual;//"Velocity actual" pointer		(CiA) - Motor(inner)
	uint8_t *pDio_DisplayOperationMode;//"Operation mode display" pointer		(CiA)
	uint8_t *pDio_PrimaryTemperature;//"Primary temperature value" pointer	(Axis1)
	uint8_t *pDio_MotorTemperature;//"Motor temperature value" pointer		(Axis1)
	uint8_t *pDio_ModulePositionActual;//"BISS-C slave 1 / Primary SSI-Position" pointer		(Axis1) - Module(outter)
	uint8_t *pDio_BusVoltage;//"Bus voltage value" pointer		0x2060(Axis1) - Drive
	uint8_t *pDio_LastError;//"Last error" pointer				(Axis1)
	uint8_t *pDio_StatusWordAxis1;//"Status mode" pointer			(Axis1)
	uint8_t *pDio_CurrentA;//"Current A" pointer					(Axis1)
	uint8_t *pDio_CurrentB;//"Current B" pointer					(Axis1)
	uint8_t *pDio_CurrentC;//"Current C" pointer					(Axis1)
	//RPDO Outputs
	uint8_t *pDio_OperationMode;//"Operation mode" pointer 		0x6060 (CiA)
	uint8_t *pDio_ControlWord;//"Control Word" pointer			0x6040 (CiA)
	uint8_t *pDio_TargetPosition;//"Target position" pointer		0x607A (CiA)
	uint8_t *pDio_TargetVelocity;//"Target velocity" pointer		0x60FF (CiA)
	uint8_t *pDio_ProfilerMaxVel;//"Profiler max. velocity" pointer
	uint8_t *pDio_ProfilerMaxAcc;//"Profiler max. acceleration" pointer
	uint8_t *pDio_ProfilerMaxDec;//"Profiler max. deceleration" pointer
	uint8_t *pDio_OperationModeAxis1;//"Operation mode" pointer	0x2014 (Axis1)
	uint8_t *pDio_ControlWordAxis1;//"Control Word" pointer		0x2010 (Axis1)
	uint8_t *pDio_PositionSetPoint;//"Target position" pointer	0x2020 (Axis1) - Motor(inner)
	uint8_t *pDio_VelocitySetPoint;//"Target velocity" pointer	0x2021 (Axis1) - Motor(inner)
}PDMapP, *PPDMapP;

/*
struct cmd
{
	ECM_SLAVE_ADDR			address;
	uint16_t				numBytes;
	ethercatCommandTypes	cmdType;
	const char *			Description;
};
*/

class CecmW {

//VARIABLES SECTION
	public:

		//ECM_NIC NicList;
		char PrimaryNIC[64];	//For external access
		char SecondaryNIC[64];	//For external access

		std::map<uint32_t, CString> errorCodesMap;//Struct will be loaded into map for easier search later
		//std::map<CString, uint32_t> errorCodesMap2;//Struct will be loaded into map for easier search later

		PDMapP slavePD[2];//An array that contains all pointers to PD variables

	private:
		char PrimaryNICNameDefault[64]	= "TwinCAT Gigabit Primary Adapter";//Now is fixed, TODO: read/load from NIC adapters array
		char SecondaryNICNameDefault[64]= "TwinCAT Gigabit Secondary Adapter";//Now is fixed, TODO: read/load from NIC adapters array

		uint8_t PDMapSizeBytes = sizeof(PDMapP);//Size (Bytes) of PDMap structure
	//protected:
		
//FUNCTIONS SECTION
	public:
		//////////////////////////////////////
		//Advanced version with Windows GUI
		////////////////////////////////////////
		CecmW(void);
		~CecmW(void);


		/*Matrix fillers. They are loaded with a set of register adresses & values
		*	@param	selSlave	Numerical ID of selected slave
		*/
		void fillProfilePosMatrix(int selSlave = 0);
		void fillProfileVelMatrix(int selSlave = 0);

		/* Main ECM thread function. Initializes EtherCAT communications
		*	
		* @param	argc		Arg counter
		* @param	argv		Pointer to arguments array
		* @param	is_running	Pointer to main GUI thread to know if EXIT/END is click
		*
		* @return				0 when finishes
		*/
		int Init(int argc, char *argv[], bool *is_running);

		/* Executes a single ramp motion
		* Ends with driver at MOTION state (Control Word = 31)
		* Sets next iteration start from OPERATION state
		* @param iTargetPos		Target Position in absolute [counts]
		* @param	selSlave	Numerical ID of selected slave
		* @return				TRUE when MOTION command is written to "Control Word" register, otherwise is FALSE (so it will still being called until it ends)
		*/
		bool MotionFRampMode(int iTargetPos, int selSlave = 0);
		
		/* Executes a single velocity motion. Not useful for dynamic/variable velocity control!
		* Ends with driver at MOTION state (Control Word = 2063)
		* Sets next iteration start from OPERATION state
		* @param iTargetVel		Target Velocity in [inc/s]. Target velocity register (0x60FF)
		* @return				TRUE when MOTION command is written to "Control Word" register, otherwise is FALSE (so it will still being called until it ends)
		*/
		static bool MotionFVelMode(int iTargetVel_inc_s);//old

		/* Executes a velocity motion. Useful for dynamic/variable velocity control!
		* Ends with driver at MOTION state (Control Word = 2063)
		* Sets next iteration start from OPERATION state (so another velocity can be set after)
		* @param fProfileVel_rev_s	Target Velocity in [rev/s], Velocity set-point register (0x021)
		* @param	selSlave	Numerical ID of selected slave
		* @return					TRUE when MOTION command is written to "Control Word" register, otherwise is FALSE (so it will still being called until it ends)
		*/
		bool MotionFProfileVelMode(float fProfileVel_rev_s, int selSlave = 0);

		/* Stops motor motion, waits in Operation enabled state.
		* This means writing a 15(decimal) value in "Control Word" register
		* @param	selSlave	Numerical ID of selected slave
		*/
		void MotionStop(int selSlave = 0);

		/* Resets (clear) STATUS FAULT bit (7th/8) by writing a decimal 128 in "Control Word" (Axis-1) register
		* @param	selSlave	Numerical ID of selected slave
		*/
		void StatusFaultReset(int selSlave = 0);


		/* Sends Asynchronous Requests to EtherCAT Slave (p.109)
		*
		* @return	On success, the function returns ECM_SUCCESS. On error, one of the error codes available
		*/
		static int SendAsyncRequest(void *pData, uint8_t cmdType, ECM_SLAVE_ADDR regAddress, uint16_t numBytes, uint16_t *pucCnt);

		//---------
		// PD map
		//---------

		/* Load pointers into process variables structure. Each call is for a single type but for many slaves.
		*
		* @param	hndMaster	An ECM_HANDLE pointer to MASTER
		* @param	str			String to search a register in PM
		* @param	VarDesc		An ECM_VAR_DESC pointer
		* @param	direction	Either EMC_INPUT_DATA or EMC_OUTPUT_DATA
		*
		* @return				TRUE(1) if succeed, FALSE otherwise
		*/
		bool SetPDmapP(ECM_HANDLE hndMaster, CString str, uint8_t **ptr, ECM_PROC_DATA_TYPE direction);

		/* Gets references to virtual and real process variables.
		*
		* @param	hndMaster	An ECM_HANDLE pointer to MASTER
		*/
		void ecmSetupProcesData(ECM_HANDLE hndMaster);


		//---------------------------
		// Device Parameters GETTERS
		//---------------------------

		/* Gets 'Status word' (CiA) register of a selected Slave - Motor(inner)
		*	@param	selSlave	Numerical ID of selected slave
		*	@return				Two-bytes with 'Status word' value
		*/
		short	GetStatus(int selSlave = 0);
		/* Gets 'Position Actual' (CiA) register of a selected Slave - Motor(inner)
		*	@param	selSlave	Numerical ID of selected slave
		*	@return				Four-bytes with 'Position actual' value in [enc. counts]. Relative: value is 0 after power on.
		*/
		int		GetActPos(int selSlave = 0);
		/* Gets 'Velocity Actual' (CiA) register of a selected Slave - Motor(inner)
		*	@param	selSlave	Numerical ID of selected slave
		*	@return				Four-bytes with 'Velocity value' in [rev/s]
		*/
		int		GetActVel(int selSlave = 0);	
		/* Gets 'Primary Temperature' (Axis1) register of a selected Slave - Drive
		*	@param	selSlave	Numerical ID of selected slave
		*	@return				Four-bytes (real) with 'Status word' value
		*/
		float	GetTemperaturePrimary(int selSlave = 0);
		/* Gets 'Motor Temperature' (Axis1) register of a selected Slave - Motor(inner)
		*	@param	selSlave	Numerical ID of selected slave
		*	@return				Four-bytes (real) with 'Status word' value
		*/
		float	GetTemperatureMotor(int selSlave = 0);
		/* Gets 'Module Position Actual' (Axis1) register of a selected Slave - Module(outter)
		*	@param	selSlave	Numerical ID of selected slave
		*	@return				Four-bytes (real) with 'Status word' value
		*/
		float	GetModuleActPos(int selSlave = 0);
				
		
		/* Gets "Current[A]" readings (A, B and C phases) from Driver/Motor
		* @param selSlave	Numerical ID of selected slave
		* @param *cA		A pointer where CurrentA (real) will be written (typ. at motorParameters[])
		* @param *cB		A pointer where CurrentB (real) will be written 
		* @param *cC		A pointer where CurrentC (real) will be written 
		*/
		void	GetCurrentsABC(double *cA, double *cB, double *cC, int selSlave = 0);

		/*
		* Gets "Bus voltage value [V]" reading from Drive/Motor
		* @param	selSlave	Numerical ID of selected slave
		* @return				Bus voltage (real) [V]
		*/
		float	GetBusVoltage(int selSlave = 0);

		/* Gets "Last error" register
		* @param	selSlave	Numerical ID of selected slave
		* @return				Last error code (decimal)
		*/
		int		GetLastError(int selSlave = 0);			//Axis1 register

		//---------------------------
		// Device Parameters SETTERS
		//---------------------------

		/* Sets Profiler limits: MaxVel MaxAcc and MaxDec
		* @param selSlave		Numerical ID of selected slave
		* @param *fLoopMaxVel	A pointer to Profiler/Motor Max. Velocity in [rev/s]
		* @param *fLoopMaxAcc	A pointer to Profiler/Motor Max. Acc&Dec. in [rev/s]
		*/
		void SetProfiler(float *fLoopMaxVel, float *fLoopMaxAcc, int selSlave = 0);
		void SetProfilerMaxVel(float fVel, int selSlave = 0);
		void SetProfilerMaxAcc(float fAcc, int selSlave = 0);
		void SetProfilerMaxDec(float fDec, int selSlave = 0);

		/* Sets Profiler in operational state (0x0F - No motion)
		* @param selSlave		Numerical ID of selected slave
		*/
		void SetOperationalState(int selSlave = 0);

		/*
		* Set a Target Velocity to the drive
		* @param	fLoopMaxVel	Velocity in [units?]
		*/
		//void		SetTargetVel(float fVel);//old?


};

#if defined(__cplusplus)
}//End of "C" mode
#endif

#endif