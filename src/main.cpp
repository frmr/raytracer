#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>

#include "EasyBMP/EasyBMP.h"

#include "rtMath.h"
#include "rtRayTracer.h"
#include "rtVec3.h"

using std::cout;
using std::endl;
using std::string;

void sampleRayTracer( const rt::Vec3 rayVector, BMP& output )
{

}

int main( const int argc, char* argv[] )
{
	int width = 800;
	int height = 600;
	float fovX = rt::halfPi;
	string outputFilename = "output";

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
		else if ( !strcmp( argv[argi], "-o" ) )
		{
			if ( argc > argi + 1 ) outputFilename = argv[++argi];
		}
		else
		{
			cout << "Argument \"" << argv[argi] << "\" not recognised." << endl;
		}
	}

	float fovY = fovX * (float) height / (float) width;

	float distToProjPlane = (float) ( width / 2 ) / tan( fovX / 2.0f );

    cout << "Using width: " << width << ", height: " << height << ", fovX: " << fovX << ", fovy: " << fovY << endl;
    cout << "Will output to " << outputFilename << ".bmp" << endl;

	rt::RayTracer rayTracer;
	rayTracer.AddSphere( rt::Vec3( 0.0f, 0.0f, 10.0f ), 1.0f, rt::Vec3( 1.0f, 0.0f, 0.0f ), 0.5f );
	rayTracer.AddLight( rt::Vec3( 5.0f, 5.0f, 5.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 1.0f );

	BMP output;
	output.SetSize( width, height );

	vector<std::thread> threads;
	int maxThreads = 4;

	rt::Vec3 rayVector( ( -width / 2.0f ) + 0.5f, 0.0f, distToProjPlane );

	for ( int x = 0; x < width; ++x )
	{
		rayVector.y = ( -height / 2.0f ) + 0.5f;

		for ( int y = 0; y < height; ++y )
		{
			//cout << rayVector.x << "\t" << rayVector.y << "\t" << rayVector.z << endl;
			rt::Vec3 sampleColor;
			rayTracer.Sample( rayVector, sampleColor );
			output( x, y )->Red = (int) ( sampleColor.x * 255.0f );
			output( x, y )->Green = (int) ( sampleColor.y * 255.0f );
			output( x, y )->Blue = (int) ( sampleColor.z * 255.0f );

			rayVector.y++;
		}

		rayVector.x++;
	}

	cout << "Outputting to " << outputFilename << ".bmp" << endl;

	output.WriteToFile( ( outputFilename + ".bmp" ).c_str() );

	cout << "Done" << endl;

    return 0;
}
