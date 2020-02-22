#pragma once

#include "Mat4.hpp"
#include "Vec3.hpp"

class RenderObject
{
private:
    Mat4 mesh; //object space verts

public:
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;

    RenderObject();
    Mat4 toWorldSpace();
    Mat4 &getMesh();
};