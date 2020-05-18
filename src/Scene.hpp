#pragma once

#include <vector>
#include <unordered_map>
#include "RenderObject.hpp"
#include "Camera.hpp"
#include "Screen.hpp"
#include "Light.hpp"

class Scene
{
private:
    //std::vector<Light> lights;
    std::unordered_map<std::string, RenderObject> objects;
    std::unordered_map<std::string, Light> lights;
    std::unordered_map<std::string, Light> transformedLights;

    void renderObject(Camera const &cam, Screen &screen, RenderObject &object);
    void drawTriangle(Screen &screen, std::vector<Vertex> vertices, RenderObject const &object, Material const * mat);
    void fillTriangle(Screen &screen, std::vector<Vertex> &verts, RenderObject const &object, Material const * mat);
    void clipTriangle(std::vector<Vertex> & triangle, int dimension, int side, std::vector<Vertex> & out);

public:
    void removeObject(std::string const &name);
    void addObject(std::string const &name, RenderObject const &object);
    RenderObject &getObject(std::string const &name);

    void removeLight(std::string const &name);
    void addLight(std::string const &name, Light const &light);
    Light &getLight(std::string const &name);

    void renderToScreen(Camera const &cam, Screen &screen);
};