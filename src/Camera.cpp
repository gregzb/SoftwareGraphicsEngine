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
    // std::cout << " a" << std::endl;
    Mat4 const &translateMat = Mat4::translate(position.negate());
    // Mat4 const &rotXMat = Mat4::rotX(-rotation.getX());
    // Mat4 const &rotYMat = Mat4::rotX(-rotation.getY());
    // Mat4 const &rotZMat = Mat4::rotX(-rotation.getZ());

    // Mat4 const &view = translateMat.multiply(rotXMat).multiply(rotYMat).multiply(rotZMat);

    Mat4 const &rotationMat = getRotationMatrix();

    return rotationMat.multiply(translateMat);
}

Mat4 Camera::getRotationMatrix() const
{
    if (lookMode == LookMode::EULER)
    {
        Mat4 const &rotXMat = Mat4::rotX(-rotation.getX());
        Mat4 const &rotYMat = Mat4::rotY(-rotation.getY());
        Mat4 const &rotZMat = Mat4::rotZ(-rotation.getZ());

        Mat4 const &view = rotXMat.multiply(rotYMat).multiply(rotZMat);
        return view;
    }
    else if (lookMode == LookMode::LOOKAT || true)
    {
        Vec4 forward = (position - rotation).normalize();

        Vec4 upDir = {0, 1, 0, 1};

        // compute the left vector
        Vec4 left = upDir.cross(forward).normalize();

        // recompute the orthonormal up vector
        Vec4 up = forward.cross(left);

        forward.set(3, 0);
        left.set(3, 0);
        up.set(3, 0);

        Mat4 temp;
        temp.addPoint(left);
        temp.addPoint(up);
        temp.addPoint(forward);
        temp.addPoint({0, 0, 0, 1});
        temp = temp.transpose();
        // std::cout << left << up << forward << std::endl;
        // std::cout << temp.toString() << std::endl;
        return temp;
    }
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
    Mat4 temp = {{{1 / r, 0, 0, 0},
                  {0, 1 / t, 0, 0},
                  {0, 0, b, c},
                  {0, 0, 0, 1}}};
    return temp;
}

bool Camera::isPerspective() const
{
    return perspective;
}

void Camera::setLookMode(LookMode lookMode_)
{
    lookMode = lookMode_;
}

void Camera::lookAt(Vec4 const &target)
{
    rotation = target;
}

LookMode Camera::getLookMode()
{
    return lookMode;
}