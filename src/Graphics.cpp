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

void Graphics::drawLine(Vec3 a, Vec3 b, Color color) const
{
    drawLine(a, b, color, false);
}

void Graphics::drawLine(Vec3 a, Vec3 b, Color color, bool flipped) const
{
    if (b.x < a.x)
    {
        return drawLine(b, a, color, flipped);
    }

    long x0r = std::lround(a.x), x1r = std::lround(b.x), y0r = std::lround(a.y), y1r = std::lround(b.y);

    long dY = y1r - y0r, dX = x1r - x0r;

    if (std::abs(dY) > dX)
    {
        return drawLine({a.y, a.x}, {b.y, b.x}, color, !flipped);
    }

    int dir = Utils::sign(dY);

    dY = std::abs(dY) * 2;

    int d = dY - dX;

    dX *= 2;

    int y = y0r;
    for (int x = x0r; x <= x1r; x++)
    {
        if (!flipped)
            screen.plot({x, y}, color);
        else
            screen.plot({y, x}, color);

        if (d > 0)
        {
            y += dir;
            d -= dX;
        }
        d += dY;
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
        if (matrix.getPoint(col).dot(normal) < 0) {
            continue; //Normal calculation might be broken, should check on it. (May need to do calculation earlier)
        }

        // if (normal.z < std::numeric_limits<double>::epsilon() * 100)
        // {
        //     continue;
        // }
        //std::cout << std::endl;
        //std::cout << "Tri: ";

        for (int i = 0; i < 3; i++)
        {
            points[i] = matrix.getPoint(col + i);
            points[i].x = Utils::map(points[i].x, -1, 1, 0, screen.getWidth());
            points[i].y = Utils::map(points[i].y, -1, 1, 0, screen.getHeight());
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
    int y0 = std::ceil(lower.y), y1 = std::ceil(higher.y);
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
            z += zStep;
        }
    }
}

void Graphics::renderObject(Camera &cam, RenderObject &object) const
{
    //instead of doing 3 mults here, get all 3 matrices and then chain and do one mult
    Mat4 tris = object.toWorldSpace();

    tris.multiplyMutate(cam.getViewMatrix());
    tris.multiplyMutate(cam.getProjectionMatrix());
    //clipping must be done here - clip from -w to w for all dimensions (or dont), -w's get discard, so clip that too
    //std::cout << tris.toString() << std::endl;
    Mat4 clipped;

    std::vector<Vec4> points(3);
    for (int col = 0; col < tris.getCols(); col += 3)
    {
        int wPosCount = 0;
        for (int i = 0; i < 3; i++)
        {
            points[i] = tris.getVec4(col + i);
            // > 0 or greater than near? not sure
            if (points[i].w > 0) {
                wPosCount += 1;
            }
        }

        if (wPosCount == 0) {
            //no clipping, dont add
        } else if (wPosCount == 1) {
            //simpler clip
            clipped.addVec4(points[0]);
            clipped.addVec4(points[1]);
            clipped.addVec4(points[2]);
        } else if (wPosCount == 2) {
            //harder clip, need two tris
            clipped.addVec4(points[0]);
            clipped.addVec4(points[1]);
            clipped.addVec4(points[2]);
        } else if (wPosCount == 3) {
            //no clipping, add
            clipped.addVec4(points[0]);
            clipped.addVec4(points[1]);
            clipped.addVec4(points[2]);
        }
    }

    clipped.perspectiveDivision();

    drawTriangles(clipped, {255, 255, 255, 255});
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