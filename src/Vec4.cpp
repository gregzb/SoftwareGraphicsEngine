#include <cmath>
#include <cassert>
#include <functional>
#include "Vec4.hpp"

Vec4::Vec4(double x_, double y_, double z_, double w_) : x(x_), y(y_), z(z_), w(w_)
{
}

Vec4::Vec4() : x(0), y(0), z(0), w(1)
{
}

double &Vec4::operator[](int idx)
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
        assert(1 == 2);
    }
}

double Vec4::magnitude()
{
    return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
}

double Vec4::dot(Vec4 other)
{
    return x * other.x + y * other.y + z * other.z;
}

Vec4 Vec4::normalize()
{
    double mag = magnitude();
    return {x / mag, y / mag, z / mag, w};
}

Vec4 Vec4::perspectiveDivision()
{
    return {x / w, y / w, z / w, w / w};
}

Vec4 Vec4::cross(Vec4 other)
{
    return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x,
        1};
}

Vec4 Vec4::negate()
{
    return {-x, -y, -z, -w};
}

Vec4 Vec4::scale(Vec4 other)
{
    return {x * other.x, y * other.y, z * other.z, w * other.w};
}

Vec4 Vec4::scale(double other)
{
    return {x * other, y * other, z * other, w * other};
}

Vec4 Vec4::operator*(Vec4 other)
{
    return scale(other);
}

Vec4 Vec4::operator*(double other)
{
    return scale(other);
}

Vec4 Vec4::div(Vec4 other)
{
    return {x / other.x, y / other.y, z / other.z, w / other.w};
}

Vec4 Vec4::div(double other)
{
    return {x / other, y / other, z / other, w / other};
}

Vec4 Vec4::operator/(Vec4 other)
{
    return div(other);
}

Vec4 Vec4::operator/(double other)
{
    return div(other);
}

Vec4 Vec4::add(Vec4 other)
{
    return {x + other.x, y + other.y, z + other.z, w + other.w};
}

Vec4 Vec4::add(double other)
{
    return {x + other, y + other, z + other, w + other};
}

Vec4 Vec4::operator+(Vec4 other)
{
    return add(other);
}

Vec4 Vec4::operator+(double other)
{
    return add(other);
}

Vec4 Vec4::sub(Vec4 other)
{
    return {x - other.x, y - other.y, z - other.z, w - other.w};
}

Vec4 Vec4::sub(double other)
{
    return {x - other, y - other, z - other, w - other};
}

Vec4 Vec4::operator-(Vec4 other)
{
    return sub(other);
}

Vec4 Vec4::operator-(double other)
{
    return sub(other);
}

Vec4 Vec4::lerp(Vec4 other, double t)
{
    return other.sub(*this).scale(t).add(*this);
}

Vec4 Vec4::round(double roundFactor) {
    return {std::lround(x*roundFactor)/roundFactor, std::lround(y * roundFactor)/roundFactor, std::lround(z*roundFactor)/roundFactor, std::lround(w*roundFactor)/roundFactor};
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
    res = res * 31 + std::hash<double>()(v.x);
    res = res * 31 + std::hash<double>()(v.y);
    res = res * 31 + std::hash<double>()(v.z);
    res = res * 31 + std::hash<double>()(v.w);
    return res;
}