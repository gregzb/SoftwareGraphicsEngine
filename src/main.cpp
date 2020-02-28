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
    std::cout << "Loading" << std::endl;
    OBJLoader obj("resources/monkey.obj");

    std::cout << "Making Screen" << std::endl;
    Screen screen(600, 600);
    //screen.clear({255, 255, 255, 255});
    screen.clear({0, 0, 0, 255});
    Camera cam(60, static_cast<double>(screen.getWidth()) / screen.getHeight(), 0.001, 1000);
    //RenderObject monkey = obj.toRenderObject("Plane");
    //std::cout << "here" << std::endl;
    std::cout << "Getting object" << std::endl;

    Scene mainScene;
    mainScene.addObject("Suzanne", obj.toRenderObject("Suzanne"));
    //std::cout << "here2" << std::endl;
    mainScene.getObject("Suzanne").setPosition({0, 0, -3});
    //monkey.setRotation({(20) * M_PI / 180, (0) * M_PI / 180, (0) * M_PI / 180});
    mainScene.getObject("Suzanne").setRotation({(20) * M_PI / 180, (20) * M_PI / 180, (0) * M_PI / 180});

    mainScene.addLight("Ambient", {LightType::Ambient, {.15, .15, .15}});
    //mainScene.addLight("Directional", {LightType::Directional, {}, {1, 1, 1}, {1, 1, 1}, {-1, -1, -1}});
    mainScene.addLight("Point", {LightType::Point, {}, {1, 1, 1}, {1, 1, 1}, {3, 3, -1}});

    std::cout << "Drawing" << std::endl;
    mainScene.renderToScreen(cam, screen);
    //screen.renderObject(cam, monkey);

    // for (int i = 0; i < 40; i++)
    // {
    //     Vec4 rot = monkey.getRotation();
    //     rot.setY(2 * M_PI / 40 * i);
    //     monkey.setRotation(rot);

    //     screen.graphics.clear({255, 255, 255, 255});
    //     screen.clearZbuf();
    //     screen.graphics.renderObject(cam, monkey);
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
    std::cout << "Displaying" << std::endl;
    screen.display();
    screen.toFileExtension("diamond.png");

    return 0;
}