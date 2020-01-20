#pragma once



#include "framework.h"
//#include "RobGUI.h"


#ifndef ECMTEST_H
#define ECMTEST_H


//Solved from link: https://stackoverflow.com/questions/30581837/linker-error-when-calling-a-c-function-from-c-code-in-different-vs2010-project
//This code and its "descending" must be compiled as "C"
#if defined(__cplusplus)
extern "C" {
#endif

	int main(int argc, char *argv[]);
	DWORD WINAPI emcThreadFunction(LPVOID lpParam);



#if defined(__cplusplus)
}
#endif

#endif