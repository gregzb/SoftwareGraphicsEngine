#include "Scene.hpp"
#include "VertexPos.hpp"
#include <cmath>
#include <functional>

void Scene::clipTriangle(std::vector<Vertex> &triangle, int dimension, int side, std::vector<Vertex> &out)
{
    //add 0.00001 to get rid of pixel innaccuracies.
    for (uint i = 0; i < triangle.size(); i++)
    {
        Vec4 const &pos1 = triangle[i].getProjPos();
        bool inside1 = pos1[dimension] * side <= pos1.getW()*1.0001+0.00001;

        Vec4 const &pos2 = triangle[(i + 1) % triangle.size()].getProjPos();
        bool inside2 = pos2[dimension] * side <= pos2.getW()*1.0001+0.00001;

        if (inside1)
        {
            out.push_back(triangle[i]);
        }

        if (inside1 != inside2)
        {
            double t = (pos1.getW()*1.0001+0.00001 - pos1[dimension] * side) / ((pos2[dimension] * side - pos2.getW()*1.0001-0.00001) - (pos1[dimension] * side - pos1.getW()*1.0001-0.00001));
            out.push_back(triangle[i].lerp(triangle[(i + 1) % triangle.size()], t));
        }
    }
}

void Scene::renderObject(Camera const &cam, Screen &screen, RenderObject &object, bool isSkybox)
{
    Mat4 const &mMat = object.getModelMatrix();
    // std::cout << cam.getViewMatrix().toString() << std::endl;
    // std::cout << std::endl;
    // std::cout << cam.getRotationMatrix().toString() << std::endl;
    Mat4 const &mvMat = isSkybox ? cam.getRotationMatrix().multiply(object.getModelMatrix()) : cam.getViewMatrix().multiply(object.getModelMatrix());
    Mat4 const &pMat = cam.isPerspective() ? cam.getPerspectiveProjectionMatrix() : cam.getOrthographicProjectionMatrix();

    Mat4 const &transposed = mvMat.transpose();
    Mat4 const &inverted = transposed.invert();

    // std::vector<Vertex> &tris = object.getMesh(); // need to not make copy or smthing?
    // std::vector<int> const &indices = object.getMeshIndices();
    std::vector<Vec4> &positions = object.getMesh();
    std::vector<Vec4> &texes = object.getTexCoords();
    std::vector<Vec4> &norms = object.getNormals();
    std::vector<Vec4> &tans = object.getTangents();
    std::vector<std::tuple<int, int, int, int>> const &indices = object.getIndices();

    std::vector<Vertex> baseTri = {Vertex({}, {}, {}, {}), Vertex({}, {}, {}, {}), Vertex({}, {}, {}, {})};

    std::vector<Vertex> triangle = {Vertex({}, {}, {}, {}), Vertex({}, {}, {}, {}), Vertex({}, {}, {}, {})};
    std::vector<Vertex> clipping1;
    clipping1.reserve(32);
    std::vector<Vertex> clipping2;
    clipping2.reserve(32);

    for (unsigned int i = 0; i < indices.size(); i += 3)
    {
        //std::cout << i << " " << indices.size() << std::endl;
        for (int j = 0; j < 3; j++)
        {
            //std::cout << "p" << std::endl;
            auto index = indices[i + j];
            //std::cout << std::get<0>(index) << " " << std::get<1>(index) << " " << std::get<2>(index) << std::endl;
            Vec4 const &po = std::get<0>(index) >= 0 ? positions[std::get<0>(index)] : Vec4();
            Vec4 const &te = std::get<1>(index) >= 0 ? texes[std::get<1>(index)] : Vec4();

            // normal and tangent - if they dont exist, throw error?

            Vec4 no = std::get<2>(index) >= 0 ? norms[std::get<2>(index)] : Vec4();
            Vec4 ta = std::get<3>(index) >= 0 ? tans[std::get<3>(index)] : Vec4();

            no.setW(0);
            no = no.transform(mMat);
            ta.setW(0);
            ta = ta.transform(mMat);

            //     /*
            //     TRANSFORMING NORMAL BY THE MV MATRIX IS WRONG.
            //     YOU MUST TRANSFORM BY TRANSPOSE OF INVERSE OF MATRIX
            //     BEFORE THAT, SET W TO 0 - NORMALS ARE A DIRECTION, NOT A POSITION
            //     */

            //    // TRANSFORM NORMAL AND TANGENT BY mvMAT, SET W TO 0

            // Vec4 no;
            // if (std::get<2>(index) >= 0)
            // {
            //     no = norms[std::get<2>(index)];
            //     no.setW(0);
            //     no = no.transform(inverted).normalize();
            // }

            //Vec4 const &no = std::get<2>(index) >= 0 ? norms[std::get<2>(index)].transform(mvMat.transpose().invert()) : Vec4();
            baseTri[j] = Vertex(po, te, no.normalize(), ta.normalize());
            baseTri[j].getVertexPos().updateWorldPos(baseTri[j].getPos().transform(mvMat));
            baseTri[j].getVertexPos().updateProjPos(baseTri[j].getWorldPos().transform(pMat));
        }

        int pointsInside = 0;
        for (int j = 0; j < 3; j++)
        {
            pointsInside += baseTri[j].getProjPos().inViewFrustum();
        }

        Material const *currMat = object.getMat(i / 3);

        switch (pointsInside)
        {
        case 3:
            for (int j = 0; j < 3; j++)
            {
                triangle[j] = baseTri[j];
                triangle[j].getVertexPos().updateProjPos(triangle[j].getProjPos().perspectiveDivision());
                triangle[j].getVertexPos().updateProjPos({Utils::map(triangle[j].getProjPos().getX(), -1, 1, 0, screen.getWidth()), Utils::map(triangle[j].getProjPos().getY(), -1, 1, 0, screen.getHeight()), triangle[j].getProjPos().getZ(), triangle[j].getProjPos().getW()});
            }
            drawTriangle(screen, triangle, object, currMat, isSkybox);
            break;
        case 2:
        case 1:
        case 0:
            /*
        Cohen Sutherland(?) - might help speed this up
        */
            clipping1.clear();
            clipping2.clear();

            for (unsigned int j = 0; j < 3; j++)
            {
                //clipping1 = baseTri;
                clipping1.push_back(baseTri[j]);
                //std::cout << baseTri[j].getProjPos() << std::endl;
            }

            for (unsigned int j = 0; j < 3; j++)
            {
                clipping2.clear();
                clipTriangle(clipping1, j, 1, clipping2);
                clipping1.clear();
                clipTriangle(clipping2, j, -1, clipping1);
            }
            //std::cout << clipping1.size() << std::endl;

            for (unsigned int j = 0; j < clipping1.size(); j++)
            {
                clipping1[j].getVertexPos().updateProjPos(clipping1[j].getProjPos().perspectiveDivision());
                clipping1[j].getVertexPos().updateProjPos({Utils::map(clipping1[j].getProjPos().getX(), -1, 1, 0, screen.getWidth()), Utils::map(clipping1[j].getProjPos().getY(), -1, 1, 0, screen.getHeight()), clipping1[j].getProjPos().getZ(), clipping1[j].getProjPos().getW()});
            }

            triangle[0] = clipping1[0];
            for (int j = 1; j < ((int)clipping1.size() - 1); j++)
            {
                triangle[1] = clipping1[j];
                triangle[2] = clipping1[j + 1];
                drawTriangle(screen, triangle, object, currMat, isSkybox);
            }
            break;
        default:
            break;
        }
    }
}

