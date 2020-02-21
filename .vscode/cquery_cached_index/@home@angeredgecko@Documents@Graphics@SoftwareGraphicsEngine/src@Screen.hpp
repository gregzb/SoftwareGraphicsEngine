#pragma once

#include <string>
#include <limits>
#include "Utils.hpp"
#include "PixelGrid.hpp"
#include "Graphics.hpp"

class Screen : public PixelGrid<Color>
{
protected:
    PixelGrid<double> zbuffer;

public:
    const Graphics graphics;

    Screen(int width, int height);

    void toFile(std::string fileName);
    void toFileExtension(std::string fileName);
    void display();
    PixelGrid<double> &zbuf();
    double &zbuf(int row, int col);
    void clearZbuf(double value = -std::numeric_limits<double>::infinity());
};