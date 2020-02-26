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

std::vector<std::vector<int>> const & OBJObject::getVertexIndices()
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

OBJMaterial const * OBJObject::getMat()
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
    std::ifstream mtlFile(mtlFileName);
    std::string line;
    while (std::getline(mtlFile, line))
    {
        std::istringstream iss(line);
        std::string infoType;
        if (!(iss >> infoType))
            break;

        if (infoType == "newmtl")
        {
        }
        else if (infoType == "Ns")
        {
        }
        else if (infoType == "Ka")
        {
        }
        else if (infoType == "Kd")
        {
        }
        else if (infoType == "Ks")
        {
        }
        else if (infoType == "Ke")
        {
        }
        else if (infoType == "Ni")
        {
        }
        else if (infoType == "d")
        {
        }
        else if (infoType == "illum")
        {
        }
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
        if (!(iss >> infoType))
            break;

        std::cout << infoType << std::endl;

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

            //currentObject->setMat(&materials.at(matName));
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

    OBJObject &obj = objects.at(name);
    for (auto face : obj.getVertexIndices()) {
        // if (face[0] > v.size() || face[1] > v.size() || face[2] > v.size())
        // std::cout << v.size() << " " << face[0] << " " << face[1] << " " << face[2] << std::endl;
        //its 1-indexed
        temp.addTriangle(v[face[0]-1], v[face[1]-1], v[face[2]-1]);
    }
    return temp;
}