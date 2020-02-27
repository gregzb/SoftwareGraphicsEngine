#pragma once

#include <vector>
#include "Vec4.hpp"

enum class CurveType
{
    Hermite,
    Bezier
};

struct Color
{
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    Color();
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    Color(int r, int g, int b, int a);
    Color(Vec4 const &v);
};

namespace Utils
{
int sign(double x);
double inverseLerp(double a, double b, double val);
double lerp(double a, double b, double t);
double map(double value, double low1, double high1, double low2, double high2);
} // namespace Utils