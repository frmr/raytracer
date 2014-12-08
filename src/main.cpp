#include <chrono>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <utility>

#include "EasyBMP/EasyBMP.h"

#include "rtMath.h"
#include "rtRayTracer.h"
#include "rtRenderBuffer.h"
#include "rtVec3.h"

using std::cout;
using std::endl;
using std::string;

std::mutex bufferLock;
std::mutex rayLock;

//---------------------------------------------------
//	Parameters: Struct containing render parameters.
//---------------------------------------------------

struct Parameters
{
	int width;
	int height;
	float fovX;
	string outputFilename;
	int threads;
	int reflections;

	//super-sampling
	int aaSamples; //Samples in each dimension

	//depth of field
	int dofSamples;
	float aperture;
	float focalDepth;

	//dithering
	bool dithering;
};

void SetupScene1( rt::RayTracer& rayTracer )
{
	rayTracer.AddSphere( rt::Vec3( -8.0f, 0.0f, 10.0f ), 1.0f, rt::Vec3( 1.0f, 0.0f, 0.0f ), rt::Vec3( 1.0f, 0.0f, 0.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.75f );
	rayTracer.AddSphere( rt::Vec3( 0.0f, -7.0f, 10.0f ), 1.0f, rt::Vec3( 0.0f, 1.0f, 0.0f ), rt::Vec3( 0.0f, 1.0f, 0.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.75f );
	rayTracer.AddSphere( rt::Vec3( 0.0f, 0.0f, 15.0f ), 1.0f, rt::Vec3( 0.0f, 0.0f, 1.0f ), rt::Vec3( 0.0f, 0.0f, 1.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.75f );
	rayTracer.AddSphere( rt::Vec3( 0.0f, 5.0f, 20.0f ), 1.0f, rt::Vec3( 1.0f, 0.0f, 1.0f ), rt::Vec3( 1.0f, 0.0f, 1.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.75f );
	rayTracer.AddSphere( rt::Vec3( 5.0f, 0.0f, 20.0f ), 1.0f, rt::Vec3( 1.0f, 1.0f, 0.0f ), rt::Vec3( 1.0f, 1.0f, 0.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.75f );

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
}


void SetupScene2( rt::RayTracer& rayTracer )
{
	rayTracer.AddLight( rt::Vec3( -20.0f, 20.0f, 10.0f ), rt::Vec3( 0.5f, 0.5f, 0.5f ) );
	rayTracer.AddLight( rt::Vec3( 20.0f, 10.0f, 5.0f ), rt::Vec3( 0.25f, 0.25f, 0.25f ) );
	rayTracer.SetAmbientLight( rt::Vec3( 0.1f, 0.1f, 0.1f ) );

	rayTracer.AddTriangle( rt::Vec3( -100.0f, -2.0f, 100.0f ), rt::Vec3( 100.0f, -2.0f, 100.0f ), rt::Vec3( 100.0f, -2.0f, -100.0f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.0f, 0.0f, 0.0f ), 0.1f );
	rayTracer.AddTriangle( rt::Vec3( -100.0f, -2.0f, 100.0f ), rt::Vec3( 100.0f, -2.0f, -100.0f ), rt::Vec3( -100.0f, -2.0f, -100.0f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.5f, 0.5f, 0.5f ), rt::Vec3( 0.0f, 0.0f, 0.0f ), 0.1f );

	rayTracer.AddSphere( rt::Vec3( -3.0f, 0.0f, 9.0f ), 2.0f, rt::Vec3( 1.0f, 0.0f, 0.0f ), rt::Vec3( 1.0f, 0.0f, 0.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.5f );		//Red
	rayTracer.AddSphere( rt::Vec3( -2.0f, -1.0f, 6.0f ), 1.0f, rt::Vec3( 0.0f, 1.0f, 0.0f ), rt::Vec3( 0.0f, 1.0f, 0.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.5f );	//Green
	rayTracer.AddSphere( rt::Vec3( 2.0f, -0.6f, 8.0f ), 1.4f, rt::Vec3( 0.0f, 0.0f, 1.0f ), rt::Vec3( 0.0f, 0.0f, 1.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.5f );		//Blue
	rayTracer.AddSphere( rt::Vec3( 5.0f, -1.0f, 7.0f ), 1.0f, rt::Vec3( 1.0f, 0.0f, 1.0f ), rt::Vec3( 1.0f, 0.0f, 1.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.5f );		//Pink
	rayTracer.AddSphere( rt::Vec3( 6.0f, -1.0f, 9.0f ), 1.0f, rt::Vec3( 0.0f, 1.0f, 1.0f ), rt::Vec3( 0.0f, 1.0f, 1.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.5f );		//Cyan
	rayTracer.AddSphere( rt::Vec3( 4.0f, 1.0f, 12.0f ), 3.0f, rt::Vec3( 1.0f, 1.0f, 0.0f ), rt::Vec3( 1.0f, 1.0f, 0.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.5f );		//Yellow
	rayTracer.AddSphere( rt::Vec3( -1.0f, -1.0f, 12.0f ), 1.0f, rt::Vec3( 1.0f, 0.5f, 0.0f ), rt::Vec3( 1.0f, 0.5f, 0.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.5f );	//Orange
	rayTracer.AddSphere( rt::Vec3( 1.0f, -1.0f, 14.0f ), 1.0f, rt::Vec3( 0.5f, 0.0f, 1.0f ), rt::Vec3( 0.5f, 0.0f, 1.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.5f );	//Purple
	rayTracer.AddSphere( rt::Vec3( -1.0f, -0.6f, 16.5f ), 1.4f, rt::Vec3( 0.0f, 0.5f, 1.0f ), rt::Vec3( 0.0f, 0.5f, 1.0f ), rt::Vec3( 1.0f, 1.0f, 1.0f ), 0.5f );	//Light blue
}

//--------------------------------------------------------------------------------------------
//  SampleRayTracer: Function called by each thread.
//	Increments the X and Y coordinates of the buffer until the picture is completely sampled.
//	Anti-aliasing and depth of field are also performed here.
//--------------------------------------------------------------------------------------------

void SampleRayTracer( const rt::RayTracer& rayTracer, const Parameters& params, rt::Vec3& rayVector, int& x, int& y, rt::RenderBuffer& buffer )
{
	rt::Vec3 myVector;
	int myX, myY;

	//Initialise random number generators
	std::random_device randDevice;
    std::mt19937 mt( randDevice() );
	std::uniform_real_distribution<float> radiusGenerator( 0.0f, params.aperture );
	std::uniform_real_distribution<float> angleGenerator( 0.0f, rt::twoPi );

	rayLock.lock();
	while ( x < params.width )
	{
		//Get the next pixel to be computed
		myVector = rayVector;
		myX = x;
		myY = y++;
		rayVector.y--;

		if ( y == params.height )
		{
			//Reset the coordinates to the start of the next column
			y = 0;
			rayVector.y = (float) params.height / 2.0f;;

			x++;
			rayVector.x++;
		}
		rayLock.unlock();

		//Perform supersampling
		rt::Vec3 totalColor;
		const double sampleIncrement = 1.0 / (double)( params.aaSamples + 1 );	//Must be double for sufficient accuracy
		const float myVectorInitialY = myVector.y;

		for ( int xi = 0; xi < params.aaSamples; ++xi )
		{
			myVector.x += sampleIncrement;
			for ( int yi = 0; yi < params.aaSamples; ++yi )
			{
				myVector.y -= sampleIncrement;

				//Perform depth of field sampling
				rt::Vec3 focalTarget = myVector.Unit() * params.focalDepth;

				//modify ray randomly within aperture
				for ( int dofSampleCount = 0; dofSampleCount < params.dofSamples; ++dofSampleCount )
				{
					rt::Vec3 sampleColor;

					//Generate a random point on the circular aperture
					const float randRadius = radiusGenerator(mt);
					rt::Vec3 dofOrigin(	randRadius * sin( angleGenerator(mt) ),
										randRadius * cos( angleGenerator(mt) ),
										0.0f );

					rayTracer.Sample( dofOrigin, ( focalTarget - dofOrigin ).Unit(), params.reflections, sampleColor );
					totalColor += sampleColor;
				}
			}
			myVector.y = myVectorInitialY;
		}

		//Write color to the buffer
		bufferLock.lock();
			*(buffer( myX, myY )) = totalColor / ( params.aaSamples * params.aaSamples * params.dofSamples );
		bufferLock.unlock();
	}
}

int main( const int argc, char* argv[] )
{
	Parameters params = { 800, 600, rt::halfPi, "output", 1, 1, 1, 1, 0.0f, 1.0f, false };

	//Get parameters from the command line
	for ( int argi = 1; argi < argc; ++argi )
	{
		if ( !strcmp( argv[argi], "-w" ) )
		{
			if ( argc > argi + 1 ) params.width = atoi( argv[++argi] );
		}
		else if ( !strcmp( argv[argi], "-h" ) )
		{
			if ( argc > argi + 1 ) params.height = atoi( argv[++argi] );
		}
		else if ( !strcmp( argv[argi], "-x" ) )
		{
			if ( argc > argi + 1 ) params.fovX = atof( argv[++argi] );
		}
		else if ( !strcmp( argv[argi], "-o" ) )
		{
			if ( argc > argi + 1 ) params.outputFilename = argv[++argi];
		}
		else if ( !strcmp( argv[argi], "-t" ) )
		{
			if ( argc > argi + 1 ) params.threads = atoi( argv[++argi] );
		}
		else if ( !strcmp( argv[argi], "-r" ) )
		{
			if ( argc > argi + 1 ) params.reflections = atoi( argv[++argi] );
		}
		else if ( !strcmp( argv[argi], "-s" ) )
		{
			if ( argc > argi + 1 ) params.aaSamples = atoi( argv[++argi] );
		}
		else if ( !strcmp( argv[argi], "-d" ) )
		{
			if ( argc > argi + 1 ) params.dofSamples = atoi( argv[++argi] );
		}
		else if ( !strcmp( argv[argi], "-a" ) )
		{
			if ( argc > argi + 1 ) params.aperture = atof( argv[++argi] );
		}
		else if ( !strcmp( argv[argi], "-f" ) )
		{
			if ( argc > argi + 1 ) params.focalDepth = atof( argv[++argi] );
		}
		else if ( !strcmp( argv[argi], "-i" ) )
		{
			if ( argc > argi + 1 ) params.dithering = atoi( argv[++argi] );
		}
		else
		{
			cout << "Argument \"" << argv[argi] << "\" not recognised." << endl;
		}
	}

	float distToProjPlane = ( (float) params.width / 2.0f ) / tan( params.fovX / 2.0f );

    cout	<< "Width:\t\t" << params.width << endl
			<< "Height:\t\t" << params.height << endl
			<< "FOVX:\t\t" << params.fovX << endl
			<< "Threads:\t" << params.threads << endl
			<< "Reflections:\t" << params.reflections << endl
			<< "AA Samples:\t" << params.aaSamples << endl
			<< "DOF Samples:\t" << params.dofSamples << endl
			<< "Aperture:\t" << params.aperture << endl
			<< "Focal Depth:\t" << params.focalDepth << endl << endl;

    cout << "Will output to " << params.outputFilename << ".bmp" << endl;

	rt::RayTracer rayTracer;
	SetupScene2( rayTracer );

	rt::RenderBuffer buffer( params.width, params.height );

	//Create output bitmap
	BMP output;
	output.SetSize( params.width, params.height );

	//Start the timer
	std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
	startTime = std::chrono::system_clock::now();

	int x = 0;
	int y = 0;
	rt::Vec3 rayVector( (float) -params.width / 2.0f, (float) params.height / 2.0f, distToProjPlane );

	//Start threads
	vector<std::thread> threads;
	for ( int i = 0; i  < params.threads; ++i )
	{
		threads.push_back( std::thread( SampleRayTracer, std::cref(rayTracer), params, std::ref( rayVector ), std::ref(x), std::ref(y), std::ref(buffer) ) );
	}

	for ( vector<std::thread>::iterator it = threads.begin(); it != threads.end(); ++it )
	{
		it->join();
	}

	if ( params.dithering )
	{
		buffer.Dither();
	}


	//Write the buffer to the output bitmap
	for ( int x = 0; x < params.width; ++x )
	{
		for ( int y = 0; y < params.height; ++y )
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

	cout << "Outputting to " << params.outputFilename << ".bmp" << endl;

	output.WriteToFile( ( params.outputFilename + ".bmp" ).c_str() );

	cout << "Done" << endl;

    return 0;
}
