#include "Utils.hpp"
#include "Vec.hpp"
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>

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

float Utils::clamp(double lower, double upper, double val)
{
    if (lower > upper)
    {
        return clamp(upper, lower, val);
    }
    return std::max(lower, std::min(val, upper));
}

// std::vector<Vec> Utils::linePixels(double x0, double y0, double z0, double x1, double y1, double z1)
// {
//     std::vector<Vec> temp;
//     if (x1 < x0)
//     {
//         temp = linePixels(x1, y1, z1, x0, y0, z0);
//         std::reverse(temp.begin(), temp.end());
//         return temp;
//     }

//     long x0r = std::lround(x0);
//     long x1r = std::lround(x1);
//     long y0r = std::lround(y0);
//     long y1r = std::lround(y1);

//     double deltaY = y1 - y0;
//     double deltaX = x1 - x0;
//     double deltaError = std::abs((y1r - y0r) / static_cast<double>(x1r - x0r));

//     int dir = Utils::sign(deltaY);

//     double error = 0;

//     double totalDist = std::sqrt(std::pow((x1r - x0r), 2) + std::pow((y1r - y0r), 2));

//     int y = y0r;
//     for (int x = x0r; x <= x1r; x++)
//     {
//         double currentDist = std::sqrt(std::pow(x - x0r, 2) + std::pow(y - y0r, 2));
//         double zT = inverseLerp(0, totalDist, currentDist);
//         double z = std::abs(totalDist) < std::numeric_limits<double>::epsilon() * 100 ? std::max(z0, z1) : lerp(z0, z1, zT);
//         z = clamp(z0, z1, z);
//         //z -= std::numeric_limits<double>::epsilon() * 10000;
//         //z = lerp(z0, z1, zT);

//         temp.push_back({{static_cast<double>(x), static_cast<double>(y), z}});
//         error += deltaError;

//         while (error >= 0.5 && y != y1r)
//         {
//             y += dir;
//             error -= 1;
//             if (error >= 0.5)
//             {
//                 currentDist = std::sqrt(std::pow(x - x0r, 2) + std::pow(y - y0r, 2));
//                 zT = inverseLerp(0, totalDist, currentDist);
//                 double z = std::abs(totalDist) < std::numeric_limits<double>::epsilon() * 100 ? std::max(z0, z1) : lerp(z0, z1, zT);
//                 z = clamp(z0, z1, z);
//                 //z -= std::numeric_limits<double>::epsilon() * 10000;
//                 //z = lerp(z0, z1, zT);

//                 temp.push_back({{static_cast<double>(x), static_cast<double>(y), z}});
//             }
//         }
//     }
//     return temp;
// }

// std::vector<Vec> Utils::linePixels(double x0, double y0, double z0, double x1, double y1, double z1, bool flipped)
// {
//     std::vector<Vec> temp;
//     if (x1 < x0)
//     {
//         temp = linePixels(x1, y1, z1, x0, y0, z0, flipped);
//         std::reverse(temp.begin(), temp.end());
//         return temp;
//     }

//     long x0r = std::lround(x0), x1r = std::lround(x1), y0r = std::lround(y0), y1r = std::lround(y1);

//     long dY = y1r - y0r, dX = x1r - x0r;

//     if (std::abs(dY) > std::abs(dX))
//     {
//         return linePixels(y0, x0, z0, y1, x1, z1, true);
//     }

//     int dir = Utils::sign(dY);

//     double dErr = std::abs(dY / static_cast<double>(dX));
//     double error = 0;

//     //double totalDist = std::sqrt(std::pow(dX, 2) + std::pow(dY, 2));
//     //double totalDist = std::sqrt(std::pow(x1-x0, 2) + std::pow(y1-y0, 2));
//     double totalDist = std::sqrt(std::pow(dX, 2) + std::pow(dY, 2));

//     int y = y0r;
//     for (int x = x0r; x <= x1r; x++)
//     {
//         double currentDist = std::sqrt(std::pow(x - x0r, 2) + std::pow(y - y0r, 2));
//         double zT = inverseLerp(0, totalDist, currentDist);
//         double z = std::abs(totalDist) < std::numeric_limits<double>::epsilon() * 100 ? std::max(z0, z1) : lerp(z0, z1, zT);
//         //z = clamp(z0, z1, z);

//         //std::cout << x << " " << y << " " << z << ", " << z0 << " " << z1 << std::endl;

//         //z -= std::numeric_limits<double>::epsilon() * 10000;
//         //z = lerp(z0, z1, zT);

//         if (!flipped)
//             temp.push_back({{static_cast<double>(x), static_cast<double>(y), z}});
//         else
//             temp.push_back({{static_cast<double>(y), static_cast<double>(x), z}});
//         error += dErr;

//         if (error >= 0.5)
//         {
//             y += dir;
//             error -= 1;
//         }
//     }
//     return temp;
// }

std::vector<Vec> Utils::linePixels(double x0, double y0, double x1, double y1, bool flipped)
{
    std::vector<Vec> temp;
    if (x1 < x0)
    {
        temp = linePixels(x1, y1, x0, y0, flipped);
        std::reverse(temp.begin(), temp.end());
        return temp;
    }

    long x0r = std::lround(x0), x1r = std::lround(x1), y0r = std::lround(y0), y1r = std::lround(y1);

    long dY = y1r - y0r, dX = x1r - x0r;

    if (std::abs(dY) > dX)
    {
        return linePixels(y0, x0, y1, x1, true);
    }

    int dir = Utils::sign(dY);

    dY = std::abs(dY) * 2;
    
    int d = dY-dX;

    dX *= 2;

    int y = y0r;
    for (int x = x0r; x <= x1r; x++)
    {
        if (!flipped)
            temp.push_back({{static_cast<double>(x), static_cast<double>(y)}});
        else
            temp.push_back({{static_cast<double>(y), static_cast<double>(x)}});

        if (d > 0)
        {
            y += dir;
            d -= dX;
        }
        d += dY;
    }
    return temp;
}

std::vector<Vec> Utils::linePixels(Vec a, Vec b)
{
    return linePixels(a[0], a[1], b[0], b[1], false);
}
