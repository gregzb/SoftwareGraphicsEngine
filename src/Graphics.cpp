#include "Graphics.hpp"
#include "Screen.hpp"
#include "Utils.hpp"
#include "Mat4.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <limits>
#include <tuple>

#include <cstdlib>

Graphics::Graphics(Screen &screen) : screen(screen){};

void Graphics::drawLine(Vec4 a, Vec4 b, Color color) const
{
    drawLine(a, b, color, false);
}

void Graphics::drawLine(Vec4 a, Vec4 b, Color color, bool flipped) const
{
    if (b.getX() < a.getX())
    {
        return drawLine(b, a, color, flipped);
    }

    long x0r = std::lround(a.getX()), x1r = std::lround(b.getX()), y0r = std::lround(a.getY()), y1r = std::lround(b.getY());

    long dY = y1r - y0r, dX = x1r - x0r;

    if (std::abs(dY) > dX)
    {
        return drawLine({a.getY(), a.getX()}, {b.getY(), b.getX()}, color, !flipped);
    }

    int dir = Utils::sign(dY);

    dY = std::abs(dY) * 2;

    int d = dY - dX;

    dX *= 2;

    int y = y0r;
    for (int x = x0r; x <= x1r; x++)
    {
        if (!flipped)
            //screen.plot({x, y}, color);
            screen(x, y) = color;
        else
            //screen.plot({y, x}, color);
            screen(y, x) = color;

        if (d > 0)
        {
            y += dir;
            d -= dX;
        }
        d += dY;
    }
}

void Graphics::drawEdges(Mat4 &matrix, Color color) const
{
    for (int col = 0; col < matrix.getCols(); col += 2)
    {
        drawLine(matrix.getPoint(col), matrix.getPoint(col + 1), color);
    }
}

// void Graphics::drawTriangles(Mat4 &matrix, Color color) const
// {
//     std::vector<Vec3> points(3);
//     //std::cout << matrix.getCols() << std::endl;
//     for (int col = 0; col < matrix.getCols(); col += 3)
//     {
//         Vec3 normal = matrix.getTriangleNormal(col);
//         // if (matrix.getPoint(col).dot(normal) < 0)
//         // {
//         //     continue; //Normal calculation might be broken, should check on it. (May need to do calculation earlier)
//         // }

//         // if (normal.z < std::numeric_limits<double>::epsilon() * 100) //this works in screen space :)
//         // {
//         //     continue;
//         // }
//         //std::cout << std::endl;
//         //std::cout << "Tri: ";

//         for (int i = 0; i < 3; i++)
//         {
//             points[i] = matrix.getPoint(col + i);
//             points[i].x = Utils::map(points[i].x, -1, 1, 0, screen.getWidth());
//             points[i].y = Utils::map(points[i].y, -1, 1, 0, screen.getHeight());
//             //std::cout << points[i];
//         }

//         //std::cout << std::endl;

//         color = {255, (col * 7) % 256, (col * 13 + 50) % 256, 255};
//         fillTriangle(points, color);
//         // for (int i = 0; i < 3; i++)
//         // {
//         //     // std::cout << "drawing from/to" << std::endl;
//         //     // std::cout << points[i][0] << " " << points[i][1] << " " << points[i][2] << std::endl;
//         //     // std::cout << verts[(i + 1) % 3][0] << " " << verts[(i + 1) % 3][1] << " " << verts[(i + 1) % 3][2] << std::endl;
//         //     //drawLine(verts[i][0], verts[i][1], verts[i][2] + 0.5, verts[(i + 1) % 3][0], verts[(i + 1) % 3][1], verts[(i + 1) % 3][2] + 0.5, randColor, true);
//         //     drawLine(points[i].add({0, 0, 1000}), points[(i + 1) % 3].add({0, 0, 1000}), {255, 255, 255, 255}, true);
//         // }
//     }
// }

