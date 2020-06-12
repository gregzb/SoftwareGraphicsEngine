# Software Graphics Engine
By: Greg Zborovsky

## Greg Zborovsky Pd. 4

## Implemented Features For Final Project:
* Proper OBJ/MTL loading
* Proper Texture Loading
* Bilinear Filtering
* Orthographic Camera
* Normal Mapping
* Gamma Correction
* Anti-Aliasing

## More Details:
* Proper OBJ/MTL loading
  * Supports triangles & quadrilaterals, UV texture coordinates, vertex normals, multiple objects, and different texture types
* Proper Texture Loading
  * Off by one errors have been eliminated, some code refactoring
* Bilinear Filtering
  * When reading from a texture, interpolate between the closest four colors to get an approximate "in-between" color
* Orthographic Camera & LookAt mode
  * Previously only had a Perspective Camera, an Orthographic Camera mirrors what we have done in class
  * LookAt mode allows me to choose a position to point my camera at rather than explicitly use Euler angles
* Normal Mapping
  * Based on a texture, offset the direction of the normals of a surface, allows for highly detailed but low-poly surfaces
* Gamma Correction
  * Raise input colors to power of 2.2 (gamma), and output to 1/2.2 - Accounts for how humans perceive light
* Anti-Aliasing
  * Done using a basic super sampling by rendering to *2w* x *2h* dimensions and scaling down to *w* x *h* using bilinear filtering
* MDL & Animation were not included, but had animation been included, I would have opted for a keyframe-based system
* Other existing features include: Offscreen clipping, Per-object transforms, Perspective-Correct Interpolation

### Default Example:
To run the default example *(Warning: Uses around **2GB** of memory)*:
Extract the resources folder from the zip in the link below:

https://drive.google.com/file/d/1hIbpT2HYk75rIr8vw1dUWTZyL2GzMVXF/view?usp=sharing

And drop the resources folder into the root folder of this project
<pre>
.
├── convertObjImgToPPM.py
├── makefile
├── README.md
├── resources
│   ├── skybox.mtl
│   ├── skybox.obj
│   ├── StandardCubeMap.ppm
│   ├── tcmodimt_2K_Albedo.ppm
│   ├── ...
└── src
    ├── Camera.cpp
    ├── Camera.hpp
    ├── ...
</pre>

Note - convertObjImgToPPM.py is primarily intended for developer use and requires using Python3 and having Pillow installed. Paths to images should also be absolute paths.

## Features:
* Render to ppm/ImageMagick/png
* Basic 3d command input
* Bresenham line drawing
* Matrix transformations in 3d
* Circle, hermite, and bezier curve drawing
* Box, sphere, and torus primitives
* Scanline triangle fill
* Object-space transformations
* Perspective/Orthographic Euler and LookAt Camera
* Texture Mapping
* Tangent Space Normal Mapping
* Sky maps
* Blinn-Phong Lighting, Phong Shading
* Gamma Correction
* OBJ/MTL Support
* 2x Supersampling

## Todo:
* Better animation support
* Quaternions
* Ray tracing
* Shadow Mapping

## Resources:
* https://www.scratchapixel.com/
* https://en.wikipedia.org/wiki/Rotation_matrix
* http://www.songho.ca/opengl/
