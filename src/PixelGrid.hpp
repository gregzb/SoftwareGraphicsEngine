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
    PixelGrid();
    PixelGrid(int width, int height);
    T &operator()(int row, int col);
    T &pixelAt(int row, int col);
    T &pixelAt(int idx);
    T const &read(int row, int col) const;
    int getHeight() const;
    int getWidth() const;

    void display() const;

    static PixelGrid<Color> const &loadTexture(std::string texName);
};