void Graphics::drawTriangle(std::vector<Vertex> vertices, RenderObject &ro) const
{
    Vec4 normal = vertices[0].getFaceNormal(vertices[1], vertices[2]);

    if (normal.getZ() < std::numeric_limits<double>::epsilon() * 100) //this works in screen space :)
    {
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        vertices[i].setPos({Utils::map(vertices[i].getPos().getX(), -1, 1, 0, screen.getWidth()), Utils::map(vertices[i].getPos().getY(), -1, 1, 0, screen.getHeight()), vertices[i].getPos().getZ(), vertices[i].getPos().getW()});
    }

    //std::cout << std::endl;

    //color = {255, (col * 7) % 256, (col * 13 + 50) % 256, 255};
    fillTriangle(vertices, ro);
    // for (int i = 0; i < 3; i++)
    // {
    //     // std::cout << "drawing from/to" << std::endl;
    //     // std::cout << points[i][0] << " " << points[i][1] << " " << points[i][2] << std::endl;
    //     // std::cout << verts[(i + 1) % 3][0] << " " << verts[(i + 1) % 3][1] << " " << verts[(i + 1) % 3][2] << std::endl;
    //     //drawLine(verts[i][0], verts[i][1], verts[i][2] + 0.5, verts[(i + 1) % 3][0], verts[(i + 1) % 3][1], verts[(i + 1) % 3][2] + 0.5, randColor, true);
    //     drawLine(vertices[i].pos.add({0, 0, -10}), vertices[(i + 1) % 3].pos.add({0, 0, -10}), {255, 0, 255, 255}, true);
    // }
}

struct InterpInfo
{
    double x;
    double z;
    double overZ;
    Vec4 tex;
};

class Interpolation
{
private:
    double startDepth, depthStepX, depthStepY;
    double startOverZ, overZStepX, overZStepY;
    Vec4 texStepX, texStepY;

public:
    Interpolation(Vertex const &min, Vertex const &mid, Vertex const &max)
    {
        double xDenom = 1.0 / (((mid.getPos().getX() - max.getPos().getX()) * (min.getPos().getY() - max.getPos().getY())) - ((min.getPos().getX() - max.getPos().getX()) * (mid.getPos().getY() - max.getPos().getY())));
        double yDenom = -xDenom;

        //std::cout << min.pos << mid.pos << max.pos << std::endl;
        //std::cout << min.texCoords << mid.texCoords << max.texCoords << std::endl;

        startDepth = min.getPos().getZ();
        depthStepX = ((mid.getPos().getZ() - max.getPos().getZ()) * (min.getPos().getY() - max.getPos().getY()) - (min.getPos().getZ() - max.getPos().getZ()) * (mid.getPos().getY() - max.getPos().getY())) * xDenom;
        depthStepY = ((mid.getPos().getZ() - max.getPos().getZ()) * (min.getPos().getX() - max.getPos().getX()) - (min.getPos().getZ() - max.getPos().getZ()) * (mid.getPos().getX() - max.getPos().getX())) * yDenom;

        startOverZ = 1 / min.getPos().getW();
        overZStepX = ((1 / mid.getPos().getW() - 1 / max.getPos().getW()) * (min.getPos().getY() - max.getPos().getY()) - (1 / min.getPos().getW() - 1 / max.getPos().getW()) * (mid.getPos().getY() - max.getPos().getY())) * xDenom;
        overZStepY = ((1 / mid.getPos().getW() - 1 / max.getPos().getW()) * (min.getPos().getX() - max.getPos().getX()) - (1 / min.getPos().getW() - 1 / max.getPos().getW()) * (mid.getPos().getX() - max.getPos().getX())) * yDenom;

        texStepX = ((mid.getTexCoords() / mid.getPos().getW() - max.getTexCoords() / max.getPos().getW()) * (min.getPos().getY() - max.getPos().getY()) - (min.getTexCoords() / min.getPos().getW() - max.getTexCoords() / max.getPos().getW()) * (mid.getPos().getY() - max.getPos().getY())) * xDenom;
        texStepY = ((mid.getTexCoords() / mid.getPos().getW() - max.getTexCoords() / max.getPos().getW()) * (min.getPos().getX() - max.getPos().getX()) - (min.getTexCoords() / min.getPos().getW() - max.getTexCoords() / max.getPos().getW()) * (mid.getPos().getX() - max.getPos().getX())) * yDenom;
        //std::cout << "steps: " << texStepX << texStepX << std::endl;
    }

