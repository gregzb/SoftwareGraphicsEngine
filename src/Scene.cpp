#include "Scene.hpp"

void Scene::removeObject(std::string const &name)
{
    if (objects.count(name) > 0)
    {
        objects.erase(name);
    }
}
void Scene::addObject(std::string const &name, RenderObject const &object)
{
    objects.insert(name, object);
}
void Scene::renderToScreen(Camera const &cam, Screen &screen)
{
}