void Scene::drawTriangle(Screen &screen, std::vector<Vertex> vertices, RenderObject const &object, Material const *mat, bool isSkybox)
{
    Vec4 normal = vertices[0].getProjPos().getNormal(vertices[1].getProjPos(), vertices[2].getProjPos());
    if (normal.getZ() < std::numeric_limits<double>::epsilon() * 100) //this works in screen space :)
    {
        return;
    }

    //std::cout << vertices[0].getNormal() << " " << vertices[1].getNormal() << " " << vertices[2].getNormal() << " " << std::endl;

    fillTriangle(screen, vertices, object, mat, isSkybox);

    // for (int i = 0; i < 3; i++) {
    //     screen.drawLine(vertices[i].getProjPos(), vertices[(i+1)%3].getProjPos(), {255, 0, 255, 255});
    // }
}

struct LineInfo
{
    double x, xStepY;
    double z, zStepY;
    double overZ, overZStepY;
    Vec4 tex, texStepY;
    Vec4 normal, normalStepY;
    Vec4 tangent, tangentStepY;
    Vec4 position, positionStepY;
};

void Scene::fillTriangle(Screen &screen, std::vector<Vertex> &verts, RenderObject const &object, Material const *mat, bool isSkybox)
{
    if (verts[0].getProjPos().getY() > verts[1].getProjPos().getY())
        std::swap(verts[0], verts[1]);
    if (verts[1].getProjPos().getY() > verts[2].getProjPos().getY())
        std::swap(verts[1], verts[2]);
    if (verts[0].getProjPos().getY() > verts[1].getProjPos().getY())
        std::swap(verts[0], verts[1]);

    Vec4 const &vert20 = verts[2].getProjPos() - verts[0].getProjPos();
    Vec4 const &vert10 = verts[1].getProjPos() - verts[0].getProjPos();
    Vec4 const &vert21 = verts[2].getProjPos() - verts[1].getProjPos();

    Vec4 const &z2 = Vec4(verts[0].getProjPos()[3], verts[0].getProjPos()[3], verts[1].getProjPos()[3]);

    Vec4 xVerts = {vert20[0], vert10[0], vert21[0]};
    Vec4 yVerts = {vert20[1], vert10[1], vert21[1]};
    Vec4 zVerts = {vert20[2], vert10[2], vert21[2]};
    Vec4 overZVerts = {1 / verts[2].getProjPos()[3] - 1 / verts[0].getProjPos()[3], 1 / verts[1].getProjPos()[3] - 1 / verts[0].getProjPos()[3], 1 / verts[2].getProjPos()[3] - 1 / verts[1].getProjPos()[3]};

    Vec4 xTexVerts = {verts[2].getTexCoords()[0] / verts[2].getProjPos()[3] - verts[0].getTexCoords()[0] / verts[0].getProjPos()[3], verts[1].getTexCoords()[0] / verts[1].getProjPos()[3] - verts[0].getTexCoords()[0] / verts[0].getProjPos()[3], verts[2].getTexCoords()[0] / verts[2].getProjPos()[3] - verts[1].getTexCoords()[0] / verts[1].getProjPos()[3]};
    Vec4 yTexVerts = {verts[2].getTexCoords()[1] / verts[2].getProjPos()[3] - verts[0].getTexCoords()[1] / verts[0].getProjPos()[3], verts[1].getTexCoords()[1] / verts[1].getProjPos()[3] - verts[0].getTexCoords()[1] / verts[0].getProjPos()[3], verts[2].getTexCoords()[1] / verts[2].getProjPos()[3] - verts[1].getTexCoords()[1] / verts[1].getProjPos()[3]};

    Vec4 xNormalVerts = {verts[2].getNormal()[0] / verts[2].getProjPos()[3] - verts[0].getNormal()[0] / verts[0].getProjPos()[3], verts[1].getNormal()[0] / verts[1].getProjPos()[3] - verts[0].getNormal()[0] / verts[0].getProjPos()[3], verts[2].getNormal()[0] / verts[2].getProjPos()[3] - verts[1].getNormal()[0] / verts[1].getProjPos()[3]};
    Vec4 yNormalVerts = {verts[2].getNormal()[1] / verts[2].getProjPos()[3] - verts[0].getNormal()[1] / verts[0].getProjPos()[3], verts[1].getNormal()[1] / verts[1].getProjPos()[3] - verts[0].getNormal()[1] / verts[0].getProjPos()[3], verts[2].getNormal()[1] / verts[2].getProjPos()[3] - verts[1].getNormal()[1] / verts[1].getProjPos()[3]};
    Vec4 zNormalVerts = {verts[2].getNormal()[2] / verts[2].getProjPos()[3] - verts[0].getNormal()[2] / verts[0].getProjPos()[3], verts[1].getNormal()[2] / verts[1].getProjPos()[3] - verts[0].getNormal()[2] / verts[0].getProjPos()[3], verts[2].getNormal()[2] / verts[2].getProjPos()[3] - verts[1].getNormal()[2] / verts[1].getProjPos()[3]};

    Vec4 xTangentVerts = {verts[2].getTangent()[0] / verts[2].getProjPos()[3] - verts[0].getTangent()[0] / verts[0].getProjPos()[3], verts[1].getTangent()[0] / verts[1].getProjPos()[3] - verts[0].getTangent()[0] / verts[0].getProjPos()[3], verts[2].getTangent()[0] / verts[2].getProjPos()[3] - verts[1].getTangent()[0] / verts[1].getProjPos()[3]};
    Vec4 yTangentVerts = {verts[2].getTangent()[1] / verts[2].getProjPos()[3] - verts[0].getTangent()[1] / verts[0].getProjPos()[3], verts[1].getTangent()[1] / verts[1].getProjPos()[3] - verts[0].getTangent()[1] / verts[0].getProjPos()[3], verts[2].getTangent()[1] / verts[2].getProjPos()[3] - verts[1].getTangent()[1] / verts[1].getProjPos()[3]};
    Vec4 zTangentVerts = {verts[2].getTangent()[2] / verts[2].getProjPos()[3] - verts[0].getTangent()[2] / verts[0].getProjPos()[3], verts[1].getTangent()[2] / verts[1].getProjPos()[3] - verts[0].getTangent()[2] / verts[0].getProjPos()[3], verts[2].getTangent()[2] / verts[2].getProjPos()[3] - verts[1].getTangent()[2] / verts[1].getProjPos()[3]};

    Vec4 xPosVerts = {verts[2].getWorldPos()[0] / verts[2].getProjPos()[3] - verts[0].getWorldPos()[0] / verts[0].getProjPos()[3], verts[1].getWorldPos()[0] / verts[1].getProjPos()[3] - verts[0].getWorldPos()[0] / verts[0].getProjPos()[3], verts[2].getWorldPos()[0] / verts[2].getProjPos()[3] - verts[1].getWorldPos()[0] / verts[1].getProjPos()[3]};
    Vec4 yPosVerts = {verts[2].getWorldPos()[1] / verts[2].getProjPos()[3] - verts[0].getWorldPos()[1] / verts[0].getProjPos()[3], verts[1].getWorldPos()[1] / verts[1].getProjPos()[3] - verts[0].getWorldPos()[1] / verts[0].getProjPos()[3], verts[2].getWorldPos()[1] / verts[2].getProjPos()[3] - verts[1].getWorldPos()[1] / verts[1].getProjPos()[3]};
    Vec4 zPosVerts = {verts[2].getWorldPos()[2] / verts[2].getProjPos()[3] - verts[0].getWorldPos()[2] / verts[0].getProjPos()[3], verts[1].getWorldPos()[2] / verts[1].getProjPos()[3] - verts[0].getWorldPos()[2] / verts[0].getProjPos()[3], verts[2].getWorldPos()[2] / verts[2].getProjPos()[3] - verts[1].getWorldPos()[2] / verts[1].getProjPos()[3]};

    Vec4 xStepY = xVerts / yVerts;
    Vec4 zStepY = zVerts / yVerts;
    Vec4 overZStepY = overZVerts / yVerts;

    Vec4 xTexStepY = xTexVerts / yVerts;
    Vec4 yTexStepY = yTexVerts / yVerts;

    Vec4 xNormalStepY = xNormalVerts / yVerts;
    Vec4 yNormalStepY = yNormalVerts / yVerts;
    Vec4 zNormalStepY = zNormalVerts / yVerts;

    Vec4 xTangentStepY = xTangentVerts / yVerts;
    Vec4 yTangentStepY = yTangentVerts / yVerts;
    Vec4 zTangentStepY = zTangentVerts / yVerts;

    Vec4 xPosStepY = xPosVerts / yVerts;
    Vec4 yPosStepY = yPosVerts / yVerts;
    Vec4 zPosStepY = zPosVerts / yVerts;

    int y = std::ceil(verts[0].getProjPos()[1]);

    double yOffset0 = y - verts[0].getProjPos()[1];
    double yOffset1 = std::ceil(verts[1].getProjPos()[1]) - verts[1].getProjPos()[1];

    Vec4 offs = {yOffset0, yOffset0, yOffset1};

    Vec4 x = {verts[0].getProjPos()[0], verts[0].getProjPos()[0], verts[1].getProjPos()[0]};
    x = x + offs * xStepY;

    Vec4 z = {verts[0].getProjPos()[2], verts[0].getProjPos()[2], verts[1].getProjPos()[2]};
    z = z + offs * zStepY;

    Vec4 overZ = {1 / verts[0].getProjPos()[3], 1 / verts[0].getProjPos()[3], 1 / verts[1].getProjPos()[3]};
    overZ = overZ + offs * overZStepY;

    Vec4 xTex = {verts[0].getTexCoords()[0], verts[0].getTexCoords()[0], verts[1].getTexCoords()[0]};
    xTex = xTex / z2 + offs * xTexStepY;
    Vec4 yTex = {verts[0].getTexCoords()[1], verts[0].getTexCoords()[1], verts[1].getTexCoords()[1]};
    yTex = yTex / z2 + offs * yTexStepY;

    Vec4 xNormal = {verts[0].getNormal()[0], verts[0].getNormal()[0], verts[1].getNormal()[0]};
    xNormal = xNormal / z2 + offs * xNormalStepY;
    Vec4 yNormal = {verts[0].getNormal()[1], verts[0].getNormal()[1], verts[1].getNormal()[1]};
    yNormal = yNormal / z2 + offs * yNormalStepY;
    Vec4 zNormal = {verts[0].getNormal()[2], verts[0].getNormal()[2], verts[1].getNormal()[2]};
    zNormal = zNormal / z2 + offs * zNormalStepY;

    Vec4 xTangent = {verts[0].getTangent()[0], verts[0].getTangent()[0], verts[1].getTangent()[0]};
    xTangent = xTangent / z2 + offs * xTangentStepY;
    Vec4 yTangent = {verts[0].getTangent()[1], verts[0].getTangent()[1], verts[1].getTangent()[1]};
    yTangent = yTangent / z2 + offs * yTangentStepY;
    Vec4 zTangent = {verts[0].getTangent()[2], verts[0].getTangent()[2], verts[1].getTangent()[2]};
    zTangent = zTangent / z2 + offs * zTangentStepY;

    Vec4 xPos = {verts[0].getWorldPos()[0], verts[0].getWorldPos()[0], verts[1].getWorldPos()[0]};
    xPos = xPos / z2 + offs * xPosStepY;
    Vec4 yPos = {verts[0].getWorldPos()[1], verts[0].getWorldPos()[1], verts[1].getWorldPos()[1]};
    yPos = yPos / z2 + offs * yPosStepY;
    Vec4 zPos = {verts[0].getWorldPos()[2], verts[0].getWorldPos()[2], verts[1].getWorldPos()[2]};
    zPos = zPos / z2 + offs * zPosStepY;

    LineInfo left{x[0], xStepY[0], z[0], zStepY[0], overZ[0], overZStepY[0], {xTex[0], yTex[0]}, {xTexStepY[0], yTexStepY[0]}, {xNormal[0], yNormal[0], zNormal[0]}, {xNormalStepY[0], yNormalStepY[0], zNormalStepY[0]}, {xTangent[0], yTangent[0], zTangent[0]}, {xTangentStepY[0], yTangentStepY[0], zTangentStepY[0]}, {xPos[0], yPos[0], zPos[0]}, {xPosStepY[0], yPosStepY[0], zPosStepY[0]}};
    LineInfo right{x[1], xStepY[1], z[1], zStepY[1], overZ[1], overZStepY[1], {xTex[1], yTex[1]}, {xTexStepY[1], yTexStepY[1]}, {xNormal[1], yNormal[1], zNormal[1]}, {xNormalStepY[1], yNormalStepY[1], zNormalStepY[1]}, {xTangent[1], yTangent[1], zTangent[1]}, {xTangentStepY[1], yTangentStepY[1], zTangentStepY[1]}, {xPos[0], yPos[0], zPos[0]}, {xPosStepY[0], yPosStepY[0], zPosStepY[0]}};
    LineInfo temp{x[2], xStepY[2], z[2], zStepY[2], overZ[2], overZStepY[2], {xTex[2], yTex[2]}, {xTexStepY[2], yTexStepY[2]}, {xNormal[2], yNormal[2], zNormal[2]}, {xNormalStepY[2], yNormalStepY[2], zNormalStepY[2]}, {xTangent[2], yTangent[2], zTangent[2]}, {xTangentStepY[2], yTangentStepY[2], zTangentStepY[2]}, {xPos[0], yPos[0], zPos[0]}, {xPosStepY[0], yPosStepY[0], zPosStepY[0]}};

    bool side = vert10.cross(vert20).getZ() >= 0;

    if (!side)
        std::swap(left, right);

    for (int i = 1; i < 3; i++)
    {
        int goal = std::ceil(verts[i].getProjPos()[1]);
        while (y < goal)
        {
            double xOffset = std::ceil(left.x) - left.x;

            double zStepX = (right.z - left.z) / (right.x - left.x);
            double zX = left.z + zStepX * xOffset;

            double overZStepX = (right.overZ - left.overZ) / (right.x - left.x);
            double overZX = left.overZ + overZStepX * xOffset;

            Vec4 texStepX = (right.tex - left.tex) / (right.x - left.x);
            Vec4 texX = left.tex + texStepX * xOffset;

            Vec4 normStepX = (right.normal - left.normal) / (right.x - left.x);
            Vec4 normX = left.normal + normStepX * xOffset;

            Vec4 tanStepX = (right.tangent - left.tangent) / (right.x - left.x);
            Vec4 tanX = left.tangent + tanStepX * xOffset;

            Vec4 posStepX = (right.position - left.position) / (right.x - left.x);
            Vec4 posX = left.position + posStepX * xOffset;

            for (int x = std::ceil(left.x); x < std::ceil(right.x); x++)
            {
                //2 is fine bc it gets normalized from 0 to 1
                double depthTester = isSkybox ? 2 : zX;
                if (screen.zbuf(y, x) > depthTester)
                {
                    //std::cout << "drawing" << std::endl;
                    screen.zbuf(y, x) = zX;
                    // screen.pixelAt(y, x) = color;

                    double regZ = 1.0 / overZX;

                    Vec4 pos = posX * regZ;

                    Vec4 norma = (normX * regZ).normalize();
                    Vec4 tangent = (tanX * regZ).normalize();
                    tangent = (tangent - norma * tangent.dot(norma)).normalize();
                    Vec4 bitangent = tangent.cross(norma);

                    //std::cout << tangent << bitangent << norma << std::endl;

                    norma.setW(0);
                    tangent.setW(0);
                    bitangent.setW(0);

                    Vec4 texAdjust = texX * regZ;

                    texAdjust.setX(std::fmod(texAdjust.getX(), 1.0));
                    if (texAdjust.getX() < 0)
                        texAdjust.setX(1 + texAdjust.getX());
                    texAdjust.setY(std::fmod(texAdjust.getY(), 1.0));
                    if (texAdjust.getY() < 0)
                        texAdjust.setY(1 + texAdjust.getY());

                    texAdjust.setY(1 - texAdjust.getY());
                    texAdjust.setY(std::fmod(texAdjust.getY(), 1.0));

                    Vec4 bump = mat->getBump(texAdjust);
                    bump = bump * 2 - 1;

                    Mat4 TBN;
                    TBN.addPoint(tangent);
                    TBN.addPoint(bitangent);
                    TBN.addPoint(norma);
                    TBN.addPoint({0, 0, 0, 1});
                    //TBN = TBN.transpose();
                    //std::cout << TBN.toString() << std::endl << std::endl;

                    Vec4 normal = TBN.multiply(Mat4({{bump[0]}, {bump[1]}, {bump[2]}, {0}})).getPoint(0).normalize();

                    //normal = norma;

                    Vec4 Ka = mat->getAmbient(texAdjust);
                    Vec4 Kd = mat->getDiffuse(texAdjust);
                    Vec4 Ks = mat->getSpecular(texAdjust);
                    double Ns = mat->getShininess(texAdjust);

                    //std::cout << Ka << " " << Kd << " " << Ks << " " << Ns << std::endl;
                    //std::cout << normal << std::endl;

                    //Ks = {};

                    Vec4 illum;

                    for (auto const &entries : transformedLights)
                    {
                        auto const &light = entries.second;
                        if (light.type == LightType::Ambient)
                        {
                            illum = illum + Ka * light.Ia;
                        }
                        else if (light.type == LightType::Directional)
                        {
                            Vec4 const &L = light.pos.negate().normalize();
                            Vec4 const &V = pos.negate().normalize();
                            Vec4 const &H = (L).normalize();
                            double NL = normal.dot(L);
                            int B = NL > 0 ? 1 : 0;
                            //std::cout << normal << std::endl;
                            //std::cout << NL << std::endl;
                            Vec4 const &diffuse = Kd * light.Id * B * (NL);
                            //Vec4 diffuse = {};
                            //std::cout << B * normal.dot(H) << std::endl;
                            //double v = std::pow(normal.dot(H), 20);
                            //Vec4 const &specular = {v, v, v};
                            Vec4 const &specular = Ks * light.Is * B * std::pow(B * normal.dot(H), Ns);
                            //Vec4 const & specular = {};
                            //Vec4 specular = {};
                            //std::cout << Kd << " " << light.Id << " " << NL << " " << diffuse << " " << specular << std::endl;
                            // Vec4 combo = diffuse + specular;
                            // if (combo[0] < 0 || combo[1] < 0 || combo[2] < 0) {
                            //     std::cout << "hmm" << std::endl;
                            // }
                            //std::cout << normal.dot(H) << " " << Ns << std::endl;
                            illum = illum + diffuse + specular;
                            //illum = illum + diffuse;
                        }
                        else if (light.type == LightType::Point)
                        {
                            Vec4 const &lDist = light.pos - pos;
                            double dist = lDist.magnitude();
                            Vec4 const &L = lDist.normalize();
                            Vec4 const &V = pos.negate().normalize();
                            Vec4 const &H = (L + V).normalize();
                            double NL = normal.dot(L);
                            int B = NL > 0 ? 1 : 0;
                            Vec4 const &diffuse = Kd * light.Id * B * (NL);
                            Vec4 const &specular = Ks * light.Is * B * std::pow(normal.dot(H), Ns);
                            illum = illum + diffuse / (dist * dist) + specular / (dist * dist);
                        }
                    }

                    // if (illum[0] <= 0 && illum[1] <= 0 && illum[2] <= 0)
                    // std::cout << illum << " " << Ks << " " << texAdjust << std::endl;

                    illum.set(0, Utils::clamp(illum[0], 0.0, 1.0));
                    illum.set(1, Utils::clamp(illum[1], 0.0, 1.0));
                    illum.set(2, Utils::clamp(illum[2], 0.0, 1.0));

                    if (isSkybox) {
                        illum = Kd;
                        //illum = {1, 0, 0, 1};
                    }

                    //gamma correct right here by raising to 1/2.2 power
                    illum = illum.pow(1 / 2.2);

                    // illum.set(0, Utils::clamp(illum[0], 0.0, 1.0));
                    // illum.set(1, Utils::clamp(illum[1], 0.0, 1.0));
                    // illum.set(2, Utils::clamp(illum[2], 0.0, 1.0));

                    screen(y, x) = illum.toColor();
                }
                zX += zStepX;
                overZX += overZStepX;
                texX = texX + texStepX;
                normX = normX + normStepX;
                tanX = tanX + tanStepX;
                posX = posX + posStepX;
            }

            left.x += left.xStepY;
            left.z += left.zStepY;
            left.overZ += left.overZStepY;
            left.tex = left.tex + left.texStepY;
            left.normal = left.normal + left.normalStepY;
            left.tangent = left.tangent + left.tangentStepY;
            left.position = left.position + left.positionStepY;
            right.x += right.xStepY;
            right.z += right.zStepY;
            right.overZ += right.overZStepY;
            right.tex = right.tex + right.texStepY;
            right.normal = right.normal + right.normalStepY;
            right.tangent = right.tangent + right.tangentStepY;
            right.position = right.position + right.positionStepY;

            y++;
        }
        if (!side)
            left = temp;
        else
            right = temp;
    }
}

