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
    Screen screen(1920, 1080);
    Camera cam(60, static_cast<double>(screen.getWidth()) / screen.getHeight(), 0.001, 1000);
    Scene mainScene;
    mainScene.addObject("Dragon", obj.toRenderObject("Dragon_Dragon_2"));
    //mainScene.addObject("Plane", obj.toRenderObject("Plane"));

    mainScene.getObject("Dragon").setPosition({.2, -.75, -3});
    mainScene.getObject("Dragon").setRotation({10 * M_PI  / 180, 0 * M_PI  / 180, 0});
    //mainScene.getObject("Plane").setPosition({-5, 4, -10});

    mainScene.addLight("Ambient", {LightType::Ambient, {.15, .15, .15}});
    mainScene.addLight("Point", {LightType::Point, {}, {6, 6, 6}, {6, 6, 6}, {2, 2, -1}});
    //mainScene.addLight("Directional", {LightType::Directional, {}, {1, 1, 1}, {1, 1, 1}, {-1, -1, -1}});

    // mainScene.renderToScreen(cam, screen);
    // screen.display();

    for (int i = 0; i < 60; i++)
    {
        Vec4 rot = mainScene.getObject("Dragon").getRotation();
        rot.setY(2 * M_PI / 60 * i);
        mainScene.getObject("Dragon").setRotation(rot);

        screen.clear({0, 0, 0, 255});
        screen.clearZbuf();

        mainScene.renderToScreen(cam, screen);

        std::string s = std::to_string(i);
        s.insert(s.begin(), 3 - s.size(), '0');

        screen.toFileExtension("img" + s + ".png");
    }


    // std::cout << "Loading" << std::endl;
    // OBJLoader obj("resources/monkey2.obj");

    // std::cout << "Making Screen" << std::endl;
    // Screen screen(600, 600);
    // //screen.clear({255, 255, 255, 255});
    // screen.clear({0, 0, 0, 255});
    // Camera cam(60, static_cast<double>(screen.getWidth()) / screen.getHeight(), 0.001, 1000);
    // //RenderObject monkey = obj.toRenderObject("Plane");
    // //std::cout << "here" << std::endl;
    // std::cout << "Getting object" << std::endl;

    // Scene mainScene;
    // mainScene.addObject("Suzanne", obj.toRenderObject("Suzanne"));
    // //std::cout << "here2" << std::endl;
    // mainScene.getObject("Suzanne").setPosition({0, 0, -3});
    // //monkey.setRotation({(20) * M_PI / 180, (0) * M_PI / 180, (0) * M_PI / 180});
    // mainScene.getObject("Suzanne").setRotation({(20) * M_PI / 180, (150) * M_PI / 180, (0) * M_PI / 180});
    // mainScene.getObject("Suzanne").setRotation({(20) * M_PI / 180, (20) * M_PI / 180, (0) * M_PI / 180});

    // mainScene.addLight("Ambient", {LightType::Ambient, {.15, .15, .15}});
    // // mainScene.addLight("Ambient", {LightType::Ambient, {1, 1, 1}});
    // // mainScene.addLight("Directional", {LightType::Directional, {}, {1, 1, 1}, {1, 1, 1}, {-1, -1, -1}});
    // mainScene.addLight("Point1", {LightType::Point, {}, {1, 0.5, 0.5}, {1, 0.5, 0.5}, {2, 2, -0.5}});
    // mainScene.getLight("Point1").Id = mainScene.getLight("Point1").Id * 10;
    // mainScene.getLight("Point1").Id = mainScene.getLight("Point1").Is * 10;
    // mainScene.addLight("Point2", {LightType::Point, {}, {0.5, 0.5, 1}, {0.5, 0.5, 1}, {-2, 2, -0.5}});
    // mainScene.getLight("Point2").Id = mainScene.getLight("Point2").Id * 10;
    // mainScene.getLight("Point2").Id = mainScene.getLight("Point2").Is * 10;
    // // mainScene.addLight("Point3", {LightType::Point, {}, {0.5, 1, 0.5}, {0.5, 1, 0.5}, {0, -.2, 0}});
    // // mainScene.getLight("Point3").Id = mainScene.getLight("Point3").Id * 2;
    // // mainScene.getLight("Point3").Id = mainScene.getLight("Point3").Is * 2;

    // // for (auto & vert : mainScene.getObject("Suzanne").getMesh()) {
    // //     std::cout << vert.getPos() << std::endl;
    // // }

    // std::cout << "Drawing" << std::endl;
    // mainScene.renderToScreen(cam, screen);

    // // std::cout << std::endl << std::endl << std::endl << std::endl << std::endl;

    // // for (auto & vert : mainScene.getObject("Suzanne").getMesh()) {
    // //     std::cout << vert.getWorldPos() << std::endl;
    // // }
    // screen.display();
    // //screen.renderObject(cam, monkey);

    // for (int i = 0; i < 60; i++)
    // {
    //     Vec4 rot = mainScene.getObject("Suzanne").getRotation();
    //     rot.setY(2 * M_PI / 60 * i);
    //     mainScene.getObject("Suzanne").setRotation(rot);

    //     screen.clear({0, 0, 0, 255});
    //     screen.clearZbuf();

    //     mainScene.renderToScreen(cam, screen);

    //     std::string s = std::to_string(i);
    //     s.insert(s.begin(), 3 - s.size(), '0');

    //     screen.toFileExtension("img" + s + ".png");
    // }

    // auto t1 = std::chrono::high_resolution_clock::now();

    // for (int i = 0; i < 40; i++)
    // {
    //     Vec4 rot = monkey.getRotation();
    //     rot.setY(2 * M_PI / 40 * i);
    //     monkey.setRotation(rot);

    //     screen.graphics.clear({255, 255, 255, 255});
    //     screen.clearZbuf();
    //     screen.graphics.renderObject(cam, monkey);
    // }

    // auto t2 = std::chrono::high_resolution_clock::now();

    // auto dt = t2 - t1;

    // std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(dt).count() << " ms" << std::endl;

    //std::cout << "here3" << std::endl;
    //screen.graphics.renderObject(cam, monkey);
    //std::cout << "here4" << std::endl;
    // std::cout << "Displaying" << std::endl;
    // screen.display();
    // screen.toFileExtension("diamond.png");

    return 0;
}