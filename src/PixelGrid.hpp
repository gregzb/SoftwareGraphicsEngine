#pragma once

#include <vector>
#include <string>

#include "Utils.hpp"

template <class T>
class PixelGrid
{
protected:
    int width;
    int height;
    std::vector<T> pixelData;

public:
    PixelGrid(int width, int height);
    T &operator()(int row, int col);
    T &pixelAt(int row, int col);
    T &pixelAt(int idx);
    int getHeight();
    int getWidth();

    static PixelGrid<Color> loadTexture(std::string texName);
};