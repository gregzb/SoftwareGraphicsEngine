#include "VertexPos.hpp"
#include <cassert>
#include <functional>

VertexPos::VertexPos(Vec4 const & pos, Vec4 const & worldPos, Vec4 const & projPos) : pos(pos), worldPos(worldPos), projPos(projPos)
{
}

void VertexPos::transform(Mat4 const &transform)
{
    this->pos = this->pos.transform(transform);
}

void VertexPos::setPos(Vec4 const &pos)
{
    this->pos = pos;
}

Vec4 const &VertexPos::getPos() const
{
    return pos;
}

void VertexPos::updateWorldPos(Vec4 const &worldPos) {
    this->worldPos = worldPos;
}
void VertexPos::updateProjPos(Vec4 const &projPos) {
    this->projPos = projPos;
}

Vec4 const &VertexPos::getWorldPos() const {
    return worldPos;
}
Vec4 const &VertexPos::getProjPos() const {
    return projPos;
}

VertexPos VertexPos::lerp(VertexPos const & other, double t) const {
    Vec4 const & pos_ = getPos().lerp(other.getPos(), t);
    Vec4 const & worldPos_ = getWorldPos().lerp(other.getWorldPos(), t);
    Vec4 const & projPos_ = getProjPos().lerp(other.getProjPos(), t);
    return {pos_, worldPos_, projPos_};
}

// NORMAL CANNOT BE HASHED!

bool VertexPos::operator==(const VertexPos &other) const
{
    return pos == other.pos;
}

std::size_t std::hash<VertexPos>::operator()(const VertexPos &vert) const
{
    size_t res = 17;
    res = res * 31 + std::hash<Vec4>()(vert.getPos());
    return res;
}