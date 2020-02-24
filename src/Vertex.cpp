#include "Vertex.hpp"
#include <cassert>
#include <functional>

Vertex::Vertex(Vec4 pos, Vec4 texCoords, Vec4 normal, Vec4 color) : pos(pos), texCoords(texCoords), normal(normal), color(color)
{
}

void Vertex::transform(Mat4 transform)
{
    Vec4 temp = this->pos;
    assert(transform.getCols() == 4);

    for (int row = 0; row < transform.getRows(); row++)
    {
        this->pos[row] = 0;
        for (int i = 0; i < transform.getCols(); i++)
        {
            this->pos[row] += transform[row][i] * temp[i];
        }
    }
}

Vec4 Vertex::getFaceNormal(Vertex &b, Vertex &c)
{
    return (b.pos.sub(pos)).cross(c.pos.sub(b.pos));
}

bool Vertex::operator==(const Vertex &other) const
{
    return pos == other.pos && texCoords == other.texCoords && normal == other.normal;
}

std::size_t std::hash<Vertex>::operator()(const Vertex &vert) const
{
    size_t res = 17;
    res = res * 31 + std::hash<Vec4>()(vert.pos);
    res = res * 31 + std::hash<Vec4>()(vert.texCoords);
    res = res * 31 + std::hash<Vec4>()(vert.normal);
    return res;
}