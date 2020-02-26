#include "RenderObject.hpp"
#include <cmath>

RenderObject::RenderObject() : vertex_mappings{}, scale(1, 1, 1){
}

Mat4 RenderObject::getModelMatrix() {
    Mat4 translateMat = Mat4::translate(position);
    Mat4 rotXMat = Mat4::rotX(rotation.x);
    Mat4 rotYMat = Mat4::rotY(rotation.y);
    Mat4 rotZMat = Mat4::rotZ(rotation.z);
    Mat4 scaleMat = Mat4::scale(scale);

    Mat4 model = translateMat.multiply(rotXMat).multiply(rotYMat).multiply(rotZMat).multiply(scaleMat);

    return model;
}

std::vector<Vertex> & RenderObject::getMesh() {
    return vertices;
}

std::vector<int> & RenderObject::getMeshIndices() {
    return indices;
}

void RenderObject::generateVertexNormals() {
    for (uint i = 0; i < indices.size(); i += 3) {
        Vec4 faceNormal = vertices[indices[i]].getFaceNormal(vertices[indices[i+1]], vertices[indices[i+2]]);
        for (int j = 0; j < 3; j++) {
            vertices[indices[i+j]].normal = vertices[indices[i+j]].normal.add(faceNormal);
        }
    }
    for (auto & vert : vertices) {
        vert.normal = vert.normal.normalize();
    }
}

void RenderObject::addPoint(Vec4 v)
{
    v = v.round(100000000.0);
    if (vertex_mappings.count(v) == 0) {
        vertex_mappings.insert({v, vertices.size()});
        vertices.push_back(v);
    }
    indices.push_back(vertex_mappings.at(v));
}
void RenderObject::addEdge(Vec4 v0, Vec4 v1)
{
    addPoint(v0);
    addPoint(v1);
}

void RenderObject::addTriangle(Vec4 v0, Vec4 v1, Vec4 v2)
{
    addPoint(v0);
    addPoint(v1);
    addPoint(v2);
}

void RenderObject::addBox(Vec4 v, Vec4 dims)
{
    double x, y, z, w, h, d;
    std::tie(x, y, z, w, h, d) = std::tie(v.x, v.y, v.z, dims.x, dims.y, dims.z);
    double x1 = x + w;
    double y1 = y - h;
    double z1 = z - d;

         addTriangle({x, y, z}, {x1, y, z}, {x, y, z1});
    //oui
    addTriangle({x1, y, z}, {x1, y, z1}, {x, y, z1});

    addTriangle({x, y, z}, {x, y1, z}, {x1, y1, z});
         addTriangle({x, y, z}, {x1, y1, z}, {x1, y, z});

    addTriangle({x, y, z}, {x, y1, z1}, {x, y1, z});
    addTriangle({x, y, z}, {x, y, z1}, {x, y1, z1});

    addTriangle({x, y1, z}, {x, y1, z1}, {x1, y1, z});
    addTriangle({x1, y1, z}, {x, y1, z1}, {x1, y1, z1});

    addTriangle({x1, y, z}, {x1, y1, z}, {x1, y1, z1});
    //oui
    addTriangle({x1, y, z}, {x1, y1, z1}, {x1, y, z1});

    addTriangle({x, y, z1}, {x1, y1, z1}, {x, y1, z1});
    addTriangle({x, y, z1}, {x1, y, z1}, {x1, y1, z1});

    // for (auto & vert : vertices) {
    //     vert.color = {rand() % 256, rand() % 256, rand() % 256, 255};
    // }
}

void RenderObject::addSphere(Vec4 v, double r, int thetaSteps, int phiSteps)
{
    double thetaStepSize = 2 * M_PI / thetaSteps;
    double phiStepSize = M_PI / phiSteps;

    std::vector<int> offsets{0, 0, 1, 0, 1, 1, 0, 1};

    for (int phiStep = 0; phiStep < phiSteps; phiStep++)
    {
        for (int thetaStep = 0; thetaStep < thetaSteps; thetaStep++)
        {
            std::vector<Vec4> points;
            for (int i = 0; i < 8; i += 2)
            {
                double phi = (phiStep + offsets[i]) * phiStepSize;
                double theta = (thetaStep + offsets[i + 1]) * thetaStepSize;
                points.push_back({r * std::cos(phi) + v.x,
                                   r * std::sin(phi) * std::cos(theta) + v.y,
                                   r * std::sin(phi) * std::sin(theta) + v.z});
            }

            addTriangle(points[0], points[1], points[2]);
            addTriangle(points[0], points[2], points[3]);
        }
    }
}

void RenderObject::addTorus(Vec4 v, double r1, double r2, int thetaSteps, int phiSteps)
{
    double thetaStepSize = 2 * M_PI / thetaSteps;
    double phiStepSize = 2 * M_PI / phiSteps;

    std::vector<int> offsets{0, 0, 1, 0, 1, 1, 0, 1};

    //for (int phiStep = phiSteps+1-2; phiStep < phiSteps+1+; phiStep++)
    // for (int phiStep = 0; phiStep < phiSteps; phiStep++)
    // {
    //     for (int thetaStep = 0; thetaStep < thetaSteps; thetaStep++)
    //     {
    for (int phiStep = 0; phiStep < phiSteps; phiStep++)
    {
        for (int thetaStep = 0; thetaStep < thetaSteps; thetaStep++)
        {
            std::vector<Vec4> points;
            for (int i = 0; i < 8; i += 2)
            {
                double phi = (phiStep % phiSteps + offsets[i]) * phiStepSize;
                double theta = (thetaStep + offsets[i + 1]) * thetaStepSize;
                points.push_back({(r2 + r1 * std::cos(phi)) * std::cos(theta) + v.x,
                                   r1 * std::sin(phi) + v.y,
                                   (r2 + r1 * std::cos(phi)) * std::sin(theta) + v.z});
            }

            addTriangle(points[0], points[1], points[2]);
            addTriangle(points[0], points[2], points[3]);
        }
    }
}