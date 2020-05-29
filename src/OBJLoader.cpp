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
            currentMaterial->addDiffuseMap(base + "/" + texName);
        }
        else if (infoType == "map_Bump")
        {
            std::string texName;
            iss >> texName;
            currentMaterial->addBumpMap(base + "/" + texName);
        }
        else if (infoType == "map_Ks")
        {
            std::string texName;
            iss >> texName;
            currentMaterial->addSpecularMap(base + "/" + texName);
        }
        else if (infoType == "map_Ns")
        {
            std::string texName;
            iss >> texName;
            currentMaterial->addShininessMap(base + "/" + texName);
        }
        else if (infoType == "map_Ke")
        {
            std::string texName;
            iss >> texName;
            currentMaterial->addEmissiveMap(base + "/" + texName);
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
            std::vector<std::string> verts;
            std::string tempStr;

            while (iss >> tempStr)
            {
                verts.push_back(tempStr);
            }

            //iss >> verts[0] >> verts[1] >> verts[2];

            std::vector<std::vector<int>> indices(3);

            for (int i = 0; i < 3; i++)
            {
                for (unsigned int j = 0; j < verts.size(); j++)
                {
                    indices[i].push_back(-1);
                }
            }

            for (unsigned int i = 0; i < verts.size(); i++)
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

            // std::cout << indices[0][0] << " " << indices[1][0] << " " << indices[2][0] << std::endl;
            // exit(1);
            for (unsigned int i = 2; i < verts.size(); i++)
            {
                currentObject->vertexIndices.push_back({indices[0][0], indices[0][i-1], indices[0][i]});
                currentObject->vertexTexIndices.push_back({indices[1][0], indices[1][i-1], indices[1][i]});
                currentObject->vertexNormalIndices.push_back({indices[2][0], indices[2][i-1], indices[2][i]});
                currentObject->triMats.push_back(currentMaterial);
            }
        }
        else if (infoType == "usemtl")
        {
            std::string matName;
            iss >> matName;
            if (Material::materials.count(matName) == 0) {
                std::cout << "Could not find a material named " << matName << std::endl;
                exit(1);
            }
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
    std::unordered_map<int, int> vertMap;
    vertMap.insert({-2, -1});
    std::unordered_map<int, int> texMap;
    texMap.insert({-2, -1});
    std::unordered_map<int, int> normMap;
    normMap.insert({-2, -1});

    if (objects.count(name) == 0)
    {
        std::cout << "Could not find object: " << name << " in an OBJ file." << std::endl;
        exit(1);
    }

    // std::cout << objects.count(name) << std::endl;
    OBJObject const &obj = objects.at(name);

    //std::cout << obj.vertexIndices.size() << std::endl;
    //std::cout << obj.name << std::endl;

    RenderObject temp(obj.smoothShading);

    bool recalc = false;
    recalc = true; // ALWAYS RECALCING FOR NOW FOR TANGENTS

    for (uint i = 0; i < obj.vertexIndices.size(); i++)
    {
        assert(obj.vertexIndices[i].size() == 3);

        //loop through indices, see which indices get used, store in set

        for (uint j = 0; j < obj.vertexIndices[i].size(); j++)
        {
            // std::cout << i << " " << j << std::endl;
            int vertIdx = obj.vertexIndices[i][j] - 1;
            int texIdx = obj.vertexTexIndices[i][j] - 1;
            int normIdx = obj.vertexNormalIndices[i][j] - 1;
            // std::cout << "hi !" << std::endl;

            // std::cout << obj.vertexIndices.size() << " " << obj.vertexIndices.size() << " " << obj.vertexNormalIndices.size() << std::endl;
            // std::cout << vertIdx << " " << texIdx << " " << normIdx << std::endl;

            if (vertMap.count(vertIdx) == 0)
            {
                vertMap.insert({vertIdx, temp.getMesh().size()});
                temp.addVertexPos(v[vertIdx]);
            }
            if (texMap.count(texIdx) == 0)
            {
                texMap.insert({texIdx, temp.getTexCoords().size()});
                temp.addTextureCoord(vt[texIdx]);
            }
            if (normIdx == -2) {
                recalc = true;
            }
            if (normMap.count(normIdx) == 0)
            {
                normMap.insert({normIdx, temp.getNormals().size()});
                temp.addNormal(vn[normIdx]);
            }
            // std::cout << "bye! " << std::endl;
            temp.addIndex({vertMap[vertIdx], texMap[texIdx], normMap[normIdx], -1});
        }

        temp.addMaterial(obj.triMats[i]);
    }

    if (recalc) {
        temp.updateVertexNormals(obj.smoothShading);
    }

    return temp;
}