    double getDepthStepX()
    {
        return depthStepX;
    }

    double getDepthStepY()
    {
        return depthStepY;
    }

    double getStartDepth()
    {
        return startDepth;
    }

    double getOverZStepX()
    {
        return overZStepX;
    }

    double getOverZStepY()
    {
        return overZStepY;
    }

    double getStartOverZ()
    {
        return startOverZ;
    }

    Vec4 getTexStepX()
    {
        return texStepX;
    }

    Vec4 getTexStepY()
    {
        return texStepY;
    }
};

void projectSide(std::vector<InterpInfo> &scanlines, Vertex &lower, Vertex &higher, int minVal, int side, Interpolation interpolation)
{
    int y0 = std::ceil(lower.getPos().getY()), y1 = std::ceil(higher.getPos().getY());
    double dY = higher.getPos().getY() - lower.getPos().getY(), dX = higher.getPos().getX() - lower.getPos().getX();
    //double dZ = higher.pos.z - lower.pos.z;
    //Vec4 dC = higher.color.sub(lower.color);

    if (dY <= 0)
        return;

    double xStep = dX / dY;
    double x = lower.getPos().getX() + (y0 - lower.getPos().getY()) * xStep;

    //min y vert index, not min y
    /**
     * dsfsfsdf
     * sdf
     * sdf
     * s
     * df
     * sd
     * fs
     * df
     * sd
     * fs
     * df
     * sdf
     **/
    //double z = interpolation.getStartDepth() + interpolation.getDepthStepX() * (x - lower.pos.x) + interpolation.getDepthStepY() * (y0 - lower.pos.y);
    double z = lower.getPos().getZ() + interpolation.getDepthStepX() * (x - lower.getPos().getX()) + interpolation.getDepthStepY() * (y0 - lower.getPos().getY());
    //double z = interpolation.getStartDepth();
    //double zStep = interpolation.getDepthStepY();
    double zStep = interpolation.getDepthStepY() + interpolation.getDepthStepX() * xStep;
    //double zStep = dZ / dY;
    // //Vec4 cStep = dC.scale(1/dY);
    //double z = lower.pos.z + (y0 - lower.pos.y) * zStep;
    //double z = lower.pos.z;
    //Vec4 c = lower.color.add(cStep.scale(y0 - lower.pos.y));

    //double overZ = interpolation.getStartOverZ() + interpolation.getOverZStepX() * (x - lower.pos.x) + interpolation.getOverZStepY() * (y0 - lower.pos.y);
    double overZ = 1 / lower.getPos().getW() + interpolation.getOverZStepX() * (x - lower.getPos().getX()) + interpolation.getOverZStepY() * (y0 - lower.getPos().getY());
    double overZStep = interpolation.getOverZStepY() + interpolation.getOverZStepX() * xStep;

    Vec4 tex = lower.getTexCoords() / lower.getPos().getW() + interpolation.getTexStepX() * (x - lower.getPos().getX()) + interpolation.getTexStepY() * (y0 - lower.getPos().getY());
    Vec4 texStep = interpolation.getTexStepY() + interpolation.getTexStepX() * xStep;

    for (int y = y0; y < y1; y++)
    {
        scanlines[(y - minVal) * 2 + side] = {x, z, overZ, tex};
        x += xStep;
        z += zStep;
        overZ += overZStep;
        tex = tex + texStep;
        //c = c.add(cStep);
    }
}

