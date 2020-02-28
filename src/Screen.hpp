#pragma once

#include <string>
#include <limits>
#include "Utils.hpp"
#include "PixelGrid.hpp"
#include "Vec4.hpp"
#include "Mat4.hpp"

class Screen : public PixelGrid<Color>
{
private:
    void drawLine(Vec4 const &a, Vec4 const &b, Color color, bool flipped);

protected:
    PixelGrid<double> zbuffer;

public:
    Screen(int width, int height);

    void toFileAscii(std::string fileName);
    void toFile(std::string fileName);
    void toFileExtension(std::string fileName);
    void display();

    void plot(Vec4 const &point, Color color);

    void drawLine(Vec4 const &a, Vec4 const &b, Color color);
    void drawEdges(Mat4 const &matrix, Color color);
    void clear(Color color);

    PixelGrid<double> &zbuf();
    double &zbuf(int row, int col);
    void clearZbuf(double value = +std::numeric_limits<double>::infinity());
};