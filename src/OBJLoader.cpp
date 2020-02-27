#include <vector>
#include <unordered_map>
#include <string>

#include <sstream>
#include <iostream>
#include <fstream>

#include <unordered_set>

#include "OBJLoader.hpp"
#include "Vec4.hpp"
#include "Utils.hpp"
#include "PixelGrid.hpp"

OBJMaterial::OBJMaterial() : mapKd(0, 0)
{
}

void OBJMaterial::setVecConstants(Vec4 Ka_, Vec4 Kd_, Vec4 Ks_, Vec4 Ke_)
{
    Ka = Ka_;
    Kd = Kd_;
    Ks = Ks_;
    Ke = Ke_;
}

void OBJMaterial::setConstants(double Ns_, double Ni_, double opaqueness_)
{
    Ns = Ns_;
    Ni = Ni_;
    opaqueness = opaqueness_;
}

void OBJMaterial::setIllumMode(int illum_)
{
    illum = illum_;
}

void OBJMaterial::setKdTex(std::string texName)
{
    //parse texname, read file, convert to pixelgrid
    mapKd = PixelGrid<Color>::loadTexture(texName);
}

void OBJMaterial::setName(std::string name)
{
    materialName = name;
}

OBJObject::OBJObject()
{
}

void OBJObject::addVertexIndices(std::vector<int> indices)
{
    vertexIndices.push_back(indices);
}

void OBJObject::addVertexTexIndices(std::vector<int> indices)
{
    vertexTexIndices.push_back(indices);
}

void OBJObject::addVertexNormalIndices(std::vector<int> indices)
{
    vertexNormalIndices.push_back(indices);
}

void OBJObject::setMat(OBJMaterial *mat_)
{
    mat = mat_;
}

void OBJObject::setShading(bool smooth_)
{
    smoothShading = smooth_;
}

void OBJObject::setName(std::string name_)
{
    objectName = name_;
}

std::vector<std::vector<int>> const &OBJObject::getVertexIndices()
{
    return vertexIndices;
}

std::vector<std::vector<int>> const &OBJObject::getVertexTexIndices()
{
    return vertexTexIndices;
}

std::vector<std::vector<int>> const &OBJObject::getVertexNormalIndices()
{
    return vertexNormalIndices;
}

OBJMaterial *OBJObject::getMat()
{
    return mat;
}

bool OBJObject::getShading()
{
    return smoothShading;
}

std::string const &OBJObject::getName()
{
    return objectName;
}

void OBJLoader::parseMTL()
{
    //resources is temporary!!!
    std::ifstream mtlFile("resources/" + mtlFileName);
    //std::cout << "resources/" + mtlFileName << std::endl;
    std::string line;

    Vec4 Ka, Kd, Ks, Ke;
    double Ns, Ni, opaqueness;

    while (std::getline(mtlFile, line))
    {
        std::istringstream iss(line);
        std::string infoType;
        iss >> infoType;

        if (infoType == "newmtl")
        {
            if (currentMaterial != nullptr)
            {
                currentMaterial->setConstants(Ns, Ni, opaqueness);
                currentMaterial->setVecConstants(Ka, Kd, Ks, Ke);
            }

            std::string matName;
            iss >> matName;

            materials.insert({matName, OBJMaterial()});
            currentMaterial = &materials.at(matName);
            currentMaterial->setName(matName);

            Ka = {};
            Kd = {};
            Ks = {};
            Ke = {};

            Ns = 0;
            Ni = 0;
            opaqueness = 1;
        }
        else if (infoType == "Ns")
        {
            double val;
            iss >> val;

            Ns = val;
        }
        else if (infoType == "Ka")
        {
            Vec4 val;
            iss >> val;

            Ka = val;
        }
        else if (infoType == "Kd")
        {
            Vec4 val;
            iss >> val;

            Kd = val;
        }
        else if (infoType == "Ks")
        {
            Vec4 val;
            iss >> val;

            Ks = val;
        }
        else if (infoType == "Ke")
        {
            Vec4 val;
            iss >> val;

            Ke = val;
        }
        else if (infoType == "Ni")
        {
            double val;
            iss >> val;

            Ni = val;
        }
        else if (infoType == "d")
        {
            double val;
            iss >> val;

            opaqueness = val;
        }
        else if (infoType == "illum")
        {
            int val;
            iss >> val;

            currentMaterial->setIllumMode(val);
        }
        else if (infoType == "map_Kd")
        {
            std::string texName;
            iss >> texName;

            currentMaterial->setKdTex(texName);
        }
    }
    if (currentMaterial != nullptr)
    {
        currentMaterial->setConstants(Ns, Ni, opaqueness);
        currentMaterial->setVecConstants(Ka, Kd, Ks, Ke);
    }
    mtlFile.close();
}

