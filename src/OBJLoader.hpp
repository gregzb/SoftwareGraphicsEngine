#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "Vec4.hpp"
#include "Utils.hpp"
#include "PixelGrid.hpp"

#include "RenderObject.hpp"
#include "OBJStructs.hpp"

// class OBJMaterial
// {
// private:
//     Vec4 Ka;
//     Vec4 Kd;
//     Vec4 Ks;
//     Vec4 Ke;

//     double Ns;
//     double Ni;
//     double opaqueness; //d

//     int illum;

//     PixelGrid<Color> mapKd;

//     std::string materialName;

// public:
//     OBJMaterial();
//     void setVecConstants(Vec4 Ka_, Vec4 Kd_, Vec4 Ks_, Vec4 Ke_);
//     void setConstants(double Ns_, double Ni_, double opaqueness_);
//     void setIllumMode(int illum_);
//     void setKdTex(std::string texName);
//     void setName(std::string name);

//     inline Vec4 getKa() {
//         return Ka;
//     }

//     inline Vec4 getKd() {
//         return Kd;
//     }

//     inline Vec4 getKs() {
//         return Ks;
//     }

//     inline Vec4 getKe() {
//         return Ke;
//     }

//     inline double getNs() {
//         return Ns;
//     }

//     inline double getNi() {
//         return Ni;
//     }

//     inline double getOpaqueness() {
//         return opaqueness;
//     }

//     inline int getIllumMode() {
//         return illum;
//     }

//     inline PixelGrid<Color> getTexture() {
//         return mapKd;
//     }

//     inline std::string getName() {
//         return materialName;
//     }

// };

// class OBJObject
// {
// private:
//     std::vector<std::vector<int>> vertexIndices;
//     std::vector<std::vector<int>> vertexTexIndices;
//     std::vector<std::vector<int>> vertexNormalIndices;

//     OBJMaterial * mat;
//     bool smoothShading;

//     std::string objectName;
// public:
//     OBJObject();
//     void addVertexIndices(std::vector<int> indices);
//     void addVertexTexIndices(std::vector<int> indices);
//     void addVertexNormalIndices(std::vector<int> indices);
//     void setMat(OBJMaterial * mat);
//     void setShading(bool smooth);
//     void setName(std::string name);
//     std::vector<std::vector<int>> const & getVertexIndices();
//     std::vector<std::vector<int>> const & getVertexTexIndices();
//     std::vector<std::vector<int>> const & getVertexNormalIndices();
//     OBJMaterial * getMat();
//     bool getShading();
//     std::string const & getName();
// };

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
    RenderObject toRenderObject(std::string name) const;
};