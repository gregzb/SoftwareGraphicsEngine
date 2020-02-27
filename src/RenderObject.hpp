#pragma once

#include "Mat4.hpp"
#include "Vertex.hpp"
#include "PixelGrid.hpp"
#include "OBJStructs.hpp"
#include <vector>
#include <unordered_map>

class RenderObject
{
private:
    //Mat4 mesh; //object space verts
    std::vector<Vertex> vertices;
    std::vector<int> indices;

    std::unordered_map<Vertex, int> vertex_mappings;

    OBJMaterial *mat;

    Vec4 position, rotation, scale;

public:
    RenderObject();
    Mat4 getModelMatrix() const;
    std::vector<Vertex> &getMesh();
    std::vector<int> &getMeshIndices();

    void setPosition(Vec4 const &position);
    void setRotation(Vec4 const &rotation);
    void setScale(Vec4 const &scale);
    Vec4 const &getPosition() const;
    Vec4 const &getRotation() const;
    Vec4 const &getScale() const;

    void setMaterial(OBJMaterial *mat);
    OBJMaterial const *getMaterial();

    void generateVertexNormals();

    void addVertex(Vertex const &vert);

    void addPoint(Vec4 const &v);
    void addEdge(Vec4 const &v0, Vec4 const &v1);
    void addTriangle(Vec4 const &v0, Vec4 const &v1, Vec4 const &v2);
    void addBox(Vec4 const &v, Vec4 const &dims);
    void addSphere(Vec4 const &v, double r, int thetaSteps, int phiSteps);
    void addTorus(Vec4 const &v, double r1, double r2, int thetaSteps, int phiSteps);
};