#pragma once

#include "Mat4.hpp"
#include "VertexPos.hpp"
#include "PixelGrid.hpp"
#include "OBJStructs.hpp"
#include "Material.hpp"
#include <vector>
#include <unordered_map>
#include <map>

class RenderObject
{
private:
    std::unordered_map<Vec4, int> vertex_mappings;

    bool smooth;

    std::vector<Vec4> vertexPositions;
    std::vector<Vec4> textureCoords;
    std::vector<Vec4> normals;
    std::vector<Material*> materials;
    std::vector<std::tuple<int, int, int>> indices;

    //std::optional<Material> mat;

    Vec4 position, rotation, scale;

public:
    RenderObject(bool smooth_ = true);
    Mat4 getModelMatrix() const;
    std::vector<Vec4> &getMesh();
    std::vector<Vec4> &getTexCoords();
    std::vector<Vec4> &getNormals();
    std::vector<std::tuple<int, int, int>> &getIndices();
    std::vector<Material*> getFaceMaterials();

    void setPosition(Vec4 const &position);
    void setRotation(Vec4 const &rotation);
    void setScale(Vec4 const &scale);
    Vec4 const &getPosition() const;
    Vec4 const &getRotation() const;
    Vec4 const &getScale() const;

    Material const * getMat(unsigned int i) const;

    void addVertexPos(Vec4 const & pos);
    void addTextureCoord(Vec4 const & pos);
    void addNormal(Vec4 const & norm);
    void addIndex(std::tuple<int, int, int> idx);
    void addMaterial(Material * mat);

    void updateVertexNormals(bool smooth);

    void addPoint(Vec4 const &v, bool weld = false);
    void addTriangle(Vec4 const &v0, Vec4 const &v1, Vec4 const &v2, bool weld = false, Material * mat = &Material::defaultMaterial);
    void addBox(Vec4 const &v, Vec4 const &dims);
    void addSphere(Vec4 const &v, double r, int thetaSteps, int phiSteps);
    void addTorus(Vec4 const &v, double r1, double r2, int thetaSteps, int phiSteps);
};