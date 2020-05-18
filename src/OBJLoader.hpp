#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "Vec4.hpp"
#include "Utils.hpp"
#include "PixelGrid.hpp"

#include "RenderObject.hpp"
#include "OBJStructs.hpp"
#include "Material.hpp"

class OBJLoader
{
private:
    std::unordered_map<std::string, OBJObject> objects;

    OBJObject * currentObject;
    Material * currentMaterial;

    std::vector<Vec4> v;
    std::vector<Vec4> vt;
    std::vector<Vec4> vn;

    void parseMTL(std::string base, std::string mtlFileName);

public:
    OBJLoader(std::string const &fileName);
    RenderObject toRenderObject(std::string name) const;
};