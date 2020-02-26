#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "Vec4.hpp"
#include "Utils.hpp"
#include "PixelGrid.hpp"

#include "RenderObject.hpp"

class OBJMaterial
{
private:
    Vec4 Ka;
    Vec4 Kd;
    Vec4 Ks;
    Vec4 Ke;
    Vec4 Ns;
    Vec4 Ni;

    double opaqueness; //d

    int illum;

    PixelGrid<Color> mapKd;

    std::string materialName;

public:
    OBJMaterial();
};

class OBJObject
{
private:
    std::vector<std::vector<int>> vertexIndices;
    std::vector<std::vector<int>> vertexTexIndices;
    std::vector<std::vector<int>> vertexNormalIndices;

    OBJMaterial * mat;
    bool smoothShading;

    std::string objectName;
public:
    OBJObject();
    void addVertexIndices(std::vector<int> indices);
    void addVertexTexIndices(std::vector<int> indices);
    void addVertexNormalIndices(std::vector<int> indices);
    void setMat(OBJMaterial * mat);
    void setShading(bool smooth);
    void setName(std::string name);
    std::vector<std::vector<int>> const & getVertexIndices();
    std::vector<std::vector<int>> const & getVertexTexIndices();
    std::vector<std::vector<int>> const & getVertexNormalIndices();
    OBJMaterial const * getMat();
    bool getShading();
    std::string const & getName();
};

class OBJLoader
{
private:
    std::unordered_map<std::string, OBJObject> objects;
    std::unordered_map<std::string, OBJMaterial> materials;

    std::string mtlFileName;

    OBJObject * currentObject;
    OBJMaterial * currentMaterial;

    std::vector<Vec4> v;
    std::vector<Vec4> vt;
    std::vector<Vec4> vn;

    void parseMTL();

public:
    OBJLoader(std::string const &fileName);
    RenderObject toRenderObject(std::string name);
};