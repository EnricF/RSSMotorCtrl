#pragma once



#include "framework.h"
//#include "RobGUI.h"


#ifndef ECMTEST_H
#define ECMTEST_H

//OPERATION MODE REGISTER
#define POS_M			0x01//To write on Operation mode CiA register
#define VEL_M			0x03//To write on Operation mode CiA register
#define PROFILE_VEL_M	0x13//To write on Axis1 Operation mode register (with profiler!)
#define PROFILE_POS_M	0x14//To write on Axis1 Operation mode register (with profiler!)

//CONTROL WORD REGISTER
#define	CTRLW_SHUTDOWN	0x06
#define	CTRLW_SWITCHON	0x07
#define	CTRLW_OPERATION	0x0F
#define	CTRLW_POSMOTION	0x1F
#define	CTRLW_VELMOTION 0x08//MSByte. It's LSByte is 0x0F (CTRLW_XXXXXX). TWO BYTES in total!
#define	CTRLW_FAULT_RST	0x80

#if defined(__cplusplus)
extern "C" {//"C" mode
#endif

enum driveSTATE {
	SHUTDOWN = 0,	//POWER ON and POWER OFF states (standby)
	SWITCHON,		//second state after POWER ON (standby)
	OPERATION,		//Motor has power, ready to receive MOTION parameters
	MOTION			//MOTOR is in MOTION
};


class CecmTest {
		//Solved from link: https://stackoverflow.com/questions/30581837/linker-error-when-calling-a-c-function-from-c-code-in-different-vs2010-project
		//This code and its "descending" must be compiled as "C"
	
	public:
		FILE * ECMlogFile;
			   
		int driveState; //To known about which state are the drive&motor pack
		bool InitFuncFinish;//Boolean to know if this function ends

	public:
		//Base version using Windows GUI
		int main(int argc, char *argv[]);
		DWORD WINAPI emcThreadFunction(LPVOID lpParam);

		//////////////////////////////////////
		//Advanced version with Windows GUI
		////////////////////////////////////////
		CecmTest(void);
		~CecmTest(void);

		int Init(int argc, char *argv[], bool *is_running);

		/*
		* Creates the commands sequence for a ramp mode motion
		*/
		//static void writePDRampSend(float fTargetPos);
		/*
		* Creates the commands sequence for a ramp mode motion
		*/
		//static void writePDRampSend(int iTargetPos);

		/*
		* Executes a single ramp motion
		* Ends with driver at MOTION state (Control Word = 31)
		* Sets next iteration start from OPERATION state
		* @param iTargetPos		Target Position in absolute [counts]
		* @return				TRUE when MOTION command is written to "Control Word" register, otherwise is FALSE (so it will still being called until it ends)
		*/
		static bool MotionFRampMode(int iTargetPos);
		/*
		* Executes a single velocity motion. Not seful for dynamic velocity control!
		* Ends with driver at MOTION state (Control Word = 2063)
		* Sets next iteration start from OPERATION state
		* @param iTargetVel		Target Velocity in [inc/s]
		* @return				TRUE when MOTION command is written to "Control Word" register, otherwise is FALSE (so it will still being called until it ends)
		*/
		static bool MotionFVelMode(int iTargetVel);

		/*
		* Executes a single velocity motion. Useful for dynamic velocity control!
		* Ends with driver at MOTION state (Control Word = 2063)
		* Sets next iteration start from OPERATION state
		* @param fProfileVel	Target Velocity in [mrev/s]
		* @return				TRUE when MOTION command is written to "Control Word" register, otherwise is FALSE (so it will still being called until it ends)
		*/
		static bool MotionFProfileVelMode(float fProfileVel);

		/*
		* Stops motor motion, this means writing a 15(decimal) value in "Control Word" register
		*/
		void MotionStop(void);

		// Device Parameters GETTERS
		short		GetStatus(void);//CiA register
		static int	GetActPos(void);//CiA register
		static int	GetActVel(void);//CiA register

		// Device Parameters SETTERS
		/*
		* Set a Target Velocity to the drive
		* @param	fLoopMaxVel	Velocity in [units?]
		*/
		void		SetTargetVel(float fVel);

		/*
		* Sets Profiler limits: MaxVel MaxAcc and MaxDec
		*/
		static void SetProfiler(float *fLoopMaxVel, float *fLoopMaxAcc);
		static void SetProfilerMaxVel(float fVel);
		static void SetProfilerMaxAcc(float fAcc);
		static void SetProfilerMaxDec(float fDec);
};

#if defined(__cplusplus)
}//End of "C" mode
#endif

#endif