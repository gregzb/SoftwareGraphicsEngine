#pragma once

#include <vector>
#include <string>
#include "PixelGrid.hpp"
#include "Vec4.hpp"

struct OBJMaterial {
    Vec4 Ka, Kd, Ks, Ke;
    double Ns, Ni, opaqueness;
    int illum;
    PixelGrid<Color> mapKd;
    std::string name;
};

struct OBJObject {
    std::vector<std::vector<int>> vertexIndices, vertexTexIndices, vertexNormalIndices;
    OBJMaterial * mat;
    bool smoothShading;
    std::string name;
};