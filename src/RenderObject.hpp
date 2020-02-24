#pragma once

#include "Mat4.hpp"
#include "Vertex.hpp"
#include <vector>
#include <unordered_map>

class RenderObject
{
private:
    //Mat4 mesh; //object space verts
    std::vector<Vertex> vertices;
    std::vector<int> indices;

    std::unordered_map<Vertex, int> vertex_mappings;

public:
    Vec4 position;
    Vec4 rotation;
    Vec4 scale;

    RenderObject();
    Mat4 getModelMatrix();
    std::vector<Vertex> &getMesh();
    std::vector<int> &getMeshIndices();

    void generateVertexNormals();

    void addPoint(Vec4 v);
    void addEdge(Vec4 v0, Vec4 v1);
    void addTriangle(Vec4 v0, Vec4 v1, Vec4 v2);
    void addBox(Vec4 v, Vec4 dims);
    void addSphere(Vec4 v, double r, int thetaSteps, int phiSteps);
    void addTorus(Vec4 v, double r1, double r2, int thetaSteps, int phiSteps);
};