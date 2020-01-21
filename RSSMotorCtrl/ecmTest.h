#pragma once



#include "framework.h"
//#include "RobGUI.h"


#ifndef ECMTEST_H
#define ECMTEST_H

#if defined(__cplusplus)
extern "C" {//"C" mode
#endif

class CecmTest {
		//Solved from link: https://stackoverflow.com/questions/30581837/linker-error-when-calling-a-c-function-from-c-code-in-different-vs2010-project
		//This code and its "descending" must be compiled as "C"
	
	public:
		FILE * ECMlogFile;

	public:
		//Base version using Windows GUI
		int main(int argc, char *argv[]);
		DWORD WINAPI emcThreadFunction(LPVOID lpParam);

		//////////////////////////////////////
		//Advanced version with Windows GUI
		////////////////////////////////////////
		CecmTest(void);
		~CecmTest(void);

		int Init(int argc, char *argv[], bool is_running);
		//int Init(int argc, char *argv[]);
		//int Init(void);

		/*
		* Creates the commands sequence for a ramp mode motion
		*/
		static void writePDRampSend(float fTargetPos);
		/*
		* Creates the commands sequence for a ramp mode motion
		*/
		static void writePDRampSend(int iTargetPos);

		/*
		* Executes a single ramp motion
		*/
		//static bool MotionFRampMode(float fTargetPos);
		/*
		* Executes a single ramp motion
		*/
		static bool MotionFRampMode(int iTargetPos);
};

#if defined(__cplusplus)
}//End of "C" mode
#endif

#endif