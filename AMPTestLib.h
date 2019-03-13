#pragma once

#ifndef _AMPTESTLIB_H
#define _AMPTESTLIB_H

#include <vector>
#include "amp_fixed.h"
#ifndef AMP_LIB_EXT

#ifdef BUILD_DLL
#define AMP_LIB_EXT __declspec(dllexport)
#else
#pragma comment(lib, "AMPTestLib.lib")
#define AMP_LIB_EXT __declspec(dllimport)
#endif	//end of BUILD_DLL

#endif	//end of AMP_LIB_EXT

AMP_LIB_EXT void EnumerateAccelerators();

#endif	//end of _AMPTESTLIB_H