#pragma once

#include <iostream>

class Vec3
{
public:
    double x, y, z, w;

    Vec3(double x, double y, double z, double w);
    Vec3(double x, double y, double z);
    Vec3(double x, double y);
    Vec3();
    double magnitude();
    double dot(Vec3 other);
    Vec3 normalize();
    Vec3 negate();
    Vec3 cross(Vec3 other);
    Vec3 scale(Vec3 other);
    Vec3 scale(double other);
    Vec3 add(Vec3 other);
    Vec3 add(double other);
    Vec3 sub(Vec3 other);
    Vec3 sub(double other);

    friend std::ostream& operator<<(std::ostream& os, const Vec3 v);
    friend std::istream& operator>>(std::istream& is, Vec3& v);
};