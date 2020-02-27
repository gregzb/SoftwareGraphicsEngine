#include "RenderObject.hpp"
#include <cmath>

RenderObject::RenderObject() : vertex_mappings{}, scale(1, 1, 1)
{
}

Mat4 RenderObject::getModelMatrix() const
{
    Mat4 const &translateMat = Mat4::translate(position);
    Mat4 const &rotXMat = Mat4::rotX(rotation.getX());
    Mat4 const &rotYMat = Mat4::rotY(rotation.getY());
    Mat4 const &rotZMat = Mat4::rotZ(rotation.getZ());
    Mat4 const &scaleMat = Mat4::scale(scale);

    Mat4 const &model = translateMat.multiply(rotXMat).multiply(rotYMat).multiply(rotZMat).multiply(scaleMat);

    return model;
}

std::vector<Vertex> &RenderObject::getMesh()
{
    return vertices;
}

std::vector<int> &RenderObject::getMeshIndices()
{
    return indices;
}

void RenderObject::setPosition(Vec4 const &position)
{
    this->position = position;
}
void RenderObject::setRotation(Vec4 const &rotation)
{
    this->rotation = rotation;
}
void RenderObject::setScale(Vec4 const &scale)
{
    this->scale = scale;
}
Vec4 const &RenderObject::getPosition() const
{
    return position;
}
Vec4 const &RenderObject::getRotation() const
{
    return rotation;
}
Vec4 const &RenderObject::getScale() const
{
    return scale;
}

void RenderObject::setMaterial(OBJMaterial *mat)
{
    this->mat = mat;
}
OBJMaterial const *RenderObject::getMaterial()
{
    return mat;
}

void RenderObject::generateVertexNormals()
{
    for (uint i = 0; i < indices.size(); i += 3)
    {
        Vec4 faceNormal = vertices[indices[i]].getFaceNormal(vertices[indices[i + 1]], vertices[indices[i + 2]]);
        for (int j = 0; j < 3; j++)
        {
            vertices[indices[i + j]].setNormal(vertices[indices[i + j]].getNormal() + faceNormal);
        }
    }
    for (auto &vert : vertices)
    {
        vert.setNormal(vert.getNormal().normalize());
    }
}

void RenderObject::addVertex(Vertex const &vert)
{
    //vert.pos = vert.pos.round(100000000.0);
    if (vertex_mappings.count(vert) == 0)
    {
        vertex_mappings.insert({vert, vertices.size()});
        vertices.push_back(vert);
    }
    indices.push_back(vertex_mappings.at(vert));
}

void RenderObject::addPoint(Vec4 const &v)
{
    Vec4 v0 = v.round(100000000.0);
    if (vertex_mappings.count(v0) == 0)
    {
        vertex_mappings.insert({v0, vertices.size()});
        vertices.push_back(v0);
    }
    indices.push_back(vertex_mappings.at(v0));
}
void RenderObject::addEdge(Vec4 const &v0, Vec4 const &v1)
{
    addPoint(v0);
    addPoint(v1);
}

void RenderObject::addTriangle(Vec4 const &v0, Vec4 const &v1, Vec4 const &v2)
{
    //std::cout << v0 << v1 << v2 << std::endl;
    addPoint(v0);
    addPoint(v1);
    addPoint(v2);
}

void RenderObject::addBox(Vec4 const &v, Vec4 const &dims)
{
    double x, y, z, w, h, d;
    x = v.getX();
    y = v.getY();
    z = v.getZ();
    w = dims.getX();
    h = dims.getY();
    d = dims.getZ();
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

void RenderObject::addSphere(Vec4 const &v, double r, int thetaSteps, int phiSteps)
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
                points.push_back({r * std::cos(phi) + v.getX(),
                                  r * std::sin(phi) * std::cos(theta) + v.getY(),
                                  r * std::sin(phi) * std::sin(theta) + v.getZ()});
            }

            addTriangle(points[0], points[1], points[2]);
            addTriangle(points[0], points[2], points[3]);
        }
    }
}

void RenderObject::addTorus(Vec4 const &v, double r1, double r2, int thetaSteps, int phiSteps)
{
    double thetaStepSize = 2 * M_PI / thetaSteps;
    double phiStepSize = 2 * M_PI / phiSteps;

    std::vector<int> offsets{0, 0, 1, 0, 1, 1, 0, 1};

    for (int phiStep = 0; phiStep < phiSteps; phiStep++)
    {
        for (int thetaStep = 0; thetaStep < thetaSteps; thetaStep++)
        {
            std::vector<Vec4> points;
            for (int i = 0; i < 8; i += 2)
            {
                double phi = (phiStep % phiSteps + offsets[i]) * phiStepSize;
                double theta = (thetaStep + offsets[i + 1]) * thetaStepSize;
                points.push_back({(r2 + r1 * std::cos(phi)) * std::cos(theta) + v.getX(),
                                  r1 * std::sin(phi) + v.getY(),
                                  (r2 + r1 * std::cos(phi)) * std::sin(theta) + v.getZ()});
            }

            addTriangle(points[0], points[1], points[2]);
            addTriangle(points[0], points[2], points[3]);
        }
    }
}