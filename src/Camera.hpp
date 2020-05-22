#pragma once

#include "Mat4.hpp"

class Camera
{
private:
    double fov, aspectRatio, near, far; //vertical fov, aspect is hori/verti
    Vec4 position, rotation;
    bool perspective;

public:
    Camera(double fov, double aspectRatio, double near, double far, bool perspective_);

    void setPosition(Vec4 const &position);
    void setRotation(Vec4 const &rotation);

    Vec4 const &getPosition() const;
    Vec4 const &getRotation() const;

    Mat4 getViewMatrix() const;
    Mat4 getRotationMatrix() const;
    Mat4 getPerspectiveProjectionMatrix() const;
    Mat4 getOrthographicProjectionMatrix() const;

    bool isPerspective() const;
};