// struct Interpolant
// {
//     double x, z, overZ;
//     Vec4 tex, normal, position;
// };

// class Interpolation
// {
// private:
//     std::pair<double, double> depthStep;
//     std::pair<double, double> overZStep;
//     std::pair<Vec4, Vec4> texStep;
//     std::pair<Vec4, Vec4> normalStep;
//     std::pair<Vec4, Vec4> positionStep;

//     Vertex const &min;
//     Vertex const &mid;
//     Vertex const &max;

//     double xDenom, yDenom;

//     template <typename T>
//     T calcStepX(T const &minVal, T const &midVal, T const &maxVal)
//     {
//         return ((midVal - maxVal) * (min.getProjPos().getY() - max.getProjPos().getY()) - (minVal - maxVal) * (mid.getProjPos().getY() - max.getProjPos().getY())) * xDenom;
//     }

//     template <typename T>
//     T calcStepY(T const &minVal, T const &midVal, T const &maxVal)
//     {
//         return ((midVal - maxVal) * (min.getProjPos().getX() - max.getProjPos().getX()) - (minVal - maxVal) * (mid.getProjPos().getX() - max.getProjPos().getX())) * yDenom;
//     }

// public:
//     Interpolation(Vertex const &min_, Vertex const &mid_, Vertex const &max_) : min(min_), mid(mid_), max(max_)
//     {
//         xDenom = 1.0 / (((mid.getProjPos().getX() - max.getProjPos().getX()) * (min.getProjPos().getY() - max.getProjPos().getY())) - ((min.getProjPos().getX() - max.getProjPos().getX()) * (mid.getProjPos().getY() - max.getProjPos().getY())));
//         yDenom = -xDenom;

