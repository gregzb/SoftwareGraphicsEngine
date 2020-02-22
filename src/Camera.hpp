#pragma once

#include "Vec3.hpp"
#include "Mat4.hpp"

class Camera
{
private:
    double fov, aspectRatio, near, far; //vertical fov, aspect is hori/verti

public:
    Vec3 position;
    Vec3 rotation;

    Camera(double fov, double aspectRatio, double near, double far);

    Mat4 getViewMatrix();
    Mat4 getProjectionMatrix();
};