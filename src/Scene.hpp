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
    std::unordered_map<std::string const &, RenderObject const &> objects;

public:
    void removeObject(std::string const &name);
    void addObject(std::string const &name, RenderObject const &object);
    void renderToScreen(Camera const &cam, Screen &screen);
};