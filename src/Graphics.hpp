#pragma once

#include "Utils.hpp"
#include "Vec3.hpp"
#include "Mat4.hpp"

class Screen;

class Graphics
{
private:
    Screen &screen;

public:
    Graphics(Screen &screen);
    void drawLine(Vec3 a, Vec3 b, Color color, bool debug = false) const;
    void drawEdges(Mat4 &matrix, Color color) const;
    void drawTriangles(Mat4 &matrix, Color color) const;
    void fillTriangle(std::vector<Vec3> &verts, Color color) const;
    void clear(Color color) const;
};