#include "Scene.hpp"
#include <cmath>
#include <functional>

void Scene::renderObject(Camera const &cam, Screen &screen, RenderObject &object)
{
    Mat4 const &mvMat = cam.getViewMatrix().multiply(object.getModelMatrix());
    Mat4 const &pMat = cam.getProjectionMatrix();

    std::vector<Vertex> &tris = object.getMesh(); // need to not make copy or smthing?
    std::vector<int> const &indices = object.getMeshIndices();

    for (auto &vert : tris)
    {
        vert.updateWorldPos(vert.getPos().transform(mvMat));
    }

    object.generateVertexNormals();

    for (auto &vert : tris)
    {
        vert.updateProjPos(vert.getWorldPos().transform(pMat));
    }

    std::vector<Vertex> clippedVertices;

    for (uint i = 0; i < indices.size(); i += 3)
    {
        int pointsInside = 0;
        for (int j = 0; j < 3; j++)
        {
            pointsInside += tris[indices[i+j]].getProjPos().inViewFrustum();
        }
    }

    /*
        clipping goes here!
        */

    for (auto &vert : tris)
    {

        vert.updateProjPos(vert.getProjPos().perspectiveDivision());
    }

    std::vector<Vertex> triangle(3);

    //std::cout << "Rendering" << std::endl;

    for (uint i = 0; i < indices.size(); i += 3)
    {
        for (int j = 0; j < 3; j++)
        {
            triangle[j] = tris[indices[i + j]];
            //std::cout << triangle[j].getProjPos() << std::endl;
        }
        //std::cout << i << std::endl;

        drawTriangle(screen, triangle, object);
    }
}

void Scene::drawTriangle(Screen &screen, std::vector<Vertex> &vertices, RenderObject const &object)
{
    Vec4 normal = vertices[0].getProjNormal(vertices[1], vertices[2]);

    if (normal.getZ() < std::numeric_limits<double>::epsilon() * 100) //this works in screen space :)
    {
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        //Vec4 const & temp = vertices[i].getProjPos();
        vertices[i].updateProjPos({Utils::map(vertices[i].getProjPos().getX(), -1, 1, 0, screen.getWidth()), Utils::map(vertices[i].getProjPos().getY(), -1, 1, 0, screen.getHeight()), vertices[i].getProjPos().getZ(), vertices[i].getProjPos().getW()});
    }

    fillTriangle(screen, vertices, object);

    // for (int i = 0; i < 3; i++) {
    //     screen.drawLine(vertices[i].getProjPos(), vertices[(i+1)%3].getProjPos(), {255, 0, 255, 255});
    // }
}

struct Interpolant
{
    double x, z, overZ;
    Vec4 tex, normal, position;
};

class Interpolation
{
private:
    std::pair<double, double> depthStep;
    std::pair<double, double> overZStep;
    std::pair<Vec4, Vec4> texStep;
    std::pair<Vec4, Vec4> normalStep;
    std::pair<Vec4, Vec4> positionStep;

    Vertex const &min;
    Vertex const &mid;
    Vertex const &max;

    double xDenom, yDenom;

    template <typename T>
    T calcStepX(T const &minVal, T const &midVal, T const &maxVal)
    {
        return ((midVal - maxVal) * (min.getProjPos().getY() - max.getProjPos().getY()) - (minVal - maxVal) * (mid.getProjPos().getY() - max.getProjPos().getY())) * xDenom;
    }

