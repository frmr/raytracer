#include <iostream>
#include <cstdlib>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

#include "EasyBMP/EasyBMP.h"

#include "rtMath.h"
#include "rtRayTracer.h"
#include "rtVec3.h"

using std::cout;
using std::endl;
using std::string;

std::mutex rayLock;

void SetupScene( rt::RayTracer& rayTracer )
{
	rayTracer.AddSphere( rt::Vec3( -5.0f, 0.0f, 10.0f ), 1.0f, rt::Vec3( 1.0f, 0.0f, 0.0f ), 0.5f );
	rayTracer.AddSphere( rt::Vec3( 0.0f, -5.0f, 10.0f ), 1.0f, rt::Vec3( 0.0f, 1.0f, 0.0f ), 0.5f );
	rayTracer.AddSphere( rt::Vec3( 0.0f, 0.0f, 15.0f ), 1.0f, rt::Vec3( 0.0f, 0.0f, 1.0f ), 0.5f );
	rayTracer.AddSphere( rt::Vec3( 0.0f, 5.0f, 20.0f ), 1.0f, rt::Vec3( 1.0f, 0.0f, 1.0f ), 0.5f );
	rayTracer.AddSphere( rt::Vec3( 5.0f, 0.0f, 20.0f ), 1.0f, rt::Vec3( 0.0f, 1.0f, 1.0f ), 0.5f );
	//rayTracer.AddSphere( rt::Vec3( 3.0f, 3.0f, 6.0f ), 1.0f, rt::Vec3( 0.0f, 1.0f, 1.0f ), 0.5f );

	rayTracer.AddTriangle( rt::Vec3( -20.0f, 20.0f, 0.0f ), rt::Vec3( -20.0f, 20.0f, 25.0f ), rt::Vec3( -20.0f, -20.0f, 0.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.5f );
	rayTracer.AddTriangle( rt::Vec3( -20.0f, 20.0f, 25.0f ), rt::Vec3( -20.0f, -20.0f, 25.0f ), rt::Vec3( -20.0f, -20.0f, 0.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.5f );

	rayTracer.AddTriangle( rt::Vec3( 20.0f, 20.0f, 25.0f ), rt::Vec3( 20.0f, 20.0f, 0.0f ), rt::Vec3( 20.0f, -20.0f, 0.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.5f );
	rayTracer.AddTriangle( rt::Vec3( 20.0f, 20.0f, 25.0f ), rt::Vec3( 20.0f, -20.0f, 0.0f ), rt::Vec3( 20.0f, -20.0f, 25.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.5f );

	rayTracer.AddTriangle( rt::Vec3( -20.0f, 20.0f, 25.0f ), rt::Vec3( 20.0f, 20.0f, 25.0f ), rt::Vec3( -20.0f, -20.0f, 25.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.5f );
	rayTracer.AddTriangle( rt::Vec3( 20.0f, 20.0f, 25.0f ), rt::Vec3( 20.0f, -20.0f, 25.0f ), rt::Vec3( -20.0f, -20.0f, 25.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.5f );

	rayTracer.AddLight( rt::Vec3( 5.0f, 5.0f, 5.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 1.0f );
}

void SampleRayTracer( const rt::RayTracer& rayTracer, const int width, const int height, rt::Vec3& rayVector, int& x, int& y, BMP& output )
{
	rt::Vec3 myVector;
	int myX, myY;

	rayLock.lock();
	while ( x < width )
	{
		myVector = rayVector.Unit();
		myX = x;
		myY = y++;
		rayVector.y--;

		if ( y == height )
		{
			y = 0;
			rayVector.y = height / 2.0f - 0.5f;

			x++;
			rayVector.x++;
		}
		rayLock.unlock();

		rt::Vec3 sampleColor;
		rayTracer.Sample( myVector, sampleColor );
		rayLock.lock();
			output( myX, myY )->Red = (int) ( sampleColor.x * 255.0f );
			output( myX, myY )->Green = (int) ( sampleColor.y * 255.0f );
			output( myX, myY )->Blue = (int) ( sampleColor.z * 255.0f );
		rayLock.unlock();
	}
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

	float distToProjPlane = ( (float) width / 2.0f ) / tan( fovX / 2.0f );

    cout << "Using width: " << width << ", height: " << height << ", fovX: " << fovX << ", fovy: " << fovY << endl;
    cout << "Will output to " << outputFilename << ".bmp" << endl;

	rt::RayTracer rayTracer;
	SetupScene( rayTracer );

	BMP output;
	output.SetSize( width, height );

	const int maxThreads = 1;
	vector<std::thread> threads;

	int x = 0;
	int y = 0;
	rt::Vec3 rayVector( -width / 2.0f + 0.5f, height / 2.0f - 0.5f, distToProjPlane );

	for ( int i = 0; i  < maxThreads; ++i )
	{
		threads.push_back( std::thread( SampleRayTracer, std::cref(rayTracer), width, height, std::ref( rayVector ), std::ref(x), std::ref(y), std::ref(output) ) );
	}

	cout << "Using " << threads.size() << " threads." << endl;

	for ( vector<std::thread>::iterator it = threads.begin(); it != threads.end(); ++it )
	{
		it->join();
	}

	cout << "Outputting to " << outputFilename << ".bmp" << endl;

	output.WriteToFile( ( outputFilename + ".bmp" ).c_str() );

	cout << "Done" << endl;

    return 0;
}
