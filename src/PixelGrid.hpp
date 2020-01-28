#pragma once

#include <vector>

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
    int getHeight();
    int getWidth();
};