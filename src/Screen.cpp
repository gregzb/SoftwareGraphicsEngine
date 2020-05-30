#include <fstream>
#include <cmath>
#include "Screen.hpp"
#include "PixelGrid.hpp"

Screen::Screen(int width, int height, bool supersample_) : PixelGrid(width * (supersample_+1), height * (supersample_+1)), supersample(supersample_), zbuffer(width * (supersample_+1), height * (supersample_+1))
{
    clearZbuf();
}

PixelGrid<Color>* Screen::updateTrueScreen() {
    if (supersample) {
        downsampled = PixelGrid<Color>(getWidth()/2, getHeight()/2);
        for (int row = 0; row < downsampled.getHeight(); row++) {
            for (int col = 0; col < downsampled.getWidth(); col++) {
                double equiRow = (row * 2.0 + 1) / getHeight();
                double equiCol = (col * 2.0 + 1) / getWidth();
                Color color = linRead({equiCol, equiRow}).toColor();
                //std::cout << equiCol << " " << equiRow << std::endl;
                //std::cout << row << " " << col << ": " << linRead({equiCol, equiRow}) << std::endl;
                downsampled(row, col) = color;
            }
        }

        return &downsampled;
    } else {
        return this;
    }
}

void Screen::plot(Vec4 const & pixel, Color color)
{
    if (zbuf(pixel.getY(), pixel.getX()) < pixel.getZ())
    {
        zbuf(pixel.getY(), pixel.getX()) = pixel.getZ();
        pixelAt(pixel.getY(), pixel.getX()) = color;
    }
}

void Screen::drawLine(Vec4 const & a, Vec4 const & b, Color color)
{
    drawLine(a, b, color, false);
}

void Screen::drawLine(Vec4 const & a, Vec4 const & b, Color color, bool flipped)
{
    if (b.getX() < a.getX())
    {
        return drawLine(b, a, color, flipped);
    }

    long x0r = std::lround(a.getX()), x1r = std::lround(b.getX()), y0r = std::lround(a.getY()), y1r = std::lround(b.getY());

    long dY = y1r - y0r, dX = x1r - x0r;

    if (std::abs(dY) > dX)
    {
        return drawLine({a.getY(), a.getX()}, {b.getY(), b.getX()}, color, !flipped);
    }

    int dir = Utils::sign(dY);

    dY = std::abs(dY) * 2;

    int d = dY - dX;

    dX *= 2;

    int y = y0r;
    for (int x = x0r; x <= x1r; x++)
    {
        if (!flipped)
            //screen.plot({x, y}, color);
            pixelAt(y, x) = color;
        else
            //screen.plot({y, x}, color);
            pixelAt(x, y) = color;

        if (d > 0)
        {
            y += dir;
            d -= dX;
        }
        d += dY;
    }
}

void Screen::drawEdges(Mat4 const &matrix, Color color)
{
    for (int col = 0; col < matrix.getCols(); col += 2)
    {
        drawLine(matrix.getPoint(col), matrix.getPoint(col + 1), color);
    }
}

void Screen::clear(Color color)
{
    for (int row = 0; row < getHeight(); row++)
    {
        for (int col = 0; col < getWidth(); col++)
        {
            pixelAt(row, col) = color;
        }
    }
}

PixelGrid<double> &Screen::zbuf()
{
    return zbuffer;
}

double &Screen::zbuf(int row, int col)
{
    return zbuffer(row, col);
}

void Screen::clearZbuf(double value)
{
    for (int row = 0; row < zbuf().getHeight(); row++)
    {
        for (int col = 0; col < zbuf().getWidth(); col++)
        {
            zbuf(row, col) = value;
        }
    }
}

void Screen::toFileAscii(std::string fileName) {
    auto displayScreen = updateTrueScreen();
    displayScreen->toFileAscii(fileName);
}
void Screen::toFile(std::string fileName) {
    auto displayScreen = updateTrueScreen();
    displayScreen->toFile(fileName);
}
void Screen::toFileExtension(std::string fileName) {
    auto displayScreen = updateTrueScreen();
    displayScreen->toFileExtension(fileName);
}
void Screen::display() {
    auto displayScreen = updateTrueScreen();
    displayScreen->display();
}