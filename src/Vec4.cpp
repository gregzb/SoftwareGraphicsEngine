#include <cmath>
#include <cassert>
#include <functional>
#include "Vec4.hpp"
#include "Mat4.hpp"

Vec4::Vec4(double x_, double y_, double z_, double w_) : x(x_), y(y_), z(z_), w(w_)
{
}

Vec4::Vec4(Color color) : x(color.r / 255.0), y(color.g / 255.0), z(color.b / 255.0), w(color.a / 255.0)
{
}

Vec4::Vec4() : x(0), y(0), z(0), w(1)
{
}

double Vec4::operator[](int idx) const
{
    switch (idx)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        assert(false);
    }
}

void Vec4::set(int idx, double val)
{
    switch (idx)
    {
    case 0:
        x = val;
        break;
    case 1:
        y = val;
        break;
    case 2:
        z = val;
        break;
    case 3:
        w = val;
        break;
    default:
        assert(false);
        break;
    }
}

void Vec4::setX(double x)
{
    this->x = x;
}
void Vec4::setY(double y)
{
    this->y = y;
}
void Vec4::setZ(double z)
{
    this->z = z;
}
void Vec4::setW(double w)
{
    this->w = w;
}

double Vec4::getX() const
{
    return x;
}
double Vec4::getY() const
{
    return y;
}
double Vec4::getZ() const
{
    return z;
}
double Vec4::getW() const
{
    return w;
}

double Vec4::magnitude() const
{
    //return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
    return std::sqrt(x * x + y * y + z * z);
}

double Vec4::dot(Vec4 const &other) const
{
    return x * other.x + y * other.y + z * other.z;
}

Vec4 Vec4::normalize() const
{
    double mag = magnitude();
    return {x / mag, y / mag, z / mag, w};
}

Vec4 Vec4::perspectiveDivision() const
{
    return {x / w, y / w, z / w, w};
}

Vec4 Vec4::cross(Vec4 const &other) const
{
    return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x,
        1};
}

Vec4 Vec4::negate() const
{
    return {-x, -y, -z, -w};
}

Vec4 Vec4::scale(Vec4 const &other) const
{
    return {x * other.x, y * other.y, z * other.z, w * other.w};
}

Vec4 Vec4::scale(double other) const
{
    return {x * other, y * other, z * other, w * other};
}

Vec4 Vec4::operator*(Vec4 const &other) const
{
    return scale(other);
}

Vec4 Vec4::operator*(double other) const
{
    return scale(other);
}

Vec4 Vec4::div(Vec4 const &other) const
{
    return {x / other.x, y / other.y, z / other.z, w / other.w};
}

Vec4 Vec4::div(double other) const
{
    return {x / other, y / other, z / other, w / other};
}

Vec4 Vec4::operator/(Vec4 const &other) const
{
    return div(other);
}

Vec4 Vec4::operator/(double other) const
{
    return div(other);
}

Vec4 Vec4::add(Vec4 const &other) const
{
    return {x + other.x, y + other.y, z + other.z, w + other.w};
}

Vec4 Vec4::add(double other) const
{
    return {x + other, y + other, z + other, w + other};
}

Vec4 Vec4::operator+(Vec4 const &other) const
{
    return add(other);
}

Vec4 Vec4::operator+(double other) const
{
    return add(other);
}

Vec4 Vec4::sub(Vec4 const &other) const
{
    return {x - other.x, y - other.y, z - other.z, w - other.w};
}

Vec4 Vec4::sub(double other) const
{
    return {x - other, y - other, z - other, w - other};
}

Vec4 Vec4::operator-(Vec4 const &other) const
{
    return sub(other);
}

Vec4 Vec4::operator-(double other) const
{
    return sub(other);
}

Vec4 Vec4::transform(Mat4 const &transform) const
{
    Vec4 temp = *this;
    assert(transform.getCols() == 4);

    for (int row = 0; row < transform.getRows(); row++)
    {
        temp.set(row, 0);
        for (int i = 0; i < transform.getCols(); i++)
        {
            temp.set(row, temp[row] + transform(row, i) * (*this)[i]);
        }
    }
    return temp;
}

Vec4 Vec4::lerp(Vec4 const &other, double t) const
{
    return (other - *this) * t + *this;
}

Vec4 Vec4::round(double roundFactor) const
{
    return {std::lround(x * roundFactor) / roundFactor, std::lround(y * roundFactor) / roundFactor, std::lround(z * roundFactor) / roundFactor, std::lround(w * roundFactor) / roundFactor};
}

Vec4 Vec4::pow(Vec4 const &other) const
{
    return {std::pow(x, other.x), std::pow(y, other.y), std::pow(z, other.z), std::pow(w, other.w)};
}
Vec4 Vec4::pow(double other) const
{
    return {std::pow(x, other), std::pow(y, other), std::pow(z, other), std::pow(w, other)};
}

Color Vec4::toColor() const
{
    return {static_cast<u_char>((*this)[0] * 255),
            static_cast<u_char>((*this)[1] * 255),
            static_cast<u_char>((*this)[2] * 255),
            static_cast<u_char>((*this)[3] * 255)};
}

bool Vec4::inViewFrustum() const {
    double w_ = std::abs(w);
    return -w_ <= x && x <= w_ && -w_ <= y && y <= w_ && -w_ <= z && z <= w_;
}

bool Vec4::operator==(const Vec4 &other) const
{
    return x == other.x && y == other.y && z == other.z && w == other.w;
}

std::ostream &operator<<(std::ostream &os, const Vec4 v)
{
    os << "{" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}";
    return os;
}

std::istream &operator>>(std::istream &is, Vec4 &v)
{
    is >> v.x >> v.y >> v.z;
    return is;
}

std::size_t std::hash<Vec4>::operator()(const Vec4 &v) const
{
    size_t res = 17;
    res = res * 31 + std::hash<double>()(v.getX());
    res = res * 31 + std::hash<double>()(v.getY());
    res = res * 31 + std::hash<double>()(v.getZ());
    res = res * 31 + std::hash<double>()(v.getW());
    return res;
}