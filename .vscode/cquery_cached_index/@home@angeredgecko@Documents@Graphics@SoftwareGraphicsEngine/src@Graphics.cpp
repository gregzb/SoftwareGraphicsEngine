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

// void Graphics::drawLine(double x0, double y0, double z0, double x1, double y1, double z1, Color color, bool debug) const
// {
//     if (x1 < x0)
//     {
//         return drawLine(x1, y1, z1, x0, y0, z0, color, debug);
//     }

//     long x0Rounded = std::lround(x0);
//     long x1Rounded = std::lround(x1);
//     long y0Rounded = std::lround(y0);
//     long y1Rounded = std::lround(y1);

//     double deltaY = y1 - y0;
//     double deltaX = x1 - x0;

//     int direction = Utils::sign(deltaY);

//     double totalDist = std::sqrt(std::pow(deltaX, 2) + std::pow(deltaY, 2));

//     double deltaError = std::abs(deltaY / deltaX);
//     double error = 0;

//     int y = y0Rounded;

//     for (int x = x0Rounded; x <= x1Rounded; x++)
//     {

//         double currentDist = std::sqrt(std::pow(x - x0, 2) + std::pow(y - y0, 2));
//         double zT = Utils::inverseLerp(0, totalDist, currentDist);
//         double z = Utils::lerp(z0, z1, zT);

//         if (screen.zbuf(y, x) < z)
//         {
//             screen.zbuf(y, x) = z;
//             screen(y, x) = color;
//             if (debug)
//                 std::cout << "Draw (" << screen.zbuf(y, x) << ")at " << x << " " << y << " " << z << std::endl;
//         }
//         else
//         {
//             if (debug)
//                 std::cout << "Zbuff blocked (" << screen.zbuf(y, x) << ") at " << x << " " << y << " " << z << std::endl;
//         }

//         error += deltaError;
//         while (error >= 0.5 && y != y1Rounded)
//         {
//             y += direction;
//             error -= 1;
//             if (error >= 0.5)
//             {
//                 currentDist = std::sqrt(std::pow(x - x0, 2) + std::pow(y - y0, 2));
//                 zT = Utils::inverseLerp(0, totalDist, currentDist);
//                 z = Utils::lerp(z0, z1, zT);
//                 if (screen.zbuf(y, x) < z)
//                 {
//                     screen.zbuf(y, x) = z;
//                     screen(y, x) = color;
//                     if (debug)
//                         std::cout << "Draw (" << screen.zbuf(y, x) << ")at " << x << " " << y << " " << z << std::endl;
//                 }
//                 else
//                 {
//                     if (debug)
//                         std::cout << "Zbuff blocked (" << screen.zbuf(y, x) << ") at " << x << " " << y << " " << z << std::endl;
//                 }
//             }
//         }
//     }
// }

