#include "Scene.hpp"
#include <cmath>
#include <functional>

void Scene::renderObject(Camera const &cam, Screen &screen, RenderObject &object)
{
    Mat4 const &mvMat = cam.getViewMatrix().multiply(object.getModelMatrix());
    Mat4 const &pMat = cam.getProjectionMatrix();

    std::vector<Vertex> tris = object.getMesh();
    std::vector<int> const &indices = object.getMeshIndices();

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

        drawTriangle(screen, triangle, object);
    }
}

void Scene::drawTriangle(Screen &screen, std::vector<Vertex> &vertices, RenderObject const &object)
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

    fillTriangle(screen, vertices, object);
}

struct Interpolant
{
    double x, z, overZ;
    Vec4 tex, normal;
};

class Interpolation
{
private:
    std::pair<double, double> depthStep;
    std::pair<double, double> overZStep;
    std::pair<Vec4, Vec4> texStep;
    std::pair<Vec4, Vec4> normalStep;

    Vertex const &min;
    Vertex const &mid;
    Vertex const &max;

    double xDenom, yDenom;

    template <typename T>
    T calcStepX(T const &minVal, T const &midVal, T const &maxVal)
    {
        return ((midVal - maxVal) * (min.getPos().getY() - max.getPos().getY()) - (minVal - maxVal) * (mid.getPos().getY() - max.getPos().getY())) * xDenom;
    }

    template <typename T>
    T calcStepY(T const &minVal, T const &midVal, T const &maxVal)
    {
        return ((midVal - maxVal) * (min.getPos().getX() - max.getPos().getX()) - (minVal - maxVal) * (mid.getPos().getX() - max.getPos().getX())) * yDenom;
    }

public:
    Interpolation(Vertex const &min_, Vertex const &mid_, Vertex const &max_) : min(min_), mid(mid_), max(max_)
    {
        xDenom = 1.0 / (((mid.getPos().getX() - max.getPos().getX()) * (min.getPos().getY() - max.getPos().getY())) - ((min.getPos().getX() - max.getPos().getX()) * (mid.getPos().getY() - max.getPos().getY())));
        yDenom = -xDenom;

        depthStep.first = calcStepX<double>(min.getPos().getZ(), mid.getPos().getZ(), max.getPos().getZ());
        depthStep.second = calcStepY<double>(min.getPos().getZ(), mid.getPos().getZ(), max.getPos().getZ());

        overZStep.first = calcStepX<double>(1.0/min.getPos().getW(), 1.0/mid.getPos().getW(), 1.0/max.getPos().getW());
        overZStep.second = calcStepY<double>(1.0/min.getPos().getW(), 1.0/mid.getPos().getW(), 1.0/max.getPos().getW());

        texStep.first = calcStepX<Vec4>(min.getTexCoords()/min.getPos().getW(), mid.getTexCoords()/mid.getPos().getW(), max.getTexCoords()/max.getPos().getW());
        texStep.second = calcStepY<Vec4>(min.getTexCoords()/min.getPos().getW(), mid.getTexCoords()/mid.getPos().getW(), max.getTexCoords()/max.getPos().getW());

        normalStep.first = calcStepX<Vec4>(min.getNormal()/min.getPos().getW(), mid.getNormal()/mid.getPos().getW(), max.getNormal()/max.getPos().getW());
        normalStep.second = calcStepY<Vec4>(min.getNormal()/min.getPos().getW(), mid.getNormal()/mid.getPos().getW(), max.getNormal()/max.getPos().getW());
    }

    std::pair<double, double> const & getDepthStep() {
        return depthStep;
    }

    std::pair<double, double> const & getOverZStep() {
        return overZStep;
    }

    std::pair<Vec4, Vec4> const & getTexStep() {
        return texStep;
    }

    std::pair<Vec4, Vec4> const & getNormalStep() {
        return normalStep;
    }
};

