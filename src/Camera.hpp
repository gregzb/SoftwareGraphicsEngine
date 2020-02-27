#pragma once

#include "Mat4.hpp"

class Camera
{
private:
    double fov, aspectRatio, near, far; //vertical fov, aspect is hori/verti
    Vec4 position, rotation;

public:
    Camera(double fov, double aspectRatio, double near, double far);

    void setPosition(Vec4 const &position);
    void setRotation(Vec4 const &rotation);

    Vec4 const &getPosition();
    Vec4 const &getRotation();

    Mat4 const &getViewMatrix();
    Mat4 const &getProjectionMatrix();
};