# Software Graphics Engine
By: Greg Zborovsky

## Greg Zborovsky pd. 4
### Features I want to implement for final project:
* Finish texture loading/OBJ Loading
  * Correct/ensure that texture loading works fine
  * Support multiple texture coordinates per vertex
  * Bilinear filtering during texture read
  * Support quadrilaterals and other convex shapes in OBJ loading
* Normal mapping
  * Will combine texture mapping and normals to alter light calculations as if triangle wasn't perfectly flat
* Orthographic Camera
* Cube Map
  * Essentially a regular cube, but normals are inverted and texture is available on the inside.
  * (Maybe allow using normals from OBJ file?)

### Features I might want to add for final project:
* MDL Support
* Animaton Support
  * Based on keyframe animation
  * Every object's/camera's/light's position/rotation/scale should be animatable, maybe special animatable features for each object type
  * Multiple interpolation types - linear, quadratic, cubic, etc
* Quaternion-based rotation
* Gamma correction

## Features:
* Render to ppm/ImageMagick/png
* Basic 3d command input
* Bresenham line drawing
* Matrix transformations in 3d
* Circle, hermite, and bezier curve drawing
* Box, sphere, and torus primitives
* ~Coordinate system stack~
* Scanline triangle fill
* Object-space transformations
* Perspective Camera
* Texture Mapping
* Blinn-Phong Lighting, Phong Shading

## Todo:
* MDL?
* Better animation support
* Gamma correction
* Quaternions
* Ray tracing
* Shadow Mapping

## Resources:
* https://www.scratchapixel.com/
* https://en.wikipedia.org/wiki/Rotation_matrix
