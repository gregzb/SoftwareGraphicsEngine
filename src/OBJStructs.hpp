#pragma once

#include <vector>
#include <string>
#include "Material.hpp"
#include "PixelGrid.hpp"
#include "Vec4.hpp"

struct OBJObject {
    std::vector<std::vector<int>> vertexIndices, vertexTexIndices, vertexNormalIndices;
    std::vector<Material*> triMats;
    
    //std::map<std::string, 
    bool smoothShading;
    std::string name;
};