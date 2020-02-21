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
std::vector<Vec3> linePixels(Vec3 a, Vec3 b, bool flipped = false);
std::vector<Vec3> linePixelsAndZ(Vec3 a, Vec3 b);
} // namespace Utils