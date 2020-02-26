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
    if (b.x < a.x)
    {
        return drawLine(b, a, color, flipped);
    }

    long x0r = std::lround(a.x), x1r = std::lround(b.x), y0r = std::lround(a.y), y1r = std::lround(b.y);

    long dY = y1r - y0r, dX = x1r - x0r;

    if (std::abs(dY) > dX)
    {
        return drawLine({a.y, a.x}, {b.y, b.x}, color, !flipped);
    }

    int dir = Utils::sign(dY);

    dY = std::abs(dY) * 2;

    int d = dY - dX;

    dX *= 2;

    int y = y0r;
    for (int x = x0r; x <= x1r; x++)
    {
        if (!flipped)
            screen.plot({x, y}, color);
        else
            screen.plot({y, x}, color);

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

void Graphics::drawTriangle(std::vector<Vertex> vertices) const
{
    Vec4 normal = vertices[0].getFaceNormal(vertices[1], vertices[2]);

    if (normal.z < std::numeric_limits<double>::epsilon() * 100) //this works in screen space :)
    {
        //return;
    }

    for (int i = 0; i < 3; i++)
    {
        vertices[i].pos.x = Utils::map(vertices[i].pos.x, -1, 1, 0, screen.getWidth());
        vertices[i].pos.y = Utils::map(vertices[i].pos.y, -1, 1, 0, screen.getHeight());
    }

    //std::cout << std::endl;

    //color = {255, (col * 7) % 256, (col * 13 + 50) % 256, 255};
    fillTriangle(vertices);
    // for (int i = 0; i < 3; i++)
    // {
    //     // std::cout << "drawing from/to" << std::endl;
    //     // std::cout << points[i][0] << " " << points[i][1] << " " << points[i][2] << std::endl;
    //     // std::cout << verts[(i + 1) % 3][0] << " " << verts[(i + 1) % 3][1] << " " << verts[(i + 1) % 3][2] << std::endl;
    //     //drawLine(verts[i][0], verts[i][1], verts[i][2] + 0.5, verts[(i + 1) % 3][0], verts[(i + 1) % 3][1], verts[(i + 1) % 3][2] + 0.5, randColor, true);
    //     drawLine(points[i].add({0, 0, 1000}), points[(i + 1) % 3].add({0, 0, 1000}), {255, 255, 255, 255}, true);
    // }
}

struct InterpInfo
{
    double x;
    double z;
    double overZ;
    //Vec4 color;
};

class Interpolation
{
private:
    double startDepth, depthStepX, depthStepY;
    double startOverZ, overZStepX, overZStepY;

public:
    Interpolation(Vertex const &min, Vertex const &mid, Vertex const &max)
    {
        double xDenom = 1.0 / (((mid.pos.x - max.pos.x) * (min.pos.y - max.pos.y)) - ((min.pos.x - max.pos.x) * (mid.pos.y - max.pos.y)));
        double yDenom = -xDenom;

        startDepth = min.pos.z;
        depthStepX = ((mid.pos.z - max.pos.z) * (min.pos.y - max.pos.y) - (min.pos.z - max.pos.z) * (mid.pos.y - max.pos.y)) * xDenom;
        depthStepY = ((mid.pos.z - max.pos.z) * (min.pos.x - max.pos.x) - (min.pos.z - max.pos.z) * (mid.pos.x - max.pos.x)) * yDenom;

        startOverZ = 1 / min.pos.z;
        overZStepX = ((1/mid.pos.w - 1/max.pos.w) * (min.pos.y - max.pos.y) - (1/min.pos.w - 1/max.pos.w) * (mid.pos.y - max.pos.y)) * xDenom;
        overZStepY = ((1/mid.pos.w - 1/max.pos.w) * (min.pos.x - max.pos.x) - (1/min.pos.w - 1/max.pos.w) * (mid.pos.x - max.pos.x)) * yDenom;
    }

    double getDepthStepX() {
        return depthStepX;
    }

    double getDepthStepY() {
        return depthStepY;
    }

    double getStartDepth() {
        return startDepth;
    }

    double getOverZStepX() {
        return overZStepX;
    }

    double getOverZStepY() {
        return overZStepY;
    }

    double getStartOverZ() {
        return startOverZ;
    }
};

void projectSide(std::vector<InterpInfo> &scanlines, Vertex &lower, Vertex &higher, int minVal, int side, Interpolation interpolation)
{
    int y0 = std::ceil(lower.pos.y), y1 = std::ceil(higher.pos.y);
    double dY = higher.pos.y - lower.pos.y, dX = higher.pos.x - lower.pos.x, dZ = higher.pos.z - lower.pos.z;
    Vec4 dC = higher.color.sub(lower.color);

    if (dY <= 0)
        return;

    double xStep = dX / dY;
    double x = lower.pos.x + (y0 - lower.pos.y) * xStep;

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
    double z = lower.pos.z + interpolation.getDepthStepX() * (x - lower.pos.x) + interpolation.getDepthStepY() * (y0 - lower.pos.y);
    //double z = interpolation.getStartDepth();
    //double zStep = interpolation.getDepthStepY();
    double zStep = interpolation.getDepthStepY() + interpolation.getDepthStepX() * xStep;
    //double zStep = dZ / dY;
    // //Vec4 cStep = dC.scale(1/dY);
    //double z = lower.pos.z + (y0 - lower.pos.y) * zStep;
    //double z = lower.pos.z;
    //Vec4 c = lower.color.add(cStep.scale(y0 - lower.pos.y));

    double overZ = interpolation.getStartOverZ() + interpolation.getOverZStepX() * (x - lower.pos.x) + interpolation.getOverZStepY() * (y0 - lower.pos.y);
    double overZStep = interpolation.getOverZStepY() + interpolation.getOverZStepX() * xStep;

    for (int y = y0; y < y1; y++)
    {
        scanlines[(y - minVal) * 2 + side] = {x, z, overZ};
        x += xStep;
        z += zStep;
        overZ += overZStep;
        //c = c.add(cStep);
    }
}

void Graphics::fillTriangle(std::vector<Vertex> &verts) const
{
    if (verts[0].pos.y > verts[1].pos.y)
        std::swap(verts[0], verts[1]);
    if (verts[1].pos.y > verts[2].pos.y)
        std::swap(verts[1], verts[2]);
    if (verts[0].pos.y > verts[1].pos.y)
        std::swap(verts[0], verts[1]);

    //std::cout << verts[0].pos << verts[1].pos << verts[2].pos << std::endl;

    Interpolation interpolation = {verts[0], verts[1], verts[2]};
    // std::cout << interpolation.getStartDepth() << std::endl;
    // std::cout << interpolation.getDepthStepX() << std::endl;
    // std::cout << interpolation.getDepthStepY() << std::endl;
    Color col = {std::rand()%255, std::rand()%255, std::rand()%255, 255};

    int side = Utils::sign(verts[1].pos.sub(verts[0].pos).cross(verts[2].pos.sub(verts[0].pos)).z);
    side = side >= 0 ? 0 : 1;

    std::vector<InterpInfo> scanlines(2 * (std::ceil(verts[2].pos.y) - std::ceil(verts[0].pos.y)));
    projectSide(scanlines, verts[0], verts[2], std::ceil(verts[0].pos.y), side, interpolation);
    projectSide(scanlines, verts[0], verts[1], std::ceil(verts[0].pos.y), 1 - side, interpolation);
    projectSide(scanlines, verts[1], verts[2], std::ceil(verts[0].pos.y), 1 - side, interpolation);
    for (uint i = 0; i < scanlines.size(); i += 2)
    {
        InterpInfo leftInterp = scanlines[i];
        InterpInfo rightInterp = scanlines[i + 1];
        //std::cout << leftInterp.z << " " << rightInterp.z << std::endl;

        int xLeft, xRight;
        xLeft = std::ceil(leftInterp.x);
        xRight = std::ceil(rightInterp.x);

        //double zStep = (rightInterp.z - leftInterp.z) / (rightInterp.x - leftInterp.x);
        //double z = leftInterp.z;
        double z = leftInterp.z + interpolation.getDepthStepX() * (xLeft - leftInterp.x);
        double overZ = leftInterp.overZ + interpolation.getOverZStepX() * (xLeft - leftInterp.x);
        //double z = leftInterp.z;

        //Vec4 cStep = (rightInterp.color.sub(leftInterp.color)).scale(1.0 / (rightInterp.x - leftInterp.x));
        //Vec4 c = leftInterp.color;

        int y = std::ceil(verts[0].pos.y) + i / 2;

        for (int x = xLeft; x < xRight; x++)
        {
            if (screen.zbuf(y, x) > z)
            {
                screen.zbuf(y, x) = z;
                screen(y, x) = col;
            }
            //z += zStep;
            z += interpolation.getDepthStepX(); // may need to recalc etc: (rightInterp.z - leftInterp.z)/(rightInterp.x - leftInterp.x));
            overZ += interpolation.getOverZStepX();
            //c = c.add(cStep);
        }
    }
}

void Graphics::renderObject(Camera &cam, RenderObject object) const
{
    //instead of doing 3 mults here, get all 3 matrices and then chain and do one mult
    //Mat4 tris = object.toWorldSpace();
    Mat4 mvMat = cam.getViewMatrix().multiply(object.getModelMatrix());
    Mat4 pMat = cam.getProjectionMatrix();
    Mat4 mvpMat = cam.getProjectionMatrix().multiply(mvMat);
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
        vert.pos = vert.pos.perspectiveDivision();
    }

    std::vector<Vertex> triangle(3);

    for (uint i = 0; i < indices.size(); i += 3)
    {
        for (int j = 0; j < 3; j++)
        {
            triangle[j] = tris[indices[i + j]];
        }

        drawTriangle(triangle);
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