#include <chrono>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

#include "EasyBMP/EasyBMP.h"

#include "rtMath.h"
#include "rtRayTracer.h"
#include "rtScreenBuffer.h"
#include "rtVec3.h"

using std::cout;
using std::endl;
using std::string;

std::mutex rayLock;
std::mutex bufferLock;

void SetupScene( rt::RayTracer& rayTracer )
{
	rayTracer.AddSphere( rt::Vec3( -8.0f, 0.0f, 10.0f ), 1.0f, rt::Vec3( 1.0f, 0.0f, 0.0f ), rt::Vec3( 1.0f, 0.0f, 0.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.75f );
	rayTracer.AddSphere( rt::Vec3( 0.0f, -7.0f, 10.0f ), 1.0f, rt::Vec3( 0.0f, 1.0f, 0.0f ), rt::Vec3( 0.0f, 1.0f, 0.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.75f );
	rayTracer.AddSphere( rt::Vec3( 0.0f, 0.0f, 15.0f ), 1.0f, rt::Vec3( 0.0f, 0.0f, 1.0f ), rt::Vec3( 0.0f, 0.0f, 1.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.75f );
	rayTracer.AddSphere( rt::Vec3( 0.0f, 5.0f, 20.0f ), 1.0f, rt::Vec3( 1.0f, 0.0f, 1.0f ), rt::Vec3( 1.0f, 0.0f, 1.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.75f );
	rayTracer.AddSphere( rt::Vec3( 5.0f, 0.0f, 20.0f ), 1.0f, rt::Vec3( 1.0f, 1.0f, 0.0f ), rt::Vec3( 1.0f, 1.0f, 0.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.75f );
	//rayTracer.AddSphere( rt::Vec3( 3.0f, 3.0f, 6.0f ), 1.0f, rt::Vec3( 0.0f, 1.0f, 1.0f ), 0.5f );

	//left side
	rayTracer.AddTriangle( rt::Vec3( -20.0f, 20.0f, 0.0f ), rt::Vec3( -20.0f, 20.0f, 25.0f ), rt::Vec3( -20.0f, -20.0f, 0.0f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.0f, 0.0f, 0.0f ), 0.0f );
	rayTracer.AddTriangle( rt::Vec3( -20.0f, 20.0f, 25.0f ), rt::Vec3( -20.0f, -20.0f, 25.0f ), rt::Vec3( -20.0f, -20.0f, 0.0f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.0f, 0.0f, 0.0f ), 0.0f );

	//right side
	rayTracer.AddTriangle( rt::Vec3( 20.0f, 20.0f, 25.0f ), rt::Vec3( 20.0f, 20.0f, 0.0f ), rt::Vec3( 20.0f, -20.0f, 0.0f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.0f, 0.0f, 0.0f ), 0.0f );
	rayTracer.AddTriangle( rt::Vec3( 20.0f, 20.0f, 25.0f ), rt::Vec3( 20.0f, -20.0f, 0.0f ), rt::Vec3( 20.0f, -20.0f, 25.0f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.0f, 0.0f, 0.0f ), 0.0f );

	//back
	rayTracer.AddTriangle( rt::Vec3( -20.0f, 20.0f, 25.0f ), rt::Vec3( 20.0f, 20.0f, 25.0f ), rt::Vec3( -20.0f, -20.0f, 25.0f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.0f, 0.0f, 0.0f ), 0.0f );
	rayTracer.AddTriangle( rt::Vec3( 20.0f, 20.0f, 25.0f ), rt::Vec3( 20.0f, -20.0f, 25.0f ), rt::Vec3( -20.0f, -20.0f, 25.0f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.0f, 0.0f, 0.0f ), 0.0f );

	//bottom
	rayTracer.AddTriangle( rt::Vec3( -20.0f, -20.0f, 25.0f ), rt::Vec3( 20.0f, -20.0f, 25.0f ), rt::Vec3( -20.0f, -20.0f, 0.0f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.0f, 0.0f, 0.0f ), 0.0f );
	rayTracer.AddTriangle( rt::Vec3( 20.0f, -20.0f, 25.0f ), rt::Vec3( 20.0f, -20.0f, 0.0f ), rt::Vec3( -20.0f, -20.0f, 0.0f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.0f, 0.0f, 0.0f ), 0.0f );

	//top
	rayTracer.AddTriangle( rt::Vec3( -20.0f, 20.0f, 0.0f ), rt::Vec3( 20.0f, 20.0f, 25.0f ), rt::Vec3( -20.0f, 20.0f, 25.0f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.0f, 0.0f, 0.0f ), 0.0f );
	rayTracer.AddTriangle( rt::Vec3( -20.0f, 20.0f, 0.0f ), rt::Vec3( 20.0f, 20.0f, 0.0f ), rt::Vec3( 20.0f, 20.0f, 25.0f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.0f, 0.0f, 0.0f ), 0.0f );

	rayTracer.AddLight( rt::Vec3( 0.0f, 5.0f, 5.0f ), rt::Vec3( 0.5f, 0.5f, 0.5f ) );
	rayTracer.AddLight( rt::Vec3( 0.0f, -5.0f, 5.0f ), rt::Vec3( 0.5f, 0.5f, 0.5f ) );
	//rayTracer.AddLight( rt::Vec3( 0.0f, -5.0f, 5.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 1.0f );
}

//void SampleRayTracer( const rt::RayTracer& rayTracer, const int width, const int height, rt::Vec3& rayVector, int& x, int& y, BMP& output )
void SampleRayTracer( const rt::RayTracer& rayTracer, const int width, const int height, rt::Vec3& rayVector, int& x, int& y, rt::ScreenBuffer& buffer )
{
	rt::Vec3 myVector;
	int myX, myY;

	rayLock.lock();
	while ( x < width )
	{
		myVector = rayVector;//.Unit();
		myX = x;
		myY = y++;
		rayVector.y--;

		if ( y == height )
		{
			y = 0;
			rayVector.y = height / 2.0f;// - 0.5f;

			x++;
			rayVector.x++;
		}
		rayLock.unlock();

		//cast multiple rays per pixel
		rt::Vec3 totalColor;
		const int sampleDimension = 3;
		const float sampleIncrement = 1.0f / (float)(sampleDimension + 1);
		const float myVectorInitialY = myVector.y;

		for ( int xi = 0; xi < sampleDimension; ++xi )
		{
			myVector.x += sampleIncrement;
			for ( int yi = 0; yi < sampleDimension; ++yi )
			{
				myVector.y -= sampleIncrement;
				//myVector = myVector.Unit();
				rt::Vec3 sampleColor;
				rayTracer.Sample( myVector.Unit(), sampleColor );
				totalColor += sampleColor;
			}
			myVector.y = myVectorInitialY;
		}

		bufferLock.lock();
			*(buffer( myX, myY )) = totalColor / ( sampleDimension * sampleDimension );
		bufferLock.unlock();
	}
}

int main( const int argc, char* argv[] )
{
	int width = 1600;
	int height = 1200;
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

	rt::ScreenBuffer buffer( width, height );

	//Create output bitmap
	BMP output;
	output.SetSize( width, height );

	const int maxThreads = 4;
	vector<std::thread> threads;

	//Start the timer
	std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
	startTime = std::chrono::system_clock::now();

	int x = 0;
	int y = 0;
	//rt::Vec3 rayVector( -width / 2.0f + 0.5f, height / 2.0f - 0.5f, distToProjPlane );
	rt::Vec3 rayVector( -width / 2.0f, height / 2.0f, distToProjPlane );

	for ( int i = 0; i  < maxThreads; ++i )
	{
		threads.push_back( std::thread( SampleRayTracer, std::cref(rayTracer), width, height, std::ref( rayVector ), std::ref(x), std::ref(y), std::ref(buffer) ) );
	}

	cout << "Using " << threads.size() << " threads." << endl;

	for ( vector<std::thread>::iterator it = threads.begin(); it != threads.end(); ++it )
	{
		it->join();
	}

	buffer.Dither();

	for ( int x = 0; x < width; ++x )
	{
		for ( int y = 0; y < height; ++y )
		{
			rt::Vec3 clipped = buffer(x,y)->UnitClip();
			output(x,y)->Red = (int) ( clipped.x * 255.0f );
			output(x,y)->Green = (int) ( clipped.y * 255.0f );
			output(x,y)->Blue = (int) ( clipped.z * 255.0f );
		}
	}

	//End timer and output render time
	endTime = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = endTime - startTime;
	cout << "Rendered in " << elapsed_seconds.count() << " seconds." << endl;

	cout << "Outputting to " << outputFilename << ".bmp" << endl;

	output.WriteToFile( ( outputFilename + ".bmp" ).c_str() );

	cout << "Done" << endl;

    return 0;
}
