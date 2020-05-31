#pragma once

#include "Mat4.hpp"

enum class LookMode {
    EULER, LOOKAT, QUATERNION
};

class Camera
{
private:
    double fov, aspectRatio, near, far; //vertical fov, aspect is hori/verti
    Vec4 position, rotation;
    bool perspective;
    LookMode lookMode;

public:
    Camera(double fov, double aspectRatio, double near, double far, bool perspective_);

    void setPosition(Vec4 const &position);
    void setRotation(Vec4 const &rotation);
    void setLookMode(LookMode lookMode_);
    void lookAt(Vec4 const& target);

    Vec4 const &getPosition() const;
    Vec4 const &getRotation() const;
    LookMode getLookMode();

    Mat4 getViewMatrix() const;
    Mat4 getRotationMatrix() const;
    Mat4 getPerspectiveProjectionMatrix() const;
    Mat4 getOrthographicProjectionMatrix() const;

    bool isPerspective() const;
};