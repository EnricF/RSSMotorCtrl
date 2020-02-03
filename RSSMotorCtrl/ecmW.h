#pragma once

#include "framework.h"
#include "ecm.h"
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
	private:

	//protected:
		
//FUNCTIONS SECTION
	public:
		//////////////////////////////////////
		//Advanced version with Windows GUI
		////////////////////////////////////////
		CecmW(void);
		~CecmW(void);

		/*
		* Main ECM thread function
		* Initializes EtherCAT communications
		*	
		* @param	argc		Arg counter
		* @param	argv		Pointer to arguments array
		* @param	is_running	Pointer to main GUI thread to know if EXIT/END is click
		*
		* @return				0 when finishes
		*/
		int Init(int argc, char *argv[], bool *is_running);

		/*
		* Executes a single ramp motion
		* Ends with driver at MOTION state (Control Word = 31)
		* Sets next iteration start from OPERATION state
		* @param iTargetPos		Target Position in absolute [counts]
		* @return				TRUE when MOTION command is written to "Control Word" register, otherwise is FALSE (so it will still being called until it ends)
		*/
		static bool MotionFRampMode(int iTargetPos);
		
		/*
		* Executes a single velocity motion. Not useful for dynamic/variable velocity control!
		* Ends with driver at MOTION state (Control Word = 2063)
		* Sets next iteration start from OPERATION state
		* @param iTargetVel		Target Velocity in [inc/s]. Target velocity register (0x60FF)
		* @return				TRUE when MOTION command is written to "Control Word" register, otherwise is FALSE (so it will still being called until it ends)
		*/
		static bool MotionFVelMode(int iTargetVel_inc_s);

		/*
		* Executes a velocity motion. Useful for dynamic/variable velocity control!
		* Ends with driver at MOTION state (Control Word = 2063)
		* Sets next iteration start from OPERATION state (so another velocity can be set after)
		* @param fProfileVel_rev_s	Target Velocity in [rev/s], Velocity set-point register (0x021)
		* @return					TRUE when MOTION command is written to "Control Word" register, otherwise is FALSE (so it will still being called until it ends)
		*/
		static bool MotionFProfileVelMode(float fProfileVel_rev_s);

		/*
		* Stops motor motion, waits in Operation enabled state.
		* This means writing a 15(decimal) value in "Control Word" register
		*/
		void MotionStop(void);

		/*
		* Resets (clear) STATUS FAULT bit (7th/8) by writing a decimal 128 in "Control Word" (Axis-1) register
		*/
		void StatusFaultReset(void);


		/*
		* Sends Asynchronous Requests to EtherCAT Slave (p.109)
		*
		* @return	On success, the function returns ECM_SUCCESS. On error, one of the error codes available
		*/
		static int SendAsyncRequest(void *pData, uint8_t cmdType, ECM_SLAVE_ADDR regAddress, uint16_t numBytes, uint16_t *pucCnt);

		//---------------------------
		// Device Parameters GETTERS
		//---------------------------
		static short	GetStatus(void);			//CiA register - Motor(inner)
		static int		GetActPos(void);			//CiA register - Motor(inner)
		static int		GetActVel(void);			//CiA register - Motor(inner)
		static float	GetTemperaturePrimary(void);//Axis1 register - Drive
		static float	GetTemperatureMotor(void);	//Axis1 register - Motor
		static float	GetModuleActPos(void);		//Axis1 register - Module(outter)
		
		/*
		* Gets "Current[A]" readings (A, B and C phases) from Driver/Motor
		* @param *cA	A pointer where CurrentA must be saved (typ. at motorParameters[])
		* @param *cB	A pointer where CurrentB must be saved
		* @param *cC	A pointer where CurrentC must be saved
		*/
		static void		GetCurrentsABC(double *cA, double *cB, double *cC);
		static int		GetLastError(void);			//Axis1 register

		//---------------------------
		// Device Parameters SETTERS
		//---------------------------

		/*
		* Sets Profiler limits: MaxVel MaxAcc and MaxDec
		* @param *fLoopMaxVel	A pointer to Profiler/Motor Max. Velocity in [rev/s]
		* @param *fLoopMaxAcc	A pointer to Profiler/Motor Max. Acc&Dec. in [rev/s]
		*/
		static void SetProfiler(float *fLoopMaxVel, float *fLoopMaxAcc);
		static void SetProfilerMaxVel(float fVel);
		static void SetProfilerMaxAcc(float fAcc);
		static void SetProfilerMaxDec(float fDec);

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