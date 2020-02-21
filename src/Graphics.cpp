#include "Graphics.hpp"
#include "Screen.hpp"
#include "Vec3.hpp"
#include "Utils.hpp"
#include "Mat4.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <limits>
#include <tuple>

Graphics::Graphics(Screen &screen) : screen(screen){};

void Graphics::drawLine(Vec3 a, Vec3 b, Color color, bool debug) const
{
    std::vector<Vec3> pixels = Utils::linePixelsAndZ(a, b);

    // double zInc = (b.z - a.z) / (pixels.size() == 1 ? 1 : (pixels.size() - 1));
    // double z = a.z;

    for (auto &pixel : pixels)
    {
        if (screen.zbuf(pixel.y, pixel.x) < pixel.z)
        {
            screen.zbuf(pixel.y, pixel.x) = pixel.z;
            screen(pixel.y, pixel.x) = color;
        }
        //screen(pixel.y, pixel.x) = color;
        //z += zInc;
    }
}

void Graphics::drawEdges(Mat4 &matrix, Color color) const
{
    for (int col = 0; col < matrix.getCols(); col += 2)
    {
        drawLine(matrix.getPoint(col), matrix.getPoint(col + 1), color);
    }
}

void Graphics::drawTriangles(Mat4 &matrix, Color color) const
{
    std::vector<Vec3> points(3);
    //std::cout << matrix.getCols() << std::endl;
    for (int col = 0; col < matrix.getCols(); col += 3)
    {
        Vec3 normal = matrix.getTriangleNormal(col);

        if (normal.z < std::numeric_limits<double>::epsilon() * 100)
        {
            continue;
        }
        //std::cout << std::endl;
        //std::cout << "Tri: ";

        for (int i = 0; i < 3; i++)
        {
            points[i] = matrix.getPoint(col + i);
            //std::cout << points[i];
        }

        //std::cout << std::endl;

        color = {255, (col * 7) % 256, (col * 13 + 50) % 256, 255};

        fillTriangle(points, color);

        // for (int i = 0; i < 3; i++)
        // {
        //     // std::cout << "drawing from/to" << std::endl;
        //     // std::cout << points[i][0] << " " << points[i][1] << " " << points[i][2] << std::endl;
        //     // std::cout << verts[(i + 1) % 3][0] << " " << verts[(i + 1) % 3][1] << " " << verts[(i + 1) % 3][2] << std::endl;
        //     //drawLine(verts[i][0], verts[i][1], verts[i][2] + 0.5, verts[(i + 1) % 3][0], verts[(i + 1) % 3][1], verts[(i + 1) % 3][2] + 0.5, randColor, true);
        //     drawLine(points[i].add({0, 0, 1000}), points[(i + 1) % 3].add({0, 0, 1000}), {255, 255, 255, 255}, true);
        // }
    }
}

struct InterpInfo
{
    int x;
    double z;
};

void projectSide(std::vector<InterpInfo> &scanlines, Vec3 const &lower, Vec3 const &higher, int minVal, int side)
{
    int x0 = std::ceil(lower.x), y0 = std::ceil(lower.y), x1 = std::ceil(higher.x), y1 = std::ceil(higher.y);
    double dY = higher.y - lower.y, dX = higher.x - lower.x, dZ = higher.z - lower.z;

    if (dY <= 0)
        return;

    double step = dX / dY;
    double zStep = dZ / dY;

    double x = lower.x + (y0 - lower.y) * step;
    double z = lower.z + (y0 - lower.y) * zStep;

    for (int y = y0; y < y1; y++)
    {
        scanlines[(y - minVal) * 2 + side] = {std::ceil(x), z};
        x += step;
        z += zStep;
    }
}

void Graphics::fillTriangle(std::vector<Vec3> &verts, Color color) const
{
    /*
    Greggo, try two things -> make line pixels return z val, and also try calc-ing z val here
    */

    if (verts[0].y > verts[1].y)
        std::swap(verts[0], verts[1]);
    if (verts[1].y > verts[2].y)
        std::swap(verts[1], verts[2]);
    if (verts[0].y > verts[1].y)
        std::swap(verts[0], verts[1]);

    int side = Utils::sign(verts[1].sub(verts[0]).cross(verts[2].sub(verts[0])).z);
    side = side >= 0 ? 0 : 1;

    std::vector<InterpInfo> scanlines(2 * (std::ceil(verts[2].y) - std::ceil(verts[0].y)));

    projectSide(scanlines, verts[0], verts[2], std::ceil(verts[0].y), side);
    projectSide(scanlines, verts[0], verts[1], std::ceil(verts[0].y), 1 - side);
    projectSide(scanlines, verts[1], verts[2], std::ceil(verts[0].y), 1 - side);

    for (uint i = 0; i < scanlines.size(); i += 2)
    {
        InterpInfo leftInterp = scanlines[i];
        InterpInfo rightInterp = scanlines[i + 1];

        double zStep = (rightInterp.z - leftInterp.z) / (rightInterp.x - leftInterp.x);
        double z = leftInterp.z;

        int y = std::ceil(verts[0].y) + i / 2;

        for (int x = leftInterp.x; x < rightInterp.x; x++)
        {
            if (screen.zbuf(y, x) < z)
            {
                screen.zbuf(y, x) = z;
                screen(y, x) = color;
            }
            //screen(y, x) = color;
            z += zStep;
        }
    }

    // std::vector<Vec3> side1Pixels = Utils::linePixelsAndZ(verts[2], verts[0]);
    // std::vector<Vec3> side1;
    // side1.push_back(side1Pixels.front());
    // for (auto pixel : side1Pixels) {
    //     if (pixel.y != side1.back().y) {
    //         side1.push_back(pixel);
    //     } else {
    //         side1.back() = pixel;
    //     }
    // }

    // std::vector<Vec3> side2Pixels = Utils::linePixelsAndZ(verts[2], verts[1]);
    // std::vector<Vec3> side2;
    // side2.push_back(side2Pixels.front());
    // for (auto pixel : side2Pixels) {
    //     if (pixel.y != side2.back().y) {
    //         side2.push_back(pixel);
    //     } else {
    //         side2.back() = pixel;
    //     }
    // }

    // std::vector<Vec3> side3Pixels = Utils::linePixelsAndZ(verts[1], verts[0]);
    // for (auto pixel : side3Pixels) {
    //     if (pixel.y != side2.back().y) {
    //         side2.push_back(pixel);
    //     }
    // }

    // for (uint i = 0; i < side1.size(); i++) {
    //     std::cout << side1[i] << side2[i] << std::endl;
    //     drawLine(side1[i], side2[i], color);
    // }

    // for (int i = 0; i < side1Pixels.size(); i++)
    // {
    //     if (side1Pixels[i][1] != side1.back()[1])
    //     {
    //         side1.push_back(side1Pixels[i]);
    //     }
    //     else
    //     {
    //         //side1.back()[2] = std::min(side1.back()[2], side1Pixels[i][2]);
    //         side1.back() = side1Pixels[i];
    //     }
    // }
}

void Graphics::clear(Color color) const
{
    for (int row = 0; row < screen.getHeight(); row++)
    {
        for (int col = 0; col < screen.getWidth(); col++)
        {
            screen(row, col) = color;
        }
    }
}