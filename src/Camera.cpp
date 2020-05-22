#include <cmath>
#include "Camera.hpp"

Camera::Camera(double fov, double aspectRatio, double near, double far, bool perspective_) : fov(fov * M_PI / 180), aspectRatio(aspectRatio), near(near), far(far), perspective(perspective_)
{
}

void Camera::setPosition(Vec4 const &position)
{
    this->position = position;
}
void Camera::setRotation(Vec4 const &rotation)
{
    this->rotation = rotation;
}

Vec4 const &Camera::getPosition() const
{
    return position;
}
Vec4 const &Camera::getRotation() const
{
    return rotation;
}

Mat4 Camera::getViewMatrix() const
{
    Mat4 const &translateMat = Mat4::translate(position.negate());
    Mat4 const &rotXMat = Mat4::rotX(-rotation.getX());
    Mat4 const &rotYMat = Mat4::rotX(-rotation.getY());
    Mat4 const &rotZMat = Mat4::rotX(-rotation.getZ());

    Mat4 const &view = translateMat.multiply(rotXMat).multiply(rotYMat).multiply(rotZMat);

    return view;
}

Mat4 Camera::getPerspectiveProjectionMatrix() const
{
    double q = 1 / std::tan(fov / 2);
    double a = q / aspectRatio;
    double b = (far + near) / (far - near);
    double c = (2 * near * far) / (far - near);
    Mat4 temp = {{{a, 0, 0, 0},
                  {0, q, 0, 0},
                  {0, 0, -b, -c},
                  {0, 0, -1, 0}}};
    return temp;
}

Mat4 Camera::getOrthographicProjectionMatrix() const
{
    double q = std::tan(fov / 2);
    double t = q * near;
    double r = t * aspectRatio;
    double b = -2 / (far - near);
    double c = -(near + far) / (far - near);
    Mat4 temp = {{{1/r, 0, 0, 0},
                  {0, 1/t, 0, 0},
                  {0, 0, b, c},
                  {0, 0, 0, 1}}};
    return temp;
}

bool Camera::isPerspective() const {
    return perspective;
}