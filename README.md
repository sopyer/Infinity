Overview
===================================

This project has 2 goals during it development - test different approaches to core system and implement various promising rendering technique and technical papers.


Compilation
===================================

Code is windows only. Use MSVC2010 for compilation. Build first external libraries with Libraries.sln. After that build Infinity.sln.


Running
===================================
Code was developed on machine with AMD video card and OpenGL 4.4 driver. In order to successfully run samples similar configuration with latest graphics drivers probably is required.


Demo keys
===================================

2D camera
---------------------------------------------
Use mouse drag to pan image and mouse wheel to zoom.

3D camera
---------------------------------------------
Click in order to grab mouse. Hold LeftCtrl+LeftAlt in order to release. Use mouse to look around and A/W/S/W/Q/E to move around.

Debug functions
---------------------------------------------
F4 - profiler view. Click individual bars in order to get more info.
F5 - shader editing. Use Alt+1/Alt+2/Alt+3 in order to switch between windows.

~+F - make single frame capture, which can be viewed in profiler view.
~+T twice - to capture few frames


Application descriptions
===================================

Anima
---------------------------------------------
Implements dual-quaternion skinning. Uses 3D camera.

Demo
---------------------------------------------
Implements clustered forward rendering. There is 1024 lights in entire level. Uses 3D camera.

Exest
---------------------------------------------
Implements CDLOD terrain. Uses 3D camera.

Clock and SVGRendering
---------------------------------------------
Implement resolution independent curve rendering. Use 2D camera.