    template <typename T>
    T calcStepY(T const &minVal, T const &midVal, T const &maxVal)
    {
        return ((midVal - maxVal) * (min.getProjPos().getX() - max.getProjPos().getX()) - (minVal - maxVal) * (mid.getProjPos().getX() - max.getProjPos().getX())) * yDenom;
    }

public:
    Interpolation(Vertex const &min_, Vertex const &mid_, Vertex const &max_) : min(min_), mid(mid_), max(max_)
    {
        xDenom = 1.0 / (((mid.getProjPos().getX() - max.getProjPos().getX()) * (min.getProjPos().getY() - max.getProjPos().getY())) - ((min.getProjPos().getX() - max.getProjPos().getX()) * (mid.getProjPos().getY() - max.getProjPos().getY())));
        yDenom = -xDenom;

        depthStep.first = calcStepX<double>(min.getProjPos().getZ(), mid.getProjPos().getZ(), max.getProjPos().getZ());
        depthStep.second = calcStepY<double>(min.getProjPos().getZ(), mid.getProjPos().getZ(), max.getProjPos().getZ());

        overZStep.first = calcStepX<double>(1.0 / min.getProjPos().getW(), 1.0 / mid.getProjPos().getW(), 1.0 / max.getProjPos().getW());
        overZStep.second = calcStepY<double>(1.0 / min.getProjPos().getW(), 1.0 / mid.getProjPos().getW(), 1.0 / max.getProjPos().getW());

        texStep.first = calcStepX<Vec4>(min.getTexCoords() / min.getProjPos().getW(), mid.getTexCoords() / mid.getProjPos().getW(), max.getTexCoords() / max.getProjPos().getW());
        texStep.second = calcStepY<Vec4>(min.getTexCoords() / min.getProjPos().getW(), mid.getTexCoords() / mid.getProjPos().getW(), max.getTexCoords() / max.getProjPos().getW());

        normalStep.first = calcStepX<Vec4>(min.getNormal() / min.getProjPos().getW(), mid.getNormal() / mid.getProjPos().getW(), max.getNormal() / max.getProjPos().getW());
        normalStep.second = calcStepY<Vec4>(min.getNormal() / min.getProjPos().getW(), mid.getNormal() / mid.getProjPos().getW(), max.getNormal() / max.getProjPos().getW());

        positionStep.first = calcStepX<Vec4>(min.getWorldPos() / min.getProjPos().getW(), mid.getWorldPos() / mid.getProjPos().getW(), max.getWorldPos() / max.getProjPos().getW());
        positionStep.second = calcStepY<Vec4>(min.getWorldPos() / min.getProjPos().getW(), mid.getWorldPos() / mid.getProjPos().getW(), max.getWorldPos() / max.getProjPos().getW());
    }

    std::pair<double, double> const &getDepthStep()
    {
        return depthStep;
    }

    std::pair<double, double> const &getOverZStep()
    {
        return overZStep;
    }

    std::pair<Vec4, Vec4> const &getTexStep()
    {
        return texStep;
    }

    std::pair<Vec4, Vec4> const &getNormalStep()
    {
        return normalStep;
    }

    std::pair<Vec4, Vec4> const &getPositionStep()
    {
        return positionStep;
    }
};

void projectSide(std::vector<Interpolant> &scanlines, Vertex &lower, Vertex &higher, int minVal, int side, Interpolation interpolation)
{
    int y0 = std::ceil(lower.getProjPos().getY()), y1 = std::ceil(higher.getProjPos().getY());
    double dY = higher.getProjPos().getY() - lower.getProjPos().getY(), dX = higher.getProjPos().getX() - lower.getProjPos().getX();

    if (dY <= 0)
        return;

    double yOffset = y0 - lower.getProjPos().getY();

    double xStep = dX / dY;
    double x = lower.getProjPos().getX() + yOffset * xStep;

    double xOffset = x - lower.getProjPos().getX();

    double z = lower.getProjPos().getZ() + interpolation.getDepthStep().first * xOffset + interpolation.getDepthStep().second * yOffset;
    double zStep = interpolation.getDepthStep().second + interpolation.getDepthStep().first * xStep;

    double overZ = 1.0 / lower.getProjPos().getW() + interpolation.getOverZStep().first * xOffset + interpolation.getOverZStep().second * yOffset;
    double overZStep = interpolation.getOverZStep().second + interpolation.getOverZStep().first * xStep;

    Vec4 tex = lower.getTexCoords() * (1.0 / lower.getProjPos().getW()) + interpolation.getTexStep().first * xOffset + interpolation.getTexStep().second * yOffset;
    Vec4 texStep = interpolation.getTexStep().second + interpolation.getTexStep().first * xStep;

    Vec4 normal = lower.getNormal() * (1.0 / lower.getProjPos().getW()) + interpolation.getNormalStep().first * xOffset + interpolation.getNormalStep().second * yOffset;
    Vec4 normalStep = interpolation.getNormalStep().second + interpolation.getNormalStep().first * xStep;

    Vec4 position = lower.getWorldPos() * (1.0 / lower.getProjPos().getW()) + interpolation.getPositionStep().first * xOffset + interpolation.getPositionStep().second * yOffset;
    Vec4 positionStep = interpolation.getPositionStep().second + interpolation.getPositionStep().first * xStep;

    for (int y = y0; y < y1; y++)
    {
        scanlines[(y - minVal) * 2 + side] = {x, z, overZ, tex, normal, position};
        x += xStep;
        z += zStep;
        overZ += overZStep;
        tex = tex + texStep;
        normal = normal + normalStep;
        position = position + positionStep;
    }
}