//         depthStep.first = calcStepX<double>(min.getProjPos().getZ(), mid.getProjPos().getZ(), max.getProjPos().getZ());
//         depthStep.second = calcStepY<double>(min.getProjPos().getZ(), mid.getProjPos().getZ(), max.getProjPos().getZ());

//         overZStep.first = calcStepX<double>(1.0 / min.getProjPos().getW(), 1.0 / mid.getProjPos().getW(), 1.0 / max.getProjPos().getW());
//         overZStep.second = calcStepY<double>(1.0 / min.getProjPos().getW(), 1.0 / mid.getProjPos().getW(), 1.0 / max.getProjPos().getW());

//         texStep.first = calcStepX<Vec4>(min.getTexCoords() / min.getProjPos().getW(), mid.getTexCoords() / mid.getProjPos().getW(), max.getTexCoords() / max.getProjPos().getW());
//         texStep.second = calcStepY<Vec4>(min.getTexCoords() / min.getProjPos().getW(), mid.getTexCoords() / mid.getProjPos().getW(), max.getTexCoords() / max.getProjPos().getW());

//         normalStep.first = calcStepX<Vec4>(min.getNormal() / min.getProjPos().getW(), mid.getNormal() / mid.getProjPos().getW(), max.getNormal() / max.getProjPos().getW());
//         normalStep.second = calcStepY<Vec4>(min.getNormal() / min.getProjPos().getW(), mid.getNormal() / mid.getProjPos().getW(), max.getNormal() / max.getProjPos().getW());

