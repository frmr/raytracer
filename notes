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
	Features
		Shadows
		Phong shading
		HDR rendering
		Reflection
		Anti-aliasing
		Depth of field
		Multi-threading
		Dithering
		Future work
	Testing methodology
		Strong scaling
		Default settings (scene 2)
			Width		640
			Height		480
			FOV			1.5708
			Threads		4
			Reflections	1
			AA Samples	2
			DOF Samples	100
			Aperture	0.4
			Focal Depth	12
			Dithering	On
		Test machine (Intel 4670K, 3.6GHz)
	Performance evaluation
		Resolution
			Aspect ratio must be maintained to prevent distortion of results
		Reflection (gradually fewer pixels result in another reflection ray being cast)
		Multi-threading
		Anti-aliasing
		Depth of field
	Conclusion
		Quality vs expense of each feature
		Profiler analysis
		