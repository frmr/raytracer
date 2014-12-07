raytracer
=========

Ray tracer implementation for CS-M00 (Research Methodology)

Assumed camera position at (0,0,0) with no rotation.
If the camera won't move, you'd be better of transforming points in scene to meet fixed camera.
Simplified process by avoiding checks for intersection behind the camera

Float used for speed boost on 32-bit systems (maybe don't mention this)

Sample() method takes advantage of high parallelisation possibilities.
Independence from traditional graphics APIs allows float colours.
American spelling of colour used because of my intention to open source the project.

Test different ray stopping methods (depth vs power)
Test different anti-aliasing approaches (multi-sampling vs scaling)
Test impact of additional lights

Dithering
Refraction
Anti-aliasing
HDR, floats can go above 1.0, but get clipped to 1.0 AFTER dithering
Dithering cannot be multi-threaded, demonstrates that not all shaders can be parallel and will slow down rendering
Count number of rays cast
Use "stochastic" in reference to depth of field and global illumination


Report
	Introduction
	Testing methodology
		Strong scaling
		Test machine (Intel 4670K, 3.6GHz)
	