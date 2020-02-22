#pragma once

#include "Vec3.hpp"
#include <vector>

enum class CurveType {
    Hermite,
    Bezier
};

class Color
{
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    Color();
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    Color(int r, int g, int b, int a);
};

namespace Utils
{
int sign(double x);
double inverseLerp(double a, double b, double val);
double lerp(double a, double b, double t);
double map(double value, double low1, double high1, double low2, double high2);
} // namespace Utils