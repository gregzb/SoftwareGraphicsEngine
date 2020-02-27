#pragma once

#include "Utils.hpp"
#include "Mat4.hpp"
#include "Camera.hpp"
#include "RenderObject.hpp"

class Screen;

class Graphics
{
private:
    Screen &screen;
    void drawLine(Vec4 a, Vec4 b, Color color, bool flipped) const;

public:
    Graphics(Screen &screen);
    void drawLine(Vec4 a, Vec4 b, Color color) const;
    void drawEdges(Mat4 &matrix, Color color) const;
    //void drawTriangles(Mat4 &matrix, Color color) const;
    void drawTriangle(std::vector<Vertex> vertices, RenderObject & ro) const;
    void fillTriangle(std::vector<Vertex> &verts, RenderObject & ro) const;
    void renderObject(Camera & cam, RenderObject object) const;
    void clear(Color color) const;
};