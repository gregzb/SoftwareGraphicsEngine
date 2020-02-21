#include "Utils.hpp"
#include "Vec3.hpp"
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

std::vector<Vec3> Utils::linePixels(Vec3 a, Vec3 b, bool flipped)
{
    std::vector<Vec3> temp;
    if (b.x < a.x)
    {
        temp = linePixels(b, a, flipped);
        // std::vector<Vec3> uglyTemp;

        // for (uint i = 0; i < temp.size(); i++)
        // {
        //     uglyTemp.push_back(temp[i]);
        // }

        std::reverse(temp.begin(), temp.end());
        return temp;
    }

    long x0r = std::lround(a.x), x1r = std::lround(b.x), y0r = std::lround(a.y), y1r = std::lround(b.y);

    long dY = y1r - y0r, dX = x1r - x0r;

    if (std::abs(dY) > dX)
    {
        return linePixels({a.y, a.x}, {b.y, b.x}, true);
    }

    int dir = sign(dY);

    dY = std::abs(dY) * 2;

    int d = dY - dX;

    dX *= 2;

    int y = y0r;
    for (int x = x0r; x <= x1r; x++)
    {
        if (!flipped)
            temp.push_back({static_cast<double>(x), static_cast<double>(y)});
        else
            temp.push_back({static_cast<double>(y), static_cast<double>(x)});

        if (d > 0)
        {
            y += dir;
            d -= dX;
        }
        d += dY;
    }
    return temp;
}

std::vector<Vec3> Utils::linePixelsAndZ(Vec3 a, Vec3 b)
{
    std::vector<Vec3> pixels = Utils::linePixels(a, b);
    double zInc = (b.z - a.z) / (pixels.size() == 1 ? 1 : (pixels.size() - 1));
    //double zInc = (b.z-a.z) / pixels.size();
    double z = a.z;
    for (uint i = 0; i < pixels.size(); i++) {
        pixels[i].z = z;
        z += zInc;
    }
    return pixels;
}