//         positionStep.first = calcStepX<Vec4>(min.getWorldPos() / min.getProjPos().getW(), mid.getWorldPos() / mid.getProjPos().getW(), max.getWorldPos() / max.getProjPos().getW());
//         positionStep.second = calcStepY<Vec4>(min.getWorldPos() / min.getProjPos().getW(), mid.getWorldPos() / mid.getProjPos().getW(), max.getWorldPos() / max.getProjPos().getW());
//     }

//     std::pair<double, double> const &getDepthStep()
//     {
//         return depthStep;
//     }

//     std::pair<double, double> const &getOverZStep()
//     {
//         return overZStep;
//     }

//     std::pair<Vec4, Vec4> const &getTexStep()
//     {
//         return texStep;
//     }

//     std::pair<Vec4, Vec4> const &getNormalStep()
//     {
//         return normalStep;
//     }

//     std::pair<Vec4, Vec4> const &getPositionStep()
//     {
//         return positionStep;
//     }
// };

// void projectSide(std::vector<Interpolant> &scanlines, Vertex &lower, Vertex &higher, int minVal, int side, Interpolation interpolation)
// {
//     int y0 = std::ceil(lower.getProjPos().getY()), y1 = std::ceil(higher.getProjPos().getY());
//     double dY = higher.getProjPos().getY() - lower.getProjPos().getY(), dX = higher.getProjPos().getX() - lower.getProjPos().getX();

