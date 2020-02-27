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
    double magnitude() const;
    double dot(Vec4 other) const;
    Vec4 normalize() const;
    Vec4 perspectiveDivision() const;
    Vec4 negate() const;
    Vec4 cross(Vec4 other) const;
    Vec4 scale(Vec4 other) const;
    Vec4 operator*(Vec4 other) const;
    Vec4 scale(double other) const;
    Vec4 operator*(double other) const;
    Vec4 div(Vec4 other) const;
    Vec4 operator/(Vec4 other) const;
    Vec4 div(double other) const;
    Vec4 operator/(double other) const;
    Vec4 add(Vec4 other) const;
    Vec4 operator+(Vec4 other) const;
    Vec4 add(double other) const;
    Vec4 operator+(double other) const;
    Vec4 sub(Vec4 other) const;
    Vec4 operator-(Vec4 other) const;
    Vec4 sub(double other) const;
    Vec4 operator-(double other) const;
    Vec4 round(double roundFactor) const;

    Vec4 lerp(Vec4 other, double t) const;

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