# Software Graphics Engine
By: Greg Zborovsky

## Greg Zborovsky pd. 4
### Features I want to implement for final project: [All base features added!]
* Finish texture loading/OBJ Loading
  * Correct/ensure that texture loading works fine - Done
  * Support multiple texture coordinates per vertex - Done
  * Bilinear filtering during texture read  - Done
  * Support quadrilaterals and other convex shapes in OBJ loading - Done
* Normal mapping
  * Will combine texture mapping and normals to alter light calculations as if triangle wasn't perfectly flat - Done
* Orthographic Camera - Done
* Sky Map
  * Essentially a regular cube, but normals are inverted and texture is available on the inside. - Done
  * (Maybe allow using normals from OBJ file?) - Done
* Gamma correction
  * Gamma correct all values related to user input and light except for non color data. (To power of 2.2, and to power of 1/2.2 at end) - Done I think?
* Basic Supersampling - Done

### Features I might want to add for final project:
* MDL Support
* Animaton Support
  * Based on keyframe animation
  * Every object's/camera's/light's position/rotation/scale should be animatable, maybe special animatable features for each object type
  * Multiple interpolation types - linear, quadratic, cubic, etc
* Quaternion-based rotation

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
* MDL?
* Better animation support
* Quaternions
* Ray tracing
* Shadow Mapping

## Resources:
* https://www.scratchapixel.com/
* https://en.wikipedia.org/wiki/Rotation_matrix
