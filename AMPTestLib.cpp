// AMPTestLib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "AMPTestLib.h"
using namespace std;
using namespace concurrency;

namespace ccy = concurrency;

//function that outputs description and paths of a device
AMP_LIB_EXT void EnumerateAccelerators()
{
	vector<ccy::accelerator> acltors = ccy::accelerator::get_all();	//get devices

	//output information
	for (auto &a : acltors)
	{
		wcout << "Description: " << a.get_description() << "\n";
		wcout << "\tDevice path: " << a.get_device_path() << "\n\n";
	}
}


