#include <iostream>
#include <cstdlib>
#include "EasyBMP/EasyBMP.h"
#include "rtMath.h"
#include "rtVec3.h"

using std::cout;
using std::endl;

int main( const int argc, char* argv[] )
{
	int width = 800;
	int height = 600;
	float fovx = rt::halfPi;

	for ( int argi = 1; argi < argc; ++argi )
	{
		if ( !strcmp( argv[argi], "-w" ) )
		{
			if ( argc > argi + 1 ) width = atoi( argv[++argi] );
		}
		else if ( !strcmp( argv[argi], "-h" ) )
		{
			if ( argc > argi + 1 ) height = atoi( argv[++argi] );
		}
		else if ( !strcmp( argv[argi], "-f" ) )
		{
			if ( argc > argi + 1 ) fovx = atof( argv[++argi] );
		}
		else
		{
			cout << "Argument \"" << argv[argi] << "\" not recognised." << endl;
		}
	}

	float fovy = fovx * (float) height / (float) width;

    cout << width << " " << height << " " << fovx << " " << fovy << endl;
    return 0;
}
