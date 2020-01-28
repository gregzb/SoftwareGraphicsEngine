#include "Utils.hpp"

Color::Color(){};
Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a){};
Color::Color(int r, int g, int b, int a)
{
    this->r = (unsigned char)r;
    this->g = (unsigned char)g;
    this->b = (unsigned char)b;
    this->a = (unsigned char)a;
}

int sign(double x)
{
    return (x > 0) - (x < 0);
}

double inverseLerp(double a, double b, double val)
{
    return (val - a) / (b - a);
}

int lerp(double a, double b, double t)
{
    return (1 - t) * a + t * b;
}