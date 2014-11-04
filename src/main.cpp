#include <iostream>
#include "EasyBMP/EasyBMP.h"
#include "rtMath.h"
#include "rtVec3.h"

int main( const int argc, char* argv[] )
{
	int width = 800;
	int height = 600;
	float aspect = (float) width / (float) height;
	int fov = rt::pi / 4.0;

	for ( int argi = 1; argi < argc; argi++ )
	{
		//if (  )
	}

    std::cout << "Hello world!" << std::endl;
    return 0;
}
