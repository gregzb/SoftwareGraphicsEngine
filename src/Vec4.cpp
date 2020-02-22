#include <cmath>
#include "Vec4.hpp"

Vec4::Vec4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w)
{
}

Vec4::Vec4() : x(0), y(0), z(0), w(0)
{
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

Vec4 Vec4::add(Vec4 other)
{
    return {x + other.x, y + other.y, z + other.z, w + other.w};
}

Vec4 Vec4::add(double other)
{
    return {x + other, y + other, z + other, w + other};
}

Vec4 Vec4::sub(Vec4 other)
{
    return {x - other.x, y - other.y, z - other.z, w - other.w};
}

Vec4 Vec4::sub(double other)
{
    return {x - other, y - other, z - other, w - other};
}

std::ostream& operator<<(std::ostream& os, const Vec4 v) {
    os << "{" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}";
    return os;
}

std::istream& operator>>(std::istream& is, Vec4& v) {
    is >> v.x >> v.y >> v.z >> v.w;
    return is;
}