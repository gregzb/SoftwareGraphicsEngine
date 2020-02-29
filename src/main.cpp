#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>
#include <limits>

#include <chrono>

#include "Utils.hpp"
#include "PixelGrid.hpp"
#include "Screen.hpp"
#include "Mat4.hpp"

#include "RenderObject.hpp"
#include "Camera.hpp"

#include "OBJLoader.hpp"
#include "Scene.hpp"
#include "Light.hpp"

int main()
{
    OBJLoader obj("resources/dragonboi.obj");
    Screen screen(500, 500);
    Camera cam(60, static_cast<double>(screen.getWidth()) / screen.getHeight(), 0.001, 1000);
    Scene mainScene;
    mainScene.addObject("Dragon", obj.toRenderObject("Dragon_Dragon_2"));
    //mainScene.addObject("Dragon", obj.toRenderObject("Suzanne"));
    //mainScene.addObject("Plane", obj.toRenderObject("Plane"));

    mainScene.getObject("Dragon").setPosition({.2, -.75, -3});
    mainScene.getObject("Dragon").setRotation({10 * M_PI  / 180, 0 * M_PI  / 180, 0});
    //mainScene.getObject("Plane").setPosition({-5, 4, -10});

    mainScene.addLight("Ambient", {LightType::Ambient, {.15, .15, .15}});
    //mainScene.addLight("Point", {LightType::Point, {}, {6, 6, 6}, {6, 6, 6}, {2, 2, -1}});
    mainScene.addLight("Directional", {LightType::Directional, {}, {.6, .6, .6}, {.6, .6, .6}, {-1, -1, -1}});

    mainScene.renderToScreen(cam, screen);
    screen.display();

    // for (int i = 0; i < 60; i++)
    // {
    //     std::cout << i << std::endl;
    //     Vec4 rot = mainScene.getObject("Dragon").getRotation();
    //     rot.setY(2 * M_PI / 60 * i);
    //     mainScene.getObject("Dragon").setRotation(rot);

    //     screen.clear({0, 0, 0, 255});
    //     screen.clearZbuf();

    //     mainScene.renderToScreen(cam, screen);

    //     std::string s = std::to_string(i);
    //     s.insert(s.begin(), 3 - s.size(), '0');

    //     screen.toFileExtension("img" + s + ".png");
    // }

    return 0;
}