#pragma once

#include <iostream>

class Vec4
{
public:
    double x, y, z, w;

    Vec4(double x, double y, double z, double w);
    Vec4();
    Vec4 negate();
    Vec4 scale(Vec4 other);
    Vec4 scale(double other);
    Vec4 add(Vec4 other);
    Vec4 add(double other);
    Vec4 sub(Vec4 other);
    Vec4 sub(double other);

    friend std::ostream& operator<<(std::ostream& os, const Vec4 v);
    friend std::istream& operator>>(std::istream& is, Vec4& v);
};