#pragma once

#include <vector>
#include <unordered_map>
#include "RenderObject.hpp"
#include "Camera.hpp"
#include "Screen.hpp"

class Scene
{
private:
    //std::vector<Light> lights;
    std::unordered_map<std::string, RenderObject> objects;

    void renderObject(Camera const &cam, Screen &screen, RenderObject &object);
    void drawTriangle(Screen &screen, std::vector<Vertex> &vertices, RenderObject const &object);
    void fillTriangle(Screen &screen, std::vector<Vertex> &verts, RenderObject const &object);

public:
    void removeObject(std::string const &name);
    void addObject(std::string const &name, RenderObject const &object);
    RenderObject &getObject(std::string const &name);
    void renderToScreen(Camera const &cam, Screen &screen);
};