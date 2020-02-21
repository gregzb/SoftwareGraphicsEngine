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

double Utils::lerp(double a, double b, double t)
{
    return (1 - t) * a + t * b;
}

float Utils::clamp(double lower, double upper, double val) {
  return std::max(lower, std::min(val, upper));
}

std::vector<Vec> Utils::linePixels(double x0, double y0, double z0, double x1, double y1, double z1)
{
    std::vector<Vec> temp;
    if (x1 < x0)
    {
        temp = linePixels(x1, y1, z1, x0, y0, z0);
        std::reverse(temp.begin(), temp.end());
        return temp;
    }

    long x0r = std::lround(x0);
    long x1r = std::lround(x1);
    long y0r = std::lround(y0);
    long y1r = std::lround(y1);

    double deltaY = y1 - y0;
    double deltaX = x1 - x0;
    double deltaError = std::abs((y1r - y0r) / static_cast<double>(x1r - x0r));

    int dir = Utils::sign(deltaY);

    double error = 0;

    double totalDist = std::sqrt(std::pow((x1r - x0r), 2) + std::pow((y1r - y0r), 2));

    int y = y0r;
    for (int x = x0r; x <= x1r; x++)
    {
        double currentDist = std::sqrt(std::pow(x - x0r, 2) + std::pow(y - y0r, 2));
        double zT = inverseLerp(0, totalDist, currentDist);
        double z = std::abs(totalDist) < std::numeric_limits<double>::epsilon() * 100 ? std::max(z0, z1) : lerp(z0, z1, zT);
        z = clamp(z0, z1, z);
        //z -= std::numeric_limits<double>::epsilon() * 10000;
        //z = lerp(z0, z1, zT);

        temp.push_back({{static_cast<double>(x), static_cast<double>(y), z}});
        error += deltaError;

        while (error >= 0.5 && y != y1r)
        {
            y += dir;
            error -= 1;
            if (error >= 0.5)
            {
                currentDist = std::sqrt(std::pow(x - x0r, 2) + std::pow(y - y0r, 2));
                zT = inverseLerp(0, totalDist, currentDist);
                double z = std::abs(totalDist) < std::numeric_limits<double>::epsilon() * 100 ? std::max(z0, z1) : lerp(z0, z1, zT);
                z = clamp(z0, z1, z);
                //z -= std::numeric_limits<double>::epsilon() * 10000;
                //z = lerp(z0, z1, zT);

                temp.push_back({{static_cast<double>(x), static_cast<double>(y), z}});
            }
        }
    }
    return temp;
}

std::vector<Vec> Utils::linePixels(Vec a, Vec b)
{
    return linePixels(a[0], a[1], a[2], b[0], b[1], b[2]);
}
