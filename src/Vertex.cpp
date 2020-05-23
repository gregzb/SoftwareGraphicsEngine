#include "Vertex.hpp"
#include "VertexPos.hpp"

Vertex::Vertex(VertexPos const &pos_, Vec4 const &tex_, Vec4 const &norm_) : position(pos_), tex(tex_), normal(norm_)
{
}

Vec4 const &Vertex::getWorldPos() const
{
    return position.getWorldPos();
}

Vec4 const &Vertex::getProjPos() const
{
    return position.getProjPos();
}

Vec4 const &Vertex::getPos() const
{
    return position.getPos();
}

VertexPos &Vertex::getVertexPos()
{
    return position;
}

Vec4 const &Vertex::getTexCoords() const
{
    return tex;
}

Vec4 const &Vertex::getNormal() const
{
    return normal;
}

Vertex Vertex::lerp(Vertex const &other, double t) const
{
    return {position.lerp(other.position, t),
            tex.lerp(other.tex, t),
            normal.lerp(other.normal, t)};
}

bool Vertex::operator==(const Vertex &other) const
{
    return position == other.position && tex == other.tex && normal == other.normal;
}

std::size_t std::hash<Vertex>::operator()(Vertex &vert) const
{
    size_t res = 17;
    res = res * 31 + std::hash<VertexPos>()(vert.getVertexPos());
    res = res * 31 + std::hash<Vec4>()(vert.getTexCoords());
    res = res * 31 + std::hash<Vec4>()(vert.getNormal());
    return res;
}