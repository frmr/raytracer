#ifndef RT_RENDER_BUFFER_H
#define RT_RENDER_BUFFER_H

#include "rtVec3.h"

#include <vector>

using std::vector;

namespace rt
{
	class RenderBuffer
	{
	private:
		vector<vector<rt::Vec3>> buffer;

	public:
		const int width;
		const int height;

	private:
		
		//----------------------------------------------------
		//	ClosestColor: Round float color components to the
		//	equivalent of8-bit per channel.
		//----------------------------------------------------
		
		static rt::Vec3 ClosestColor( rt::Vec3& color )
		{
			return rt::Vec3(	(float) ( (int) ( color.x * 255.0f ) ) / 255.0f,
								(float) ( (int) ( color.y * 255.0f ) ) / 255.0f,
								(float) ( (int) ( color.z * 255.0f ) ) / 255.0f );
		}

	public:
		
		//----------------------------------------------------
		//	Dither: Apply Floyd–Steinberg dithering, reducing
		//	the float-based colors to standard 24-bit color.
		//----------------------------------------------------
		
		void Dither()
		{
			for ( int y = 0; y < height - 1; ++y )
			{
				for ( int x = 1; x < width - 1; ++x )
				{
					rt::Vec3 oldPixel = buffer[x][y];
					rt::Vec3 newPixel = ClosestColor( oldPixel );
					buffer[x][y] = newPixel;
					rt::Vec3 quant_error = oldPixel - newPixel;
					buffer[x+1][y]		=	buffer[x+1][y] + quant_error * 7.0f / 16.0f;
					buffer[x-1][y+1]	=	buffer[x-1][y+1] + quant_error * 3.0f / 16.0f;
					buffer[x][y+1]		=	buffer[x][y+1] + quant_error * 5.0f / 16.0f;
					buffer[x+1][y+1]	=	buffer[x+1][y+1] + quant_error * 1.0f / 16.0f;
				}
			}
		}

		rt::Vec3* operator()( const int x, const int y )
		{
			if ( x >= 0 && x < width && y >= 0 && y < height )
			{
				return &buffer[x][y];
			}
			return nullptr;
		}

	public:
		RenderBuffer( const int width, const int height )
			:	buffer( width, vector<rt::Vec3>( height, rt::Vec3() ) ),
				width( width ),
				height( height )

		{
		}

	};
}

#endif // RT_RENDER_BUFFER_H
