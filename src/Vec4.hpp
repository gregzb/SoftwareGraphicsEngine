#pragma once

#include <iostream>
#include <cstddef>

#include "Utils.hpp"

class Mat4;

class Vec4
{
private:
    double x, y, z, w;

public:
    double operator[](int idx) const;
    void set(int idx, double val);

    void setX(double x);
    void setY(double y);
    void setZ(double z);
    void setW(double w);

    double getX() const;
    double getY() const;
    double getZ() const;
    double getW() const;

    Vec4(double x, double y, double z = 0, double w = 1);
    Vec4(Color color);
    Vec4();
    double magnitude() const;
    double dot(Vec4 const &other) const;
    Vec4 normalize() const;
    Vec4 perspectiveDivision() const;
    Vec4 negate() const;
    Vec4 cross(Vec4 const &other) const;
    Vec4 scale(Vec4 const &other) const;
    Vec4 operator*(Vec4 const &other) const;
    Vec4 scale(double other) const;
    Vec4 operator*(double other) const;
    Vec4 div(Vec4 const &other) const;
    Vec4 operator/(Vec4 const &other) const;
    Vec4 div(double other) const;
    Vec4 operator/(double other) const;
    Vec4 add(Vec4 const &other) const;
    Vec4 operator+(Vec4 const &other) const;
    Vec4 add(double other) const;
    Vec4 operator+(double other) const;
    Vec4 sub(Vec4 const &other) const;
    Vec4 operator-(Vec4 const &other) const;
    Vec4 sub(double other) const;
    Vec4 operator-(double other) const;
    Vec4 round(double roundFactor) const;

    Vec4 pow(Vec4 const &other) const;
    Vec4 pow(double other) const;

    Color toColor() const;

    Vec4 transform(Mat4 const &transform) const;

    Vec4 lerp(Vec4 const &other, double t) const;

    bool inViewFrustum() const;

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