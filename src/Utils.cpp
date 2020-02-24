#include "Utils.hpp"
#include <cmath>
#include <vector>
#include <algorithm>

Color::Color(){};
Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a){};
Color::Color(int r, int g, int b, int a)
{
    this->r = (unsigned char)r;
    this->g = (unsigned char)g;
    this->b = (unsigned char)b;
    this->a = (unsigned char)a;
}
Color::Color(Vec4 v) {
    this->r = static_cast<u_char>(v[0]);
    this->g = static_cast<u_char>(v[1]);
    this->b = static_cast<u_char>(v[2]);
    this->a = static_cast<u_char>(v[3]);
}

int Utils::sign(double x)
{
    return (x > 0) - (x < 0);
}

double Utils::inverseLerp(double a, double b, double val)
{
    return (val - a) / (b - a);
}

double Utils::lerp(double a, double b, double t)
{
    return (1 - t) * a + t * b;
}

double Utils::map(double value, double low1, double high1, double low2, double high2) { //optimize this usage in screen space area
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}