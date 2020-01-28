#pragma once

#include "Utils.hpp"
#include "Matrix.hpp"
#include "Vec.hpp"

class Screen;

class Graphics
{
private:
    Screen &screen;

public:
    Graphics(Screen &screen);
    void drawLine(double x0, double y0, double z0, double x1, double y1, double z1, Color color) const;
    void drawEdges(Matrix &matrix, Color color) const;
    void drawTriangles(Matrix &matrix, Color color) const;
    void fillTriangle(std::vector<Vec> &verts, Color color) const;
    void clear(Color color) const;
};