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
    //OBJLoader obj("resources/dragonboi.obj");
    //std::cout << "here0" << std::endl;
    OBJLoader obj("resources/onetest.obj");
    //std::cout << "here1" << std::endl;
    Screen screen(1000, 1000);
    Camera cam(60, static_cast<double>(screen.getWidth()) / screen.getHeight(), .1, 1000);
    Scene mainScene;
    //std::cout << "here!" << std::endl;
    //std::cout << "here2" << std::endl;
    RenderObject r = obj.toRenderObject("zAset_rock_assembly_M_thkqfbjda_LOD0_Aset_rock_assembly_M_thkqfbjda_LOD0");
    //std::cout << "here2.5" << std::endl;
    mainScene.addObject("Rock", r);
    //std::cout << "here3" << std::endl;
    mainScene.getObject("Rock").setRotation({30 * M_PI  / 180, 40 * M_PI  / 180, 0});
    mainScene.getObject("Rock").setPosition({0, -0.5, -2.5});
    //std::cout << "here4" << std::endl;
    // mainScene.addObject("Dragon", obj.toRenderObject("Dragon_Dragon_2"));
    // //mainScene.addObject("Dragon", obj.toRenderObject("Suzanne"));
    // //mainScene.addObject("Plane", obj.toRenderObject("Plane"));

    // mainScene.getObject("Dragon").setPosition({.4, -.75, -3});
    // //mainScene.getObject("Dragon").setPosition({0, 0, -1.5});
    // mainScene.getObject("Dragon").setRotation({10 * M_PI  / 180, 20 * M_PI  / 180, 0});

    //mainScene.getObject("Plane").setPosition({-5, 4, -10});

    // RenderObject obj1;
    // //obj1.addTriangle({-.2, -.2, 1}, {1, 0, -3}, {1, 1, -3});
    // obj1.addTriangle({-2, 0, -3}, {1, 0, -3}, {1, 1, -3});

    // mainScene.addObject("tri", obj1);

    // auto v = mainScene.getObject("Rock").getMeshIndices();

    // auto size = v.size() / 3;

    //mainScene.getObject("Rock").getMeshIndices() = std::vector<int>(v.begin() + (287 * size / 1000) * 3, v.begin() + (288 * size / 1000) * 3);

    mainScene.addLight("Ambient", {LightType::Ambient, {.2, .2, .2}});
    //mainScene.addLight("Ambient", {LightType::Ambient, {1, 1, 1}});
    //mainScene.addLight("Point", {LightType::Point, {}, {6, 6, 6}, {6, 6, 6}, {2, 2, -1}});
    mainScene.addLight("Directional", {LightType::Directional, {}, {1, 1, 1}, {1, 1, 1}, {-2, -2, -1}});
    mainScene.renderToScreen(cam, screen);

    // std::vector<Vec4> positions;

    // for (auto v : mainScene.getObject("Rock").getMeshIndices()) {
    //     std::cout << v << std::endl;
    //     positions.push_back(mainScene.getObject("Rock").getMesh()[v].getTexCoords());
    //     std::cout << mainScene.getObject("Rock").getMesh()[v].getPos() << " " << mainScene.getObject("Rock").getMesh()[v].getTexCoords() << std::endl;
    // }

    // Material * mat = mainScene.getObject("Rock").getMat(mainScene.getObject("Rock").getMeshIndices()[0]/3);
    // for (int i = 0; i < positions.size(); i++) {
    //     Vec4 pos1 = positions[i];
    //     Vec4 pos2 = positions[(i+1)%positions.size()];

    //     //Vec4 pos3 = {std::lround((1-pos1.getY()) * (mat->kdMap.value().getHeight() - 1)), std::lround(pos1.getX() * (mat->kdMap.value().getWidth() - 1))};
    //     //Vec4 pos4 = {std::lround((1-pos2.getY()) * (mat->kdMap.value().getHeight() - 1)), std::lround(pos2.getX() * (mat->kdMap.value().getWidth() - 1))};

    //     Vec4 pos3 = {std::lround(pos1.getX() * (mat->kdMap.value().getHeight() - 1)), std::lround((1-pos1.getY()) * (mat->kdMap.value().getWidth() - 1))};
    //     Vec4 pos4 = {std::lround(pos2.getX() * (mat->kdMap.value().getHeight() - 1)), std::lround((1-pos2.getY()) * (mat->kdMap.value().getWidth() - 1))};

    //     //kdMap.value().read(std::lround((1-pos.getY()) * (kdMap.value().getHeight() - 1)), std::lround(pos.getX() * (kdMap.value().getWidth() - 1)));

    //     mat->kdMap.value().drawLine(pos3, pos4, {255, 0, 255, 0});
    // }

    // mat->kdMap.value().display();



    screen.display();

    // std::cout << mainScene.getObject("Rock").getMeshIndices().size() << std::endl;
    // for ()

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