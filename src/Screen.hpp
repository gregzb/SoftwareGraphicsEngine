#pragma once

#include <string>
#include <limits>
#include "Graphics.hpp"
#include "Utils.hpp"
#include "PixelGrid.hpp"

class Screen : public PixelGrid<Color>
{
protected:
    PixelGrid<double> zbuffer;

public:
    const Graphics graphics;

    Screen(int width, int height);

    void toFileAscii(std::string fileName);
    void toFile(std::string fileName);
    void toFileExtension(std::string fileName);
    void display();

    void plot(Vec4 point, Color color);

    PixelGrid<double> &zbuf();
    double &zbuf(int row, int col);
    void clearZbuf(double value = +std::numeric_limits<double>::infinity());
};