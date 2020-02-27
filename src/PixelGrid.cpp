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
    if (row >= height || row < 0 || col >= width || col < 0) {
        return pixelData[height * width];
    }
    return pixelData[row * width + col];
}

template <class T>
T &PixelGrid<T>::pixelAt(int idx)
{
    if (idx < 0 || idx >= height*width) {
        return pixelData[height * width];
    }
    return pixelData[idx];
}

template <class T>
PixelGrid<T>::PixelGrid(int width, int height) : width(width), height(height)
{
    pixelData.resize(width * height + 1);
}

template <class T>
int PixelGrid<T>::getHeight()
{
    return height;
}

template <class T>
int PixelGrid<T>::getWidth()
{
    return width;
}

template<>
PixelGrid<Color> PixelGrid<Color>::loadTexture(std::string texName) {
    //static_assert(std::is_same<T, Color>::value);
    std::ifstream texFile(texName, std::ios::in | std::ios::binary);

    std::string fileHeader;
    texFile >> fileHeader >> fileHeader >> fileHeader >> fileHeader;
    int width, height, maxColor;
    texFile >> width >> height >> maxColor;

    PixelGrid<Color> tempScreen(width, height);

    int counter = 0;
    int idx = 0;
    u_char temp;
    while(texFile >> temp) {
        if (counter == 0) {
            tempScreen.pixelAt(idx).r = temp;
        } else if (counter == 1) {
            tempScreen.pixelAt(idx).g = temp;
        } else if (counter == 2) {
            tempScreen.pixelAt(idx).b = temp;
        }
        counter++;
        counter %= 3;
        if (counter == 0) {
            idx++;
            if (idx < 100)
            std::cout << idx-1 << ": " << (int) tempScreen.pixelAt(idx-1).r << " " << (int) tempScreen.pixelAt(idx-1).g << " " << (int) tempScreen.pixelAt(idx-1).b << std::endl;
        }
    }

    //now read byte by byte, figure out why this changes background

    return tempScreen;
}

template class PixelGrid<Color>;
template class PixelGrid<double>;