#include "Utils.hpp"
#include "Vec.hpp"
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

int Utils::lerp(double a, double b, double t)
{
    return (1 - t) * a + t * b;
}

std::vector<std::pair<int, double>> Utils::getFillVals(double x0, double y0, double z0, double x1, double y1, double z1)
{
    std::vector<std::pair<int, double>> temp;

    if (x1 < x0)
    {
        temp = Utils::getFillVals(x1, y1, z1, x0, y0, z0);
        std::reverse(temp.begin(), temp.end());
        return temp;
    }

    long x0Rounded = std::lround(x0);
    long x1Rounded = std::lround(x1);
    long y0Rounded = std::lround(y0);
    long y1Rounded = std::lround(y1);

    double deltaY = y1 - y0;
    double deltaX = x1 - x0;

    int direction = Utils::sign(deltaY);

    double totalDist = std::sqrt(std::pow(deltaX, 2) + std::pow(deltaY, 2));

    double deltaError = std::abs(deltaY / deltaX);
    double error = 0;

    int y = y0Rounded;

    double currentDist = std::sqrt(std::pow(x0Rounded - x0, 2) + std::pow(y - y0, 2));
    double zT = Utils::inverseLerp(0, totalDist, currentDist);
    double z = Utils::lerp(z0, z1, zT);

    temp.push_back({x0Rounded, z});

    for (int x = x0Rounded; x <= x1Rounded; x++)
    {
        error += deltaError;
        while (error >= 0.5 && y != y1Rounded)
        {
            y += direction;
            error -= 1;
            // if (error >= 0.5)
            // {
            currentDist = std::sqrt(std::pow(x - x0, 2) + std::pow(y - y0, 2));
            zT = Utils::inverseLerp(0, totalDist, currentDist);
            z = Utils::lerp(z0, z1, zT);

            temp.push_back({x, z});
            // }
        }
    }
    return temp;
}

std::vector<std::pair<int, double>> Utils::getFillVals(Vec a, Vec b)
{
    return getFillVals(a[0], a[1], a[2], b[0], b[1], b[2]);
}