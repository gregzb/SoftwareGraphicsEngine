#pragma once

#include <iostream>
#include <cstddef>

class Vec4
{
public:
    double x, y, z, w;

    double &operator[](int idx);

    Vec4(double x, double y, double z = 0, double w = 1);
    Vec4();
    double magnitude();
    double dot(Vec4 other);
    Vec4 normalize();
    Vec4 perspectiveDivision();
    Vec4 negate();
    Vec4 cross(Vec4 other);
    Vec4 scale(Vec4 other);
    Vec4 scale(double other);
    Vec4 add(Vec4 other);
    Vec4 add(double other);
    Vec4 sub(Vec4 other);
    Vec4 sub(double other);

    Vec4 lerp(Vec4 other, double t);

    bool operator==(const Vec4 &other) const;

    friend std::ostream &operator<<(std::ostream &os, const Vec4 v);
    friend std::istream &operator>>(std::istream &is, Vec4 &v);
};

namespace std
{

template <>
struct hash<Vec4>
{
    std::size_t operator()(const Vec4 &v) const;
};

} // namespace std