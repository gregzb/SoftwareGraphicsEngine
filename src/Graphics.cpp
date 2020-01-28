#include "Graphics.hpp"
#include "Screen.hpp"
#include "Matrix.hpp"
#include "Vec.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <limits>

Graphics::Graphics(Screen &screen) : screen(screen){};

void Graphics::drawLine(double x0, double y0, double z0, double x1, double y1, double z1, Color color) const
{
    if (x1 < x0)
        return drawLine(x1, y1, z1, x0, y0, z0, color);

    double yOrig = y0;

    double total_dist = std::sqrt(std::pow(x1 - x0, 2) + std::pow(y1 - yOrig, 2));

    double deltaY = y1 - y0;
    int direction = sign(deltaY);
    double deltaError = std::abs(deltaY / static_cast<double>(x1 - x0));
    double error = 0;
    for (long x = std::lround(x0); x <= std::lround(x1); x++)
    {

        double currentDist = std::sqrt(std::pow(x - x0, 2) + std::pow(y0 - yOrig, 2));
        double zT = inverseLerp(0, total_dist, currentDist);
        double zLerped = lerp(z0, z1, zT);

        if (screen.zbuf(std::lround(y0), std::lround(x)) < zLerped)
        {
            screen.zbuf(std::lround(y0), std::lround(x)) = zLerped;
            screen(std::lround(y0), std::lround(x)) = color;
        }
        error += deltaError;
        while (error >= 0.5 && std::lround(y0) != std::lround(y1))
        {
            y0 += direction;
            currentDist = std::sqrt(std::pow(x - x0, 2) + std::pow(y0 - yOrig, 2));
            zT = inverseLerp(0, total_dist, currentDist);
            zLerped = lerp(z0, z1, zT);
            error -= 1;
            if (error >= 0.5)
            {
                if (screen.zbuf(std::lround(y0), std::lround(x)) < zLerped)
                {
                    screen.zbuf(std::lround(y0), std::lround(x)) = zLerped;
                    screen(std::lround(y0), std::lround(x)) = color;
                }
            }
        }
    }
}

void Graphics::drawEdges(Matrix &matrix, Color color) const
{
    for (int col = 0; col < matrix.getColumns(); col += 2)
    {
        drawLine(matrix[0][col], matrix[1][col], matrix[2][col], matrix[0][col + 1], matrix[1][col + 1], matrix[2][col + 1], color);
    }
}

void Graphics::drawTriangles(Matrix &matrix, Color color) const
{
    for (int col = 0; col < matrix.getColumns(); col += 3)
    {
        Vec normal = matrix.getTriangleNormal(col);
        std::vector<Vec> verts;
        for (int i = 0; i < 3; i++)
        {
            verts.push_back({{matrix[0][col + i], matrix[1][col + i], matrix[2][col + i]}});
        }

        if (normal[2] < std::numeric_limits<double>::epsilon() * 10)
        {
            continue;
        }

        Color randColor{col % 255, (col + 50) % 255, (col + 100) % 255, 255};

        fillTriangle(verts, randColor);

        randColor = {255, 255, 255, 255};
    }
}

void Graphics::fillTriangle(std::vector<Vec> &verts, Color color) const
{
    if (verts[0][1] > verts[1][1])
        std::swap(verts[0], verts[1]);
    if (verts[1][1] > verts[2][1])
        std::swap(verts[1], verts[2]);
    if (verts[0][1] > verts[1][1])
        std::swap(verts[0], verts[1]);

    Vec p1 = verts[2];
    Vec p2 = verts[2];

    Vec BT = verts[0].sub(verts[2]);
    BT = BT.scale(1 / std::abs(BT[1]));

    Vec MT = verts[1].sub(verts[2]);
    if (std::abs(MT[1]) > 0.00001)
    {
        MT = MT.scale(1 / std::abs(MT[1]));
        bool entered = false;
        while (p1[1] >= verts[1][1])
        {
            entered = true;
            drawLine(p1[0], p1[1], p1[2], p2[0], p2[1], p2[2], color);
            p1 = p1.add(BT);
            p2 = p2.add(MT);
        }
        if (entered)
        {
            p1 = p1.sub(BT);
            p2 = p2.sub(MT);
        }
    }
    else
    {
        p2 = verts[1];
    }

    p2 = verts[1];
    p1[1] = p2[1];

    Vec BM = verts[0].sub(verts[1]);
    if (std::abs(BM[1]) > 0.00001)
    {
        BM = BM.scale(1 / std::abs(BM[1]));
        while (p1[1] >= verts[0][1])
        {
            drawLine(p1[0], p1[1], p1[2], p2[0], p2[1], p2[2], color);
            p1 = p1.add(BT);
            p2 = p2.add(BM);
        }
    }
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