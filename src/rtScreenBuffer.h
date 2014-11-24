#ifndef RT_SCREEN_BUFFER_H
#define RT_SCREEN_BUFFER_H

#include "rtVec3.h"

#include <vector>

using std::vector;

namespace rt
{
	class ScreenBuffer
	{
	private:
		vector<vector<rt::Vec3>> buffer;

	public:
		const int width;
		const int height;

	public:
		void Dither()
		{
//			for each y from top to bottom
//			   for each x from left to right
//				  oldpixel  := pixel[x][y]
//				  newpixel  := find_closest_palette_color(oldpixel)
//				  pixel[x][y]  := newpixel
//				  quant_error  := oldpixel - newpixel
//				  pixel[x+1][y  ] := pixel[x+1][y  ] + quant_error * 7/16
//				  pixel[x-1][y+1] := pixel[x-1][y+1] + quant_error * 3/16
//				  pixel[x  ][y+1] := pixel[x  ][y+1] + quant_error * 5/16
//				  pixel[x+1][y+1] := pixel[x+1][y+1] + quant_error * 1/16

			for ( int y = 1; y < height - 1; ++y )
			{
				for ( int x = 1; x < width - 1; ++x )
				{
					rt::Vec3 oldPixel = buffer[y][x] * 255.0f;
					rt::Vec3 newPixel = oldPixel.Round();
					buffer[y][x] = newPixel;
					rt::Vec3 quant_error = oldPixel - newPixel;
					buffer[y][x+1]		=	buffer[y][x+1] + quant_error * 7.0f / 16.0f;
					buffer[y+1][x-1]	=	buffer[y+1][x-1] + quant_error * 3.0f / 16.0f;
					buffer[y+1][x]		=	buffer[y+1][x] + quant_error * 5.0f / 16.0f;
					buffer[y+1][x+1]	=	buffer[y+1][x+1] + quant_error * 1.0f / 16.0f;
				}
			}

		}

		rt::Vec3* operator()( const int x, const int y )
		{
			if ( x >= 0 && x < width && y >= 0 && y < height )
			{
				return &buffer[y][x];
			}
			return nullptr;
		}

	public:
		ScreenBuffer( const int width, const int height )
			:	width( width ),
				height( height ),
				buffer( height, vector<rt::Vec3>( width, rt::Vec3() ) )
		{
		}

	};
}

#endif // RT_SCREEN_BUFFER_H
