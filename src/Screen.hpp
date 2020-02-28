#pragma once

#include <string>
#include <limits>
#include "Graphics.hpp"
#include "Utils.hpp"
#include "PixelGrid.hpp"

class Screen : public PixelGrid<Color>
{
private:
    void drawLine(Vec4 const &a, Vec4 const &b, Color color, bool flipped) const;

protected:
    PixelGrid<double> zbuffer;

public:
    const Graphics graphics;

    Screen(int width, int height);

    void toFileAscii(std::string fileName);
    void toFile(std::string fileName);
    void toFileExtension(std::string fileName);
    void display();

    void plot(Vec4 point, Color color);

    void drawLine(Vec4 const &a, Vec4 const &b, Color color) const;
    void drawEdges(Mat4 &matrix, Color color) const;
    void drawTriangle(std::vector<Vertex> vertices, RenderObject &ro) const;
    void fillTriangle(std::vector<Vertex> &verts, RenderObject &ro) const;
    void renderObject(Camera &cam, RenderObject object) const;
    void clear(Color color) const;

    PixelGrid<double> &zbuf();
    double &zbuf(int row, int col);
    void clearZbuf(double value = +std::numeric_limits<double>::infinity());
};