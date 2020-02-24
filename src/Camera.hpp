#pragma once

#include "Mat4.hpp"

class Camera
{
private:
    double fov, aspectRatio, near, far; //vertical fov, aspect is hori/verti

public:
    Vec4 position;
    Vec4 rotation;

    Camera(double fov, double aspectRatio, double near, double far);

    Mat4 getViewMatrix();
    Mat4 getProjectionMatrix();
};