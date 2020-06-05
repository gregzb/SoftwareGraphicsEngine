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

    OBJLoader obj("resources/testscene.obj");
    Screen screen(1000, 1000, true);
    //Camera cam(170, static_cast<double>(screen.getWidth()) / screen.getHeight(), .1, 1000, false);
    Camera cam(40, static_cast<double>(screen.getWidth()) / screen.getHeight(), .1, 1000, true);
    Scene mainScene;

    // RenderObject r = obj.toRenderObject("Aset_other__L_ufnscjdga_LOD2");
    // r.updateVertexNormals();
    // mainScene.addObject("Sphere", r);
    // mainScene.getObject("Sphere").setPosition({0, 0, 0});
    // mainScene.getObject("Sphere").setRotation({0 * M_PI / 180, -60 * M_PI / 180, 0});

    auto objects = obj.toRenderObjects();
    for (auto & object : objects) {
        object.second.updateVertexNormals();
        mainScene.addObject(object.first, object.second);
    }

    //mainScene.addLight("Ambient", {LightType::Ambient, {0.125, 0.125, 0.125}});
    //mainScene.addLight("Ambient", {LightType::Ambient, {1, 1, 1}});
    // mainScene.addLight("Directional", {LightType::Directional, {}, {1, 1, 1}, {1, 1, 1}, {-0.5, -1, -1}});
    //mainScene.addLight("Directional", {LightType::Directional, {}, {1, 1, 1}, {1, 1, 1}, {-2, -1, 1}});
    //mainScene.addLight("Directional", {LightType::Directional, {}, {1, 1, 1}, {1, 1, 1}, {-0.8, -0.5, 0.9}});
    mainScene.addLight("Point1", {LightType::Point, {}, Vec4(1.2, 0.9, 0.9) * 10, Vec4(1.2, 0.9, 0.9) * 10, {10.57, 2.5, 3.7}});
    mainScene.addLight("Point2", {LightType::Point, {}, Vec4(1.3, 0.9, 0.9) * 7.5, Vec4(1.3, 0.9, 0.9) * 7.5, {-1.1, 2.4, 3.78}});
    mainScene.addLight("Point3", {LightType::Point, {}, Vec4(1.3, 0.9, 0.9) * 12, Vec4(1.3, 0.9, 0.9) * 12, {-11.1, 2.3, 1.7}});
    mainScene.addLight("Point4", {LightType::Point, {}, Vec4(1.3, 0.9, 0.9) * 10, Vec4(1.3, 0.9, 0.9) * 10, {3.7, 2.45, -4.15}});
    mainScene.addLight("Point5", {LightType::Point, {}, Vec4(1.3, 0.9, 0.9) * 5, Vec4(1.3, 0.9, 0.9) * 5, {-6.4, 2.7, -5.2}});
    mainScene.addLight("Point6", {LightType::Point, {}, Vec4(1.3, 0.9, 0.9) * 7, Vec4(1.3, 0.9, 0.9) * 7, {17, 2.35, -3.75}});
    mainScene.addLight("Directional", {LightType::Directional, {}, Vec4(0.2, 0.2, 0.4)*0.67, Vec4(0.2, 0.2, 0.4)*0.67, {-0.8, -0.7, -1.7}});

    OBJLoader obj2("resources/skybox.obj");
    RenderObject skyBox = obj2.toRenderObject("Cube");
    skyBox.updateVertexNormals();
    //skyBox.setRotation({0, 340 * M_PI / 180, 0});
    skyBox.setRotation({0, (-24 + 90) * M_PI / 180, 0});
    mainScene.setSkybox(skyBox);

    cam.setPosition({25.952, 2.8147, -3.3683});
    cam.setLookMode(LookMode::EULER);
    cam.setRotation({-2 * M_PI / 180, 104 * M_PI / 180, 0 * M_PI / 180});

    //double dist = 20;
    // cam.setPosition({dist, 5, 0});
    // cam.setLookMode(LookMode::LOOKAT);
    // cam.lookAt({0, 1, 0, 0});

    //cam.setRotation({-20 * M_PI / 180, 0* M_PI/180, 0});
    //cam.setRotation({270 * M_PI / 180, 0 * M_PI / 180, 0});

    screen.clear({255, 255, 255, 255});
    screen.clearZbuf();

    // mainScene.renderToScreen(cam, screen);

    // screen.display();

    auto t1 = std::chrono::high_resolution_clock::now();
    mainScene.renderToScreen(cam, screen);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto microTimeToRender = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto renderTime = microTimeToRender / (1000000.0);
    std::cout << renderTime << " s" << std::endl;

    //screen.display();
    screen.toFileExtension("oooo.png");

    // system("mkdir -p ./animation/");
    // system("rm ./animation/*");

    // for (int i = 0; i < 60; i++)
    // {
    //     std::cout << i << std::endl;
    //     // Vec4 rot = mainScene.getObject("Sphere").getRotation();
    //     // rot.setY(2 * M_PI / 60 * i);
    //     // mainScene.getObject("Sphere").setRotation(rot);
    //     // Vec4 rot = cam.getRotation();
    //     // rot.setY(2 * M_PI / 60 * i);
    //     // cam.setRotation(rot);

    //     mainScene.getLight("Directional").pos = {-2*cos(2 * M_PI / 60 * i), -1, sin(2 * M_PI / 60 * i)};
    //     std::cout << mainScene.getLight("Directional").pos << std::endl;
    //     mainScene.getSkybox().setRotation({0, 300 * M_PI / 180 + 2 * M_PI / 60 * i, 0});

    //     // double x = std::sin(2 * M_PI / 60 * i) * dist;
    //     // double y = cam.getPosition()[1];
    //     // double z = std::cos(2 * M_PI / 60 * i) * dist;
    //     // cam.setPosition({x, y, z});

    //     // std::cout << cam.getPosition() << std::endl;

    //     screen.clear({0, 0, 0, 255});
    //     screen.clear({255, 255, 255, 255});
    //     screen.clearZbuf();

    //     mainScene.renderToScreen(cam, screen);

    //     std::string s = std::to_string(i);
    //     s.insert(s.begin(), 5 - s.size(), '0');

    //     screen.toFileExtension("./animation/img" + s + ".png");
    // }

    // int delay = 4;
    // std::string outputName = "animation.gif";

    // system(("convert -delay " + std::to_string(delay) + " ./animation/* " + outputName).c_str());
    // std::cout << "The animation was rendered into " << outputName << std::endl;
    // system(("animate " + outputName).c_str());

    return 0;
}