void Graphics::fillTriangle(std::vector<Vertex> &verts, RenderObject &ro) const
{
    if (verts[0].getPos().getY() > verts[1].getPos().getY())
        std::swap(verts[0], verts[1]);
    if (verts[1].getPos().getY() > verts[2].getPos().getY())
        std::swap(verts[1], verts[2]);
    if (verts[0].getPos().getY() > verts[1].getPos().getY())
        std::swap(verts[0], verts[1]);

    Interpolation interpolation = {verts[0], verts[1], verts[2]};

    //int side = Utils::sign(verts[1].pos.sub(verts[0].pos).cross(verts[2].pos.sub(verts[0].pos)).z);
    int side = Utils::sign((verts[1].getPos() - verts[0].getPos()).cross(verts[2].getPos() - verts[0].getPos()).getZ());
    side = side >= 0 ? 0 : 1;

    std::vector<InterpInfo> scanlines(2 * (std::ceil(verts[2].getPos().getY()) - std::ceil(verts[0].getPos().getY())));
    projectSide(scanlines, verts[0], verts[2], std::ceil(verts[0].getPos().getY()), side, interpolation);
    projectSide(scanlines, verts[0], verts[1], std::ceil(verts[0].getPos().getY()), 1 - side, interpolation);
    projectSide(scanlines, verts[1], verts[2], std::ceil(verts[0].getPos().getY()), 1 - side, interpolation);
    for (uint i = 0; i < scanlines.size(); i += 2)
    {
        InterpInfo leftInterp = scanlines[i];
        InterpInfo rightInterp = scanlines[i + 1];
        //std::cout << leftInterp.x << " " << rightInterp.x << std::endl;
        //std::cout << leftInterp.tex << " " << rightInterp.tex << std::endl;

        int xLeft, xRight;
        xLeft = std::ceil(leftInterp.x);
        xRight = std::ceil(rightInterp.x);

        //double zStep = (rightInterp.z - leftInterp.z) / (rightInterp.x - leftInterp.x);
        //double z = leftInterp.z;
        double z = leftInterp.z + interpolation.getDepthStepX() * (xLeft - leftInterp.x);
        double overZ = leftInterp.overZ + interpolation.getOverZStepX() * (xLeft - leftInterp.x);
        Vec4 tex = leftInterp.tex + interpolation.getTexStepX() * (xLeft - leftInterp.x);
        //double z = leftInterp.z;

        //Vec4 cStep = (rightInterp.color.sub(leftInterp.color)).scale(1.0 / (rightInterp.x - leftInterp.x));
        //Vec4 c = leftInterp.color;

        int y = std::ceil(verts[0].getPos().getY()) + i / 2;

        for (int x = xLeft; x < xRight; x++)
        {
            if (screen.zbuf(y, x) > z)
            {
                screen.zbuf(y, x) = z;
                Vec4 texAdjust = tex;
                texAdjust.setX(texAdjust.getX()/overZ);
                texAdjust.setY(texAdjust.getY()/overZ);
                
                texAdjust.setX(std::fmod(texAdjust.getX(), 1));
                if (texAdjust.getX() < 0)
                    texAdjust.setX(1 + texAdjust.getX());
                texAdjust.setY(std::fmod(texAdjust.getY(), 1));
                if (texAdjust.getY() < 0)
                    texAdjust.setY(1 + texAdjust.getY());

                texAdjust.setY(1 - texAdjust.getY());
                //texAdjust.x = 1 - texAdjust.x;

                //std::cout << overZ << std::endl;
                //std::cout << texAdjust << std::endl;
                //std::cout << std::lround(texAdjust.y * (ro.getTexture().getHeight()-1)) << " " << std::lround(texAdjust.x * (ro.getTexture().getWidth()-1)) << std::endl;
                // if (std::lround(texAdjust.x * (ro.getTexture().getWidth() - 1)) == 1209)
                // {
                //     std::cout << texAdjust << std::endl;
                // }
                Color col = ro.getMaterial()->mapKd.read(std::lround(texAdjust.getY() * (ro.getMaterial()->mapKd.getHeight() - 1)), std::lround(texAdjust.getX() * (ro.getMaterial()->mapKd.getWidth() - 1)));
                screen(y, x) = col;
            }
            //z += zStep;
            z += interpolation.getDepthStepX(); // may need to recalc etc: (rightInterp.z - leftInterp.z)/(rightInterp.x - leftInterp.x));
            overZ += interpolation.getOverZStepX();
            tex = tex + interpolation.getTexStepX();
            //c = c.add(cStep);
        }
    }
}

