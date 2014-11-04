#include <iostream>
#include <cstdlib>
#include "EasyBMP/EasyBMP.h"

#include "rtRayTracer.h"
#include "rtVec3.h"

using std::cout;
using std::endl;

int main( const int argc, char* argv[] )
{
	int width = 800;
	int height = 600;
	float fovX = rt::halfPi;

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
			if ( argc > argi + 1 ) fovX = atof( argv[++argi] );
		}
		else
		{
			cout << "Argument \"" << argv[argi] << "\" not recognised." << endl;
		}
	}

	float fovY = fovX * (float) height / (float) width;

    cout << "Using width: " << width << ", height: " << height << ", fovX: " << fovX << ", fovy: " << fovY << endl;

	rt::RayTracer ray();

	float fovIncX = fovX / width;
	float fovIncY = fovY / height;

	float fovSampleX = -fovX / 0.5f;
	float fovSampleY = -fovY / 0.5f;

	for ( int x = 0; x < width; ++x )
	{
		for ( int y = 0; y < height; ++y )
		{


			fovSampleY += fovIncY;
		}

		fovSampleX += fovIncX;
	}

    return 0;
}
