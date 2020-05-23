#include "RenderObject.hpp"
#include <cmath>

RenderObject::RenderObject(bool smooth_) : smooth(smooth_), scale(1, 1, 1)
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

std::vector<Vec4> &RenderObject::getMesh()
{
    return vertexPositions;
}

std::vector<Vec4> &RenderObject::getTexCoords()
{
    return textureCoords;
}

std::vector<Vec4> &RenderObject::getNormals()
{
    return normals;
}

std::vector<std::tuple<int, int, int>> &RenderObject::getIndices()
{
    return indices;
}

std::vector<Material *> RenderObject::getFaceMaterials()
{
    return materials;
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

Material const *RenderObject::getMat(unsigned int i) const
{
    return materials[i];
}

// void RenderObject::setMaterial(Material *mat_)
// {
//     mat = *mat_;
// }
// bool RenderObject::hasMaterial() const {
//     return mat.has_value();
// }
// Material const *RenderObject::getMaterial() const
// {
//     return &mat.value();
// }

void RenderObject::addVertexPos(Vec4 const &pos)
{
    vertexPositions.push_back(pos);
}

void RenderObject::addTextureCoord(Vec4 const &pos)
{
    textureCoords.push_back(pos);
}

void RenderObject::addNormal(Vec4 const &norm)
{
    normals.push_back(norm);
}

void RenderObject::addIndex(std::tuple<int, int, int> idx)
{
    indices.push_back(idx);
}

void RenderObject::addMaterial(Material *mat)
{
    materials.push_back(mat);
}

void RenderObject::updateVertexNormals(bool smooth)
{
    normals.clear();
    if (!smooth)
    {
        for (unsigned int i = 0; i < indices.size(); i += 3)
        {
            Vec4 const &p0 = vertexPositions[std::get<0>(indices[i])];
            Vec4 const &p1 = vertexPositions[std::get<0>(indices[i + 1])];
            Vec4 const &p2 = vertexPositions[std::get<0>(indices[i + 2])];
            Vec4 const &norm = p0.getNormal(p1, p2).normalize();
            std::get<2>(indices[i]) = normals.size();
            std::get<2>(indices[i + 1]) = normals.size();
            std::get<2>(indices[i + 2]) = normals.size();
            addNormal(norm);
        }
    }
    else
    {
        //std::cout << "smooth" << std::endl;
        //std::vector<Vec4> tempNormals(indices.size());
        std::unordered_map<int, Vec4> tempNormals;
        for (unsigned int i = 0; i < indices.size(); i += 3)
        {
            Vec4 const &p0 = vertexPositions[std::get<0>(indices[i])];
            Vec4 const &p1 = vertexPositions[std::get<0>(indices[i + 1])];
            Vec4 const &p2 = vertexPositions[std::get<0>(indices[i + 2])];
            Vec4 const &norm = p0.getNormal(p1, p2);
            tempNormals[std::get<0>(indices[i])] = tempNormals[std::get<0>(indices[i])] + norm;
            tempNormals[std::get<0>(indices[i + 1])] = tempNormals[std::get<0>(indices[i + 1])] + norm;
            tempNormals[std::get<0>(indices[i + 2])] = tempNormals[std::get<0>(indices[i + 2])] + norm;
        }
        for (unsigned int i = 0; i < indices.size(); i++)
        {
            Vec4 const &calculated = tempNormals[std::get<0>(indices[i])].normalize();
            //tempNormals[i] = tempNormals[i].normalize();
            std::get<2>(indices[i]) = normals.size();
            addNormal(calculated);
        }
    }
}

// void RenderObject::generateVertexNormals()
// {
//     for (auto &vert : vertices)
//     {
//         vert.setNormal({});
//     }
//     for (uint i = 0; i < indices.size(); i += 3)
//     {
//         Vec4 faceNormal = vertices[indices[i]].getFaceNormal(vertices[indices[i + 1]], vertices[indices[i + 2]]);
//         for (int j = 0; j < 3; j++)
//         {
//             vertices[indices[i + j]].setNormal(vertices[indices[i + j]].getNormal() + faceNormal);
//         }
//     }
//     for (auto &vert : vertices)
//     {
//         //std::cout << vert.getNormal() << std::endl;
//         vert.setNormal(vert.getNormal().normalize());
//         //std::cout << vert.getNormal() << std::endl;
//     }
//     // int i = 0;
//     //std::cout << vertices[0].getWorldPos() << vertices[1].getWorldPos() << vertices[2].getWorldPos() << std::endl;
//     //std::cout << vertices[indices[i]].getFaceNormal(vertices[indices[i + 1]], vertices[indices[i + 2]]) << std::endl;
// }

void RenderObject::addPoint(Vec4 const &v, bool weld)
{
    if (!weld)
    {
        indices.push_back({vertexPositions.size(), -1, -1});
        vertexPositions.push_back(v);
    }
    else
    {
        Vec4 const &v0 = v.round(100000000.0);
        //std::cout << v0 << std::endl;
        if (vertex_mappings.count(v0) == 0)
        {
            indices.push_back({vertexPositions.size(), -1, -1});
            vertex_mappings.insert({v0, vertexPositions.size()});
            vertexPositions.push_back(v0);
        }
        else
        {
            //std::cout << "welded" << std::endl;
            indices.push_back({vertex_mappings.at(v0), -1, -1});
        }
    }
}

void RenderObject::addTriangle(Vec4 const &v0, Vec4 const &v1, Vec4 const &v2, bool weld, Material *mat)
{
    //std::cout << v0 << v1 << v2 << std::endl;
    addPoint(v0, weld);
    addPoint(v1, weld);
    addPoint(v2, weld);
    addMaterial(mat);
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

    addTriangle({x, y, z}, {x1, y, z}, {x, y, z1}, true);
    //oui
    addTriangle({x1, y, z}, {x1, y, z1}, {x, y, z1}, true);

    addTriangle({x, y, z}, {x, y1, z}, {x1, y1, z}, true);
    addTriangle({x, y, z}, {x1, y1, z}, {x1, y, z}, true);

    addTriangle({x, y, z}, {x, y1, z1}, {x, y1, z}, true);
    addTriangle({x, y, z}, {x, y, z1}, {x, y1, z1}, true);

    addTriangle({x, y1, z}, {x, y1, z1}, {x1, y1, z}, true);
    addTriangle({x1, y1, z}, {x, y1, z1}, {x1, y1, z1}, true);

    addTriangle({x1, y, z}, {x1, y1, z}, {x1, y1, z1}, true);
    //oui
    addTriangle({x1, y, z}, {x1, y1, z1}, {x1, y, z1}, true);

    addTriangle({x, y, z1}, {x1, y1, z1}, {x, y1, z1}, true);
    addTriangle({x, y, z1}, {x1, y, z1}, {x1, y1, z1}, true);

    // for (auto & vert : vertices) {
    //     vert.color = {rand() % 256, rand() % 256, rand() % 256, 255};
    // }
    updateVertexNormals(smooth);
}

void RenderObject::addSphere(Vec4 const &v, double r, int thetaSteps, int phiSteps)
{
    double thetaStepSize = 2 * M_PI / thetaSteps;
    double phiStepSize = M_PI / phiSteps;

    std::vector<std::vector<Vec4>> dp(phiSteps + 1);
    for (auto &row : dp)
    {
        for (int i = 0; i <= thetaSteps; i++)
        {
            row.push_back({0, 0, 0, -1});
        }
    }

    std::vector<int> offsets{0, 0, 1, 0, 1, 1, 0, 1};

    for (int phiStep = 0; phiStep < phiSteps; phiStep++)
    {
        for (int thetaStep = 0; thetaStep < thetaSteps; thetaStep++)
        {
            std::vector<Vec4> points;
            points.reserve(4);
            for (int i = 0; i < 8; i += 2)
            {
                int newPhiStep = phiStep + offsets[i];
                int newThetaStep = thetaStep + offsets[i + 1];

                if (dp[newPhiStep][newThetaStep].getW() == -1)
                {
                    double phi = newPhiStep * phiStepSize;
                    double theta = newThetaStep * thetaStepSize;
                    Vec4 point = {r * std::cos(phi),
                                  r * std::sin(phi) * std::cos(theta),
                                  r * std::sin(phi) * std::sin(theta)};
                    point.setW(0);
                    dp[newPhiStep][newThetaStep] = v + point;
                }
                points.push_back(dp[newPhiStep][newThetaStep]);
            }
            if (phiStep == 0)
            {
                addTriangle(points[0], points[1], points[2], true);
            }
            else if (phiStep == phiSteps - 1)
            {
                addTriangle(points[0], points[2], points[3], true);
            }
            else
            {
                addTriangle(points[0], points[1], points[2], true);
                addTriangle(points[0], points[2], points[3], true);
            }
        }
    }

    updateVertexNormals(smooth);
}

void RenderObject::addTorus(Vec4 const &v, double r1, double r2, int thetaSteps, int phiSteps)
{
    double thetaStepSize = 2 * M_PI / thetaSteps;
    double phiStepSize = 2 * M_PI / phiSteps;

    std::vector<std::vector<Vec4>> dp(phiSteps + 1);
    for (auto &row : dp)
    {
        for (int i = 0; i <= thetaSteps; i++)
        {
            row.push_back({0, 0, 0, -1});
        }
    }

    std::vector<int> offsets{0, 0, 1, 0, 1, 1, 0, 1};

    for (int phiStep = 0; phiStep < phiSteps; phiStep++)
    {
        for (int thetaStep = 0; thetaStep < thetaSteps; thetaStep++)
        {
            std::vector<Vec4> points;
            points.reserve(4);
            for (int i = 0; i < 8; i += 2)
            {
                int newPhiStep = phiStep + offsets[i];
                int newThetaStep = thetaStep + offsets[i + 1];

                if (dp[newPhiStep][newThetaStep].getW() == -1)
                {
                    double phi = newPhiStep * phiStepSize;
                    double theta = newThetaStep * thetaStepSize;
                    Vec4 point = {(r2 + r1 * std::cos(phi)) * std::cos(theta),
                                  r1 * std::sin(phi),
                                  (r2 + r1 * std::cos(phi)) * std::sin(theta)};
                    point.setW(0);
                    dp[newPhiStep][newThetaStep] = v + point;
                }
                points.push_back(dp[newPhiStep][newThetaStep]);
            }

            addTriangle(points[0], points[1], points[2], true);
            addTriangle(points[0], points[2], points[3], true);
        }
    }
    updateVertexNormals(smooth);
}