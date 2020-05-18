#pragma once

#include <vector>
#include <string>

#include "Utils.hpp"
#include "Vec4.hpp"

template <class T>
class PixelGrid
{
// private:
//     void drawLine(Vec4 const &a, Vec4 const &b, T color, bool flipped);

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

    static PixelGrid<Color> loadTexture(std::string texName);

    //void drawLine(Vec4 const &a, Vec4 const &b, T color);
};