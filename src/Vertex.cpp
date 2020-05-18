#include "Vertex.hpp"
#include <cassert>
#include <functional>

Vertex::Vertex(Vec4 pos, Vec4 texCoords, Vec4 normal, Vec4 worldPos, Vec4 projPos) : pos(pos), texCoords(texCoords), normal(normal), worldPos(worldPos), projPos(projPos)
{
}

void Vertex::transform(Mat4 const &transform)
{
    // Vec4 temp = this->pos;
    // assert(transform.getCols() == 4);

    // for (int row = 0; row < transform.getRows(); row++)
    // {
    //     this->pos.set(row, 0);
    //     for (int i = 0; i < transform.getCols(); i++)
    //     {
    //         this->pos.set(row, this->pos[row] + transform(row, i) * temp[i]);
    //     }
    // }
    this->pos = this->pos.transform(transform);
}

Vec4 Vertex::getFaceNormal(Vertex const &b, Vertex const &c) const
{
    //return b.pos.sub(pos)).cross(c.pos.sub(b.pos));
    return (b.worldPos - worldPos).cross(c.worldPos - b.worldPos);
}

Vec4 Vertex::getProjNormal(Vertex const &b, Vertex const &c) const {
    return (b.projPos - projPos).cross(c.projPos - b.projPos);
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

void Vertex::updateWorldPos(Vec4 const &worldPos) {
    this->worldPos = worldPos;
}
void Vertex::updateProjPos(Vec4 const &projPos) {
    this->projPos = projPos;
}

Vec4 const &Vertex::getWorldPos() const {
    return worldPos;
}
Vec4 const &Vertex::getProjPos() const {
    return projPos;
}

Vertex Vertex::lerp(Vertex const & other, double t) const {
    Vec4 const & pos_ = getPos().lerp(other.getPos(), t);
    Vec4 const & texCoords_ = getTexCoords().lerp(other.getTexCoords(), t);
    Vec4 const & normal_ = getNormal().lerp(other.getNormal(), t);
    Vec4 const & worldPos_ = getWorldPos().lerp(other.getWorldPos(), t);
    Vec4 const & projPos_ = getProjPos().lerp(other.getProjPos(), t);
    return {pos_, texCoords_, normal_, worldPos_, projPos_};
}

// NORMAL CANNOT BE HASHED!

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