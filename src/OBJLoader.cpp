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

// OBJMaterial::OBJMaterial() : mapKd(0, 0)
// {
// }

// void OBJMaterial::setVecConstants(Vec4 Ka_, Vec4 Kd_, Vec4 Ks_, Vec4 Ke_)
// {
//     Ka = Ka_;
//     Kd = Kd_;
//     Ks = Ks_;
//     Ke = Ke_;
// }

// void OBJMaterial::setConstants(double Ns_, double Ni_, double opaqueness_)
// {
//     Ns = Ns_;
//     Ni = Ni_;
//     opaqueness = opaqueness_;
// }

// void OBJMaterial::setIllumMode(int illum_)
// {
//     illum = illum_;
// }

// void OBJMaterial::setKdTex(std::string texName)
// {
//     //parse texname, read file, convert to pixelgrid
//     mapKd = PixelGrid<Color>::loadTexture(texName);
// }

// void OBJMaterial::setName(std::string name)
// {
//     materialName = name;
// }

// OBJObject::OBJObject()
// {
// }

// void OBJObject::addVertexIndices(std::vector<int> indices)
// {
//     vertexIndices.push_back(indices);
// }

// void OBJObject::addVertexTexIndices(std::vector<int> indices)
// {
//     vertexTexIndices.push_back(indices);
// }

// void OBJObject::addVertexNormalIndices(std::vector<int> indices)
// {
//     vertexNormalIndices.push_back(indices);
// }

// void OBJObject::setMat(OBJMaterial *mat_)
// {
//     mat = mat_;
// }

// void OBJObject::setShading(bool smooth_)
// {
//     smoothShading = smooth_;
// }

// void OBJObject::setName(std::string name_)
// {
//     objectName = name_;
// }

// std::vector<std::vector<int>> const &OBJObject::getVertexIndices()
// {
//     return vertexIndices;
// }

// std::vector<std::vector<int>> const &OBJObject::getVertexTexIndices()
// {
//     return vertexTexIndices;
// }

// std::vector<std::vector<int>> const &OBJObject::getVertexNormalIndices()
// {
//     return vertexNormalIndices;
// }

// OBJMaterial *OBJObject::getMat()
// {
//     return mat;
// }

// bool OBJObject::getShading()
// {
//     return smoothShading;
// }

// std::string const &OBJObject::getName()
// {
//     return objectName;
// }

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
            std::string matName;
            iss >> matName;

            materials.insert({matName, OBJMaterial()});
            currentMaterial = &materials.at(matName);
            currentMaterial->name = matName;
        }
        else if (infoType == "Ns")
        {
            double val;
            iss >> val;

            currentMaterial->Ns = val;
        }
        else if (infoType == "Ka")
        {
            Vec4 val;
            iss >> val;

            currentMaterial->Ka = val;
        }
        else if (infoType == "Kd")
        {
            Vec4 val;
            iss >> val;

            currentMaterial->Kd = val;
        }
        else if (infoType == "Ks")
        {
            Vec4 val;
            iss >> val;

            currentMaterial->Ks = val;
        }
        else if (infoType == "Ke")
        {
            Vec4 val;
            iss >> val;

            currentMaterial->Ke = val;
        }
        else if (infoType == "Ni")
        {
            double val;
            iss >> val;

            currentMaterial->Ni = val;
        }
        else if (infoType == "d")
        {
            double val;
            iss >> val;

            currentMaterial->opaqueness = val;
        }
        else if (infoType == "illum")
        {
            int val;
            iss >> val;

            currentMaterial->illum = val;
        }
        else if (infoType == "map_Kd")
        {
            std::string texName;
            iss >> texName;

            currentMaterial->mapKd = PixelGrid<Color>::loadTexture(texName);
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
        iss >> infoType;

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
            currentObject->name = objectName;
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
                    if (temp != "")
                    indices[counter][i] = std::stoi(temp);
                    counter++;
                }
            }

            currentObject->vertexIndices.push_back(indices[0]);
            currentObject->vertexTexIndices.push_back(indices[1]);
            currentObject->vertexNormalIndices.push_back(indices[2]);
        }
        else if (infoType == "usemtl")
        {
            std::string matName;
            iss >> matName;

            currentObject->mat = &materials.at(matName);
        }
        else if (infoType == "s")
        {
            std::string smooth;
            iss >> smooth;
            currentObject->smoothShading = smooth == "1";
        }

    }
    objFile.close();
}

RenderObject OBJLoader::toRenderObject(std::string name) const
{
    RenderObject temp;
    std::unordered_set<int> inserted;
    // std::cout << objects.size() << std::endl;
    // for (auto thing : objects) {
    //     std::cout << thing.first << std::endl;
    // }

    OBJObject const &obj = objects.at(name);
    // for (auto const & face : obj.getVertexIndices()) {
    //     for (auto const & vertex : face) {
    //         auto pos = v[vertex-1];
    //         auto tex = vt[vertex-1];

    //         temp.addVertex({pos, tex});
    //     }
    // }
    // std::cout << obj.getVertexIndices().size() << std::endl;
    // std::cout << obj.getVertexTexIndices().size() << std::endl;
    for(uint i = 0; i < obj.vertexIndices.size(); i++) {
        for (uint j = 0; j < obj.vertexIndices[i].size(); j++) {
            auto pos = v[obj.vertexIndices[i][j]-1];
            auto tex = vt[obj.vertexTexIndices[i][j]-1];

            //std::cout << obj.getVertexIndices()[i][j]-1 << " " << obj.getVertexTexIndices()[i][j]-1 << std::endl;

            temp.addVertex({pos, tex});
        }
    }
    temp.setMaterial(obj.mat);
    //temp.setTexture(obj.mat->mapKd);
    //std::cout << obj.getMat()->getTexture().getWidth() << " " << obj.getMat()->getTexture().getHeight() << std::endl;
    //exit(0);
    //obj.getMat()->getTexture().display();
    // for (auto face : obj.getVertexIndices())
    // {
    //     // if (face[0] > v.size() || face[1] > v.size() || face[2] > v.size())
    //     // std::cout << v.size() << " " << face[0] << " " << face[1] << " " << face[2] << std::endl;
    //     //its 1-indexed
    //     temp.addTriangle(v[face[0] - 1], v[face[1] - 1], v[face[2] - 1]);
    // }
    return temp;
}