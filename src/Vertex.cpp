#include "Vertex.hpp"
#include <cassert>
#include <functional>

Vertex::Vertex(Vec4 pos, Vec4 texCoords, Vec4 normal, Vec4 color) : pos(pos), texCoords(texCoords), normal(normal), color(color)
{
}

void Vertex::transform(Mat4 const &transform)
{
    Vec4 temp = this->pos;
    assert(transform.getCols() == 4);

    for (int row = 0; row < transform.getRows(); row++)
    {
        this->pos.set(row, 0);
        for (int i = 0; i < transform.getCols(); i++)
        {
            this->pos.set(row, this->pos[row] + transform(row, i) * temp[i]);
        }
    }
}

Vec4 Vertex::getFaceNormal(Vertex const &b, Vertex const &c) const
{
    //return b.pos.sub(pos)).cross(c.pos.sub(b.pos));
    return (b.pos - pos).cross(c.pos - b.pos);
}

void Vertex::setPos(Vec4 const &pos)
{
    this->pos = pos;
}

void Vertex::setTexCoords(Vec4 const &texCoords)
{
    this->texCoords = texCoords;
}
void Vertex::setNormal(Vec4 const &normal)
{
    this->normal = normal;
}
void Vertex::setColor(Vec4 const &color)
{
    this->color = color;
}

Vec4 const &Vertex::getPos() const
{
    return pos;
}
Vec4 const &Vertex::getTexCoords() const
{
    return texCoords;
}
Vec4 const &Vertex::getNormal() const
{
    return normal;
}
Vec4 const &Vertex::getColor() const
{
    return color;
}

bool Vertex::operator==(const Vertex &other) const
{
    return pos == other.pos && texCoords == other.texCoords && normal == other.normal;
}

std::size_t std::hash<Vertex>::operator()(const Vertex &vert) const
{
    size_t res = 17;
    res = res * 31 + std::hash<Vec4>()(vert.getPos());
    res = res * 31 + std::hash<Vec4>()(vert.getTexCoords());
    res = res * 31 + std::hash<Vec4>()(vert.getNormal());
    return res;
}