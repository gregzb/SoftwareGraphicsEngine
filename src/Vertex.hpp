#pragma once

#include "Vec4.hpp"
#include "Mat4.hpp"

class Vertex
{
public:
    Vec4 pos;
    Vec4 texCoords;
    Vec4 normal;
    Vec4 color;

    Vertex(Vec4 pos = {}, Vec4 texCoords = {}, Vec4 normal = {}, Vec4 color = {255, 255, 255, 255});
    void transform(Mat4 transform);
    Vec4 getFaceNormal(Vertex &b, Vertex &c);

    bool operator==(const Vertex &other) const;
};

namespace std
{

template <>
struct hash<Vertex>
{
    std::size_t operator()(const Vertex &vert) const;
};

}