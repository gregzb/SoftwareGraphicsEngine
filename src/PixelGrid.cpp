#include "PixelGrid.hpp"
#include "Utils.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <type_traits>

template <class T>
T &PixelGrid<T>::operator()(int row, int col)
{
    return pixelAt(row, col);
}

template <class T>
T &PixelGrid<T>::pixelAt(int row, int col)
{
    if (row >= height || row < 0 || col >= width || col < 0)
    {
        //std::cout << "out: " << row << " " << col << std::endl;
        return pixelData[height * width];
    }
    return pixelData[row * width + col];
}

template <class T>
T &PixelGrid<T>::pixelAt(int idx)
{
    if (idx < 0 || idx >= height * width)
    {
        //std::cout << "her1 " << idx << std::endl;
        return pixelData[height * width];
    }
    return pixelData[idx];
}

template <class T>
T const &PixelGrid<T>::read(int row, int col) const {
    if (row >= height || row < 0 || col >= width || col < 0)
    {
        //std::cout << "out: " << row << " " << col << std::endl;
        return pixelData[height * width];
    }
    return pixelData[row * width + col];
}

template <class T>
PixelGrid<T>::PixelGrid(int width, int height) : width(width), height(height)
{
    pixelData.resize(width * height + 1);
}

template <class T>
PixelGrid<T>::PixelGrid() : width(0), height(0)
{
    pixelData.resize(1);
}

template <class T>
int PixelGrid<T>::getHeight() const
{
    return height;
}

template <class T>
int PixelGrid<T>::getWidth() const
{
    return width;
}

template <>
void PixelGrid<Color>::display() const
{
    FILE *f;

    f = popen("display", "w");

    fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
    //for (int y = height - 1; y >= 0; y--)
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Color color = read(y, x);
            fprintf(f, "%d %d %d ", color.r, color.g, color.b);
        }
        fprintf(f, "\n");
    }
    // for (int x = 0; x < width; x++)
    // {
    //     for (int y = height - 1; y >= 0; y--)
    //     {
    //         Color color = pixelAt(y, x);
    //         fprintf(f, "%d %d %d ", color.r, color.g, color.b);
    //     }
    //     fprintf(f, "\n");
    // }
    pclose(f);
}

template <>
PixelGrid<Color> const &PixelGrid<Color>::loadTexture(std::string texName)
{
    //static_assert(std::is_same<T, Color>::value);
    std::ifstream texFile(texName, std::ios::in | std::ios::binary);

    //std::cout << texName << std::endl;

    std::string fileHeader;
    //texFile >> fileHeader >> fileHeader >> fileHeader >> fileHeader;
    texFile >> fileHeader;
    int width, height, maxColor;
    texFile >> width >> height >> maxColor;

    PixelGrid<Color> tempScreen(width, height);

    int counter = 0;
    int idx = 0;
    char temp;
    texFile.read(&temp, 1);
    //std::cout << temp << std::endl;
    while (texFile.read(&temp, 1))
    {
        //std::cout << (int)(u_char) temp << std::endl;
        if (counter == 0)
        {
            tempScreen.pixelAt(idx).r = temp;
        }
        else if (counter == 1)
        {
            tempScreen.pixelAt(idx).g = temp;
        }
        else if (counter == 2)
        {
            tempScreen.pixelAt(idx).b = temp;
        }
        counter++;
        counter %= 3;
        if (counter == 0)
        {
            idx++;
            //tempScreen.pixelAt(idx-1).b = temp
            // if (idx < 1282)
            // std::cout << idx-1 << ": " << (int) tempScreen.pixelAt(idx-1).r << " " << (int) tempScreen.pixelAt(idx-1).g << " " << (int) tempScreen.pixelAt(idx-1).b << std::endl;
        }
    }

    //now read byte by byte, figure out why this changes background

    return tempScreen;
}

template class PixelGrid<Color>;
template class PixelGrid<double>;