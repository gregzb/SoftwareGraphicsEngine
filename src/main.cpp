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
    Material::TexFiltering = TextureFiltering::BILINEAR;

    // OBJLoader obj("resources/dragonboi.obj");
    // std::cout << "here0" << std::endl;
    //OBJLoader obj("resources/table.obj");
    //OBJLoader obj("resources/onetest.obj");
    //OBJLoader obj("resources/airboat.obj");
    // OBJLoader obj("resources/monkey2.obj");
    OBJLoader obj("resources/towerthing.obj");
    //std::cout << "here" << std::endl;
    //OBJLoader obj("resources/cube.obj");
    // OBJLoader obj("resources/sword.obj");
    //OBJLoader obj("resources/smalltex.obj");
    Screen screen(500, 500);
    //Camera cam(170, static_cast<double>(screen.getWidth()) / screen.getHeight(), .1, 1000, false);
    Camera cam(60, static_cast<double>(screen.getWidth()) / screen.getHeight(), .1, 1000, true);
    Scene mainScene;

    //RenderObject r = obj.toRenderObject("zAset_rock_assembly_M_thkqfbjda_LOD0_Aset_rock_assembly_M_thkqfbjda_LOD0");
    RenderObject r = obj.toRenderObject("Aset_other__L_ufnscjdga_LOD2");
    // RenderObject r = obj.toRenderObject("sword_Plane");
    //RenderObject r = obj.toRenderObject("Cube");
    //RenderObject r = obj.toRenderObject("boat");
    //RenderObject r = obj.toRenderObject("Suzanne");
    //RenderObject r = obj.toRenderObject("Aset_wood_misc_M_tl1xfcbfa_LOD2");
    // RenderObject r(false);
    // r.addSphere({}, 1, 30, 15);
    mainScene.addObject("Sphere", r);
    mainScene.getObject("Sphere").setPosition({0, -0.8, -2.5});
    //mainScene.getObject("Sphere").setPosition({0, -.5, -2});
    mainScene.getObject("Sphere").setRotation({20 * M_PI / 180, -60 * M_PI / 180, 0});
    //mainScene.getObject("Sphere").setRotation({25 * M_PI / 180, 2 * M_PI / 60 * 35, 0});

    //mainScene.addLight("Ambient", {LightType::Ambient, {1, 1, 1}});

    mainScene.addLight("Ambient", {LightType::Ambient, {0.05, 0.05, 0.05}});
    //mainScene.addLight("Ambient", {LightType::Ambient, {1, 1, 1}});
    // //mainScene.addLight("Ambient", {LightType::Ambient, {1, 1, 1}});
    // //mainScene.addLight("Point", {LightType::Point, {}, {6, 6, 6}, {6, 6, 6}, {2, 2, -1}});
    mainScene.addLight("Directional", {LightType::Directional, {}, {.95, .95, .95}, {.95, .95, .95}, {-1, -1, -1}});
    //mainScene.addLight("Glow", {LightType::Directional, {}, {0, 0, 1}, {0, 0, 1}, {2, 2, 1}});
    screen.clear({255, 255, 255, 255});
    mainScene.renderToScreen(cam, screen);

    screen.display();


    system("mkdir -p ./animation/");
    system("rm ./animation/*");

    for (int i = 0; i < 60; i++)
    {
        std::cout << i << std::endl;
        Vec4 rot = mainScene.getObject("Sphere").getRotation();
        rot.setY(2 * M_PI / 60 * i);
        mainScene.getObject("Sphere").setRotation(rot);

        screen.clear({0, 0, 0, 255});
        screen.clear({255, 255, 255, 255});
        screen.clearZbuf();

        mainScene.renderToScreen(cam, screen);

        std::string s = std::to_string(i);
        s.insert(s.begin(), 5 - s.size(), '0');

        screen.toFileExtension("./animation/img" + s + ".png");
    }

    int delay = 4;
    std::string outputName = "animation.gif";

    system(("convert -delay " + std::to_string(delay) + " ./animation/* " + outputName).c_str());
    std::cout << "The animation was rendered into " << outputName << std::endl;
    system(("animate " + outputName).c_str());

    return 0;
}