//     if (dY <= 0)
//         return;

//     double yOffset = y0 - lower.getProjPos().getY();

//     double xStep = dX / dY;
//     double x = lower.getProjPos().getX() + yOffset * xStep;

//     double xOffset = x - lower.getProjPos().getX();

//     double z = lower.getProjPos().getZ() + interpolation.getDepthStep().first * xOffset + interpolation.getDepthStep().second * yOffset;
//     double zStep = interpolation.getDepthStep().second + interpolation.getDepthStep().first * xStep;

//     double overZ = 1.0 / lower.getProjPos().getW() + interpolation.getOverZStep().first * xOffset + interpolation.getOverZStep().second * yOffset;
//     double overZStep = interpolation.getOverZStep().second + interpolation.getOverZStep().first * xStep;

//     Vec4 tex = lower.getTexCoords() * (1.0 / lower.getProjPos().getW()) + interpolation.getTexStep().first * xOffset + interpolation.getTexStep().second * yOffset;
//     Vec4 texStep = interpolation.getTexStep().second + interpolation.getTexStep().first * xStep;

//     Vec4 normal = lower.getNormal() * (1.0 / lower.getProjPos().getW()) + interpolation.getNormalStep().first * xOffset + interpolation.getNormalStep().second * yOffset;
//     Vec4 normalStep = interpolation.getNormalStep().second + interpolation.getNormalStep().first * xStep;

