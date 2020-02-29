#include <fstream>
#include <cmath>
#include "Screen.hpp"
#include "PixelGrid.hpp"

Screen::Screen(int width, int height) : PixelGrid(width, height), zbuffer(width, height)
{
    clearZbuf();
}

void Screen::toFileAscii(std::string fileName)
{
    std::ofstream myfile;
    myfile.open(fileName);
    myfile << "P3\n"
           << width << " " << height << "\n255\n";

    for (int row = getHeight() - 1; row >= 0; row--)
    {
        for (int col = 0; col < getWidth(); col++)
        {
            Color color = pixelAt(row, col);
            myfile << " " << +color.r << " " << +color.g << " " << +color.b << "  ";
        }
        myfile << "\n";
    }
    myfile.close();
}

void Screen::toFile(std::string fileName)
{
    std::ofstream myfile;
    myfile.open(fileName);
    myfile << "P6\n"
           << width << " " << height << "\n255\n";

    for (int row = getHeight() - 1; row >= 0; row--)
    {
        for (int col = 0; col < getWidth(); col++)
        {
            Color color = pixelAt(row, col);
            myfile << color.r << color.g << color.b;
        }
    }
    myfile.close();
}

void Screen::toFileExtension(std::string fileName)
{
    std::string command = "convert - ";
    command.append(fileName);

    FILE *f = popen(command.c_str(), "w");
    fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
    for (int y = height - 1; y >= 0; y--)
    //for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Color color = pixelAt(y, x);
            fprintf(f, "%d %d %d ", color.r, color.g, color.b);
        }
        fprintf(f, "\n");
    }
    pclose(f);
}

void Screen::display()
{
    FILE *f;

    f = popen("display", "w");

    fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
    for (int y = height - 1; y >= 0; y--)
    {
        for (int x = 0; x < width; x++)
        {
            Color color = pixelAt(y, x);
            fprintf(f, "%d %d %d ", color.r, color.g, color.b);
        }
        fprintf(f, "\n");
    }
    pclose(f);
}

void Screen::plot(Vec4 const & pixel, Color color)
{
    if (zbuf(pixel.getY(), pixel.getX() < pixel.getZ()))
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