void projectSide(std::vector<Interpolant> &scanlines, Vertex &lower, Vertex &higher, int minVal, int side, Interpolation interpolation)
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
    double z = lower.getPos().getZ() + interpolation.getDepthStep().first * (x - lower.getPos().getX()) + interpolation.getDepthStep().second * (y0 - lower.getPos().getY());
    double zStep = interpolation.getDepthStep().second + interpolation.getDepthStep().first * xStep;

    double overZ = 1 / lower.getPos().getW() + interpolation.getOverZStep().first * (x - lower.getPos().getX()) + interpolation.getOverZStep().second * (y0 - lower.getPos().getY());
    double overZStep = interpolation.getOverZStep().second + interpolation.getOverZStep().first * xStep;

    Vec4 tex = lower.getTexCoords() / lower.getPos().getW() + interpolation.getTexStep().first * (x - lower.getPos().getX()) + interpolation.getTexStep().second * (y0 - lower.getPos().getY());
    Vec4 texStep = interpolation.getTexStep().second + interpolation.getTexStep().first * xStep;

    Vec4 normal = lower.getNormal() / lower.getPos().getW() + interpolation.getNormalStep().first * (x - lower.getPos().getX()) + interpolation.getNormalStep().second * (y0 - lower.getPos().getY());
    Vec4 normalStep = interpolation.getNormalStep().second + interpolation.getNormalStep().first * xStep;

    for (int y = y0; y < y1; y++)
    {
        scanlines[(y - minVal) * 2 + side] = {x, z, overZ, tex, normal};
        x += xStep;
        z += zStep;
        overZ += overZStep;
        tex = tex + texStep;
        normal = normal + normalStep;
    }
}

void Scene::fillTriangle(Screen &screen, std::vector<Vertex> &verts, RenderObject const &object)
{
    if (verts[0].getPos().getY() > verts[1].getPos().getY())
        std::swap(verts[0], verts[1]);
    if (verts[1].getPos().getY() > verts[2].getPos().getY())
        std::swap(verts[1], verts[2]);
    if (verts[0].getPos().getY() > verts[1].getPos().getY())
        std::swap(verts[0], verts[1]);

    Interpolation interpolation(verts[0], verts[1], verts[2]);

    int side = Utils::sign((verts[1].getPos() - verts[0].getPos()).cross(verts[2].getPos() - verts[0].getPos()).getZ());
    side = side >= 0 ? 0 : 1;

    std::vector<Interpolant> scanlines(2 * (std::ceil(verts[2].getPos().getY()) - std::ceil(verts[0].getPos().getY())));
    projectSide(scanlines, verts[0], verts[2], std::ceil(verts[0].getPos().getY()), side, interpolation);
    projectSide(scanlines, verts[0], verts[1], std::ceil(verts[0].getPos().getY()), 1 - side, interpolation);
    projectSide(scanlines, verts[1], verts[2], std::ceil(verts[0].getPos().getY()), 1 - side, interpolation);
    for (uint i = 0; i < scanlines.size(); i += 2)
    {
        Interpolant leftInterp = scanlines[i];
        Interpolant rightInterp = scanlines[i + 1];

        int xLeft, xRight;
        xLeft = std::ceil(leftInterp.x);
        xRight = std::ceil(rightInterp.x);

        double z = leftInterp.z + interpolation.getDepthStep().first * (xLeft - leftInterp.x);
        double overZ = leftInterp.overZ + interpolation.getOverZStep().first * (xLeft - leftInterp.x);
        Vec4 tex = leftInterp.tex + interpolation.getTexStep().first * (xLeft - leftInterp.x);
        Vec4 normal = leftInterp.normal + interpolation.getNormalStep().first * (xLeft - leftInterp.x);

        int y = std::ceil(verts[0].getPos().getY()) + i / 2;

        for (int x = xLeft; x < xRight; x++)
        {
            if (screen.zbuf(y, x) > z)
            {
                screen.zbuf(y, x) = z;
                
                Vec4 texAdjust = tex;
                texAdjust.setX(texAdjust.getX() / overZ);
                texAdjust.setY(texAdjust.getY() / overZ);

                texAdjust.setX(std::fmod(texAdjust.getX(), 1.0));
                if (texAdjust.getX() < 0)
                    texAdjust.setX(1 + texAdjust.getX());
                texAdjust.setY(std::fmod(texAdjust.getY(), 1.0));
                if (texAdjust.getY() < 0)
                    texAdjust.setY(1 + texAdjust.getY());

                texAdjust.setY(1 - texAdjust.getY());

                Color col = object.getMaterial()->mapKd.read(std::lround(texAdjust.getY() * (object.getMaterial()->mapKd.getHeight() - 1)), std::lround(texAdjust.getX() * (object.getMaterial()->mapKd.getWidth() - 1)));
                screen(y, x) = col;
            }
            //z += zStep;
            z += interpolation.getDepthStep().first; // may need to recalc etc: (rightInterp.z - leftInterp.z)/(rightInterp.x - leftInterp.x));
            overZ += interpolation.getOverZStep().first;
            tex = tex + interpolation.getTexStep().first;
            normal = normal + interpolation.getNormalStep().first;
        }
    }
}

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

RenderObject &Scene::getObject(std::string const &name) {
    return objects.at(name);
}

void Scene::renderToScreen(Camera const &cam, Screen &screen)
{
    for (auto & obj : objects) {
        renderObject(cam, screen, obj.second);
    }
}