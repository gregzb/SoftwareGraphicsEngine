#pragma once

#include "Vec4.hpp"
#include "Mat4.hpp"

class VertexPos
{
private:
    Vec4 pos;
    Vec4 worldPos;
    Vec4 projPos;

public:
    VertexPos(Vec4 const & pos = {}, Vec4 const & worldPos = {}, Vec4 const & projPos = {});
    void transform(Mat4 const &transform);

    void setPos(Vec4 const &pos);

    void updateWorldPos(Vec4 const &worldPos);
    void updateProjPos(Vec4 const &projPos);

    Vec4 const &getWorldPos() const;
    Vec4 const &getProjPos() const;
    Vec4 const &getPos() const;

    VertexPos lerp(VertexPos const & other, double t) const;

    bool operator==(const VertexPos &other) const;
};

namespace std
{

template <>
struct hash<VertexPos>
{
    std::size_t operator()(const VertexPos &vert) const;
};

} // namespace std