//     Vec4 position = lower.getWorldPos() * (1.0 / lower.getProjPos().getW()) + interpolation.getPositionStep().first * xOffset + interpolation.getPositionStep().second * yOffset;
//     Vec4 positionStep = interpolation.getPositionStep().second + interpolation.getPositionStep().first * xStep;

//     for (int y = y0; y < y1; y++)
//     {
//         scanlines[(y - minVal) * 2 + side] = {x, z, overZ, tex, normal, position};
//         x += xStep;
//         z += zStep;
//         overZ += overZStep;
//         tex = tex + texStep;
//         normal = normal + normalStep;
//         position = position + positionStep;
//     }
// }

// void Scene::fillTriangle(Screen &screen, std::vector<Vertex> &verts, RenderObject const &object, Material const *mat)
// {
//     if (verts[0].getProjPos().getY() > verts[1].getProjPos().getY())
//         std::swap(verts[0], verts[1]);
//     if (verts[1].getProjPos().getY() > verts[2].getProjPos().getY())
//         std::swap(verts[1], verts[2]);
//     if (verts[0].getProjPos().getY() > verts[1].getProjPos().getY())
//         std::swap(verts[0], verts[1]);

//     Interpolation interpolation(verts[0], verts[1], verts[2]);

//     int side = Utils::sign((verts[1].getProjPos() - verts[0].getProjPos()).cross(verts[2].getProjPos() - verts[0].getProjPos()).getZ());
//     side = side >= 0 ? 0 : 1;

//     std::vector<Interpolant> scanlines(2 * (std::ceil(verts[2].getProjPos().getY()) - std::ceil(verts[0].getProjPos().getY())));
//     projectSide(scanlines, verts[0], verts[2], std::ceil(verts[0].getProjPos().getY()), side, interpolation);
//     projectSide(scanlines, verts[0], verts[1], std::ceil(verts[0].getProjPos().getY()), 1 - side, interpolation);
//     projectSide(scanlines, verts[1], verts[2], std::ceil(verts[0].getProjPos().getY()), 1 - side, interpolation);
//     for (uint i = 0; i < scanlines.size(); i += 2)
//     {
//         Interpolant leftInterp = scanlines[i];
//         Interpolant rightInterp = scanlines[i + 1];

//         int xLeft, xRight;
//         xLeft = std::ceil(leftInterp.x);
//         xRight = std::ceil(rightInterp.x);

//         double xOffset = xLeft - leftInterp.x;