void Graphics::renderObject(Camera &cam, RenderObject object) const
{
    //instead of doing 3 mults here, get all 3 matrices and then chain and do one mult
    //Mat4 tris = object.toWorldSpace();
    Mat4 const &mvMat = cam.getViewMatrix().multiply(object.getModelMatrix());
    Mat4 const &pMat = cam.getProjectionMatrix();
    //Mat4 const &mvpMat = cam.getProjectionMatrix().multiply(mvMat);
    // tris.multiplyMutate(cam.getViewMatrix());
    // tris.multiplyMutate(cam.getProjectionMatrix());

    std::vector<Vertex> &tris = object.getMesh();
    std::vector<int> &indices = object.getMeshIndices();

    for (auto &vert : tris)
    {
        vert.transform(mvMat);
    }

    object.generateVertexNormals();

    for (auto &vert : tris)
    {
        vert.transform(pMat);
        vert.setPos(vert.getPos().perspectiveDivision());
    }

    std::vector<Vertex> triangle(3);

    for (uint i = 0; i < indices.size(); i += 3)
    {
        for (int j = 0; j < 3; j++)
        {
            triangle[j] = tris[indices[i + j]];
        }

        drawTriangle(triangle, object);
    }

    //tris.multiplyMutate(mvpMat);

    //std::cout << tris.toString() << std::endl;

    //clipping must be done here - clip from -w to w for all dimensions (or dont), -w's get discard, so clip that too
    //std::cout << tris.toString() << std::endl;
    // Mat4 clipped;

    // std::vector<Vec4> points(3);
    // for (int col = 0; col < tris.getCols(); col += 3)
    // {
    //     int wPosCount = 0;
    //     for (int i = 0; i < 3; i++)
    //     {
    //         points[i] = tris.getVec4(col + i);
    //         // > 0 or greater than near? not sure
    //         if (points[i].w > 0)
    //         {
    //             wPosCount += 1;
    //         }
    //     }

    //     for (int i = 0; i < 3; i++)
    //     {
    //         points[i].x /= points[i].w;
    //         points[i].y /= points[i].w;
    //         points[i].z /= points[i].w;
    //     }

    //     if (wPosCount == 0)
    //     {
    //         //no clipping, dont add
    //         std::`` << "0 pos" << std::endl;
    //     }
    //     else if (wPosCount == 1)
    //     {
    //         //simpler clip
    //         std::cout << "1 pos" << std::endl;
    //         clipped.addVec4(points[0]);
    //         clipped.addVec4(points[1]);
    //         clipped.addVec4(points[2]);
    //     }
    //     else if (wPosCount == 2)
    //     {
    //         //harder clip, need two tris
    //         std::cout << "2 pos" << std::endl;
    //         clipped.addVec4(points[0]);
    //         clipped.addVec4(points[1]);
    //         clipped.addVec4(points[2]);
    //     }
    //     else if (wPosCount == 3)
    //     {
    //         //no clipping, add
    //         std::cout << "3 pos" << std::endl;
    //         clipped.addVec4(points[0]);
    //         clipped.addVec4(points[1]);
    //         clipped.addVec4(points[2]);
    //     }
    // }

    // //clipped.perspectiveDivision();

    // drawTriangles(clipped, {255, 255, 255, 255});
}

void Graphics::clear(Color color) const
{
    for (int row = 0; row < screen.getHeight(); row++)
    {
        for (int col = 0; col < screen.getWidth(); col++)
        {
            screen(row, col) = color;
        }
    }
}