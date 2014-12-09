raytracer makes extensive use of C++11, including threads,
which are not yet implemented in GCC for Windows (MinGW).

The source must be compiled with a compatible compiler,
such as 64-bit TDM on Windows or possibly GCC for Linux.

On Linux, raytracer can be compiled with the included
makefile.

A windows executable is included in the bin folder.

The following switches can be used with the supplied executable:
	-w [value]	Sets render width to [value]
	-h [value]	Sets render height to [value]
	-x [value]	Sets the horizontal field of view to [value]
	-o [value]	Changes the output filename to [value], which will be written as a bitmap
	-t [value]	Sets the number of threads to use to [value]
	-r [value]	Sets the maximum reflection depth to [value]
	-s [value]	Sets the number of anti-aliasing samples to use to [value]
	-d [value]	Sets the number of depth of field samples to use to [value]
	-a [value]	Sets the radius of the aperture to [value]
	-f [value]	Sets the focal distance to [value]
	-i [value]	Turns dithering on or off when value is 1 or 0, respectively

The EasyBMP library is included in the src folder under
the BSD Revised License.
