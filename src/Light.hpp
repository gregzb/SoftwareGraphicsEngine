#pragma once
#include "Vec4.hpp"

enum class LightType {
    Ambient,
    Directional,
    Point
};

struct Light {
    LightType type;
    Vec4 Ia, Id, Is;
    Vec4 pos;
};