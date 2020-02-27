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

int main()
{
    OBJLoader obj("resources/monkey.obj");

    Screen screen(500, 500);
    screen.graphics.clear({255, 255, 255, 255});
    Camera cam(60, static_cast<double>(screen.getWidth()) / screen.getHeight(), 0.001, 1000);
    //RenderObject monkey = obj.toRenderObject("Plane");
    //std::cout << "here" << std::endl;
    RenderObject monkey = obj.toRenderObject("Suzanne");
    //std::cout << "here2" << std::endl;
    monkey.setPosition({0, 0, -3});
    //monkey.setRotation({(20) * M_PI / 180, (0) * M_PI / 180, (0) * M_PI / 180});
    monkey.setRotation({(20) * M_PI / 180, (20) * M_PI / 180, (0) * M_PI / 180});

    screen.graphics.renderObject(cam, monkey);

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

    //std::cout << "here3" << std::endl;
    //screen.graphics.renderObject(cam, monkey);
    //std::cout << "here4" << std::endl;
    screen.display();

    return 0;
}