OBJLoader::OBJLoader(std::string const &fileName)
{
    currentObject = nullptr;
    currentMaterial = nullptr;

    std::ifstream objFile(fileName);
    std::string line;
    while (std::getline(objFile, line))
    {
        std::istringstream iss(line);
        std::string infoType;
        //std::cout << line << std::endl;
        iss >> infoType;
        // if (!(iss >> infoType))
        //     break; //dont break, just go next line

        // std::cout << infoType << std::endl;

        if (infoType == "mtllib")
        {
            iss >> mtlFileName;
            parseMTL();
        }
        else if (infoType == "o")
        {
            std::string objectName;
            iss >> objectName;

            objects.insert({objectName, OBJObject()});
            currentObject = &objects.at(objectName);
            currentObject->setName(objectName);
        }
        else if (infoType == "v")
        {
            Vec4 pos;
            iss >> pos;
            v.push_back(pos);
        }
        else if (infoType == "vt")
        {
            Vec4 pos;
            iss >> pos;
            vt.push_back(pos);
        }
        else if (infoType == "vn")
        {
            Vec4 pos;
            iss >> pos;
            vn.push_back(pos);
        }
        else if (infoType == "f")
        {
            std::vector<std::string> verts(3);
            iss >> verts[0] >> verts[1] >> verts[2];

            std::vector<std::vector<int>> indices(3);

            for (int i = 0; i < 3; i++)
            {
                indices[i].push_back(-1);
                indices[i].push_back(-1);
                indices[i].push_back(-1);
            }

            for (int i = 0; i < 3; i++)
            {
                int counter = 0;
                std::istringstream vertStream(verts[i]);
                std::string temp;
                while (std::getline(vertStream, temp, '/'))
                {
                    indices[counter][i] = std::stoi(temp);
                    counter++;
                }
            }

            currentObject->addVertexIndices(indices[0]);
            currentObject->addVertexTexIndices(indices[1]);
            currentObject->addVertexNormalIndices(indices[2]);
        }
        else if (infoType == "usemtl")
        {
            std::string matName;
            iss >> matName;

            //std::cout << matName << std::endl;

            currentObject->setMat(&materials.at(matName));
        }
        else if (infoType == "s")
        {
            std::string smooth;
            iss >> smooth;
            if (smooth == "1")
            {
                currentObject->setShading(true);
            }
            else
            {
                currentObject->setShading(false);
            }
        }

        // process pair (a,b)
    }
    objFile.close();
}

RenderObject OBJLoader::toRenderObject(std::string name)
{
    RenderObject temp;
    std::unordered_set<int> inserted;
    // std::cout << objects.size() << std::endl;
    // for (auto thing : objects) {
    //     std::cout << thing.first << std::endl;
    // }

    OBJObject &obj = objects.at(name);
    for (auto const & face : obj.getVertexIndices()) {
        for (auto const & vertex : face) {
            auto pos = v[vertex-1];
            auto tex = vt[vertex-1];

            temp.addVertex({pos, tex});
        }
    }
    temp.setTexture(obj.getMat()->getTexture());
    // for (auto face : obj.getVertexIndices())
    // {
    //     // if (face[0] > v.size() || face[1] > v.size() || face[2] > v.size())
    //     // std::cout << v.size() << " " << face[0] << " " << face[1] << " " << face[2] << std::endl;
    //     //its 1-indexed
    //     temp.addTriangle(v[face[0] - 1], v[face[1] - 1], v[face[2] - 1]);
    // }
    return temp;
}