#pragma once

#include "Vec4.hpp"
#include "Mat4.hpp"

class Vertex
{
private:
    Vec4 pos;
    Vec4 texCoords;
    Vec4 normal;
    Vec4 color;

public:
    Vertex(Vec4 pos = {}, Vec4 texCoords = {}, Vec4 normal = {}, Vec4 color = {255, 255, 255, 255});
    void transform(Mat4 const &transform);
    Vec4 getFaceNormal(Vertex const &b, Vertex const &c) const;

    void setPos(Vec4 const & pos);
    void setTexCoords(Vec4 const & texCoords);
    void setNormal(Vec4 const & normal);
    void setColor(Vec4 const & color);

    Vec4 const &getPos() const;
    Vec4 const &getTexCoords() const;
    Vec4 const &getNormal() const;
    Vec4 const &getColor() const;

    bool operator==(const Vertex &other) const;
};

namespace std
{

template <>
struct hash<Vertex>
{
    std::size_t operator()(const Vertex &vert) const;
};

} // namespace std