//         double z = leftInterp.z + interpolation.getDepthStep().first * xOffset;
//         double overZ = leftInterp.overZ + interpolation.getOverZStep().first * xOffset;
//         Vec4 tex = leftInterp.tex + interpolation.getTexStep().first * xOffset;
//         Vec4 norm = leftInterp.normal + interpolation.getNormalStep().first * xOffset;
//         Vec4 pos_ = leftInterp.position + interpolation.getPositionStep().first * xOffset;

//         int y = std::ceil(verts[0].getProjPos().getY()) + i / 2;

//         for (int x = xLeft; x < xRight; x++)
//         {
//             if (screen.zbuf(y, x) > z)
//             {
//                 screen.zbuf(y, x) = z;

//                 double regZ = 1.0 / overZ;

//                 Vec4 pos = pos_ * regZ;

//                 Vec4 normal = (norm * regZ).normalize();

//                 Vec4 texAdjust = tex * regZ;

//                 texAdjust.setX(std::fmod(texAdjust.getX(), 1.0));
//                 if (texAdjust.getX() < 0)
//                     texAdjust.setX(1 + texAdjust.getX());
//                 texAdjust.setY(std::fmod(texAdjust.getY(), 1.0));
//                 if (texAdjust.getY() < 0)
//                     texAdjust.setY(1 + texAdjust.getY());

//                 texAdjust.setY(1 - texAdjust.getY());

//                 //std::cout << texAdjust << std::endl;

//                 Vec4 Ka = mat->getAmbient({texAdjust.getY(), texAdjust.getX()});
//                 Vec4 Kd = mat->getDiffuse({texAdjust.getY(), texAdjust.getX()});
//                 Vec4 Ks = mat->getSpecular({texAdjust.getY(), texAdjust.getX()});
//                 double Ns = mat->getShininess({texAdjust.getY(), texAdjust.getX()});

//                 Vec4 illum;

//                 for (auto const &entries : transformedLights)
//                 {
//                     auto const &light = entries.second;
//                     if (light.type == LightType::Ambient)
//                     {
//                         illum = illum + Ka * light.Ia;
//                     }
//                     else if (light.type == LightType::Directional)
//                     {
//                         Vec4 const &L = light.pos.negate().normalize();
//                         Vec4 const &V = pos.negate().normalize();
//                         Vec4 const &H = (L + V).normalize();
//                         double NL = normal.dot(L);
//                         int B = NL > 0 ? 1 : 0;
//                         Vec4 const &diffuse = Kd * light.Id * B * (NL);
//                         Vec4 const &specular = Ks * light.Is * B * std::pow(normal.dot(H), Ns);
//                         illum = illum + diffuse + specular;
//                     }
//                     else if (light.type == LightType::Point)
//                     {
//                         Vec4 const &lDist = light.pos - pos;
//                         double dist = lDist.magnitude();
//                         Vec4 const &L = lDist.normalize();
//                         Vec4 const &V = pos.negate().normalize();
//                         Vec4 const &H = (L + V).normalize();
//                         double NL = normal.dot(L);
//                         int B = NL > 0 ? 1 : 0;
//                         Vec4 const &diffuse = Kd * light.Id * B * (NL);
//                         Vec4 const &specular = Ks * light.Is * B * std::pow(normal.dot(H), Ns);
//                         illum = illum + diffuse / (dist * dist) + specular / (dist * dist);
//                     }
//                 }

//                 illum.set(0, Utils::clamp(illum[0], 0, 1));
//                 illum.set(1, Utils::clamp(illum[1], 0, 1));
//                 illum.set(2, Utils::clamp(illum[2], 0, 1));

//                 screen(y, x) = illum.toColor();
//             }
//             //z += zStep;
//             z += interpolation.getDepthStep().first; // may need to recalc etc: (rightInterp.z - leftInterp.z)/(rightInterp.x - leftInterp.x);
//             overZ += interpolation.getOverZStep().first;
//             tex = tex + interpolation.getTexStep().first;
//             norm = norm + interpolation.getNormalStep().first;
//             pos_ = pos_ + interpolation.getPositionStep().first;
//         }
//     }
// }

void Scene::removeObject(std::string const &name)
{
    if (objects.count(name) > 0)
    {
        objects.erase(name);
    }
}
void Scene::addObject(std::string const &name, RenderObject const &object)
{
    objects.insert({name, object});
}

void Scene::setSkybox(RenderObject const &object)
{
    skybox = object;
}

void Scene::removeSkybox()
{
    skybox.reset();
}

RenderObject &Scene::getObject(std::string const &name)
{
    return objects.at(name);
}

void Scene::removeLight(std::string const &name)
{
    if (lights.count(name) > 0)
    {
        lights.erase(name);
    }
}
void Scene::addLight(std::string const &name, Light const &light)
{
    lights.insert({name, light});
}
Light &Scene::getLight(std::string const &name)
{
    return lights.at(name);
}

void Scene::renderToScreen(Camera const &cam, Screen &screen)
{
    Mat4 const &vMat = cam.getViewMatrix();
    transformedLights.clear();
    for (auto const &light : lights)
    {
        Light temp = light.second;
        temp.pos = temp.pos.transform(vMat);
        //std::cout << temp.pos << std::endl;
        transformedLights.insert({light.first, temp});
    }
    for (auto &obj : objects)
    {
        renderObject(cam, screen, obj.second);
    }
    if (skybox.has_value())
    {
        renderObject(cam, screen, skybox.value(), true);
    }
}