#include <fstream>
#include "Screen.hpp"
#include "PixelGrid.hpp"

Screen::Screen(int width, int height) : PixelGrid(width, height), zbuffer(width, height), graphics(*this)
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

void Screen::plot(Vec4 pixel, Color color)
{
    if (zbuf(pixel.getY(), pixel.getX() < pixel.getZ()))
    {
        zbuf(pixel.getY(), pixel.getX()) = pixel.getZ();
        pixelAt(pixel.getY(), pixel.getX()) = color;
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