void Scene::fillTriangle(Screen &screen, std::vector<Vertex> &verts, RenderObject const &object)
{
    if (verts[0].getProjPos().getY() > verts[1].getProjPos().getY())
        std::swap(verts[0], verts[1]);
    if (verts[1].getProjPos().getY() > verts[2].getProjPos().getY())
        std::swap(verts[1], verts[2]);
    if (verts[0].getProjPos().getY() > verts[1].getProjPos().getY())
        std::swap(verts[0], verts[1]);

    Interpolation interpolation(verts[0], verts[1], verts[2]);

    int side = Utils::sign((verts[1].getProjPos() - verts[0].getProjPos()).cross(verts[2].getProjPos() - verts[0].getProjPos()).getZ());
    side = side >= 0 ? 0 : 1;

    std::vector<Interpolant> scanlines(2 * (std::ceil(verts[2].getProjPos().getY()) - std::ceil(verts[0].getProjPos().getY())));
    projectSide(scanlines, verts[0], verts[2], std::ceil(verts[0].getProjPos().getY()), side, interpolation);
    projectSide(scanlines, verts[0], verts[1], std::ceil(verts[0].getProjPos().getY()), 1 - side, interpolation);
    projectSide(scanlines, verts[1], verts[2], std::ceil(verts[0].getProjPos().getY()), 1 - side, interpolation);
    for (uint i = 0; i < scanlines.size(); i += 2)
    {
        Interpolant leftInterp = scanlines[i];
        Interpolant rightInterp = scanlines[i + 1];

        int xLeft, xRight;
        xLeft = std::ceil(leftInterp.x);
        xRight = std::ceil(rightInterp.x);

        double xOffset = xLeft - leftInterp.x;

        double z = leftInterp.z + interpolation.getDepthStep().first * xOffset;
        double overZ = leftInterp.overZ + interpolation.getOverZStep().first * xOffset;
        Vec4 tex = leftInterp.tex + interpolation.getTexStep().first * xOffset;
        Vec4 norm = leftInterp.normal + interpolation.getNormalStep().first * xOffset;
        Vec4 pos_ = leftInterp.position + interpolation.getPositionStep().first * xOffset;

        int y = std::ceil(verts[0].getProjPos().getY()) + i / 2;

        for (int x = xLeft; x < xRight; x++)
        {
            if (screen.zbuf(y, x) > z)
            {
                screen.zbuf(y, x) = z;

                double regZ = 1.0 / overZ;

                Vec4 pos = pos_ * regZ;

                Vec4 normal = (norm * regZ).normalize();

                Vec4 texAdjust = tex * regZ;

                texAdjust.setX(std::fmod(texAdjust.getX(), 1.0));
                if (texAdjust.getX() < 0)
                    texAdjust.setX(1 + texAdjust.getX());
                texAdjust.setY(std::fmod(texAdjust.getY(), 1.0));
                if (texAdjust.getY() < 0)
                    texAdjust.setY(1 + texAdjust.getY());

                texAdjust.setY(1 - texAdjust.getY());

                //std::cout << texAdjust << std::endl;

                Color col = object.getMaterial()->mapKd.read(std::lround(texAdjust.getY() * (object.getMaterial()->mapKd.getHeight() - 1)), std::lround(texAdjust.getX() * (object.getMaterial()->mapKd.getWidth() - 1)));
                col = {215, 255, 215, 255};

                Vec4 const &Ka = col;
                Vec4 const &Kd = Ka;
                Vec4 const &Ks = object.getMaterial()->Ks;

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
                        Vec4 const &H = (L + V).normalize();
                        double NL = normal.dot(L);
                        int B = NL > 0 ? 1 : 0;
                        Vec4 const &diffuse = Kd * light.Id * B * (NL);
                        Vec4 const &specular = Ks * light.Is * B * std::pow(normal.dot(H), object.getMaterial()->Ns);
                        illum = illum + diffuse + specular;
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
                        Vec4 const &specular = Ks * light.Is * B * std::pow(normal.dot(H), object.getMaterial()->Ns);
                        illum = illum + diffuse / (dist * dist) + specular / (dist * dist);
                    }
                }

                illum.set(0, Utils::clamp(illum[0], 0, 1));
                illum.set(1, Utils::clamp(illum[1], 0, 1));
                illum.set(2, Utils::clamp(illum[2], 0, 1));

                screen(y, x) = illum.toColor();
            }
            //z += zStep;
            z += interpolation.getDepthStep().first; // may need to recalc etc: (rightInterp.z - leftInterp.z)/(rightInterp.x - leftInterp.x));
            overZ += interpolation.getOverZStep().first;
            tex = tex + interpolation.getTexStep().first;
            norm = norm + interpolation.getNormalStep().first;
            pos_ = pos_ + interpolation.getPositionStep().first;
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
}