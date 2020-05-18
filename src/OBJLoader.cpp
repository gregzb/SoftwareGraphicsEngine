#include <vector>
#include <unordered_map>
#include <string>

#include <sstream>
#include <iostream>
#include <fstream>

#include <unordered_set>
#include <cassert>

#include "OBJLoader.hpp"
#include "Vec4.hpp"
#include "Utils.hpp"
#include "PixelGrid.hpp"
#include "Material.hpp"

void OBJLoader::parseMTL(std::string base, std::string mtlFileName)
{
    //resources is temporary!!!
    std::string totalName = base + "/" + mtlFileName;
    std::ifstream mtlFile(totalName);
    //std::cout << "resources/" + mtlFileName << std::endl;
    std::string line;

    while (std::getline(mtlFile, line))
    {
        std::istringstream iss(line);
        std::string infoType;
        iss >> infoType;

        if (infoType == "newmtl")
        {
            std::string matName;
            iss >> matName;

            Material::materials.insert({matName, Material(matName)});
            currentMaterial = &Material::materials.at(matName);
        }
        else if (infoType == "Ns")
        {
            double val;
            iss >> val;
            currentMaterial->setNs(val);
        }
        else if (infoType == "Ka")
        {
            Vec4 val;
            iss >> val;
            currentMaterial->setKa(val);
        }
        else if (infoType == "Kd")
        {
            Vec4 val;
            iss >> val;

            currentMaterial->setKd(val);
        }
        else if (infoType == "Ks")
        {
            Vec4 val;
            iss >> val;

            currentMaterial->setKs(val);
        }
        else if (infoType == "Ke")
        {
            Vec4 val;
            iss >> val;

            currentMaterial->setKe(val);
        }
        else if (infoType == "Ni")
        {
            double val;
            iss >> val;

            //currentMaterial->Ni = val;
        }
        else if (infoType == "d")
        {
            double val;
            iss >> val;

            //currentMaterial->opaqueness = val;
        }
        else if (infoType == "illum")
        {
            int val;
            iss >> val;

            //currentMaterial->illum = val;
        }
        else if (infoType == "map_Kd")
        {
            std::string texName;
            iss >> texName;
            currentMaterial->addDiffuseMap(base+"/"+texName);
        }
        else if (infoType == "map_Bump")
        {
            std::string texName;
            iss >> texName;
            currentMaterial->addBumpMap(base+"/"+texName);
        }
        else if (infoType == "map_Ks")
        {
            std::string texName;
            iss >> texName;
            currentMaterial->addSpecularMap(base+"/"+texName);
        }
        else if (infoType == "map_Ns")
        {
            std::string texName;
            iss >> texName;
            currentMaterial->addShininessMap(base+"/"+texName);
        }
        else if (infoType == "map_Ke")
        {
            std::string texName;
            iss >> texName;
            currentMaterial->addEmissiveMap(base+"/"+texName);
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
            std::string mtlFileName;
            iss >> mtlFileName;
            parseMTL("resources", mtlFileName);
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
            currentObject->triMats.push_back(currentMaterial);
        }
        else if (infoType == "usemtl")
        {
            std::string matName;
            iss >> matName;
            currentMaterial = &Material::materials.at(matName);
            //currentObject->geometryMaterialMap.insert({matName, {}});
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
    for (uint i = 0; i < obj.vertexIndices.size(); i++)
    {
        assert(obj.vertexIndices[i].size() == 3);
        for (uint j = 0; j < obj.vertexIndices[i].size(); j++)
        {
            //std::cout << obj.vertexIndices[i][j] << " " << obj.vertexTexIndices[i][j] << std::endl;
            //std::cout << v[obj.vertexIndices[i][j] - 1] << " " << vt[obj.vertexTexIndices[i][j] - 1] << std::endl;
            auto pos = v[obj.vertexIndices[i][j] - 1];
            auto tex = vt[obj.vertexTexIndices[i][j] - 1];
            //std::cout << pos << tex << std::endl;

            //std::cout << obj.getVertexIndices()[i][j]-1 << " " << obj.getVertexTexIndices()[i][j]-1 << std::endl;

            temp.addVertex({pos, tex});
        }
        temp.addMatTri(obj.triMats[i]);
    }
    //temp.setMaterial(obj.);
    //temp.setMaterial(obj.mat);
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