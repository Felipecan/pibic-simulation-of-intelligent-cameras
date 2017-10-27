#include <stdio.h>
#include <iostream>
#include <unistd.h>

#include <string.h>
#include "VirtualBusFederate.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdlib.h>

using namespace cv;
using namespace std;

int main( int argc, char **argv )
{
	if( argc > 1 )
	{
		// create and run the federate
		VirtualBusFederate *federate;
		federate = new VirtualBusFederate();
		federate->runFederate( argv[1] );

		federate->loop(0);
		federate->finalize();

		// clean up
		delete federate;
	}
	
	return 0;
}
