// first.cpp

/*    Copyright 2009 10gen Inc.
*
*    Licensed under the Apache License, Version 2.0 (the "License");
*    you may not use this file except in compliance with the License.
*    You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
*    Unless required by applicable law or agreed to in writing, software
*    distributed under the License is distributed on an "AS IS" BASIS,
*    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*    See the License for the specific language governing permissions and
*    limitations under the License.
*/

/**
* this is a good first example of how to use mongo from c++
*/

// It is the responsibility of the mongo client consumer to ensure that any necessary windows
// headers have already been included before including the driver facade headers.
#include "stdafx.h"
//#if defined(_WIN32)
//#include <winsock2.h>
//#include <windows.h>
//#endif
//
//#include "mongo/client/dbclient.h"
//#include <boost/system/config.hpp>

#include "Simulator.h"
#include "VicsecPPInterractor.h"
#include "UniformNoiseRotation.h"
#include "RectangularTransitionalBorders.h"
#include "DataSnap.h"
#include "SimpleSaver.h"

int main(int argc, const char **argv)
{
	Simulator::CSimulator s(200, 10, 10,
		Simulator::CVicsecPPInterractor(),
		Simulator::CRectangularTransitionalBorders(10, 10),
		Simulator::CUniformNoiseRotation());

	s.ChangeNoise(100);

	int i = 0;
	while (i++ < 2)
	{
		std::cout << i;
		s.Interract();
		std::cout << i;
	}

	CDataSnap dSnap(s);

	CSimpleSaver saver;
	saver.SaveVelocityVsNoise("VelocityVsNoise.txt", dSnap);

	return EXIT_SUCCESS;
}