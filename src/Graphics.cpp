#include "Graphics.hpp"
#include "Screen.hpp"
#include "Matrix.hpp"
#include "Vec.hpp"
#include "Utils.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <limits>

Graphics::Graphics(Screen &screen) : screen(screen){};

void Graphics::drawLine(double x0, double y0, double z0, double x1, double y1, double z1, Color color) const
{

    if (x1 < x0)
    {
        drawLine(x1, y1, z1, x0, y0, z0, color);
        return;
    }

    double deltaX = x1 - x0;
    int xDir = Utils::sign(deltaX);

    long x0Rounded = std::lround(x0);
    long x1Rounded = std::lround(x1);
    long y0Rounded = std::lround(y0);
    long y1Rounded = std::lround(y1);

    double deltaY = y1 - y0;
    int direction = Utils::sign(deltaY);

    double totalDist = std::sqrt(std::pow(deltaX, 2) + std::pow(deltaY, 2));

    double deltaError = std::abs(deltaY / deltaX);
    //double error = (x0 - x0Rounded) * deltaError;
    double error = 0;
    // if (y1 < y0)
    // {
    //     error -= deltaError;
    // }
    // if (deltaY > deltaX)
    // {
    //     error = std::max(error, 0.5 - deltaError + std::numeric_limits<double>::epsilon() * 100);
    // }
    // error = std::max(error, 0.5-deltaError + std::numeric_limits<double>::epsilon() * 100);
    if (std::isinf(deltaError))
    {
        error = deltaError;
    }

    int y = y0Rounded;

    for (int x = x0Rounded; x - x1Rounded != xDir || xDir == 0; x += xDir)
    {

        double currentDist = std::sqrt(std::pow(x - x0, 2) + std::pow(y - y0, 2));
        double zT = Utils::inverseLerp(0, totalDist, currentDist);
        double z = Utils::lerp(z0, z1, zT);

        if (screen.zbuf(y, x) < z)
        {
            screen.zbuf(y, x) = z;
            screen(y, x) = color;
        }

        error += deltaError;
        while (error >= 0.5 && y != y1Rounded)
        {
            y += direction;
            error -= 1;
            if (error >= 0.5)
            {
                currentDist = std::sqrt(std::pow(x - x0, 2) + std::pow(y - y0, 2));
                zT = Utils::inverseLerp(0, totalDist, currentDist);
                z = Utils::lerp(z0, z1, zT);
                if (screen.zbuf(y, x) < z)
                {
                    screen.zbuf(y, x) = z;
                    screen(y, x) = color;
                }
            }
        }
        if (xDir == 0)
            break;
    }
    while (error >= 0.5 && y != y1Rounded)
    {
        y += direction;
        error -= 1;
        if (error >= 0.5)
        {
            double currentDist = std::sqrt(std::pow(x1Rounded - x0, 2) + std::pow(y - y0, 2));
            double zT = Utils::inverseLerp(0, totalDist, currentDist);
            double z = Utils::lerp(z0, z1, zT);
            if (screen.zbuf(y, x1Rounded) < z)
            {
                screen.zbuf(y, x1Rounded) = z;
                screen(y, x1Rounded) = color;
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

        //fillTriangle(verts, randColor);

        randColor = {255, 255, 255, 255};

        for (int i = 0; i < 3; i++)
        {
            drawLine(verts[i][0], verts[i][1], verts[i][2], verts[(i + 1) % 3][0], verts[(i + 1) % 3][1], verts[(i + 1) % 3][2], randColor);
            // std::cout << "drawing from/to" << std::endl;
            // std::cout << verts[i][0] << " " << verts[i][1] << " " << verts[i][2] << std::endl;
            // std::cout << verts[(i + 1) % 3][0] << " " << verts[(i + 1) % 3][1] << " " << verts[(i + 1) % 3][2] << std::endl;
        }

        // drawLine(verts[0][0], verts[0][1], verts[0][2], verts[1][0], verts[1][1], verts[1][2], randColor);
        // drawLine(verts[1][0], verts[1][1], verts[1][2], verts[2][0], verts[2][1], verts[2][2], randColor);
        // drawLine(verts[0][0], verts[0][1], verts[0][2], verts[2][0], verts[2][1], verts[2][2], randColor);
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

    //std::cout << "side1" << std::endl;

    auto side1 = Utils::getFillVals(verts[2], verts[0]);

    //std::cout << "side2" << std::endl;
    auto side2 = Utils::getFillVals(verts[2], verts[1]);

    //std::cout << "side3" << std::endl;
    auto side3 = Utils::getFillVals(verts[1], verts[0]);
    int remove = side2.size() + side3.size() > side1.size();
    side2.insert(side2.end(), side3.begin() + remove, side3.end());

    //std::cout << "sizes: " << side1.size() << " " << side2.size() << std::endl;

    int topY = std::lround(verts[2][1]);
    int bottomY = std::lround(verts[0][1]);

    int counter = 0;

    //std::cout << "about to draw" << std::endl;

    for (int y = topY; y >= bottomY; y--, counter++)
    {
        // std::cout << "drawing at y = " << y << ", counter = " << counter << std::endl;
        // std::cout << side1[counter].first << " " << side1[counter].second << std::endl;
        // std::cout << side2[counter].first << " " << side2[counter].second << std::endl;
        // std::cout << side2.size() << std::endl;
        drawLine(side1[counter].first, y, side1[counter].second, side2[counter].first, y, side2[counter].second, color);
    }

    //std::cout << "done drawing" << std::endl;

    //std::cout << side1.size() << " " << side2.size() << " " << side3.size() << std::endl;

    // Vec p1 = verts[2];
    // Vec p2 = verts[2];

    // Vec BT = verts[0].sub(verts[2]);
    // BT = BT.scale(1 / std::abs(BT[1]));

    // Vec MT = verts[1].sub(verts[2]);
    // if (std::abs(MT[1]) > 0.00001)
    // {
    //     MT = MT.scale(1 / std::abs(MT[1]));
    //     bool entered = false;
    //     while (p1[1] >= verts[1][1])
    //     {
    //         entered = true;
    //         drawLine(p1[0], p1[1], p1[2], p2[0], p2[1], p2[2], color);
    //         p1 = p1.add(BT);
    //         p2 = p2.add(MT);
    //     }
    //     if (entered)
    //     {
    //         p1 = p1.sub(BT);
    //         p2 = p2.sub(MT);
    //     }
    // }
    // else
    // {
    //     p2 = verts[1];
    // }

    // p2 = verts[1];
    // p1[1] = p2[1];

    // Vec BM = verts[0].sub(verts[1]);
    // if (std::abs(BM[1]) > 0.00001)
    // {
    //     BM = BM.scale(1 / std::abs(BM[1]));
    //     while (p1[1] >= verts[0][1])
    //     {
    //         drawLine(p1[0], p1[1], p1[2], p2[0], p2[1], p2[2], color);
    //         p1 = p1.add(BT);
    //         p2 = p2.add(BM);
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