void Graphics::drawLine(double x0, double y0, double z0, double x1, double y1, double z1, Color color, bool debug) const
{
    std::vector<Vec> pixels = Utils::linePixels(x0, y0, z0, x1, y1, z1);
    for (auto pixel : pixels)
    {
        double x, y, z;
        x = pixel[0];
        y = pixel[1];
        z = pixel[2];
        //if (screen.zbuf(y, x) <= std::ceil(z * 100000) / 100000)
        if (screen.zbuf(y, x) <= z)
        {
            double old = screen.zbuf(y, x);
            screen.zbuf(y, x) = z;
            //screen.zbuf(y, x) = std::ceil(z * 100000) / 100000;
            screen(y, x) = color;
            if (debug)
                std::cout << "Draw (" << old << ") at " << x << " " << y << " " << z << std::endl;
        }
        else
        {
            if (debug || 1)
                std::cout << "Zbuff blocked (" << screen.zbuf(y, x) << ") at " << x << " " << y << " " << z << std::endl;
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

        std::cout << std::endl;

        for (int i = 0; i < verts.size(); i++) {
          std::cout << verts[i][0] << " " << verts[i][1] << " " << verts[i][2] << std::endl;
        }

        randColor = {255, 255, 255, 255};

        for (int i = 0; i < 3; i++)
        {
            std::cout << "drawing from/to" << std::endl;
            std::cout << verts[i][0] << " " << verts[i][1] << " " << verts[i][2] << std::endl;
            std::cout << verts[(i + 1) % 3][0] << " " << verts[(i + 1) % 3][1] << " " << verts[(i + 1) % 3][2] << std::endl;
            drawLine(verts[i][0], verts[i][1], verts[i][2] + 0.01, verts[(i + 1) % 3][0], verts[(i + 1) % 3][1], verts[(i + 1) % 3][2] + 0.01, randColor, true);
        }

        // bool debug = false;

        // drawLine(verts[2][0], verts[2][1], verts[2][2] + 1, verts[0][0], verts[0][1], verts[0][2] + 1, randColor, debug);
        // drawLine(verts[2][0], verts[2][1], verts[2][2] + 1, verts[1][0], verts[1][1], verts[1][2] + 1, randColor, debug);
        // drawLine(verts[1][0], verts[1][1], verts[1][2] + 1, verts[0][0], verts[0][1], verts[0][2] + 1, randColor, debug);

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

    std::vector<Vec> side1Pixels = Utils::linePixels(verts[2], verts[0]);
    std::vector<Vec> side1;
    side1.push_back(side1Pixels.front());
    for (int i = 0; i < side1Pixels.size(); i++)
    {
        if (side1Pixels[i][1] != side1.back()[1])
        {
            side1.push_back(side1Pixels[i]);
        }
        else
        {
            side1.back()[2] = std::min(side1.back()[2], side1Pixels[i][2]);
        }
    }

    // std::cout << "2" << std::endl;

    std::vector<Vec> side2Pixels = Utils::linePixels(verts[2], verts[1]);
    std::vector<Vec> side3Pixels = Utils::linePixels(verts[1], verts[0]);
    // std::cout << "from ";
    // std::cout << verts[2][0] << " " << verts[2][1] << " " << verts[2][2];
    // std::cout << " to ";
    // std::cout << verts[1][0] << " " << verts[1][1] << " " << verts[1][2];
    // std::cout << std::endl;
    // for (auto pi : side2Pixels)
    // {
    //     std::cout << pi[0] << " " << pi[1] << " " << pi[2] << std::endl;
    // }
    std::vector<Vec> side2;
    side2.push_back(side2Pixels.front());
    for (int i = 0; i < side2Pixels.size(); i++)
    {
        if (side2Pixels[i][1] != side2.back()[1])
        {
            side2.push_back(side2Pixels[i]);
        }
        else
        {
            side2.back()[2] = std::min(side2.back()[2], side2Pixels[i][2]);
        }
    }
    for (int i = 0; i < side3Pixels.size(); i++)
    {
        if (side3Pixels[i][1] != side2.back()[1])
        {
            side2.push_back(side3Pixels[i]);
        }
        else
        {
          if (i >= 1)
            side2.back()[2] = std::min(side2.back()[2], side3Pixels[i][2]);
        }
    }

    // std::cout << "3" << std::endl;

    // std::cout << side1.size() << " " << side2.size() << std::endl;

    for (int i = 0; i < side1.size(); i++)
    {
        // std::cout << i << std::endl;
        // std::cout << side1[i][0] << " " << side1[i][1] << " " << side1[i][2] << std::endl;
        // std::cout << side2[i][0] << " " << side2[i][1] << " " << side2[i][2] << std::endl;
        drawLine(side1[i][0], side1[i][1], side1[i][2], side2[i][0], side2[i][1], side2[i][2], color);
    }

    // std::cout << "4" << std::endl;

    // //std::cout << "side1" << std::endl;

    // auto side1 = Utils::getFillVals(verts[2], verts[0]);

    // std::cout << verts[2][0] << " " << verts[2][1] << " " << verts[2][2] << std::endl;

    // //std::cout << "side2" << std::endl;
    // auto side2 = Utils::getFillVals(verts[2], verts[1]);

    // //std::cout << "side3" << std::endl;
    // auto side3 = Utils::getFillVals(verts[1], verts[0]);

    // std::cout << "sizes: " << side1.size() << " " << side2.size() << " " << side3.size() << std::endl;

    // int remove = side2.size() + side3.size() > side1.size();
    // side2.insert(side2.end(), side3.begin() + remove, side3.end());

    // int topY = std::lround(verts[2][1]);
    // int bottomY = std::lround(verts[0][1]);

    // int counter = 0;

    // //std::cout << "about to draw" << std::endl;

    // for (int y = topY; y >= bottomY; y--, counter++)
    // {
    //     std::cout << "drawing at y = " << y << ", counter = " << counter << std::endl;
    //     std::cout << side1[counter].first << " " << side1[counter].second << std::endl;
    //     std::cout << side2[counter].first << " " << side2[counter].second << std::endl;
    //     std::cout << side2.size() << std::endl;
    //     drawLine(side1[counter].first, y, side1[counter].second, side2[counter].first, y, side2[counter].second, color);
    // }

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
