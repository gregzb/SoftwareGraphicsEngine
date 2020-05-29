#pragma once

#include <functional>
#include "VertexPos.hpp"
#include "Vec4.hpp"

class Vertex
{
    VertexPos position;
    Vec4 tex;
    Vec4 normal;
    Vec4 tangent;

public:
    Vertex(VertexPos const &pos_, Vec4 const &tex_, Vec4 const &norm_, Vec4 const& tan_);

    Vec4 const &getWorldPos() const;
    Vec4 const &getProjPos() const;
    Vec4 const &getPos() const;

    VertexPos & getVertexPos();
    Vec4 const & getTexCoords() const;
    Vec4 const & getNormal() const;
    Vec4 const & getTangent() const;

    Vertex lerp(Vertex const & other, double t) const;

    bool operator==(const Vertex &other) const;
};

namespace std
{

template <>
struct hash<Vertex>
{
    std::size_t operator()(Vertex &vert) const;
};
}