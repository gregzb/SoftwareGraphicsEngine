#include "PixelGrid.hpp"
#include "Utils.hpp"

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
    return pixelData[row * height + col];
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

template class PixelGrid<Color>;
template class PixelGrid<double>;